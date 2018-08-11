#include "MySQLDriver.h"
#include <pthread.h>

using namespace server::mysqldb;

/* ResultSet */
ResultSet::ResultSet(MYSQL *mysql, MYSQL_RES *result) : mysql_(mysql), result_(result), row_(NULL) {
}

ResultSet::~ResultSet() {
	if (result_ != NULL) {
		mysql_free_result(result_);
	}
}

Column ResultSet::get(int index) const {
	if (row_ == NULL)
		throw Exception(-1, "End of result set");

	unsigned int columns = mysql_num_fields(result_);
	if (index == 0 || index > (int)columns) {
		throw Exception(-1, "Invalid field index: %d", index);
	}
	return Column(row_[index - 1], lengths_[index - 1]);
}

Column ResultSet::get(const char *name) const {
	if (row_ == NULL)
		throw Exception(-1, "End of result set");

	MYSQL_FIELD *fields = mysql_fetch_fields(result_);
	unsigned int columns = mysql_num_fields(result_);

	for (unsigned int i = 0; i != columns; ++i) {
		if (strcmp(fields[i].name, name) == 0) {
			return Column(row_[i], lengths_[i]);
		}
	}

	throw Exception(-1, "Invalid field name: %s", name);
}

bool ResultSet::next() {
	if (result_ == NULL)
		return false;

	if ((row_ = mysql_fetch_row(result_)) != NULL) {
		lengths_ = mysql_fetch_lengths(result_);
		return true;
	} else {
		return false;
	}
}

long long ResultSet::getInt(int index) const {
	return get(index).toInt();
}

long long ResultSet::getInt(const char *name) const {
	return get(name).toInt();
}

std::string ResultSet::getString(int index) const {
	return get(index).toString();
}

std::string ResultSet::getString(const char *name) const {
	return get(name).toString();
}

/* Connection */
static pthread_once_t g_thread_once;
static MYSQL g_mysql;

static void init_mysql_driver() {
	assert(mysql_init(&g_mysql) != NULL);
}

Connection::Connection(): mysql_(NULL) {
	pthread_once(&g_thread_once, &init_mysql_driver);
}

Connection::~Connection() {
	disconnect();
}

void Connection::connect(const std::string &user, const std::string &passwd, const std::string &database, const std::string &host, unsigned short port) {
	if (mysql_ != NULL) {
		return;
	}

	user_ = user;
	passwd_ = passwd;
	database_ = database;
	host_ = host;
	port_ = port;

	/* 
	XXX:
	mysql_init() was not totally thread-safed, once it was called successfully, it becomes thread-safed.
	there are three ways to solve this problem:
		1. call mysql_init() before mysql_real_connect() in the same thread;
		2. call mysql_thread_init() before using mysql client APIs in different threads, and call mysql_thread_end() before exiting thread;
		3. call mysql_init() once to do the global initialize before using any mysql client APIs;
	*/
	mysql_ = mysql_init(NULL);
	assert(mysql_ != NULL);
	if (mysql_real_connect(mysql_, host.c_str(), user.c_str(), passwd.c_str(), database.c_str(), port, NULL, 0) != mysql_) {
		throw Exception(mysql_);
	}
}

void Connection::reconnect() {
	disconnect();
	connect(user_, passwd_, database_, host_, port_);
}

Statement Connection::createStatement() {
	return Statement(mysql_);
}

void Connection::disconnect() { 
	if (mysql_ != NULL) {
		mysql_close(mysql_);
		mysql_ = NULL;
	}
}

void Connection::begin() {
	if (mysql_query(mysql_, "begin") != 0) {
		throw Exception(mysql_);
	}
}

void Connection::commit() {
	if (mysql_commit(mysql_) != 0) {
		throw Exception(mysql_);
	}
}

void Connection::rollback() {
	if (mysql_rollback(mysql_) != 0) {
		throw Exception(mysql_);
	}
}

void Connection::setAutoCommit(bool b) {
	my_bool ret = mysql_autocommit(mysql_, b ? 1 : 0);

	if (ret != 0) {
		throw Exception(mysql_);
	}
}

void Connection::setCharSet(const std::string &name) {
//	if (mysql_set_character_set(mysql_, name.c_str()) != 0) {
//		throw Exception(mysql_);
//	}
	//std::string sql = "set names " + name;
	//return query(sql).status();
	std::string sql = "set names " + name;
	if(mysql_query(mysql_, sql.c_str()) != 0){
		throw Exception(mysql_);
	}
}

bool Connection::connected() const {
	return (mysql_ != NULL);
}

/* Statement */
Statement::Statement(MYSQL *mysql): mysql_(mysql) {
}

Statement::~Statement() {
}

Statement &Statement::operator<<(const Parameter &p) {
	switch (p.type) {
		case Parameter::INTEGER:
		{
			char buf[32];
			snprintf(buf, sizeof(buf), "%lld", (long long)p.data.integer);
			sql_.append(buf);
			break;
		}
		case Parameter::STRING:
		{
			sql_.append(escape(p.data.string));
			break;
		}
		case Parameter::BLOB:
		{
			sql_.append(escape(*(p.data.blob)));
			break;
		}
		default:
			break;
	}

	return *this;
}

void Statement::prepare(const std::string &sql) {
	sql_ = sql;
}

ResultSet Statement::execute() {
	if (mysql_real_query(mysql_, sql_.data(), sql_.size()) != 0) {
		if (mysql_errno(mysql_) != 0)
			throw Exception(mysql_);
		else 
			return ResultSet(mysql_, NULL);
	}

	if (mysql_errno(mysql_) != 0) {
		throw Exception(mysql_);
	}

	MYSQL_RES *result = mysql_store_result(mysql_);
	return ResultSet(mysql_, result);
}

static std::string join(const std::vector<int64_t> &vec, const char *c) {
	char buf[23];
	std::string out;

	for (std::vector<int64_t>::size_type i = 0, j = vec.size(); i < j; ++i) {
#if __WORDSIZE == 64
		int len = snprintf(buf, sizeof(buf), "%ld", vec[i]);
#else
		int len = snprintf(buf, sizeof(buf), "%lld", vec[i]);
#endif
		out.append(buf, len);
		if (i + 1 < vec.size())
			out.append(c);
	}

	return out;
}

static std::string::size_type replace(std::string &sql, const char *from, const std::string &to, std::string::size_type pos) {
	sql.erase(pos, strlen(from));
	sql.insert(pos, to);
	return pos + to.size();
}

std::string::size_type Statement::bindInt(std::string::size_type pos, const char *from, int64_t i) {
	std::string to;
	to.resize(23);
	
	char *buf = (char *) to.data();
#if __WORDSIZE == 64
	int len = snprintf(buf, 23, "%ld", i);
#else
	int len = snprintf(buf, 23, "%lld", i);
#endif
	to.resize(len);
	return replace(sql_, from, to, pos);
}

std::string Statement::escape(const std::string &fr) {
	std::string to;
	if (fr.empty()) {
		to.assign("''");
	} else {
		to.resize(fr.size() * 2 + 3);
		char *buf = (char *) to.data();
		buf[0] = '\'';
		int len = mysql_real_escape_string(mysql_, buf + 1, fr.data(), fr.size());
		buf[len + 1] = '\'';
		to.resize(len + 2);
	}

	return to;
}

std::string::size_type Statement::bindString(std::string::size_type pos, const char *from, const char *data, size_t size) {
	std::string to;

	if (size == 0) {
		to.assign("''");
	} else {      
		to.resize(size * 2 + 3);
		char *buf = (char *) to.data();
		buf[0] = '\'';
		int len = mysql_real_escape_string(mysql_, buf + 1, data, size);
		buf[len + 1] = '\'';
		to.resize(len + 2);
	}             
	return replace(sql_, from, to, pos);
}

void Statement::bindParams(const std::vector<Parameter> &param) {
	int idx;
	char from[4];
	std::string::size_type pos = 0;

	do {
		pos = sql_.find(":", pos);
		if (pos == std::string::npos)
			break;

		idx = atoi(sql_.c_str() + pos + 1);
		assert(idx != 0 && (int)param.size() >= idx);
		snprintf(from, sizeof(from), ":%d", idx);
		const Parameter &p = param[idx - 1];

		switch (p.type) {
			case Parameter::INTEGER:
			{
				pos = bindInt(pos, from, p.data.integer);
				break;
			}
			case Parameter::STRING:
			{
				pos = bindString(pos, from, p.data.string, strlen(p.data.string));
				break;
			}
			case Parameter::BLOB:
			{
				pos = bindString(pos, from, p.data.blob->data(), p.data.blob->size());
				break;
			}
			case Parameter::INT_VECTOR:
			{
				std::string vec = join(*(p.data.int_vec), ",");
				pos = replace(sql_, from, vec, pos);
				break;
			}
			default:
			{
				break;
			}
		}
	} while (pos < sql_.size());
}

