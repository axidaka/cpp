#include "SimpleMySQLFactory.h"

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
	}
	catch (Exception &e) {
		//log(Error, "thread[%d] sql execute error, code:%d, reason:%s", gettid(), e.code(), e.what());
		if (callback)
			callback->onException(e);
		return e.code();
	}

	return 0;
}

Connection *TLSMySQLFactory::getConnection() {
	Connection* conn = cache.get();
	if (conn == NULL)
	{
		conn = new Connection;
	}

	if (!conn->connected())
	{
		try
		{
			conn->connect(config.user, config.passwd, config.database, config.host, config.port);
			if (config.charset.empty())
			{
				conn->setCharSet("utf8");
			}
			else
			{
				conn->setCharSet(config.charset);
			}

			conn->setAutoCommit(false);
			cache.set(conn);
		}
		catch (Exception&  e)
		{	
			printf("connect mysql://%s:***@%s:%d/%s/%s failed: %s",
				config.user.c_str(),
				config.host.c_str(),
				config.port,
				config.database.c_str(),
				config.charset.c_str(),
				e.what());
			conn->disconnect();
			delete conn;
			cache.set(NULL);
			return NULL;
		}
	}

	return conn;
}


void TLSMySQLFactory::terminate(Connection * c)
{
	Connection *conn = cache.get();
	if (conn){
		conn->disconnect();
		delete conn;
		cache.set(NULL);
	}
}

bool TemplateHelper::execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args)
{
	for (int i = 0; i < 2; i++)
	{
		Connection* conn = factory->getConnection();

		int err = executeSQL(callback, preview(), conn, sql, args);
		if (err == 0)
		{
			return true;
		}
		else if (err <= 2018)
		{
			factory->terminate(conn);
		}
		else
		{
			break;
		}
			
	}

	return false;
}

struct C : public Callback
{
	C(std::vector<int>& o) :x(o){}

	virtual void onResult(ResultSet &result)
	{
		while (result.next())
		{
			x.push_back(result.getInt(1));
		}
	}

	// 使用引用类型, 引用对象是函数输出out
	std::vector<int> &x;
};

int NodeMySQL::getNodes(std::vector<int> &out)
{
	TemplateHelper tpl(fac);
	
	C cb(out);
	
	bool res = tpl.execute(&cb, "select node from nodes;");
	return res ? 0 : -1;
}
