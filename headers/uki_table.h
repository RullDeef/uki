/*
    Данный модуль содержит функции для вывода таблиц.

    (C) 2020 - Rull Deef
*/

#ifndef __UKI_TABLE_H_
#define __UKI_TABLE_H_

#include <sys/types.h>
#include <stdint.h>
#include "uki_defines.h"

typedef struct
{
    uint32_t rows;
    uint32_t cols;
    char title[UKI_TABLE_TITLE_BUF_LEN];
    char data[UKI_TABLE_MAX_ROWS][UKI_TABLE_MAX_COLS][UKI_TABLE_STR_BUF_LEN];
} uki_table_t;

uki_table_t uki_table_create(uint32_t rows, uint32_t cols, const char *title);

// true если записалось удачно
bool uki_table_set(uki_table_t *table, uint32_t row, uint32_t col, const char *str);
bool uki_table_set_fmt(uki_table_t *table, uint32_t row, uint32_t col, const char *fmt, ...);

void uki_table_print(const uki_table_t *table);

#endif
