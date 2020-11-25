#ifndef __TESTENV_H_
#define __TESTENV_H_

#include <stdio.h>

#define BEGIN_TEST(test_fn) \
    int test_fn(void)       \
    {                       \
        int __status = 0;

#define END_TEST     \
    return __status; \
    }

#define ASSERT_TYPE(a, comp, b)                                    \
    {                                                              \
        if (!((a)comp(b)))                                         \
        {                                                          \
            printf("assertion " #a " " #comp " " #b " failed.\n"); \
            __status++;                                            \
        }                                                          \
    }

#define ASSERT_EQ(a, b) ASSERT_TYPE(a, ==, b)
#define ASSERT_NE(a, b) ASSERT_TYPE(a, !=, b)

#define FAST_TEST(test_fn)                                \
    {                                                     \
        int r = test_fn();                                \
        if (r == 0)                                       \
            printf(#test_fn ": passed\n");                \
        else                                              \
            printf(#test_fn ": failed %d assertions", r); \
    }

#endif // __TESTENV_H_
