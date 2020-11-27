#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "allocators.h"
#include "menu.h"

#define UKI_MENU_POOL_SIZE (UKI_MENU_MAX_MENUS_COUNT + 1U)

enum __uki_menu_type
{
    __UKI_MENU_FREE = 0,
    __UKI_MENU_CTX,
    __UKI_MENU_CMD,
};

/*
    Choose one of the options below:
    1. opt 1
    2. opt 2
    3. opt 3
    >>>
*/
struct __uki_menu_ctx_opt
{
    void *data;
    char *opt_str;
    struct __uki_menu_ctx_opt *next;
};

struct __uki_menu_ctx
{
    struct __uki_menu_ctx_opt *opt;
    char info[UKI_MENU_INFO_SIZE];
    char prompt[UKI_MENU_PROMPT_SIZE];
};

/*
    Menu info here.

    Available commands:
        run
            executes complex algorithm

        help
            shows this info page

        exit
            exits from menu

    >>>
*/
struct __uki_menu_cmd
{
    char prompt[UKI_MENU_PROMPT_SIZE];
};

struct __uki_menu
{
    int type;
    void *ptr;
};

static struct __uki_menu menu_pool[UKI_MENU_POOL_SIZE];

uki_menu_t uki_menu_create_ctx(void)
{
    uki_menu_t id = 0U;

    while (++id < UKI_MENU_POOL_SIZE)
        if (!uki_menu_is_valid(id))
            break;

    if (id == UKI_MENU_POOL_SIZE)
        id = 0U;
    else
    {
        menu_pool[id].type = __UKI_MENU_CTX;

        struct __uki_menu_ctx *menu = UKI_MALLOC(sizeof(struct __uki_menu_ctx));
        if (menu == NULL)
        {
            menu_pool[id].type = __UKI_MENU_FREE;
            id = 0U;
        }
        else
        {
            menu->opt = NULL;
            strcpy(menu->info, "");       // TODO: make a default via macro or global var.
            strcpy(menu->prompt, ">>> "); // TODO: make a default via macro or global var.

            menu_pool[id].ptr = (void *)menu;
        }
    }

    return id;
}

uki_menu_t uki_menu_create_cmd(void);

void uki_menu_destroy(uki_menu_t id)
{
    if (uki_menu_is_valid(id))
    {
        if (menu_pool[id].type == __UKI_MENU_CTX)
        {
            struct __uki_menu_ctx *menu = (struct __uki_menu_ctx *)menu_pool[id].ptr;
            struct __uki_menu_ctx_opt *opt = menu->opt;

            while (opt != NULL)
            {
                struct __uki_menu_ctx_opt *next = opt->next;
                UKI_FREE(opt);
                opt = next;
            }

            UKI_FREE(menu);
        }
        else // type == cmd
        {

        }

        menu_pool[id].type = __UKI_MENU_FREE;
    }
}

bool uki_menu_is_valid(uki_menu_t id)
{
    return id != 0U && id < UKI_MENU_POOL_SIZE && menu_pool[id].type != __UKI_MENU_FREE;
}

int uki_menu_ctx_info_set(uki_menu_t id, const char *info)
{
    if (info == NULL)
        return -1;
    
    if (!uki_menu_is_valid(id))
        return -2;

    if (menu_pool[id].type != __UKI_MENU_CTX)
        return -3;

    struct __uki_menu_ctx *menu = (struct __uki_menu_ctx *)menu_pool[id].ptr;
    strncpy(menu->info, info, UKI_MENU_INFO_SIZE - 1U);

    return 0;
}

int uki_menu_ctx_opt_add(uki_menu_t id, const char *opt_str, void *data)
{
    if (opt_str == NULL)
        return -1;

    if (!uki_menu_is_valid(id))
        return -2;

    if (menu_pool[id].type != __UKI_MENU_CTX)
        return -3;

    int result = 0;

    struct __uki_menu_ctx *menu = (struct __uki_menu_ctx *)menu_pool[id].ptr;
    struct __uki_menu_ctx_opt **opt = &(menu->opt);

    while (*opt != NULL)
        opt = &((*opt)->next);

    *opt = UKI_MALLOC(sizeof(struct __uki_menu_ctx_opt) + strlen(opt_str) + 1U);
    if (opt == NULL)
        result = -4; // TODO: add descriptive error codes
    else
    {
        (*opt)->opt_str = (char*)(*opt + 1U);
        (*opt)->data = data;
        (*opt)->next = NULL;

        strcpy((*opt)->opt_str, opt_str);
    }

    return result;
}

int uki_menu_ctx_run(uki_menu_t id, void **data)
{
    if (data == NULL)
        return -1;

    if (!uki_menu_is_valid(id))
        return -2;
    
    if (menu_pool[id].type != __UKI_MENU_CTX)
        return -3;

    struct __uki_menu_ctx *menu = (struct __uki_menu_ctx *)menu_pool[id].ptr;

    printf("%s\n", menu->info);

    unsigned int opts_count = 0U;
    for (struct __uki_menu_ctx_opt *opt = menu->opt; opt != NULL; opt = opt->next)
        printf("  %2d. %s\n", ++opts_count, opt->opt_str);

    printf("%s", menu->prompt);

    { // get opt number
        char input_buf[256];
        if (fgets(input_buf, 256, stdin) != NULL)
        {
            int opt_num = 0;
            sscanf(input_buf, "%d", &opt_num);

            *data = NULL;
            struct __uki_menu_ctx_opt *opt = menu->opt;
            while (opt != NULL && --opt_num >= 0)
            {
                *data = opt->data;
                opt = opt->next;
            }

            if (opt_num > 0)
                *data = NULL;
        }
    }

    return 0;
}

void uki_menu_cleanup(void)
{
    for (uki_menu_t id = 1U; id < UKI_MENU_POOL_SIZE; id++)
        uki_menu_destroy(id);
}
