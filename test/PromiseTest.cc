#include <gtest/gtest.h>
#include <future>
#include <numeric>

void do_work(std::promise<void> barrier)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    barrier.set_value();
}

TEST(PromiseTestSuite, UsePromiseVoidAsBarrier)
{
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::move(barrier));

    barrier_future.wait();

    new_work_thread.join();
}


void do_accumulate(std::vector<int>::iterator first,
                   std::vector<int>::iterator last,
                   std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);
}

TEST(PromiseTestSuite, SetValue)
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(do_accumulate, numbers.begin(), numbers.end(),
                            std::move(accumulate_promise));
    accumulate_future.wait();

    ASSERT_EQ(accumulate_future.get(), 21);
    work_thread.join();
}
