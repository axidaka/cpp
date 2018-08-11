#ifndef SIMPLE_MYSQL_FACTORY_H
#define SIMPLE_MYSQL_FACTORY_H

#include <vector>

#include "MySQLFactory.h"
#include "MySQLTemplate.h"

using namespace server::mysqldb;

// ʹ���ֲ߳̾��洢(TLS)������MYSQL���Ӷ���
class TLSMySQLFactory
{
public:

	TLSMySQLFactory(){};
	~TLSMySQLFactory(){};

	void setConfig(const MySQLConfig &c){
		this->config = c;
	}

	Connection* getConnection();
	void terminate(Connection* c);

private:
	
	ThreadLocal<Connection> cache;
	MySQLConfig config;

};


class TemplateHelper : public SQLTemplate
{
public:
	TemplateHelper(TLSMySQLFactory *c) :factory(c){}

	virtual bool execSQL(Callback *callback, const char *sql, const std::vector<Parameter> *args);

private:
	TLSMySQLFactory* factory;
};

class NodeMySQL
{
public:
	int getNodes(std::vector<int> &out);
	void setMySQLFactory(TLSMySQLFactory* tlsfac){
		this->fac = tlsfac;
	}
private:
	TLSMySQLFactory *fac;
};


#endif