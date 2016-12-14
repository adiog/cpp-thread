#include <gtest/gtest.h>
#include <mutex>
#include <human>

std::recursive_mutex aRecursiveMutex;

unsigned int calculateFactorialInIsolation(unsigned int n)
{
    std::unique_lock<std::recursive_mutex> anUniqueLock(aRecursiveMutex);
    if (n < 2) {
        return 1;
    } else {
        return calculateFactorialInIsolation(n-1);
    }
}

TEST(RecursiveMutexTestSuite, rawTest)
{
    Declare aSilniaOd5 as calculateFactorialInIsolation(5);

}