#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "allocators.h"
#include "table.h"

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
