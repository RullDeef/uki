#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "allocators.h"
#include "../include/menu.h"

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
struct __uki_menu_cmd_opt
{
    char *cmd_str;
    char *info;
    int (*func)(int, const char **);
    struct __uki_menu_cmd_opt *next;
};

struct __uki_menu_cmd
{
    struct __uki_menu_cmd_opt *opt;
    char info[UKI_MENU_INFO_SIZE];
    char prompt[UKI_MENU_PROMPT_SIZE];
};

struct __uki_menu_frame
{
    uki_menu_t menu_id;
    int argc;
    const char **argv;
    struct __uki_menu_frame *prev;
    bool exit;
};

struct __uki_menu
{
    int type;
    void *ptr;
};

static struct __uki_menu menu_pool[UKI_MENU_POOL_SIZE];

static struct __uki_menu_frame *menu_frame; // for cmd menu nesting

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

uki_menu_t uki_menu_create_cmd(void)
{
    uki_menu_t id = 0U;

    while (++id < UKI_MENU_POOL_SIZE)
        if (!uki_menu_is_valid(id))
            break;

    if (id == UKI_MENU_POOL_SIZE)
        id = 0U;
    else
    {
        menu_pool[id].type = __UKI_MENU_CMD;

        struct __uki_menu_cmd *menu = UKI_MALLOC(sizeof(struct __uki_menu_cmd));
        if (menu == NULL)
        {
            menu_pool[id].type = __UKI_MENU_FREE;
            id = 0U;
        }
        else
        {
            menu->opt = NULL;
            strcpy(menu->info, "");
            strcpy(menu->prompt, ">>> ");

            menu_pool[id].ptr = (void *)menu;
        }
    }

    return id;
}

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
            struct __uki_menu_cmd *menu = (struct __uki_menu_cmd *)menu_pool[id].ptr;
            struct __uki_menu_cmd_opt *opt = menu->opt;

            while (opt != NULL)
            {
                struct __uki_menu_cmd_opt *next = opt->next;
                UKI_FREE(opt);
                opt = next;
            }

            UKI_FREE(menu);
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

int uki_menu_cmd_info_set(uki_menu_t id, const char *info)
{
    if (info == NULL)
        return -1;

    if (!uki_menu_is_valid(id))
        return -2;

    if (menu_pool[id].type != __UKI_MENU_CMD)
        return -3;

    struct __uki_menu_cmd *menu = (struct __uki_menu_cmd *)menu_pool[id].ptr;
    strncpy(menu->info, info, UKI_MENU_INFO_SIZE - 1U);

    return 0;
}

int uki_menu_cmd_opt_add(uki_menu_t id, const char *cmd_str, int (*func)(int, const char **), const char *info)
{
    if (cmd_str == NULL || func == NULL)
        return -1;

    if (!uki_menu_is_valid(id))
        return -2;

    if (menu_pool[id].type != __UKI_MENU_CMD)
        return -3;

    int result = 0;

    struct __uki_menu_cmd *menu = (struct __uki_menu_cmd *)menu_pool[id].ptr;
    struct __uki_menu_cmd_opt **opt = &(menu->opt);

    while (*opt != NULL)
        opt = &((*opt)->next);

    *opt = UKI_MALLOC(sizeof(struct __uki_menu_cmd_opt) + strlen(cmd_str) + 1U + (info ? (strlen(info) + 1U) : 0U));
    if (opt == NULL)
        result = -4; // TODO: add descriptive error codes
    else
    {
        (*opt)->cmd_str = (char *)(*opt + 1U);
        (*opt)->info = (char *)(*opt + 1U) + strlen(cmd_str) + (info ? 1U : 0U);
        (*opt)->func = func;
        (*opt)->next = NULL;

        strcpy((*opt)->cmd_str, cmd_str);
        if (info != NULL)
            strcpy((*opt)->info, info);
    }

    return result;
}

int uki_menu_cmd_run(uki_menu_t id)
{
    if (!uki_menu_is_valid(id))
        return -2;

    if (menu_pool[id].type != __UKI_MENU_CMD)
        return -3;

    struct __uki_menu_cmd *menu = (struct __uki_menu_cmd *)menu_pool[id].ptr;

    printf("%s\n", menu->info);
    for (struct __uki_menu_cmd_opt *opt = menu->opt; opt != NULL; opt = opt->next)
        printf("    %s\n        %s\n", opt->cmd_str, opt->info ? opt->info : "(?)");

    // init curr stack frame
    struct __uki_menu_frame frame;
    frame.menu_id = id;
    frame.exit = false;
    frame.prev = menu_frame;
    menu_frame = &frame; // link with prev. frame

    int result = 0;

    // run mainloop
    while (!menu_frame->exit)
    {
        printf("%s", menu->prompt);

        char input_buf[256];
        if (fgets(input_buf, 256, stdin) != NULL)
        {
            const char *argv[128];
            int argc = 0;

            // simple parse args
            char *arg_ptr = input_buf;
            while (*arg_ptr == ' ' || *arg_ptr == '\t')
                arg_ptr++;

            if (*arg_ptr != '\n')
            {
                argv[argc++] = arg_ptr;
                while (*arg_ptr != '\0')
                {
                    if (*arg_ptr == ' ' || *arg_ptr == '\t' || *arg_ptr == '\n')
                    {
                        while (*arg_ptr == ' ' || *arg_ptr == '\t' || *arg_ptr == '\n')
                            *(arg_ptr++) = '\0';
                        if (*arg_ptr != '\0')
                            argv[argc++] = arg_ptr;
                    }
                    else
                        arg_ptr++;
                }

                // debug
                // printf("argc = %d, argv = \"", argc);
                // for (int i = 0; i < argc; i++)
                //     printf("%s%s", (i == 0 ? "" : " "), argv[i]);
                // printf("\"\n");

                // find command with corresponding name
                for (struct __uki_menu_cmd_opt *opt = menu->opt; opt != NULL; opt = opt->next)
                {
                    if (strcmp(argv[0], opt->cmd_str) == 0) // TODO: check only first word in cmd_str
                    {
                        result = opt->func(argc, argv);
                        if (result != 0)
                            frame.exit = true;
                        break;
                    }
                }
            }
        }
    }

    // revert stack frame
    menu_frame = frame.prev;
    return result;
}

int uki_default_cmd_exit(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    if (menu_frame != NULL)
        menu_frame->exit = true;
    return 0;
}

void uki_menu_cleanup(void)
{
    for (uki_menu_t id = 1U; id < UKI_MENU_POOL_SIZE; id++)
        uki_menu_destroy(id);
}
