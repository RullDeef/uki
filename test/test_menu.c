#include "uki.h"
#include "testenv.h"

BEGIN_TEST(test_menus)
    uki_menu_t menu = uki_menu_create_ctx();
    int result;

    result = uki_menu_ctx_info_set(menu, NULL);
    ASSERT_NE(result, 0);

    result = uki_menu_ctx_info_set(menu, "some very very very very very very very very very very very very very very very very very very long info string");
    ASSERT_EQ(result, 0);

    result = uki_menu_ctx_info_set(menu, "choose second option to pass");
    ASSERT_EQ(result, 0);

    result = uki_menu_ctx_opt_add(menu, "some cool opt", (void *)100);
    ASSERT_EQ(result, 0);

    result = uki_menu_ctx_opt_add(menu, "another cool opt", (void *)20);
    ASSERT_EQ(result, 0);

    int data = 0;
    result = uki_menu_ctx_run(menu, (void **)&data);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(data, 20);

    uki_menu_destroy(menu);
END_TEST
