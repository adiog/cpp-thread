#include "FivePhilosophers.h"

#include <iostream>
#include <mutex>
#include <vector>
#include <thread>


struct Fork
{
    std::mutex mutex;
};

void philosopher(int i, Fork &left, Fork &right)
{
    constexpr int numberOfActions = 100;

    for (int j = 0; j < numberOfActions; j++) {
        {
            std::unique_lock<std::mutex> leftLock(left.mutex, std::defer_lock);
            std::unique_lock<std::mutex> rightLock(right.mutex, std::defer_lock);

            std::lock(leftLock, rightLock);

            std::cout << i << ": eating" << std::endl;

            std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        }
        {
            std::cout << i << ": thinking" << std::endl;

            std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        }
    }
};

void dine()
{
    constexpr int FORKS = 5;

    std::vector<Fork> forks(FORKS);
    std::vector<std::thread> threads(FORKS);

    for (int i = 0; i < FORKS; i++)
    {
        threads[i] = std::thread(philosopher, i, std::ref(forks[i]), std::ref(forks[(i + 1) % FORKS]));
    }

    for (int i = 0; i < FORKS; i++)
    {
        threads[i].join();
    }
}