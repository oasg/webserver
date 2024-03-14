#include"log.h"
#include"singleton.h"
void TestLog()
{
    auto log = Singleton<Log>::GetInst();
    log->write(log_level::DEBUG,"this is a debug log");
    log->write(log_level::INFO,"this is a info log");
    log->write(log_level::WARN,"this is a warn log");
    log->write(log_level::ERROR,"this is a error log");
}