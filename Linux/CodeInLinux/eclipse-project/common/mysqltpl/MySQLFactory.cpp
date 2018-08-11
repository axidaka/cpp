#include "MySQLFactory.h"
//#include "core/sox/logger.h"

namespace server {
namespace mysqldb {

/* MySQLFactory */
MySQLFactory::MySQLFactory() {}

MySQLFactory::~MySQLFactory() {}

void MySQLFactory::addSource(const std::string &name, const MySQLConfig &config) {
	std::pair<std::string, Source *> p;
	p.first = name;
	p.second = new Source;
	p.second->config = config;

	sources_.insert(p);
}

Connection *MySQLFactory::getConnection(const std::string &name) {
	std::map<std::string, Source *>::const_iterator it = sources_.find(name);
	if (it == sources_.end()) {
		return NULL;
	}

	Connection *conn = it->second->cache.get();
	if (conn == NULL) {
		conn = new Connection();
		it->second->cache.set(conn);
	}

	if (!conn->connected()) {
		//log(Error, "mysql db:%s not connect, try reconnect", name.c_str());

		try {
			conn->connect(
				it->second->config.user,
				it->second->config.passwd,
				it->second->config.database,
				it->second->config.host,
				it->second->config.port);

			if (it->second->config.charset.empty())
			{
				conn->setCharSet("utf8");
			}else
			{
				conn->setCharSet(it->second->config.charset);
			}
		} catch (Exception &e) {
			//log(Error, "connect mysql://%s:***@%s:%d/%s/%s failed: %s",
					// it->second->config.user.c_str(),
					// it->second->config.host.c_str(),
					// it->second->config.port,
					// it->second->config.database.c_str(),
					// it->second->config.charset.c_str(),
					// e.what());
			conn->disconnect();
			return NULL;
		}

	}

	return conn;
}

}	//mysqldb
}	//server
