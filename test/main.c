#include <stdio.h>
#include "testenv.h"

#define UKI_IMPL
#include "uki.h"

int test_tables(void);

int main(void)
{
    uki_init();

    FAST_TEST(test_tables);

    uki_destroy();
    return 0;
}
