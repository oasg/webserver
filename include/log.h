

#ifndef LOG_H
#define LOG_H
#include <cstdio>
#include <memory>
<<<<<<< HEAD
=======
#include <thread>
#include"blockqueue.h"
#include "buffer.h"
>>>>>>> 994470e (Add log component)
enum log_level{
    DEBUG = 1,
    INFO,
    WARN,
    ERROR
};
<<<<<<< HEAD
class Log{
    public:
        void init(log_level level,const char* path = "./log",
            const char* suffix = ".log",
            int max_queue_capacity = 1024
        );
        static Log* getInstance();
        static void FlushLogThread();

        void write(log_level level,const char* format,...);
        void flush();
        int GetLevel();
        void SetLevel(int level);
        bool IsOpen(){ };
        Log() = delete;
        ~Log() = delete;
        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;

    private:
        void AsyncWrite_();
    private:
=======


class Log{
    public:
        void init(log_level level, const char *path, const char *suffix, int max_queue_capacity);
        
        static Log* getInst();
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
        void DoFlashLog_();
        void AsyncWrite_();
>>>>>>> 994470e (Add log component)
        static const int LOG_PATH_LEN = 256;
        static const int LOG_NAME_LEN = 256;
        static const int MAX_LINES = 50000;
        const char* path_;
        const char* suffix_;
        int MAX_LINES_;
        int lineCount;
        int toDay;
        log_level level_;
<<<<<<< HEAD
        bool isAsync_;
        FILE* fp_;
        




};
#define LOG_BASE(level, format, ...) \
    do {\
        Log* log = Log::Instance();\
=======
        bool isAsync_;  //whether async write
        FILE* fp_;
        std::thread writeThread_;
        std::unique_ptr<BlockQueue<std::string>> log_queue_;
        std::mutex mtx_;
        Buffer buff_;       // 输出的内容，缓冲区
        
};

#define LOG_BASE(level, format, ...) \
    do {\
        Log* log = Log::GetInst();\
>>>>>>> 994470e (Add log component)
        if (log->IsOpen() && log->GetLevel() <= level) {\
            log->write(level, format, ##__VA_ARGS__); \
            log->flush();\
        }\
    } while(0);

#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);    
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);
#endif // !LOG_H