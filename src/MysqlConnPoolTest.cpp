#include "connection_pool.h"  
   
namespace ConnectMySQL {  
   
//初始化连接池  
ConnPool *connpool = ConnPool::GetInstance();  
   
void run() {    
    Connection *con;  
    Statement *state;  
    ResultSet *result;  
   
    // 从连接池中获取mysql连接  
    con = connpool->GetConnection();  
   
    state = con->createStatement();  
    state->execute("use holy");  
   
    // 查询  
    result = state->executeQuery("select * from student where id < 1002");  
   
    // 输出查询  
    while (result->next()) {  
        int id = result->getInt("id");  
        string name = result->getString("name");  
        cout << id << " : " << name << endl;  
    }  
    delete state;  
    connpool->ReleaseConnection(con);  
}  
}  
   
int main(int argc, char* argv[]) {  
    ConnectMySQL::run();  
    return 0;  
}  

