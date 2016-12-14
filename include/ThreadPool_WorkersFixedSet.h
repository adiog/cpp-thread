#ifndef CPP_THREAD_THREADPOOL_WORKERSFIXEDSET_H
#define CPP_THREAD_THREADPOOL_WORKERSFIXEDSET_H

#include <memory>
#include <future>
#include <thread>
#include <queue>
#include <condition_variable>

namespace WorkersFixedSet {

class ThreadPool
{
  public:
    ThreadPool(int numberOfConcurrentThreads)
            : isActive(true)
    {
        for(int workerIndex = 0; workerIndex < numberOfConcurrentThreads; ++workerIndex)
        {
            workers.emplace_back([this]()
            {
                while(isActive)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> anUniqueLock(aMutex);

                        aConditionVariable.wait(anUniqueLock, [&](){ return ((!isActive) || (!threadQueue.empty()));});

                        if (!isActive)
                        {
                            return;
                        }

                        task = std::move(threadQueue.front());

                        threadQueue.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool()
    {
        isActive = false;

        aConditionVariable.notify_all();

        for(auto& worker : workers)
        {
            worker.join();
        }
    }

    template<typename Task, typename... Args>
    auto enqueue(Task &&task, Args &&...args)
        -> std::future<typename std::result_of<Task(Args...)>::type>
    {
        using ResultType = typename std::result_of<Task(Args...)>::type;

        auto aPackagedTask = std::make_shared<std::packaged_task<ResultType()>>(
            std::bind(std::forward<Task>(task), std::forward<Args>(args)...)
        );

        auto aFuture = aPackagedTask->get_future();

        {
            std::unique_lock<std::mutex> anUniqueLock{aMutex};

            threadQueue.emplace([aPackagedTask](){ (*aPackagedTask)(); });
        }

        aConditionVariable.notify_one();

        return aFuture;
    }

  private:
    std::atomic_bool isActive;
    std::mutex aMutex;
    std::condition_variable aConditionVariable;

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> threadQueue;
};
}

#endif
