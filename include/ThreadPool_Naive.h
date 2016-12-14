#ifndef CPP_THREAD_THREADPOOL_NAIVE_H
#define CPP_THREAD_THREADPOOL_NAIVE_H

#include <memory>
#include <thread>
#include <condition_variable>

namespace Naive {

template<typename Task, typename... Args>
std::function<void(Args &&...args)> ThreadPoolTask(
        Task &&task,
        std::condition_variable &conditionVariable,
        std::mutex &mutex,
        int &freeSlots)
{
    return [&](Args &&...args) {
        {
            std::unique_lock<std::mutex> aFreeSlotsLock(mutex);
            conditionVariable.wait(aFreeSlotsLock, [&]() { return freeSlots > 0; });
            freeSlots--;
        }
        std::thread{std::forward<Task>(task), std::forward<Args>(args)...}.join();
        {
            std::unique_lock<std::mutex> aFreeSlotsLock(mutex);
            freeSlots++;
        }
        conditionVariable.notify_one();
    };
}

class ThreadPool
{
  public:
    ThreadPool(int numberOfConcurrentThreads)
            : threadPoolSize(numberOfConcurrentThreads)
            , freeSlots(numberOfConcurrentThreads)
    {
    }

    template<typename Task, typename... Args>
    void push_back(Task &&task, Args &&...args)
    {
        auto aThreadPoolTask = ThreadPoolTask<Task&&, Args&&...>(
                std::forward<Task>(task),
                conditionVariable,
                threadPoolMutex,
                freeSlots
        );

        std::thread{aThreadPoolTask, std::forward<Args>(args)...}.detach();
    }

    void resize(int numberOfConcurrentThreads)
    {
        std::unique_lock<std::mutex> uniqueLock(threadPoolMutex);
        int difference = numberOfConcurrentThreads - threadPoolSize;
        freeSlots += difference;
        conditionVariable.notify_all();
    }

  private:
    int threadPoolSize;
    int freeSlots;
    std::mutex threadPoolMutex;
    std::condition_variable conditionVariable;
};

}

#endif //CPP_THREAD_THREADPOOL_H
