#include <gtest/gtest.h>
#include <functional>
#include <thread>
#include <adiog/repeat>
#include <ThreadPool_Naive.h>
#include <xrange>

class ThreadPoolTestSuite : public ::testing::Test
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
TEST_F(ThreadPoolTestSuite, SamplePool)
{
    Naive::ThreadPool threadPool{6};

    for(auto i : xrange(30))
    {
        //threadPool.push_back(aThreadFunction, int(i));
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}