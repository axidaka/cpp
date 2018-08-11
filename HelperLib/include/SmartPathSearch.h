#pragma once
#include <map>
#include <vector>
#include <queue>

#include <atlstr.h>
#include <atlpath.h>
#include <atlsync.h>

const int DIRECTORY_DEFAULT_SCORE = 10;
const int DIRECTORY_SUB_SCORE = 6;
const int PATTERN_DEFAULT_SCORE = 10;

class ISmartPathSearchAnswer
{
public:
	//·µ»ØtrueÍ£Ö¹ËÑË÷£¬·µ»Øfalse¼ÌÐøËÑË÷
	virtual bool OnSearchPath(const ATL::CPath &_dir, int _search_count) = NULL;
	//·µ»ØtrueÍ£Ö¹ËÑË÷£¬·µ»Øfalse¼ÌÐøËÑË÷
	virtual bool OnSearchFound(const ATL::CPath &_dir, const ATL::CPath &_path, int _search_count) = NULL;
	//ËÑË÷½áÊø
	virtual void OnSearchFinish(int _search_count) = NULL;
};

struct PATTERN_INFO
{
public:
	PATTERN_INFO(const CString &_pattern_string, int _score);

	const CString& GetString() const;
	int GetScore() const;

private:
	CString m_string;
	int m_score;
};

struct DIR_INFO
{
public:
	DIR_INFO(const ATL::CPath &_directory_path, int _score);
	const ATL::CPath& GetPath() const;
	int GetScore() const;

private:
	int m_score;
	ATL::CPath m_path;
};

class SmartPathSearch
{
public:
	SmartPathSearch(ISmartPathSearchAnswer* _answer);
	~SmartPathSearch(void);

	int AddTargetFile(const CString &_filename);
	int AddPatternString(const CString &_pattern, int _score = PATTERN_DEFAULT_SCORE);
	bool Start(bool _find_all_target = false);
	void Stop();
	void Reset();

private:
	friend UINT WINAPI SearchThread(SmartPathSearch *_this);

	int InitializeSearchDirectory();
	ATL::CPath& ExtractHighScoreDirectory(ATL::CPath &_dir_path);
	int CalcScore(const ATL::CPath &_directory_path);
	bool IsTargetDirectory(const ATL::CPath &_directory_path, ATL::CPath &file_path);
	int GetDirectoryTableCount();

private:
	std::vector<CString> m_target_file_list;
	std::vector<PATTERN_INFO> m_pattern_list;
	std::map<int, std::queue<DIR_INFO>> m_directory_table;
	HANDLE m_search_thread_handle;
	ISmartPathSearchAnswer *m_answer;
	bool m_find_all_target;
	ATL::CEvent m_exit_event;
};
