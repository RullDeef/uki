#include "allocators.h"
#include "table.h"

#include <stdbool.h>
#include <string.h>

#define TABLE_POOL_SIZE (MAX_TABLES_COUNT + 1U)

static struct __uki_table
{
    unsigned int rows;
    unsigned int cols;

    char ***data;
} tables_pool[TABLE_POOL_SIZE];

static bool pointer_in_table(struct __uki_table table, char *ptr)
{
    return (char*)table.data < ptr && ptr < (char*)table.data +
        (table.rows * sizeof(char **) +
        table.rows * table.cols * (sizeof(char *) + UKI_TABLE_STRBUF_SIZE));
}

uki_table uki_table_create(unsigned int rows, unsigned int cols)
{
    if (rows == 0U || cols == 0U)
        return 0U;

    uki_table id;
    for (id = 1U; id < TABLE_POOL_SIZE; id++)
        if (!uki_table_is_valid(id))
            break;

    if (id == TABLE_POOL_SIZE)
        id = 0U;
    else
    {
        tables_pool[id].rows = rows;
        tables_pool[id].cols = cols;

        tables_pool[id].data = malloc(
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
                    tables_pool[id].data[row][col] = (char *)((char **)(tables_pool[id].data + rows) + rows * cols) + (row * cols + col) * UKI_TABLE_STRBUF_SIZE;
            }
        }
    }

    return id;
}

void uki_table_destroy(uki_table id)
{
    if (uki_table_is_valid(id))
    {
        for (unsigned int row = 0U; row < tables_pool[id].rows; row++)
            for (unsigned int col = 0U; col < tables_pool[id].cols; col++)
                if (!pointer_in_table(tables_pool[id], tables_pool[id].data[row][col]))
                    free(tables_pool[id].data[row][col]);

        free(tables_pool[id].data);
        tables_pool[id].data = NULL;
    }
}

bool uki_table_is_valid(uki_table id)
{
    return tables_pool[id].data != NULL;
}

int uki_table_write(uki_table id, unsigned int row, unsigned int col, const char *str)
{
    if (id == 0 || tables_pool[id].data == NULL)
        return -1;

    if (row >= tables_pool[id].rows || col >= tables_pool[id].cols)
        return -2;

    size_t len = strlen(str);
    if (len < UKI_TABLE_STRBUF_SIZE)
    {
        strcpy(tables_pool[id].data[row][col], str);
    }
    else if (pointer_in_table(tables_pool[id], tables_pool[id].data[row][col]))
    {
        tables_pool[id].data[row][col] = malloc(len + 1);
        if (tables_pool[id].data[row][col] == NULL)
            return -3;
    }
    else if (strlen(tables_pool[id].data[row][col]) < len)
    {
        free(tables_pool[id].data[row][col]);
        tables_pool[id].data[row][col] = malloc(len + 1);
        if (tables_pool[id].data[row][col] == NULL)
            return -3;
    }

    strcpy(tables_pool[id].data[row][col], str);

    return 0;
}

int uki_table_print(uki_table id, FILE *file)
{
    if (id == 0 || tables_pool[id].data == NULL)
        return -1;

    int result = 0;

    for (unsigned int row = 0U; row < tables_pool[id].rows; row++)
    {
        for (unsigned int col = 0U; col < tables_pool[id].cols; col++)
        {
            result += fprintf(file, "%s, ", tables_pool[id].data[row][col]);
            if (result < 0)
                return -2;
        }

        result += fprintf(file, "\n");
    }

    return result;
}

void uki_table_cleanup(void)
{
    for (uki_table id = 1U; id < TABLE_POOL_SIZE; id++)
        if (uki_table_is_valid(id))
            uki_table_destroy(id);
}
