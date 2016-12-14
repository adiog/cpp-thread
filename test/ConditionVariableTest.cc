#include <gtest/gtest.h>
#include <mutex>
#include <condition_variable>

TEST(ConditionVariableTestSuite, PositiveCondition)
{
    std::condition_variable aConditionVariable;
    std::mutex aMutex;

    {
        std::unique_lock<std::mutex> anUniqueLock(aMutex);
        bool alwaysTrue = true;
        aConditionVariable.wait(anUniqueLock, [&](){ return alwaysTrue;});
    }
}