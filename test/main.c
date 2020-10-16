#include <stdio.h>
#include "uki.h"

int main(void)
{
    int32_t num;

    if (uki_input_int32("Введите число: ", "Неверный ввод!\n", &num))
    {
        printf("Введено число: %d\n", num);
    }

    return 0;
}
