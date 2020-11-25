#ifndef __UKI_TABLE_H_
#define __UKI_TABLE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef UKI_TABLE_STRBUF_SIZE
#define UKI_TABLE_STRBUF_SIZE 64U
#endif

#define MAX_TABLES_COUNT 255U
typedef unsigned int uki_table;

uki_table uki_table_create(unsigned int rows, unsigned int cols);
void uki_table_destroy(uki_table id);

bool uki_table_is_valid(uki_table id);

int uki_table_write(uki_table id, unsigned int row, unsigned int col, const char *str);

int uki_table_print(uki_table id, FILE *file);

// frees all allocated tables
void uki_table_cleanup(void);

#endif // __UKI_TABLE_H_
