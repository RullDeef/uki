#ifndef __UKI_TABLE_H_
#define __UKI_TABLE_H_

#include <stdio.h>
#include <stdint.h>

#define MAX_TABLES_COUNT 255U
typedef unsigned int uki_table;

uki_table uki_table_create(unsigned int rows, unsigned int cols);
void uki_table_destroy(uki_table table);

int uki_table_write(uki_table table, unsigned int row, unsigned int col, const char *str);

int uki_table_print(uki_table table, FILE *file);

#endif // __UKI_TABLE_H_
