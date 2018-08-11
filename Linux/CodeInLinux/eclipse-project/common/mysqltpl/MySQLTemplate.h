/*
 * Author:	longjiangyuan@chinaduo.com
 * Description:	C++ wrapper for mysqlclient
 * Change Log:
 * +------------+-------------------------------------------------------------------------------------+
 * | Date       | Description                                                                         |
 * +------------+-------------------------------------------------------------------------------------+
 * | 2011-11-18 | 1. First version.                                                                   |
 * +------------+-------------------------------------------------------------------------------------+
 * | 2011-12-18 | 1. Callback::onException() will be called, when try to fetch non-exists column.     |
 * +------------+-------------------------------------------------------------------------------------+
 * | 2012-01-04 | 1. Add log for sql preview.                                                         |
   |            | 2. Add detail log when error accur.                                                 |
 * |            | 3. Only reconnect when unrecoverable error accur(errcode<=2018).                    |
 * |            | 4. If error accur when execute non-transactional query, will reconnect and retry.   |
 * +--------------------------------------------------------------------------------------------------+
 */

#ifndef MYSQLLIB_TEMPLATE_H
#define MYSQLLIB_TEMPLATE_H

#include "MySQLFactory.h"
#include <vector>

namespace server {
namespace mysqldb {

struct Callback {
	virtual void onPreview(const std::string &sql) {}

	virtual void onResult(ResultSet &result) {}

	virtual void onException(const Exception &ex) {}
};	//Callback

class SQLTemplate {
public:
	SQLTemplate(): preview_(false) {}

	virtual ~SQLTemplate() {}

	bool execute(Callback *callback, const char *sql) {
		return execSQL(callback, sql, NULL);
	}

	template<typename Arg1>
	bool execute(Callback *callback, const char *sql, const Arg1 &a1) {
		std::vector<Parameter> param;
		param.push_back(Parameter(a1));

		return execSQL(callback, sql, &param);
	}

	template<typename Arg1, typename Arg2>
	bool execute(Callback *callback, const char *sql, const Arg1 &a1, const Arg2 &a2) {
		std::vector<Parameter> param;
		param.push_back(Parameter(a1));
		param.push_back(Parameter(a2));

		return execSQL(callback, sql, &param);
	}

	template<typename Arg1, typename Arg2, typename Arg3>
	bool execute(Callback *callback, const char *sql, const Arg1 &a1, const Arg2 &a2, const Arg3 &a3) {
		std::vector<Parameter> param;
		param.push_back(Parameter(a1));
		param.push_back(Parameter(a2));
		param.push_back(Parameter(a3));

		return execSQL(callback, sql, &param);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	bool execute(Callback *callback, const char *sql, const Arg1 &a1, const Arg2 &a2, const Arg3 &a3, const Arg4 &a4) {
		std::vector<Parameter> param;
		param.push_back(Parameter(a1));
		param.push_back(Parameter(a2));
		param.push_back(Parameter(a3));
		param.push_back(Parameter(a4));

		return execSQL(callback, sql, &param);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	bool execute(Callback *callback, const char *sql, const Arg1 &a1, const Arg2 &a2, const Arg3 &a3, const Arg4 &a4, const Arg5 &a5) {
		std::vector<Parameter> param;
		param.push_back(Parameter(a1));
		param.push_back(Parameter(a2));
		param.push_back(Parameter(a3));
		param.push_back(Parameter(a4));
		param.push_back(Parameter(a5));

		return execSQL(callback, sql, &param);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	bool execute(Callback *callback, const char *sql, const Arg1 &a1, const Arg2 &a2, const Arg3 &a3, const Arg4 &a4, const Arg5 &a5, const Arg6 &a6) {
		std::vector<Parameter> param;
		param.push_back(Parameter(a1));
		param.push_back(Parameter(a2));
		param.push_back(Parameter(a3));
		param.push_back(Parameter(a4));
		param.push_back(Parameter(a5));
		param.push_back(Parameter(a6));

		return execSQL(callback, sql, &param);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	bool execute(Callback *callback, const char *sql, const Arg1 &a1, const Arg2 &a2, const Arg3 &a3, const Arg4 &a4, const Arg5 &a5, const Arg6 &a6, const Arg7 &a7) {
		std::vector<Parameter> param;
		param.push_back(Parameter(a1));
		param.push_back(Parameter(a2));
		param.push_back(Parameter(a3));
		param.push_back(Parameter(a4));
		param.push_back(Parameter(a5));
		param.push_back(Parameter(a6));
		param.push_back(Parameter(a7));

		return execSQL(callback, sql, &param);
	}

	template<typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
    bool execute(Callback *callback, const char *sql, const Arg1 &a1, const Arg2 &a2, const Arg3 &a3, const Arg4 &a4, const Arg5 &a5, const Arg6 &a6, const Arg7 &a7, const Arg8 &a8) {
        std::vector<Parameter> param;
        param.push_back(Parameter(a1));
        param.push_back(Parameter(a2));
        param.push_back(Parameter(a3));
        param.push_back(Parameter(a4));
        param.push_back(Parameter(a5));
        param.push_back(Parameter(a6));
        param.push_back(Parameter(a7));
        param.push_back(Parameter(a8));

        return execSQL(callback, sql, &param);
    }

	virtual bool execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args) = 0;

	bool preview() { return preview_; }

	void setPreview(bool yes) { preview_ = yes; }

private:
	bool preview_;
};

class MySQLTransaction;

class MySQLTemplate: public SQLTemplate {
public:
	MySQLTemplate(MySQLFactory *f, const std::string &name): factory_(f), dbname_(name) {}

	virtual ~MySQLTemplate() {}

	MySQLTransaction beginTransaction();

	bool execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args);

private:
	MySQLFactory *factory_;
	std::string dbname_;
};	//MySQLTemplate

class MySQLTransaction: public SQLTemplate {
public:
	explicit MySQLTransaction(Connection *conn): conn_(conn) {}

	virtual ~MySQLTransaction() {}

	bool active() { return (conn_ != NULL && conn_->connected()); }

	bool begin();

	bool execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args);

	bool commit();

	void rollback();

private:
	Connection *conn_;
};	//MySQLTransaction



class MySQLTransactionExp: public SQLTemplate {
public:
        explicit MySQLTransactionExp(Connection *conn): conn_(conn) {assert(conn);}

        virtual ~MySQLTransactionExp() {}

        bool active() { return conn_ && conn_->connected(); }

        bool begin();

        bool execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args);

        bool commit();

        void rollback();

	void close();
private:
        Connection *conn_;
};      //MySQLTransactionExp

}
}
#endif
