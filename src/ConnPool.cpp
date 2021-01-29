#include <stdexcept>  
#include <exception>  
#include <stdio.h>  
#include "ConnPool.h"  
   
using namespace std;  
using namespace sql;  
   
ConnPool *ConnPool::connPool = new ConnPool("tcp://127.0.0.1:3306", USER, PASSWORD, 10);   
   
//连接池的构造函数  
ConnPool::ConnPool(string url, string userName, string password, int maxSize) {  
    this->maxSize = maxSize;  
    this->curSize = 0;  
    this->username = userName;  
    this->password = password;  
    this->url = url;
	
    try {  
        this->driver = sql::mysql::get_driver_instance();  
    } catch (sql::SQLException&e) {  
        cout << "驱动连接出错" << endl;  
    } catch (std::runtime_error&e) {  
        cout << "运行出错了" << endl;
    } 
	
    this->InitConnection(maxSize / 2);  
}  
   
//获取连接池对象，单例模式  
ConnPool *ConnPool::GetInstance() {  
    return connPool;  
}  
   
//初始化连接池，创建最大连接数的一半连接数量  
bool ConnPool::InitConnection(int iInitialSize) {  
    Connection*conn;  
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
   
//创建连接,返回一个Connection  
Connection* ConnPool::CreateConnection() {  
    Connection*conn;  
    try {  
        conn = driver->connect(this->url, this->username, this->password); //建立连接  
		cout << "conn: " << conn << endl;
	
        return conn;  
    } catch (sql::SQLException&e) {  
		cout << "创建连接出错: " << e.what() <<endl;
        return NULL;  
    } catch (std::runtime_error&e) {  
        perror("运行时出错");  
        return NULL;  
    }  
}  
   
//在连接池中获得一个连接  
Connection* ConnPool::GetConnection() {  
    Connection *con;  
    pthread_mutex_lock(&lock);  
   
    if (connList.size() > 0) {   //连接池容器中还有连接  
        con = connList.front(); //得到第一个连接  
        connList.pop_front();   //移除第一个连接  
        if (con->isClosed()) {   //如果连接已经被关闭，删除后重新建立一个  
            delete con;  
            con = this->CreateConnection();  
        }  
        //如果连接为空，则创建连接出错  
        if (con == nullptr) {  
            --curSize;
        }  
        pthread_mutex_unlock(&lock);  
		
        return con;  
    } else {  
        if (curSize < maxSize) { //还可以创建新的连接  
            con = this->CreateConnection();  
            if (con) {  
                ++curSize;  
                pthread_mutex_unlock(&lock);  
                return con;  
            } else {  
                pthread_mutex_unlock(&lock);  
                return nullptr;  
            }  
        } else { //建立的连接数已经达到maxSize  
            pthread_mutex_unlock(&lock);  
            return nullptr;  
        }  
    }  
}  
   
//回收数据库连接  
void ConnPool::ReleaseConnection(sql::Connection * conn) {  
    if (conn) {  
        pthread_mutex_lock(&lock);  
        connList.push_back(conn);  
        pthread_mutex_unlock(&lock);  
    }  
}  
   
//连接池的析构函数  
ConnPool::~ConnPool() {  
    this->DestoryConnPool();  
}  
   
//销毁连接池,首先要先销毁连接池的中连接  
void ConnPool::DestoryConnPool() {  
    list<Connection*>::iterator icon;  
    pthread_mutex_lock(&lock);  
    for (icon = connList.begin(); icon != connList.end(); ++icon) {  
        this->DestoryConnection(*icon); //销毁连接池中的连接  
    }  
    curSize = 0;  
    connList.clear(); //清空连接池中的连接  
    pthread_mutex_unlock(&lock);  
}  
   
//销毁一个连接  
void ConnPool::DestoryConnection(Connection* conn) {  
    if (conn) {  
        try {  
            conn->close();  
        } catch (sql::SQLException&e) {  
            perror(e.what());  
        } catch (std::exception&e) {  
            perror(e.what());  
        }  
        delete conn;  
    }  
}  

