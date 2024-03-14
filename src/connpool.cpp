#include "connpool.h"
#include <cassert>
#include "log.h"
SqlConnPool::SqlConnPool(const char *host, int port, const char *user, const char *pwd, const char *dbName, int connSize)
{
    assert(connSize>0);
    for(int i = 0;i<connSize;i++){
        MYSQL* conn = nullptr;
        conn = mysql_init(conn);
        if(!conn){
            LOG_ERROR("mysql init error");
            assert(conn);
        }
        conn = mysql_real_connect(conn,host,user,pwd,dbName,port,nullptr,0);
        if(!conn){
            LOG_ERROR("mysql connect error");
            assert(conn);
        }
        mysql_set_character_set(conn,"utf8");
        _connQue.emplace(conn);
    }
    maxConn = connSize;
    sem_init(&semId_,0,connSize);
}

SqlConnPool::~SqlConnPool()
{
    std::lock_guard<std::mutex> locker(_lock);
    while(!_connQue.empty()){
        MYSQL* conn = _connQue.front();
        _connQue.pop();
        mysql_close(conn);
    }
    mysql_library_end();
    sem_destroy(&semId_);
}

MYSQL *SqlConnPool::getConn()
{
    MYSQL* conn = nullptr;
    if(_connQue.empty()){
        LOG_WARN("SqlConnPool busy");
        return nullptr;
    }
    sem_wait(&semId_);
    std::lock_guard<std::mutex> locker(_lock);
    conn = _connQue.front();
    _connQue.pop();
    return conn;
}

void SqlConnPool::freeConn(MYSQL *conn)
{
    assert(conn);
    std::lock_guard<std::mutex> locker(_lock);
    _connQue.push(conn);
    sem_post(&semId_); //add one free resource
}



SqlConn::SqlConn(MYSQL **sql, std::shared_ptr<SqlConnPool> connpool):_connPool(connpool)
{
    _sql = connpool->getConn();
}
SqlConn::~SqlConn()
{
    _connPool->freeConn(_sql);
}
