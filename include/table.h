#ifndef __UKI_TABLE_H_
#define __UKI_TABLE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "defines.h"

typedef unsigned int uki_table_t;

uki_table_t uki_table_create(unsigned int rows, unsigned int cols);
void uki_table_destroy(uki_table_t id);

bool uki_table_is_valid(uki_table_t id);

int uki_table_write(uki_table_t id, unsigned int row, unsigned int col, const char *str);

int uki_table_print(uki_table_t id, FILE *file);

// frees all allocated tables
void uki_table_cleanup(void);

#endif // __UKI_TABLE_H_
