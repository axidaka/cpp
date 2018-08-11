#include "StdAfx.h"
#include "BaseFunc.h"
#include "SmartPathSearch.h"

// =====================================================================================================================
// =======================================================================================================================

SmartPathSearch::SmartPathSearch(ISmartPathSearchAnswer *_answer) : m_answer(_answer), m_find_all_target(false), m_exit_event(TRUE, FALSE), m_search_thread_handle(NULL)
{
}

// =====================================================================================================================
// =======================================================================================================================
SmartPathSearch::~SmartPathSearch(void)
{
	this->Stop();
}

// =====================================================================================================================
// =======================================================================================================================
int SmartPathSearch::AddTargetFile(const CString &_filename)
{
	ATLASSERT(!_filename.IsEmpty());
	m_target_file_list.push_back(_filename);
	return m_target_file_list.size();
}

// =====================================================================================================================
// =======================================================================================================================
int SmartPathSearch::AddPatternString(const CString &_pattern, int _score /* PATTERN_DEFAULT_SCORE */)
{
	ATLASSERT(!_pattern.IsEmpty());
	m_pattern_list.push_back(PATTERN_INFO(_pattern, _score));
	return m_pattern_list.size();
}

// =====================================================================================================================
// =======================================================================================================================
bool SmartPathSearch::Start(bool _find_all_target /* false */)
{
	m_find_all_target = _find_all_target;
	this->Stop();
	if (InitializeSearchDirectory() <= 0) {
		return false;
	}

	m_search_thread_handle = reinterpret_cast<HANDLE> (_beginthreadex(NULL, 0, (unsigned int(__stdcall *) (void *))SearchThread, this, NULL, NULL));
	return true;
}

// =====================================================================================================================
// =======================================================================================================================
UINT WINAPI SearchThread(SmartPathSearch *_this)
{
	ATLTRACE(_T("搜索线程启动。\n"));

	_this->m_exit_event.Reset();

	//~~~~~~~~~~~~~~~~~~~~~~
	int search_count = 0;
	ATL::CPath temp_full_path;
	ATL::CPath current_dir;
	ATL::CPath current_path;
	WIN32_FIND_DATA find_data;
	HANDLE find_handle;
	//~~~~~~~~~~~~~~~~~~~~~~

	while (!_this->m_directory_table.empty() && (WAIT_OBJECT_0 != WaitForSingleObject(_this->m_exit_event, 0))) {
		search_count++;
		current_dir = _this->ExtractHighScoreDirectory(current_dir);

		if (_this->m_answer->OnSearchPath(current_dir, search_count)) {
			break;
		}

		if (_this->IsTargetDirectory(current_dir, current_path)) {
			if (_this->m_answer->OnSearchFound(current_dir, current_path, search_count)) {
				break;
			}

			continue;
		}

		find_handle = FindFirstFile((current_dir + _T("*")), &find_data);
		if (find_handle != INVALID_HANDLE_VALUE) {
			do
			{
				if (find_data.cFileName[0] == _T('.')) {
					continue;
				}

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

					// 只关心目录
					temp_full_path = ATL::CPath(current_dir + find_data.cFileName + _T("\\"));

					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					DIR_INFO temp_dir_info(temp_full_path, _this->CalcScore(temp_full_path));
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

					_this->m_directory_table[temp_dir_info.GetScore()].push(temp_dir_info);
				}
			} while
				(
				FindNextFile(find_handle, &find_data)
				&& (WAIT_OBJECT_0 != WaitForSingleObject(_this->m_exit_event, 0))
				);
			FindClose(find_handle);
		}
	}

	_this->m_answer->OnSearchFinish(search_count);
	ATLTRACE(_T("搜索线程结束。\n"));
	_endthreadex(0);
	return 0;
}

ATL::CPath & SmartPathSearch::ExtractHighScoreDirectory(ATL::CPath & _dir_path) {
	ATLASSERT(!m_directory_table.empty());
	static int high_score;
	high_score = m_directory_table.
		rbegin()
		->first;
	_dir_path = m_directory_table[high_score].
		front()
		.GetPath();
	m_directory_table[high_score].pop();
	if (m_directory_table[high_score].empty()) {
		m_directory_table.erase(high_score);
	}
	return _dir_path;
}

// =====================================================================================================================
// =======================================================================================================================
int SmartPathSearch::InitializeSearchDirectory()
{
	m_directory_table.clear();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	DWORD drive_string_size = GetLogicalDriveStrings(0, NULL);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (drive_string_size == 0) {
		return 0;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	wchar_t szBuffer[s_nMaxBuffer];
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (GetLogicalDriveStringsW(sizeof(szBuffer), szBuffer)) {

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		std::vector<CString> drive_list;
		TCHAR *dist_name = szBuffer;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		while (*dist_name != NULL) {

			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			UINT dist_type = GetDriveType(dist_name);
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			if (dist_type == DRIVE_FIXED || dist_type == DRIVE_REMOTE) {
				drive_list.push_back(dist_name);
			}

			dist_name += _tcslen(dist_name) + 1;
		}

		for (std::vector<CString>::reverse_iterator rit = drive_list.rbegin(); rit != drive_list.rend(); ++rit) {

			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			DIR_INFO temp_dir_info(rit->GetString(), CalcScore(rit->GetString()));
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			m_directory_table[temp_dir_info.GetScore()].push(temp_dir_info);
		}
	}

	return this->GetDirectoryTableCount();
}

// =====================================================================================================================
// =======================================================================================================================
int SmartPathSearch::CalcScore(const ATL::CPath &_directory_path)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int score = DIRECTORY_DEFAULT_SCORE;
	int dir_level = 0;
	int pos = _directory_path.m_strPath.Find(_T('\\'), 0);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	while (-1 != pos) {
		dir_level++;
		pos = _directory_path.m_strPath.Find(_T('\\'), pos + 1);
	}

	score = score - (dir_level - 1) * DIRECTORY_SUB_SCORE;

	//~~~~~~~~~~~~~~~~~~~~~~~
	int high_pattern_score = 0;
	//~~~~~~~~~~~~~~~~~~~~~~~

	for (std::vector<PATTERN_INFO>::const_iterator it = m_pattern_list.begin(); it != m_pattern_list.end(); ++it) {
		if (it->GetScore() > high_pattern_score && _directory_path.MatchSpec(it->GetString())) {
			high_pattern_score = it->GetScore();
		}
	}

	return score + high_pattern_score;
}

// =====================================================================================================================
// =======================================================================================================================
void SmartPathSearch::Stop()
{
	m_exit_event.Set();
	Sleep(10);	// 等待搜索线程
	if (m_search_thread_handle) {
		CloseHandle(m_search_thread_handle);
		m_search_thread_handle = NULL;
	}
}

// =====================================================================================================================
// =======================================================================================================================
bool SmartPathSearch::IsTargetDirectory(const ATL::CPath &_directory_path, ATL::CPath &file_path)
{
	//~~~~~~~~~~~~~~~~~
	ATL::CPath temp_path;
	//~~~~~~~~~~~~~~~~~

	if (m_find_all_target) {
		for (std::vector<CString>::const_iterator it = m_target_file_list.begin(); it != m_target_file_list.end(); ++it) {
			temp_path.m_strPath = _directory_path.m_strPath + *it;
			if (FALSE == temp_path.FileExists()) {
				return false;
			}
		}

		file_path = _directory_path;
		return true;
	}
	else {
		for (std::vector<CString>::const_iterator it = m_target_file_list.begin(); it != m_target_file_list.end(); ++it) {
			temp_path.m_strPath = _directory_path.m_strPath + *it;
			if (TRUE == temp_path.FileExists()) {
				file_path = temp_path;
				return true;
			}
		}

		return false;
	}
}

// =====================================================================================================================
// =======================================================================================================================
void SmartPathSearch::Reset()
{
	this->Stop();
	m_target_file_list.clear();
	m_pattern_list.clear();
}

// =====================================================================================================================
// =======================================================================================================================
int SmartPathSearch::GetDirectoryTableCount()
{
	//~~~~~~~~~~
	int count = 0;
	//~~~~~~~~~~

	for (std::map < int, std::queue < DIR_INFO >> ::const_iterator it = m_directory_table.begin();
		it != m_directory_table.end(); ++it) {
		count += it->second.size();
	}

	return count;
}

// =====================================================================================================================
// =======================================================================================================================
DIR_INFO::DIR_INFO(const ATL::CPath &_directory_path, int _score)
{
	ATLASSERT(!_directory_path.m_strPath.IsEmpty());
	m_path = _directory_path;
	m_score = _score;
}

// =====================================================================================================================
// =======================================================================================================================
int DIR_INFO::GetScore() const
{
	return m_score;
}

const ATL::CPath & DIR_INFO::GetPath() const
{
	return m_path;
}

// =====================================================================================================================
// =======================================================================================================================
PATTERN_INFO::PATTERN_INFO(const CString &_pattern_string, int _score) : m_string(_pattern_string), m_score(_score)
{
}

// =====================================================================================================================
// =======================================================================================================================
const CString &PATTERN_INFO::GetString() const
{
	return m_string;
}

// =====================================================================================================================
// =======================================================================================================================
int PATTERN_INFO::GetScore() const
{
	return m_score;
}
