#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

std::once_flag aFlag;
std::atomic_int anAtomicInt;

void aSimpleDoOnceTask()
{
    std::call_once(aFlag, [](){ anAtomicInt++; });
}

TEST(OnceFlagTestSuite, SimpleAccess)
{
    std::thread aThreadNo1(aSimpleDoOnceTask);
    std::thread aThreadNo2(aSimpleDoOnceTask);
    std::thread aThreadNo3(aSimpleDoOnceTask);
    std::thread aThreadNo4(aSimpleDoOnceTask);

    aThreadNo1.join();
    aThreadNo2.join();
    aThreadNo3.join();
    aThreadNo4.join();

    ASSERT_EQ(anAtomicInt, 1);
}

std::once_flag aFlagNo2;
std::atomic_int anAtomicIntNo2;

void aMayThrowDoOnceSubtask(bool doThrow)
{
    if (doThrow)
    {
        throw std::exception();
    }

    anAtomicIntNo2++;
}
 
void aMayThrowDoOnceTask(bool doThrow)
{
  try
  {
      std::call_once(aFlagNo2, aMayThrowDoOnceSubtask, doThrow);
  }
  catch (...)
  {
  }
}
 
TEST(OnceFlagTestSuite, OnceTaskThrowing)
{
    std::thread aThreadNo1(aMayThrowDoOnceTask, true);
    std::thread aThreadNo2(aMayThrowDoOnceTask, true);
    std::thread aThreadNo3(aMayThrowDoOnceTask, false);
    std::thread aThreadNo4(aMayThrowDoOnceTask, false);

    aThreadNo1.join();
    aThreadNo2.join();
    aThreadNo3.join();
    aThreadNo4.join();

    ASSERT_EQ(anAtomicIntNo2, 1);
}
