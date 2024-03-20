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
        void push_back(T&& item);
        void push_front(T&& item);
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
        std::deque<T> _deq;
        std::mutex _mtx;
        bool is_close;
        size_t _cap;
        std::condition_variable _cv_consumer;
        std::condition_variable _cv_producer;

};

template <typename T>
inline BlockQueue<T>::BlockQueue(size_t maxsize):_cap(maxsize)
{
    is_close = false;
}

template <typename T>
BlockQueue<T>::~BlockQueue()
{
    close();
}

template <typename T>
inline void BlockQueue<T>::clear()
{
    std::lock_guard<std::mutex> lock(_mtx);
    _deq.clear();
}

template <typename T>
void BlockQueue<T>::close()
{
    clear();
    is_close = true;
    _cv_consumer.notify_all();
    _cv_producer.notify_all();
}

template <typename T>
bool BlockQueue<T>::empty()
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.empty();
}

template <typename T>
bool BlockQueue<T>::full()
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.size() >= _cap;
}

template <typename T>
void BlockQueue<T>::push_back(T&& item)
{
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.size() == _cap)
    {
        _cv_producer.wait(lock);  //wait for consumer use the element
    }
    _deq.push_back(item);
    _cv_consumer.notify_one();
}


template <typename T>
void BlockQueue<T>::push_front(T&& item)
{
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.size() == _cap)
    {
        _cv_producer.wait(lock);  //wait for consumer use the element
    }
    _deq.push_front(item);
    _cv_consumer.notify_one();
}

template <typename T>
bool BlockQueue<T>::pop(T& item)
{
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.empty() && !is_close)
    {
        _cv_consumer.wait(lock);
    }
    if(is_close)
    {
        return false;
    }
    item = _deq.front();
    _deq.pop_front();
    _cv_producer.notify_one();
    return true;
}

template <typename T>
bool BlockQueue<T>::pop(T& item, int timeout)
{
    std::unique_lock<std::mutex> lock(_mtx);
    while(_deq.empty() && !is_close)
    {
        if(_cv_consumer.wait_for(lock, std::chrono::seconds(timeout)) == std::cv_status::timeout)
        {
            return false;
        }
    }
    if(is_close)
    {
        return false;
    }
    item = _deq.front();
    _deq.pop_front();
    _cv_producer.notify_one();
    return true;
}

template <typename T>
T BlockQueue<T>::front()
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.front();
}

template <typename T>
T BlockQueue<T>::back()
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.back();
}

template <typename T>
size_t BlockQueue<T>::capacity()
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _cap;
}

template <typename T>
size_t BlockQueue<T>::size()
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _deq.size();
}

template <typename T>
void BlockQueue<T>::flush()
{
    _cv_consumer.notify_one();
}
#endif
