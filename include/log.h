

#ifndef LOG_H
#define LOG_H
#include <cstdio>
#include <memory>
#include <thread>
#include"blockqueue.h"
#include "buffer.h"
enum log_level{
    DEBUG = 0,
    INFO,
    WARN,
    ERROR
};
class Log{
    public:
        void init(log_level level, const char *path, const char *suffix, int max_queue_capacity);
        
        static Log* getInst();
        static void DoFlashLog_();
        void write(log_level level,const char* format,...);
        void flush();
        log_level GetLevel();
        void SetLevel(log_level level);
        bool IsOpen();
        
        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;
        
    private:
        Log();
        ~Log();
        void AppendLogLevelTitle(log_level level);
        void AsyncWrite_();
        static const int LOG_PATH_LEN = 256;
        static const int LOG_NAME_LEN = 256;
        static const int MAX_LINES = 50000;
        const char* path_;
        const char* suffix_;
        int MAX_LINES_;
        int lineCount;
        int toDay;
        bool isAsync_;  //whether async write
        FILE* fp_;
        std::thread writeThread_;
        std::unique_ptr<BlockQueue<std::string>> log_queue_;
        std::mutex mtx_;
        Buffer buff_;       // 输出的内容，缓冲区
        log_level level_;
        bool is_open_;
        
};

#define LOG_BASE(level, format, ...) \
    do {\
        Log* log = Log::getInst();\
        if (log->IsOpen() && log->GetLevel() <= level) {\
            log->write(level, format, ##__VA_ARGS__); \
            log->flush();\
        }\
    } while(0);

#define LOG_DEBUG(format, ...) do {LOG_BASE(log_level::DEBUG, format, ##__VA_ARGS__)} while(0);    
#define LOG_INFO(format, ...) do {LOG_BASE(log_level::INFO, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(log_level::WARN, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(log_level::ERROR, format, ##__VA_ARGS__)} while(0);

#endif // !LOG_H