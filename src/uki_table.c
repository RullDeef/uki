#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "uki_table.h"

static void imp__safe_strcpy(char *dst, const char *src, uint8_t buf_size)
{
    if (strlen(src) + 1 > buf_size)
    {
        strncpy(dst, src, buf_size - 4);
        strcpy(dst + buf_size - 4, "...");
    }
    else
        strcpy(dst, src);
}

static void imp__center_str(char *str, uint8_t size)
{
    if (strlen(str) < size)
    {
        uint8_t len = strlen(str);
        memmove(str + (size - len) / 2, str, (len + 1) * sizeof(char));
        memset(str, ' ', (size - len) / 2);
        memset(str + (size - len) / 2 + len, ' ', size - len);
        str[size] = '\0';
    }
}

static uint8_t imp__calc_table_length(const uki_table_t *table)
{
    uint8_t length = 1;

    for (uint8_t col = 0; col < table->cols; col++)
    {
        uint8_t col_length = 0;
        for (uint8_t row = 0; row < table->rows; row++)
            col_length = UKI_MAX(col_length, strlen(table->data[row][col]));
        length += col_length + 3;
    }

    return length;
}

static void imp__calc_cols_lengths(const uki_table_t *table, uint8_t *cols)
{
    for (uint8_t col = 0; col < table->cols; col++)
    {
        cols[col] = 0;
        for (uint8_t row = 0; row < table->rows; row++)
            cols[col] = UKI_MAX(cols[col], strlen(table->data[row][col]));
    }
}

uki_table_t uki_table_create(uint8_t rows, uint8_t cols, const char *title)
{
    uki_table_t table;
    memset(&table, 0, sizeof(uki_table_t));

    table.rows = rows;
    table.cols = cols;

    if (title != NULL)
        imp__safe_strcpy(table.title, title, UKI_TABLE_TITLE_BUF_LEN);

    return table;
}

bool uki_table_set(uki_table_t *table, uint8_t row, uint8_t col, const char *str)
{
    if (row >= table->rows || col >= table->cols)
        return false;
    
    imp__safe_strcpy(table->data[row][col], str, UKI_TABLE_STR_BUF_LEN);
    return true;
}

bool uki_table_set_fmt(uki_table_t *table, uint8_t row, uint8_t col, const char *fmt, ...)
{
    va_list arglist;

    char out_str[UKI_MAX_TEMP_STR_LEN];
    va_start(arglist, fmt);
    vsprintf(out_str, fmt, arglist);
    va_end(arglist);

    imp__safe_strcpy(out_str, out_str, UKI_TABLE_STR_BUF_LEN - 1);
    return uki_table_set(table, row, col, out_str);
}

void uki_table_print(const uki_table_t *table)
{
    // total table length (including borders)
    uint8_t length = imp__calc_table_length(table);
    printf("len = %d\n", length);
    // max length of each col
    uint8_t cols[UKI_TABLE_MAX_COLS];
    imp__calc_cols_lengths(table, cols);

    if (table->title != NULL)
    {
        printf("┌");
        for (uint8_t i = 0; i < length - 2; i++)
            printf("─");
        printf("┐\n");

        char centered_title[UKI_TABLE_TITLE_BUF_LEN];
        imp__safe_strcpy(centered_title, table->title, length - 4 + 1);
        imp__center_str(centered_title, length - 4);
        printf("│ %s │\n", centered_title);

        printf("├");
        for (uint8_t col = 0; col < table->cols; col++)
        {
            for (uint8_t i = 0; i < cols[col] + 2; i++)
                printf("─");
            if (col + 1 < table->cols)
                printf("┬");
        }
        printf("┤\n");
    }
    else
    {
        printf("┌");
        for (uint8_t col = 0; col < table->cols; col++)
        {
            for (uint8_t i = 0; i < cols[col] + 2; i++)
                printf("─");
            if (col + 1 < table->cols)
                printf("┬");
        }
        printf("┐\n");
    }

    // print actual data
    for (uint8_t row = 0; row < table->rows; row++)
    {
        printf("│");

        for (uint8_t col = 0; col < table->cols; col++)
        {
            char centered_str[UKI_TABLE_STR_BUF_LEN];
            imp__safe_strcpy(centered_str, table->data[row][col], UKI_TABLE_STR_BUF_LEN);
            imp__center_str(centered_str, cols[col]);
            printf(" %s │", centered_str);
        }
        
        printf("\n");
    }

    printf("└");
    for (uint8_t col = 0; col < table->cols; col++)
    {
        for (uint8_t i = 0; i < cols[col] + 2; i++)
            printf("─");
        if (col + 1 < table->cols)
            printf("┴");
    }
    printf("┘\n");
}
