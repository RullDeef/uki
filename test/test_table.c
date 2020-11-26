#include "uki.h"
#include "testenv.h"

int test_table_bad_dims(void);
int test_table_1x1(void);
int test_table_1x2(void);
int test_table_2x1(void);

BEGIN_TEST(test_tables)
    SUB_TEST(test_table_bad_dims);
    SUB_TEST(test_table_1x1);
    SUB_TEST(test_table_1x2);
    SUB_TEST(test_table_2x1);
END_TEST

BEGIN_TEST(test_table_bad_dims)
    ASSERT_EQ(uki_table_create(0U, 0U), 0U);
    ASSERT_EQ(uki_table_create(1U, 0U), 0U);
    ASSERT_EQ(uki_table_create(0U, 1U), 0U);
END_TEST

BEGIN_TEST(test_table_1x1)
    uki_table_t t = uki_table_create(1U, 1U);
    ASSERT_NE(t, 0U);

    int result = uki_table_print(t, stdout);
    ASSERT_TYPE(result, >=, 0);

    result = uki_table_write(t, 1U, 0U, "hello");
    ASSERT_TYPE(result, <, 0);

    result = uki_table_write(t, 0U, 1U, "!!");
    ASSERT_TYPE(result, <, 0);

    result = uki_table_write(t, 0U, 0U, NULL);
    ASSERT_TYPE(result, <, 0);

    result = uki_table_write(t, 0U, 0U, "world");
    ASSERT_TYPE(result, >=, 0);

    result = uki_table_print(t, stdout);
    ASSERT_TYPE(result, >=, 0);

    uki_table_destroy(t);
END_TEST

BEGIN_TEST(test_table_1x2)
    uki_table_t t = uki_table_create(1U, 2U);

    int result = uki_table_print(t, stdout);
    ASSERT_TYPE(result, >, 0);

    result = uki_table_write(t, 0U, 0U, "(0, 0)");
    ASSERT_TYPE(result, >=, 0);

    result = uki_table_print(t, stdout);
    ASSERT_TYPE(result, >, 0);

    result = uki_table_write(t, 0U, 1U, "(0, 1)");
    ASSERT_TYPE(result, >=, 0);

    result = uki_table_print(t, stdout);
    ASSERT_TYPE(result, >, 0);

    uki_table_destroy(t);
END_TEST

int test_table_2x1(void) { return 0; }
