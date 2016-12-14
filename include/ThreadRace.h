#ifndef CPP_THREAD_THREADRACE_H
#define CPP_THREAD_THREADRACE_H

#include <memory>
#include <thread>
#include <condition_variable>


template<typename Task, typename... Args, typename WinRaceCallback>
std::function<void(Args &&...args)> ThreadRaceTask(
        Task task,
        WinRaceCallback winRaceCallback)
{
    return [=](Args &&...args) {
        std::thread{task, std::forward<Args>(args)...}.join();
        winRaceCallback();
    };
}

class ThreadRace {
public:
    ThreadRace(int numberOfWinners, std::function<void()> finishRaceCallback)
            : numberOfRemainingWinners(numberOfWinners)
            , finishRaceCallback(finishRaceCallback)
    {
        std::thread{[this](){this->finishRace();}}.detach();
    }

    void finishRace()
    {
        std::unique_lock<std::mutex> uniqueLock(threadRaceMutex);
        conditionVariable.wait(uniqueLock, [this](){
            return this->numberOfRemainingWinners <= 0;
        });
        finishRaceCallback();
    }

    template<typename Task, typename... Args>
    void push_back(Task &&task, Args &&...args)
    {
        auto threadRaceTask = ThreadRaceTask<Task, Args...>(
                std::forward<Task>(task),
                [this](){this->winRaceCallback();}
        );
        std::thread{threadRaceTask, std::forward<Args>(args)...}.detach();
    }

    void winRaceCallback()
    {
        {
            std::unique_lock<std::mutex> uniqueLock(threadRaceMutex);
            numberOfRemainingWinners--;
        }
        conditionVariable.notify_one();
    }

private:

    int numberOfRemainingWinners;
    std::mutex threadRaceMutex;
    std::condition_variable conditionVariable;
    std::function<void()> finishRaceCallback;
};


#endif //CPP_THREAD_THREADRACE_H
