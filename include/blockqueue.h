#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include<deque>
#include <condition_variable>
#include <mutex>
#include <sys/time.h>
template<typename T>
class BlockQueue{
    public:
        explicit BlockQueue(size_t maxsize = 1000);
        ~BlockQueue();
        bool empty();
        bool full();
        void push_back();
        void push_front();
        bool pop(T& item);
        bool pop(T& item, int timeout);
        void clear();
        T front();
        T back();
        size_t capacity();
        size_t size();
        void flush();
        void close();
    private:
        deque<T> _deq;
        mutex _mtx;
        bool is_close;
        size_t _cap;
        std::condition_variable _cv_consumer;
        std::condition_variable _cv_producer;


};
#endif