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
<<<<<<< HEAD
        void push_back();
        void push_front();
=======
        void push_back(T&& item);
        void push_front(T&& item);
>>>>>>> 994470e (Add log component)
        bool pop(T& item);
        bool pop(T& item, int timeout);
        void clear();
        T front();
        T back();
        size_t capacity();
        size_t size();
        void flush();
        void close();
<<<<<<< HEAD
    private:
        deque<T> _deq;
        mutex _mtx;
=======
        
    private:
        std::deque<T> _deq;
        std::mutex _mtx;
>>>>>>> 994470e (Add log component)
        bool is_close;
        size_t _cap;
        std::condition_variable _cv_consumer;
        std::condition_variable _cv_producer;
<<<<<<< HEAD


};
#endif
=======
};
#endif

>>>>>>> 994470e (Add log component)
