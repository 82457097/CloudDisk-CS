#pragma once
#include <mysql_connection.h>  
#include <mysql_driver.h>  
#include <cppconn/exception.h>  
#include <cppconn/driver.h>  
#include <cppconn/connection.h>  
#include <cppconn/resultset.h>  
#include <cppconn/prepared_statement.h>  
#include <cppconn/statement.h>  
#include <pthread.h>  
#include <list>
#include "logger.h"
   
using namespace std;  
using namespace sql;  

#define	USER			"root"
#define	PASSWORD		"qwj19961202"
#define	DB_NAME			"filedata"
#define	TABLE_NAME		"data"
#define	SQL_LEN			2048

class ConnPool {  
private:  
    int curSize; 		//当前已建立的数据库连接数量  
    int maxSize; 		//连接池中定义的最大数据库连接数  
    string username;  
    string password;  
    string url;  
    list<Connection*> connList; 	//连接池的容器队列  STL list 双向链表
    pthread_mutex_t lock; 			//线程锁  
    static ConnPool* connPool;  
    Driver* driver;  

	ConnPool(string url, string user, string password, int maxSize); //构造方法 
	
    Connection* CreateConnection(); 					//创建一个连接  
    
    bool InitConnection(int iInitialSize); 				//初始化数据库连接池  
    
    void DestoryConnection(Connection* conn); 			//销毁数据库连接对象    
    
public:  
    ~ConnPool();  
    Connection* GetConnection(); 						//获得数据库连接  
    
    void ReleaseConnection(Connection* conn); 			//将数据库连接放回到连接池的容器中  
    
    static ConnPool* GetInstance(); 					//获取数据库连接池对象 
    
    void DestoryConnPool(); 							//销毁数据库连接池
};  

