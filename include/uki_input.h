/*

    uki - библиотека для рутинных функций по работе с консолью, и псевдоинтерфейсом.

    Данный модуль содержит функции по обработке ввода с клавиатуры.

    Префикс constaint означает, что ввод будет проверяться на заданные ограничения.
    Префикс force означает, что ввод будет требоваться в цикле, пока не будет введён корректно.

    Все функции возвращающие логическое значение возвращают true, если ввод был корректным.

    (C) 2020 - Rull Deef

*/

#ifndef __UKI_INPUT_H_
#define __UKI_INPUT_H_

#include <sys/types.h>
#include <stdbool.h>
#include "uki_defines.h"

bool uki_input_int32(const char *msg, const char *err_msg, int32_t *value);
bool uki_input_constraint_int32(const char *msg, const char *err_msg, int32_t min, int32_t max, int32_t *value);
bool uki_force_input_int32(const char *msg, const char *repeat_msg, int32_t *value);
bool uki_force_constraint_input_int32(const char *msg, const char *repeat_msg, int32_t min, int32_t max, int32_t *value);

#endif
