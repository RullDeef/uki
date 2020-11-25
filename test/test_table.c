#include "uki.h"
#include "testenv.h"

BEGIN_TEST(test_tables)
    ASSERT_EQ(uki_table_create(0U, 0U), 0U);
    ASSERT_EQ(uki_table_create(1U, 0U), 0U);
    ASSERT_EQ(uki_table_create(0U, 1U), 0U);

    uki_table t1 = uki_table_create(1U, 1U);
    ASSERT_NE(t1, 0U);
    uki_table_destroy(t1);
END_TEST
