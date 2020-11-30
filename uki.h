/*
    uki - tiny simple CLI library.

    (C) 2020 - Rull Deef
*/

#ifndef __UKI_H_
#define __UKI_H_

#ifndef __UKI_DEFINES_H_
#define __UKI_DEFINES_H_

#define UKI_MIN(a, b) ((a) < (b) ? (a) : (b))
#define UKI_MAX(a, b) ((a) > (b) ? (a) : (b))

#ifndef UKI_TABLE_PRINT_MAX_WIDTH
#define UKI_TABLE_PRINT_MAX_WIDTH 80U
#endif

#ifndef UKI_TABLE_STRBUF_SIZE
#define UKI_TABLE_STRBUF_SIZE 64U
#endif

#ifndef UKI_TABLE_MAX_TABLES_COUNT
#define UKI_TABLE_MAX_TABLES_COUNT 255U
#endif

#ifndef UKI_MENU_INFO_SIZE
#define UKI_MENU_INFO_SIZE 64U
#endif

#ifndef UKI_MENU_PROMPT_SIZE
#define UKI_MENU_PROMPT_SIZE 32U
#endif

#ifndef UKI_MENU_MAX_MENUS_COUNT
#define UKI_MENU_MAX_MENUS_COUNT 31U
#endif

#endif // __UKI_DEFINES_H_

#ifndef __UKI_ALLOCATORS_H_
#define __UKI_ALLOCATORS_H_

#ifndef UKI_MALLOC
#include <stdlib.h>
#define UKI_MALLOC(size) malloc(size)
#endif

#ifndef UKI_FREE
#include <stdlib.h>
#define UKI_FREE(ptr) free(ptr)
#endif

#endif // __UKI_ALLOCATORS_H_

#ifndef __UKI_TABLE_H_
#define __UKI_TABLE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned int uki_table_t;

uki_table_t uki_table_create(unsigned int rows, unsigned int cols);
void uki_table_destroy(uki_table_t id);

bool uki_table_is_valid(uki_table_t id);

int uki_table_write(uki_table_t id, unsigned int row, unsigned int col, const char *str);

int uki_table_print(uki_table_t id, FILE *file);

// frees all allocated tables
void uki_table_cleanup(void);

#endif // __UKI_TABLE_H_

#ifndef __UKI_MENU_H_
#define __UKI_MENU_H_

#include <stdbool.h>

typedef unsigned int uki_menu_t;

uki_menu_t uki_menu_create_ctx(void);
uki_menu_t uki_menu_create_cmd(void);
void uki_menu_destroy(uki_menu_t id);

bool uki_menu_is_valid(uki_menu_t id);

int uki_menu_ctx_info_set(uki_menu_t id, const char *info);
int uki_menu_ctx_opt_add(uki_menu_t id, const char *opt_str, void *data);
int uki_menu_ctx_run(uki_menu_t id, void **data);

int uki_menu_cmd_info_set(uki_menu_t id, const char *info);
int uki_menu_cmd_opt_add(uki_menu_t id, const char *cmd_str, int (*func)(int, const char **), const char *info);
int uki_menu_cmd_run(uki_menu_t id);

int uki_default_cmd_exit(int argc, const char **argv);

// frees all(!) allocated menus
void uki_menu_cleanup(void);

#endif // __UKI_MENU_H_

int uki_init(void);
void uki_destroy(void);

#endif // __UKI_H_

#ifdef UKI_IMPL

#ifndef __UKI_IMPL_H_
#define __UKI_IMPL_H_

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define UKI_TABLE_POOL_SIZE (UKI_TABLE_MAX_TABLES_COUNT + 1U)

struct __uki_table
{
    unsigned int rows;
    unsigned int cols;

    char ***data;
};

static struct __uki_table tables_pool[UKI_TABLE_POOL_SIZE];

static bool pointer_in_table(struct __uki_table table, char *ptr)
{
    return (char*)table.data < ptr && ptr < (char*)table.data +
        (table.rows * sizeof(char **) +
        table.rows * table.cols * (sizeof(char *) + UKI_TABLE_STRBUF_SIZE));
}

static unsigned int get_max_index(unsigned int *array, unsigned int size)
{
    unsigned int i = 0U;
    unsigned int max = array[i];
    unsigned int max_i = 0U;

    while (++i < size)
    {
        if (array[i] >= max)
        {
            max = array[i];
            max_i = i;
        }
    }

    return max_i;
}

static unsigned int *get_cols_sizes(struct __uki_table table)
{
    // the last one for total size
    unsigned int *cols = UKI_MALLOC((table.cols + 1U) * sizeof(unsigned int));

    if (cols != NULL)
    {
        cols[table.cols] = 0U;

        for (unsigned int col = 0U; col < table.cols; col++)
        {
            cols[col] = 0U;
            for (unsigned int row = 0U; row < table.rows; row++)
            {
                unsigned int size = strlen(table.data[row][col]);
                cols[col] = UKI_MAX(cols[col], size);
            }
            cols[table.cols] += cols[col];
        }

        if (cols[table.cols] > UKI_TABLE_PRINT_MAX_WIDTH)
        {
            // undefined
            UKI_FREE(cols);
            cols = NULL;
        }
        else
        {
            // shrink some of the sizes
            while (cols[table.cols] + table.cols + 1U > UKI_TABLE_PRINT_MAX_WIDTH)
            {
                unsigned int max_i = get_max_index(cols, table.cols);
                cols[max_i]--;
                cols[table.cols]--;
            }
        }
    }

    return cols;
}

uki_table_t uki_table_create(unsigned int rows, unsigned int cols)
{
    if (rows == 0U || cols == 0U)
        return 0U;

    uki_table_t id;
    for (id = 1U; id < UKI_TABLE_POOL_SIZE; id++)
        if (!uki_table_is_valid(id))
            break;

    if (id == UKI_TABLE_POOL_SIZE)
        id = 0U;
    else
    {
        tables_pool[id].rows = rows;
        tables_pool[id].cols = cols;

        tables_pool[id].data = UKI_MALLOC(
            rows * sizeof(char **) +
            rows * cols * (sizeof(char *) + UKI_TABLE_STRBUF_SIZE));
        if (tables_pool[id].data == NULL)
            id = 0U;
        else
        {
            for (unsigned int row = 0U; row < rows; row++)
            {
                tables_pool[id].data[row] = (char **)(tables_pool[id].data + rows) + row * cols;
                for (unsigned int col = 0U; col < cols; col++)
                {
                    tables_pool[id].data[row][col] = (char *)((char **)(tables_pool[id].data + rows) + rows * cols) + (row * cols + col) * UKI_TABLE_STRBUF_SIZE;
                    tables_pool[id].data[row][col][0] = '\0';
                }
            }
        }
    }

    return id;
}

void uki_table_destroy(uki_table_t id)
{
    if (uki_table_is_valid(id))
    {
        for (unsigned int row = 0U; row < tables_pool[id].rows; row++)
            for (unsigned int col = 0U; col < tables_pool[id].cols; col++)
                if (!pointer_in_table(tables_pool[id], tables_pool[id].data[row][col]))
                    UKI_FREE(tables_pool[id].data[row][col]);

        UKI_FREE(tables_pool[id].data);
        tables_pool[id].data = NULL;
    }
}

bool uki_table_is_valid(uki_table_t id)
{
    return tables_pool[id].data != NULL;
}

int uki_table_write(uki_table_t id, unsigned int row, unsigned int col, const char *str)
{
    if (id == 0 || tables_pool[id].data == NULL)
        return -1;

    if (row >= tables_pool[id].rows || col >= tables_pool[id].cols || str == NULL)
        return -2;

    size_t len = strlen(str);
    if (len < UKI_TABLE_STRBUF_SIZE)
    {
        strcpy(tables_pool[id].data[row][col], str);
    }
    else if (pointer_in_table(tables_pool[id], tables_pool[id].data[row][col]))
    {
        tables_pool[id].data[row][col] = UKI_MALLOC(len + 1);
        if (tables_pool[id].data[row][col] == NULL)
            return -3;
    }
    else if (strlen(tables_pool[id].data[row][col]) < len)
    {
        UKI_FREE(tables_pool[id].data[row][col]);
        tables_pool[id].data[row][col] = UKI_MALLOC(len + 1);
        if (tables_pool[id].data[row][col] == NULL)
            return -3;
    }

    strcpy(tables_pool[id].data[row][col], str);

    return 0;
}

static int safe_fprintf(int result, FILE *file, const char *fmt, ...)
{
    if (result >= 0)
    {
        va_list va;
        va_start(va, fmt);
        int wrote = vfprintf(file, fmt, va);
        va_end(va);
        if (wrote < 0)
            result = -1;
        else
            result += wrote;
    }

    return result;
}

int uki_table_print(uki_table_t id, FILE *file)
{
    if (id == 0 || tables_pool[id].data == NULL)
        return -1;

    unsigned int *col_sizes = get_cols_sizes(tables_pool[id]);
    if (col_sizes == NULL)
        return -2;

    int result = safe_fprintf(0, file, "\u250C");
    for (unsigned int i = 0U; i < tables_pool[id].cols; i++)
    {
        if (i != 0U)
            result = safe_fprintf(result, file, "\u252C");
        for (unsigned int j = 0U; j < col_sizes[i]; j++)
            result = safe_fprintf(result, file, "\u2500");
    }
    result = safe_fprintf(result, file, "\u2510\n");

    for (unsigned int row = 0U; row < tables_pool[id].rows; row++)
    {
        for (unsigned int col = 0U; col < tables_pool[id].cols; col++)
        {
            if (col == 0U)
                result = safe_fprintf(result, file, "\u2502");
            result = safe_fprintf(result, file, "%*.*s", col_sizes[col], col_sizes[col], tables_pool[id].data[row][col]);
            result = safe_fprintf(result, file, "\u2502");
        }

        result = safe_fprintf(result, file, "\n");
    }

    result = safe_fprintf(result, file, "\u2514");
    for (unsigned int i = 0U; i < tables_pool[id].cols; i++)
    {
        if (i != 0U)
            result = safe_fprintf(result, file, "\u2534");
        for (unsigned int j = 0U; j < col_sizes[i]; j++)
            result = safe_fprintf(result, file, "\u2500");
    }
    result = safe_fprintf(result, file, "\u2518\n");

    if (result < 0)
        result = -3;

    UKI_FREE(col_sizes);
    return result;
}

void uki_table_cleanup(void)
{
    for (uki_table_t id = 1U; id < UKI_TABLE_POOL_SIZE; id++)
        if (uki_table_is_valid(id))
            uki_table_destroy(id);
}

#include <stdio.h>
#include <string.h>

#ifndef __UKI_MENU_H_
#define __UKI_MENU_H_

#ifndef __UKI_ALLOCATORS_H_
#define __UKI_ALLOCATORS_H_

#ifndef UKI_MALLOC
#include <stdlib.h>
#define UKI_MALLOC(size) malloc(size)
#endif

#ifndef UKI_FREE
#include <stdlib.h>
#define UKI_FREE(ptr) free(ptr)
#endif

#endif // __UKI_ALLOCATORS_H_

#include <stdbool.h>

typedef unsigned int uki_menu_t;

uki_menu_t uki_menu_create_ctx(void);
uki_menu_t uki_menu_create_cmd(void);
void uki_menu_destroy(uki_menu_t id);

bool uki_menu_is_valid(uki_menu_t id);

int uki_menu_ctx_info_set(uki_menu_t id, const char *info);
int uki_menu_ctx_opt_add(uki_menu_t id, const char *opt_str, void *data);
int uki_menu_ctx_run(uki_menu_t id, void **data);

int uki_menu_cmd_info_set(uki_menu_t id, const char *info);
int uki_menu_cmd_opt_add(uki_menu_t id, const char *cmd_str, int (*func)(int, const char **), const char *info);
int uki_menu_cmd_run(uki_menu_t id);

int uki_default_cmd_exit(int argc, const char **argv);

// frees all(!) allocated menus
void uki_menu_cleanup(void);

#endif // __UKI_MENU_H_

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

int uki_init(void)
{
    return 0;
}

void uki_destroy(void)
{
    uki_table_cleanup();
}

#endif // __UKI_IMPL_H_

#endif // UKI_IMPL
