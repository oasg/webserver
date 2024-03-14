#include "log.h"
#include <ctime>
#include <sys/stat.h>
#include <cassert>
#include <cstdarg>

void Log::init(log_level level, const char *path, const char *suffix, int max_queue_capacity)
{
    level_ = level;
    path_ = path;
    suffix_ = suffix;
    lineCount = 0;
    if(max_queue_capacity>1){
        isAsync_ = true;
        log_queue_ = std::make_unique<BlockQueue<std::string>>(max_queue_capacity);  
        writeThread_ = std::thread(DoFlashLog_);

    }else{
        isAsync_ = false;
    }
    time_t timer = time(NULL);
    struct tm* systime = localtime(&timer);
    char filename[LOG_PATH_LEN] = {0};
    snprintf(filename,LOG_PATH_LEN-1,"%s/%04d_%02d_%02d%s",
        path_,
        systime->tm_year + 1900,
        systime->tm_mon + 1,
        systime->tm_mday,
        suffix_);
    toDay = systime->tm_mday;
    {
        std::lock_guard<std::mutex> locker(mtx_);
        fp_ = fopen(filename,"a");
        if(fp_ == nullptr){
            mkdir(path_,0777);
            fp_ = fopen(filename,"a");
        }
        assert(fp_ != nullptr);
    }

}
Log *Log::getInst()
{
    static Log log;
    return &log;
}
void Log::write(log_level level, const char *format, ...)
{
    struct timeval now = {0,0};
    gettimeofday(&now,NULL);
    time_t toSec = now.tv_sec;
    struct tm* sysTime = localtime(&toSec);
    struct tm t = *sysTime;
    va_list vaList;
    if(toDay!=t.tm_mday||(lineCount && (lineCount & MAX_LINES == 0))){
        std::unique_lock<std::mutex> locker(mtx_);
        locker.unlock();
        char newFilename[LOG_PATH_LEN];
        char tail[36] = {0};
        snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
        if(toDay!=t.tm_mday){ //new day
            snprintf(newFilename, LOG_PATH_LEN - 72, "%s/%s%s", path_, tail, suffix_);
            toDay = t.tm_mday;
            lineCount = 0;
        }else{  //same day
            snprintf(newFilename, LOG_PATH_LEN - 72, "%s/%s-%d%s", path_, tail, (lineCount / MAX_LINES), suffix_);
        }
        locker.lock();
        flush();
        fclose(fp_);
        fp_ = fopen(newFilename, "a");  //re create a file and open
        assert(fp_ != nullptr);
    }
    {
        std::unique_lock<std::mutex> locker(mtx_);
        lineCount++;
        int32_t pos = ftell(fp_);
        int n = snprintf(buff_.BeginWrite(),128,"[%d-%02d-%02d %02d:%02d:%02d.%06ld]",
            t.tm_year + 1900,
            t.tm_mon + 1,
            t.tm_mday,
            t.tm_hour,
            t.tm_min,
            t.tm_sec,
            now.tv_usec);
        buff_.HasWritten(n);
        AppendLogLevelTitle(level);
        va_start(vaList,format);
        int m = vsnprintf(buff_.BeginWrite(),buff_.WritableBytes(),format,vaList);
        va_end(vaList);
        buff_.HasWritten(m);
        buff_.Append("\n\0",2);  //next line
        if(isAsync_ && !log_queue_->full()){
            log_queue_->push_back(buff_.RetrieveAllToStr());
        }else{
            fputs(buff_.Peek(),fp_);
        }
        buff_.RetrieveAll(); //clear buffer
    }
}

log_level Log::GetLevel()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return level_;
}

void Log::SetLevel(log_level level)
{
    std::lock_guard<std::mutex> locker(mtx_);
    level_ = level;
}

Log::~Log()
{
    while(!log_queue_->empty()){
        log_queue_->flush();
    }
    log_queue_->close();
    writeThread_ . join();
    if(fp_){
        std::lock_guard<std::mutex> locker(mtx_);
        flush();
        fclose(fp_);
    }

}

void Log::AppendLogLevelTitle(log_level level)
{
    switch (level)
    {
    case 0:
        buff_.Append("[debug]:", 9);
        break;
    case 1:
        buff_.Append("[info]:", 8);
        break;
    case 2:
        buff_.Append("[warn]:", 8);
        break;
    case 3:
        buff_.Append("[error]:", 9);
        break;
    default:
        buff_.Append("[info]:", 8);
        break;
    }
}

void Log::DoFlashLog_()
{
}

void Log::AsyncWrite_()
{
    std::string str;
    while(log_queue_->pop(str)){
        std::lock_guard<std::mutex> locker(mtx_);
        fputs(str.c_str(),fp_);
    }
}
