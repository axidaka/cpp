#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include "MySQLDriver.h"
#include "MySQLFactory.h"
#include "MySQLTemplate.h"
#include "SimpleMySQLFactory.h"

using namespace std;
using namespace server::mysqldb;

int main()
{
	MySQLConfig sqlConfig;
	sqlConfig.host = "localhost";
	sqlConfig.port = 3306;
	sqlConfig.user = "root";
	sqlConfig.passwd = "root";
	sqlConfig.database = "mysql_test";
	sqlConfig.charset = "utf8";


	TLSMySQLFactory fac;
	fac.setConfig(sqlConfig);

	//NodeMySQL nodemysql;
	//nodemysql.setMySQLFactory(&fac);
	//std::vector<int> out;
	//nodemysql.getNodes(out);
	//for (int i = 0; i < out.size(); i ++)
	//{
	//	printf("%d\n", out[i]);
	//}


	struct GenID_cb : public Callback{
		GenID_cb(std::vector<int> &o) :ret(o){}
		virtual void onResult(ResultSet &result){
			while (result.next()){
				printf("NodeCb onResult: %lld\n", result.getInt(1));
				ret.push_back(result.getInt(1));
				count = ret.size();
			}
		}
		std::vector<int> &ret;
		int count;
	};

	// 使用事务
	Connection* conn = fac.getConnection();
	MySQLTransactionExp tx(conn);
	if (tx.active())
	{
		try
		{
			tx.begin();
			
			Callback insertcb;
			if (tx.execute(&insertcb, "insert into gen_id values(3), (4), (5);"))
			{
				tx.commit();
			}
			else
			{
				tx.rollback();
			}

			std::vector<int> ret;
			GenID_cb cb(ret); 
			if (tx.execute(&cb, "select * from gen_id;"))
			{
				tx.commit();
			}
			else
			{
				tx.rollback();
			}
		}
		catch (Exception &e)
		{
			printf("sql execute error: code:%d, reason:%s", e.code(), e.what());
			try{
				tx.rollback();
			}
			catch (Exception &ex){
				printf("sql rollback error: code:%d reaseon:%s", e.code(), e.what());
				fac.terminate(conn);
			}
		}


	}

	//// MySQLFactory 内部实现了mysql的连接
	//MySQLFactory fac;
	//fac.addSource("mysql_test", sqlConfig);

	//MySQLTemplate mysqltp(&fac, "mysql_test");


	//struct db : public Callback{
	//	virtual void onResult(ResultSet &result){
	//		while (result.next()){
	//			std::string path = result.getString(1);
	//			//int64_t v = result.getInt(2);
	//			printf("%s \n", path.c_str());

	//		}
	//	}
	//} xx;

	//bool init = mysqltp.execute(&xx, "show databases;");
	//if (!init){
	//	printf("init error exit\n");
	//}


	//MYSQL mysql;
	//mysql_init(&mysql);
	//mysql_real_connect(&mysql, sqlConfig.host.c_str(), sqlConfig.user.c_str(), sqlConfig.passwd.c_str(), 
	//	sqlConfig.database.c_str(), sqlConfig.port, NULL, 0);
	
	// 插入数据
	//string insert_sql = "insert into gen_id(id) values(1);";

	//创建存储过程
	//mysql > delimiter //
	//	mysql > create procedure p01()
	//	->begin
	//	->insert into gen_id(id) values(2);
	//  ->end;
	//  -> //
	//	Query OK, 0 rows affected(0.02 sec)
	//string sql = "call p01()";

	//string sql = "select * from gen_id;";
	//mysql_query(&mysql, sql.c_str());

	//MYSQL_RES* result = NULL;
	//result = mysql_store_result(&mysql);
	//int rowCount = mysql_num_rows(result);
	//int fieldcount = mysql_num_fields(result);

	//MYSQL_FIELD* field = NULL;
	//for (int i = 0; i < fieldcount; i ++)
	//{
	//	field = mysql_fetch_field_direct(result, i);
	//	printf("%s \t\t", field->name);
	//}

	//printf("\n");
	//MYSQL_ROW row = NULL;
	//row = mysql_fetch_row(result);
	//while (NULL != row)
	//{
	//	for (int i = 0; i < fieldcount; i ++)
	//	{
	//		cout << row[i] << "\t\t";
	//		//printf("%s \t\t", row[i]);
	//	}
	//	printf("\n");
	//	row = mysql_fetch_row(result);
	//}

	//mysql_close(&mysql);

	return 0;
}
