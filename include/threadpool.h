#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<thread>
#include<vector>
#include<queue>
#include<functional>
#include<mutex>
#include<future>
#include <condition_variable>


class ThreadPool{
    public:
        ThreadPool(int nthreads){
            for(int i = 0;i<nthreads;i++){
                _threads.emplace_back([this](){
                    while(true){
                        std::unique_lock lock(_mutex);
                        _condition.wait(lock,[this](){return !_tasks.empty();});
                        auto task = std::move(_tasks.front());
                        _tasks.pop();
                        lock.unlock();
                        task();
                    }
                });
            }
        }
        template<typename F,typename... Args>
        auto submit(F&& f,Args&&... args)->std::future<decltype(f(args...))>{
            auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(
                std::bind(std::forward<F>(f),std::forward<Args>(args)...);
            )
            std::future<decltype(f(args...))> res = task->get_future();
            std::unique_lock lock(_mutex);
            _tasks.emplace([task](){(*tasks)()});
            lock.unlock();
            _condition.notify_all();
            return res;
        }
        ~ThreadPool(){
            for(int i = 0;i<_threads.size();i++){
                submit([](){});
            }
            for(auto& thread:_threads){
                thread.join();
            }
        }
    private:
        std::vector<std::thread> _threads;
        std::queue<std::function<void()>> _tasks;
        std::mutex _mutex;
        std::condition_variable _condition;

};

#endif