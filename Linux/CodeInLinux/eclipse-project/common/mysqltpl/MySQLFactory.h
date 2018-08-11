#ifndef MYSQL_FACTORY_H
#define MYSQL_FACTORY_H

#include "MySQLDriver.h"
#include <pthread.h>
#include <map>

template<typename T>
class ThreadLocal {
public:
	ThreadLocal() { 
		int ret = pthread_key_create(&key_, &ThreadLocal<T>::deleter);
		assert(ret == 0);
		set(NULL);
	}

	T *get() {
		return (T *) pthread_getspecific(key_);
	}
	
	void set(T *t) {
		assert(pthread_setspecific(key_, (void *) t) == 0);
	}
	
private:
	ThreadLocal(const ThreadLocal<T> &) {}

	static void deleter(void *arg) {
		T *t = (T *) arg;
		if (t != NULL) {
			delete t;
		}
	}

	pthread_key_t key_;
};

namespace server {
namespace mysqldb {

struct MySQLConfig {
	std::string host;
	unsigned short port;
	std::string user;
	std::string passwd;
	std::string database;
	std::string charset;
};

class MySQLFactory {
public:
	MySQLFactory();

	virtual ~MySQLFactory();

	void addSource(const std::string &name, const MySQLConfig &config);

	Connection *getConnection(const std::string &name);

private:
	struct Source {
		ThreadLocal<Connection> cache;
		MySQLConfig config;
	};

	std::map<std::string, Source *> sources_;
};

}
}
#endif
