//
// Created by adiog on 12/22/16.
//

#ifndef CPP_THREAD_COROUTINETEMPLATE_H
#define CPP_THREAD_COROUTINETEMPLATE_H

#include <mutex>
#include <future>
#include <thread>

#define CO_YIELD(value) \
    isRunning = false; \
    promise.set_value(value); \
    cv.wait(unique_lock, [this](){return isTerminated || isRunning;}); \
    if (isTerminated) return;

#define CO_RETURN(value) \
    isRunning = false; \
    promise.set_value(value); \
    isReturned = true; \
    return;

class CoRutClass {
public:
    CoRutClass() = default;
    ~CoRutClass() {
        isTerminated = true;
        cv.notify_all();
        thr.join();
    }

    using ReturnType = int;

    void corut_thread(int factor) {
        std::unique_lock <std::mutex> unique_lock(mutex);

        CO_YIELD(666)


        for (int i = 0; i < 5; ++i) {
            CO_YIELD(i*factor)
        }

        CO_RETURN(541)

    }

    template<typename... Args>
    ReturnType start(Args... args) {
        if (isStarted) {
            throw std::runtime_error("");
        } else {
            isStarted = true;
            thr = std::thread{[&]() { this->corut_thread(args...); }};
            return resume();
        }
    }

    ReturnType resume() {
        return this->operator()();
    }

    ReturnType operator()() {
        ReturnType return_value = promise.get_future().get();
        {
            std::unique_lock<std::mutex> unique_lock(mutex);
            promise = std::promise<ReturnType>{};
            isRunning = true;
        }
        cv.notify_one();
        return return_value;
    }

    bool isFinished() { return isReturned; }

private:
    std::thread thr;
    std::mutex mutex;
    std::condition_variable cv;
    std::promise <ReturnType> promise;
    bool isStarted = false;
    bool isRunning = false;
    bool isReturned = false;
    bool isTerminated = false;
} cr;

#endif //CPP_THREAD_COROUTINETEMPLATE_H
