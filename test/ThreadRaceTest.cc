#include <gtest/gtest.h>
#include <functional>
#include <thread>
#include <adiog/repeat>
#include <ThreadRace.h>
#include <xrange>

class ThreadRaceTestSuite : public ::testing::Test
{


};

namespace
{
    std::function<void(int)> aThreadFunction =
        [](int iThreadIndex) {
            std::cout << "#" << iThreadIndex << ": start." << std::endl;
            repeat(5, [&iThreadIndex]() {
                std::cout << "#" << iThreadIndex << ": working..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(10 * (1 + iThreadIndex % 5)));
            });
            std::cout << "#" << iThreadIndex << ": end." << std::endl;
        };
}

TEST_F(ThreadRaceTestSuite, SampleRace)
{
    ThreadRace threadRace{2, [](){std::cout << "<<<BOOYAH!>>>" << std::endl;}};

    for(auto i : xrange(5)) {
        threadRace.push_back(aThreadFunction, int(i));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}