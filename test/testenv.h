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

#define ASSERT_TYPE(a, comp, b)                                                            \
    {                                                                                      \
        if (!((a)comp(b)))                                                                 \
        {                                                                                  \
            printf(__FILE__ ":%d: assertion " #a " " #comp " " #b " failed.\n", __LINE__); \
            __status++;                                                                    \
        }                                                                                  \
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

#define SUB_TEST(test_fn)                                 \
    {                                                     \
        int r = test_fn();                                \
        if (r == 0)                                       \
            printf("subtest " #test_fn ": passed\n");     \
        else                                              \
        {                                                 \
            printf(#test_fn ": failed %d assertions", r); \
            __status++;                                   \
        }                                                 \
    }

#endif // __TESTENV_H_
