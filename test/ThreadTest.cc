#include <gtest/gtest.h>
#include <thread>

TEST(ThreadTestSuite, RunAndJoin)
{
    const int n = 10;

    std::vector<bool> aFlagCollection(n, false);

    std::function<void(int)> aThreadFunction =
            [&aFlagCollection](int iThreadIndex) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                aFlagCollection[iThreadIndex] = true;
            };

    std::vector<std::thread> aThreadPool;

    for (int i = 0; i < n; ++i)
    {
        aThreadPool.emplace_back(aThreadFunction, i);
    }

    for(auto &aThread : aThreadPool)
    {
        aThread.join();
    }

    for (int i = 0; i < n; ++i)
    {
        ASSERT_TRUE(aFlagCollection[i]);
    }
}

TEST(ThreadTestSuite, InplaceJoin)
{
    bool aFlag = false;

    std::function<void()> aThreadFunction =
            [&aFlag]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                aFlag = true;
            };

    std::thread{aThreadFunction}.join();

    ASSERT_TRUE(aFlag);
}

TEST(ThreadTestSuite, InplaceDetach)
{
    std::function<void()> aThreadFunction =
            []() {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            };

    std::thread{aThreadFunction}.detach();
}
