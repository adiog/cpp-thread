#include <gtest/gtest.h>
#include <functional>
#include <thread>
#include <adiog/repeat>
#include <ThreadPool_WorkersFixedSet.h>
#include <xrange>

class ThreadPoolv2TestSuite : public ::testing::Test
{


};

namespace {

std::function<void(int)> aThreadFunction =
    [](int iThreadIndex) {
        std::cout << "#" << iThreadIndex << ": start." << std::endl;
        repeat(5, [&iThreadIndex]()
        {
            std::cout << "#" << iThreadIndex << ": working..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10*(1+iThreadIndex%5)));
        });
        std::cout << "#" << iThreadIndex << ": end." << std::endl;
    };
}
TEST_F(ThreadPoolv2TestSuite, SamplePool)
{
    WorkersFixedSet::ThreadPool threadPool{2};

    for(auto i : xrange(10))
    {
        threadPool.enqueue(aThreadFunction, int(i));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}