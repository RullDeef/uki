#include <stdio.h>

#define UKI_IMPL
#include "uki.h"

int main(void)
{
    uki_init();

    int rows = 5;
    int cols = 4;
    uki_table table = uki_table_create(rows, cols);

    uki_table_print(table, stdout);

    int row = 2, col = 2;
    uki_table_write(table, row, col, "this is string!");

    uki_table_print(table, stdout);

    uki_table_destroy(table);

    uki_destroy();
    return 0;
}
