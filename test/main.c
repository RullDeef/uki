#include <stdio.h>
#include "uki.h"

void test_input(void)
{
    int32_t num;

    if (uki_input_int32("Введите число: ", "Неверный ввод!\n", &num))
        printf("Введено число: %d\n", num);
    
    if (uki_input_int32_constraint("Введите число от 10 до 20: ", "Не правильно!\n", 10, 20, &num))
        printf("Введено число: %d\n", num);

    if (uki_input_int32_force_constraint("Введите число от -50 до 100: ", "Это не то, повтори.\n", -50, 100, &num))
        printf("Введено число: %d\n", num);
}

void test_table(void)
{
    uki_table_t table = uki_table_create(2, 3, "title!");

    uki_table_set(&table, 0, 0, "hello");
    uki_table_set(&table, 0, 1, "dear");
    uki_table_set(&table, 0, 2, "World!");

    uki_table_set(&table, 1, 0, "there");
    uki_table_set(&table, 1, 1, "must be");
    uki_table_set(&table, 1, 2, "no errors");

    uki_table_print(&table);
}

int main(void)
{
    // test_input();
    test_table();
    return 0;
}
