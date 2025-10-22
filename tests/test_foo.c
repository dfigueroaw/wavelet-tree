#include <unity.h>
#include "foo.h"

void setUp(void) {}

void tearDown(void) {}

void test_foo(void)
{
    TEST_ASSERT_EQUAL(2, add(1, 1));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_foo);
    return UNITY_END();
}
