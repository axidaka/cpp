#include "MySQLTemplate.h"
//#include "core/sox/logger.h"
#include <syscall.h>

namespace server {
namespace mysqldb {

static int gettid() {
	return (int) syscall(SYS_gettid);
}

static int executeSQL(Callback *callback, bool preview, Connection *conn, const char *sql, const std::vector<Parameter> *param) {
	if (conn == NULL) {
		//log(Error, "thread[%d] get sql connection failed", gettid());
		if (callback) {
			callback->onException(Exception(-1, "get connection failed"));
		}
		return false;
	}

	Statement stmt = conn->createStatement();
	stmt.prepare(sql);

	if (param != NULL) {
		stmt.bindParams(*param);
	}
	if (preview) {
		//log(Debug, "thread[%d] %s", gettid(), stmt.preview().c_str());
		if (callback) {
			callback->onPreview(stmt.preview());
		}
	}

	try {
		ResultSet result = stmt.execute();
		if (callback) {
			callback->onResult(result);
		}
	} catch (Exception &e) {
		//log(Error, "thread[%d] sql execute error, code:%d, reason:%s", gettid(), e.code(), e.what());
		if (callback)
			callback->onException(e);
		return e.code();
	}

	return 0;
}

/* MySQLTemplate */
MySQLTransaction MySQLTemplate::beginTransaction() {
	MySQLTransaction tx(factory_->getConnection(dbname_));
	tx.setPreview(preview());
	tx.begin();
	return tx;
}

bool MySQLTemplate::execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *param) {
	static int max_reconnect = 2;

	for (int i = 0; i < max_reconnect; ++i) {
		Connection *conn = factory_->getConnection(dbname_);

		int err = executeSQL(callback, preview(), conn, sql, param);
		if (err == 0) {
			return true;
		} else if (err <= 2018) {
			//Fatal error, unrecoverable
			conn->disconnect();
		} else {
			break;
		}
	}

	return false;
}

/* MySQLTransaction */
bool MySQLTransaction::begin() {
	if (conn_ == NULL)
		return false;

	try {
		conn_->begin();
	} catch (Exception &e) {
		//log(Error, "thread[%d] begin transaction failed: %s", gettid(), e.what());
		if (e.code() <= 2018)
			conn_->disconnect();
		return false;
	}

	if (preview()) {
		//log(Debug, "thread[%d] begin", gettid());
	}
	return true;
}

bool MySQLTransaction::execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args) {
	int err = executeSQL(callback, preview(), conn_, sql, args);
	if (err == 0) {
		return true;
	} else if (err <= 2018) {
		//Fatal error, unrecoverable
		conn_->disconnect();
	}

	conn_ = NULL;
	return false;
}


bool MySQLTransaction::commit() {
	if (conn_ == NULL)
		return false;
	try {
		conn_->commit();
	} catch (Exception &e) {
		//log(Error, "thread[%d] commit failed: %s", gettid(), e.what());
		if (e.code() <= 2018)
			conn_->disconnect();
		return false;
	}

	if (preview()) {
		//log(Debug, "thread[%d] commit", gettid());
	}
	return true;
}

void MySQLTransaction::rollback() {
	if (conn_ == NULL)
		return;
	conn_->rollback();
}

bool MySQLTransactionExp::begin() {
        conn_->begin();

        if (preview()) {
                //log(Debug, "thread[%d] begin", gettid());
        }
        return true;
}

static int executeSQLExp(Callback *callback, bool preview, Connection *conn, const char *sql, const std::vector<Parameter> *param) {
        Statement stmt = conn->createStatement();
        stmt.prepare(sql);

        if (param != NULL) {
                stmt.bindParams(*param);
        }
        if (preview) {
                //log(Debug, "thread[%d] %s", gettid(), stmt.preview().c_str());
                if (callback) {
                        callback->onPreview(stmt.preview());
                }
        }

	ResultSet result = stmt.execute();
	if (callback) {
		callback->onResult(result);
	}

	return 0;
}

bool MySQLTransactionExp::execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args) {
	int err = executeSQLExp(callback, preview(), conn_, sql, args);
	if (err == 0) {
		return true;
	} 
	return false;
}


bool MySQLTransactionExp::commit() {
	conn_->commit();

	if (preview()) {
		//log(Debug, "thread[%d] commit", gettid());
	}
	return true;
}

void MySQLTransactionExp::rollback() {
	conn_->rollback();
}

void MySQLTransactionExp::close(){
	conn_->disconnect();
	conn_ = NULL;
}
}	//mysqldb
}	//server
