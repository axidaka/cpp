#ifndef MYSQLLIB_DRIVER_H
#define MYSQLLIB_DRIVER_H

#include "mysql/mysql.h"
#include "mysql/errmsg.h"
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

namespace server {
namespace mysqldb {

class Column {
public:
	explicit Column(const char *d, unsigned int s): data_(d), size_(s) {}

	~Column() {}

	inline const char *data() const { return data_; }

	inline unsigned int size() const { return size_; }

	std::string toString() const { return std::string(data_, size_); }

	long long toInt() const { return atoll(data_); }

	bool null() const { return data_ == NULL; }

private:
	const char *data_;
	unsigned int size_;
};	//Column

class Exception {
public:
	explicit Exception(int code, const char *fmt, ...): code_(code) {
		va_list va;
		va_start(va, fmt);
		vsnprintf(msg_, sizeof(msg_), fmt, va);
		va_end(va);
	}

	explicit Exception(MYSQL *mysql) {
		code_ = mysql_errno(mysql);
		strcpy(msg_, mysql_error(mysql));
	}

	~Exception() {}

	inline int code() const { return code_; }

	inline const char *what() const { return msg_; }
private:
	int code_;
	char msg_[256];
};	//Exception

class ResultSet {
public:
	explicit ResultSet(MYSQL *mysql, MYSQL_RES *result);

	~ResultSet();

	inline uint32_t insertId() const { return mysql_insert_id(mysql_); }

	inline uint32_t affectedRows() const { return mysql_affected_rows(mysql_); };

	bool next();

	long long getInt(int index) const;

	long long getInt(const char *name) const;

	std::string getString(int index) const;

	std::string getString(const char *name) const;
	
	Column get(int index) const;

	Column get(const char *name) const;

private:
	MYSQL *mysql_;
	MYSQL_RES *result_;
	MYSQL_ROW row_;
	unsigned long *lengths_;
};	//ResultSet

class Statement;

class Connection {
public:
	friend class Statement;

	Connection();

	~Connection();

	void setAutoCommit(bool yes);

	void setCharSet(const std::string &name);

	void connect(const std::string &user, const std::string &passwd, const std::string &db, const std::string &host, unsigned short port);

	void reconnect();

	Statement createStatement();

	void begin();

	void rollback();

	void commit();

	void disconnect();

	bool connected() const;

private:
	std::string user_;
	std::string passwd_;
	std::string database_;
	std::string host_;
	unsigned short port_;
	MYSQL *mysql_;
};	//Connection

class Parameter {
	friend class Statement;

        enum ParamType {
                INTEGER		= 0,
		STRING		= 1,
		BLOB		= 2,
                INT_VECTOR	= 3,
        } type;

        union {
                int64_t integer;
		const char *string;
                const std::string *blob;
                const std::vector<int64_t> *int_vec;
        } data;

public:
	explicit Parameter(int i): type(INTEGER) { data.integer = (int64_t) i; }

        explicit Parameter(int64_t i): type(INTEGER) { data.integer = (int64_t) i; }

	explicit Parameter(const char *str) {
		type = STRING;
		data.string = str;
	}

        explicit Parameter(const std::string &blob) {
                type = BLOB;
                data.blob = &blob;
        }

        explicit Parameter(const std::vector<int64_t> &vec) {
                type = INT_VECTOR;
                data.int_vec = &vec;
        }
};      //Parameter

class Statement {
public:
	explicit Statement(MYSQL *mysql);

	~Statement();

	inline Statement &operator<<(int i) {
		char buf[12];
		int len = snprintf(buf, sizeof(buf), "%d", i);
		sql_.append(buf, len);
		return *this;
	}

	inline Statement &operator<<(long long i) {
		char buf[23];
		int len = snprintf(buf, sizeof(buf), "%lld", i);
		sql_.append(buf, len);
		return *this;
	}

	inline Statement &operator<<(const char *str) {
		sql_.append(str, strlen(str));
		return *this;
	}

	inline Statement &operator<<(const std::string &str) {
		sql_.append(str);
		return *this;
	}

	Statement &operator<<(const Parameter &param);

	std::string escape(const std::string &fr);

	void prepare(const std::string &sql);

	inline const std::string &preview() const {
		return sql_;
	}

	void bindParams(const std::vector<Parameter> &param);

	ResultSet execute();

private:
	std::string::size_type bindInt(std::string::size_type pos, const char *from, int64_t i);

	std::string::size_type bindString(std::string::size_type pos, const char *from, const char *data, size_t size);

	MYSQL *mysql_;
	std::string sql_;
};	//Statment

}	//mysqldb
}	//server
#endif	//MYSQL_SQL_H
