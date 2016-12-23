#include <gtest/gtest.h>
#include <CoRoutineTemplate.h>

TEST(CoRoutine, ObviousCase)
{
    CoRutClass cr;
    ASSERT_EQ(cr.start(5), 666);
    ASSERT_EQ(cr.resume(), 0);
    ASSERT_EQ(cr.resume(), 5);
    ASSERT_EQ(cr.resume(), 10);
    ASSERT_EQ(cr.resume(), 15);
    ASSERT_EQ(cr.resume(), 20);
    ASSERT_EQ(cr.resume(), 541);
}

