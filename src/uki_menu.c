#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "uki_utils.h"
#include "uki_input.h"
#include "uki_menu.h"

static void imp__print_menu(const uki_menu_t *menu)
{
    if (strlen(menu->title) > 0)
        printf("%s\n", menu->title);

    for (uint8_t i = 0; i < menu->opts_amount; i++)
        printf("%2d. %s\n", i + 1, menu->opts[i].name);
}

uki_menu_t uki_menu_create(const char *title, uint8_t opts_amount, ...)
{
    uki_menu_t menu;
    memset(&menu, 0, sizeof(uki_menu_t));

    menu.opts_amount = opts_amount;
    if (title != NULL)
        uki__safe_strcpy(menu.title, title, UKI_MENU_TITLE_BUF_LEN);

    va_list arglist;
    va_start(arglist, opts_amount);
    for (uint8_t i = 0; i < opts_amount; i++)
    {
        const char *name = va_arg(arglist, const char*);
        uki__safe_strcpy(menu.opts[i].name, name, UKI_MENU_OPT_NAME_BUF_LEN);
        menu.opts[i].func = va_arg(arglist, uki_menu_opt_fn_t);
    }
    va_end(arglist);

    return menu;
}

int uki_menu_opt_exit(void *data)
{
    data = data; // -Wextra requires
    return UKI_MENU_EXIT;
}

int uki_menu_run(const uki_menu_t *menu, void *data)
{
    int result = UKI_OK;

    while (result == UKI_OK)
    {
        imp__print_menu(menu);

        int32_t opt;
        if (uki_input_int32_minmax("> ", "", 1, menu->opts_amount + 1, &opt))
            result = menu->opts[opt - 1].func(data);
    }

    if (result == UKI_MENU_EXIT)
        result = UKI_OK;
    
    return result;
}
