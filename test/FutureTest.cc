#include <thread>
#include <future>
#include <gtest/gtest.h>


TEST(FutureTestSuite, AsyncWaitGet)
{
    int sampleArg = 6;
    std::future<int> aFuture = std::async([&sampleArg]() { return sampleArg * (sampleArg + 1); });

    aFuture.wait();

    ASSERT_EQ(aFuture.get(), 42);
}

TEST(FutureTestSuite, AsyncWaitCatch)
{
    std::future<void> aFuture = std::async([]() { throw std::runtime_error("postpone"); });

    aFuture.wait();

    ASSERT_THROW(aFuture.get(), std::runtime_error);
}

TEST(FutureTestSuite, PackagedTaskWaitGet)
{
    std::packaged_task<int()> aPackagedTask([](){ return 42; });
    std::future<int> aFuture = aPackagedTask.get_future();
    std::thread(std::move(aPackagedTask)).detach();

    aFuture.wait();

    ASSERT_EQ(aFuture.get(), 42);
}

TEST(FutureTestSuite, FutureFromPromise)
{
    std::promise<int> aPromise;
    std::future<int> aFuture = aPromise.get_future();
    std::thread([&aPromise] { aPromise.set_value_at_thread_exit(42); }).detach();

    aFuture.wait();

    ASSERT_EQ(aFuture.get(), 42);
}

