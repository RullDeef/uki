#include <stdio.h>
#include "uki.h"

int test_input(void *data)
{
    data = data;
    int32_t num;

    if (uki_input_int32("Введите число: ", "Неверный ввод!\n", &num))
        printf("Введено число: %d\n", num);
    
    if (uki_input_int32_minmax("Введите число от 10 до 20: ", "Не правильно!\n", 10, 20, &num))
        printf("Введено число: %d\n", num);

    if (uki_input_int32_force_minmax("Введите число от -50 до 100: ", "Это не то, повтори.\n", -50, 100, &num))
        printf("Введено число: %d\n", num);
    
    return UKI_OK;
}

int test_table(void *data)
{
    data = data;
    uki_table_t table = uki_table_create(2, 3,
        "This is a way too long title ever writen!");

    uki_table_set(&table, 0, 0, "hello");
    uki_table_set(&table, 0, 1, "dear");
    uki_table_set(&table, 0, 2, "World!");

    uki_table_set_fmt(&table, 1, 0, "%.8f", 3.1415926525f);
    uki_table_set(&table, 1, 1, "must be");
    uki_table_set(&table, 1, 2, "no errors");

    uki_table_print(&table);
    return UKI_OK;
}

void test_menu(void)
{
    uki_menu_t menu = uki_menu_create("My cool menu!!", 3,
        "test input", test_input,
        "test table", test_table,
        "exit", uki_menu_opt_exit
    );

    uki_menu_run(&menu, NULL);
}

int main(void)
{
    // test_input();
    // test_table();
    test_menu();
    return 0;
}
