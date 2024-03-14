#ifndef CONNPOOL_H
#define CONNPOOL_H
#include<mysql/mysql.h>
#include<queue>
#include<mutex>
#include <semaphore.h>
#include <memory>
class SqlConnPool{
public:
    explicit SqlConnPool(const char* host, int port,
              const char* user,const char* pwd, 
              const char* dbName, int connSize);
    ~SqlConnPool();
    MYSQL* getConn();
    void freeConn(MYSQL* conn);
    int getFreeConnCount();
private:
int _maxConn;
std::queue<MYSQL*> _connQue;
std::mutex _lock;
sem_t semId_;
};

class SqlConn{
    public:
        explicit SqlConn(MYSQL** sql,std::shared_ptr<SqlConnPool> connpool);
        ~SqlConn();
    private:
        MYSQL* _sql;
        std::shared_ptr<SqlConnPool> _connPool;
};
#endif