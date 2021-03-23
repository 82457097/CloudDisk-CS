#include <stdexcept>  
#include <exception>  
#include <stdio.h>  
#include "MysqlConnPool.h"  
   
using namespace std;  
using namespace sql;  
   
ConnPool* ConnPool::connPool = new ConnPool("tcp://127.0.0.1:3306", USER, PASSWORD, 10);   
   
ConnPool::ConnPool(string url, string userName, string password, int maxSize) {  
    this->maxSize = maxSize;  
    this->curSize = 0;  
    this->username = userName;  
    this->password = password;  
    this->url = url;
	
    try {  
        this->driver = sql::mysql::get_driver_instance();  
    } catch (sql::SQLException&e) {  
        cout << "驱动连接出错：" << e.what() << endl;  
    } catch (std::runtime_error&e) {  
        cout << "运行出错了" << e.what() << endl;
    } 
	
    this->InitConnection(maxSize / 2);  
}  
    
ConnPool* ConnPool::GetInstance() {  
    return connPool;  
}  
    
bool ConnPool::InitConnection(int iInitialSize) {  
    Connection* conn;  
    pthread_mutex_lock(&lock);  
    for (int i = 0; i < iInitialSize; i++) {  
        conn = this->CreateConnection();  
        if (conn) {  
            connList.push_back(conn);  
            ++(this->curSize);  
        } else {  
            cout << "create connection failed!" << endl;
			return false;
        }  
    }  
    pthread_mutex_unlock(&lock);  

	return true;
}  
     
Connection* ConnPool::CreateConnection() {  
    Connection* conn;  
    try {  
        conn = driver->connect(this->url, this->username, this->password); //建立连接  
        return conn;  
    } catch (sql::SQLException&e) {  
		cout << "创建连接出错: " << e.what() <<endl;
        return nullptr;  
    } catch (std::runtime_error&e) {  
        cout << "运行时出错: " << e.what() << endl; 
        return nullptr;  
    }  
}  
   
Connection* ConnPool::GetConnection() {  
    Connection* con;  
    pthread_mutex_lock(&lock);  
   
    if (connList.size() > 0) {   //连接池容器中还有连接  
        con = connList.front(); //得到第一个连接  
        connList.pop_front();   //移除第一个连接  
        if (con->isClosed()) {   //如果连接已经被关闭，删除后重新建立一个  
            delete con;  
            con = this->CreateConnection();  
        }  
 
        if (con == nullptr) {  
            --curSize;
        }  
        pthread_mutex_unlock(&lock);  
		
        return con;  
    } else {  
        if (curSize < maxSize) {   
            con = this->CreateConnection();  
            if (con) {  
                ++curSize;  
                pthread_mutex_unlock(&lock);  
                return con;  
            } else {  
                pthread_mutex_unlock(&lock);  
                return nullptr;  
            }  
        } else {  
            pthread_mutex_unlock(&lock);  
            return nullptr;  
        }  
    }  
}  
   
void ConnPool::ReleaseConnection(sql::Connection * conn) {  
    if (conn) {  
        pthread_mutex_lock(&lock);  
        connList.push_back(conn);  
        pthread_mutex_unlock(&lock);  
    }  
}  
    
ConnPool::~ConnPool() {  
    this->DestoryConnPool();  
}  
    
void ConnPool::DestoryConnPool() {  
    list<Connection*>::iterator icon;  
    pthread_mutex_lock(&lock);  
    for (icon = connList.begin(); icon != connList.end(); ++icon) {  
        this->DestoryConnection(*icon);  
    }  
    curSize = 0;  
    connList.clear();
    pthread_mutex_unlock(&lock);  

	LOG("connection pool destory success.");
}  
    
void ConnPool::DestoryConnection(Connection* conn) {  
    if (conn) {  
        try {  
            conn->close();  
        } catch (sql::SQLException&e) {  
            cout << e.what() << endl;
        } catch (std::exception&e) {  
            cout << e.what() << endl;  
        }  
        delete conn;  
    }  
}  

