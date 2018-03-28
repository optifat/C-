//
// Created by pavel on 3/28/18.
//

#ifndef UNTITLED6_THREADPOOL_H
#define UNTITLED6_THREADPOOL_H

#include "functional"
#include "thread"
#include "mutex"
#include "vector"
#include "queue"
#include "iostream"
#include "condition_variable"
#include "future"

class ThreadPool
{
public:
    using Task = std::function<void()>;
    explicit ThreadPool(std::size_t N)
    {
        run(N);
    }

    ~ThreadPool()
    {
        stop();
    }

    template<class T>
    auto enqueue(T task)->std::future<decltype(task())>
    {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mTasks.emplace([=] {
                (*wrapper)();
            });
        }
        mEventVar.notify_one();
        return wrapper -> get_future();
    }

private:
    bool stopping = false;
    std::queue<Task> mTasks;
    std::vector<std::thread> mThreads;
    std::condition_variable mEventVar;
    std::mutex mEventMutex;

    void run(std::size_t N)
    {
        for (auto i = 0u; i < N; ++i) {
            mThreads.emplace_back([=]{
                while(true){
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock{mEventMutex};

                        mEventVar.wait(lock, [=]{return stopping || !mTasks.empty();});

                        if(stopping && mTasks.empty())
                            break;
                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void stop(){
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            stopping = true;
        }
        mEventVar.notify_all();

        for(auto &thread : mThreads)
            thread.join();
    }
};

#endif //UNTITLED6_THREADPOOL_H
