/*
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

typedef bool (*uki_constraint_int32_t)(int32_t);
typedef bool (*uki_constraint_uint32_t)(uint32_t);
typedef bool (*uki_constraint_float_t)(float);
typedef bool (*uki_constraint_double_t)(double);
typedef bool (*uki_constraint_str_t)(const char*);

bool uki_input_int32(const char *msg, const char *err_msg, int32_t *value);
bool uki_input_int32_minmax(const char *msg, const char *err_msg, int32_t min, int32_t max, int32_t *value);
bool uki_input_int32_constraint(const char *msg, const char *err_msg, int32_t *value, uki_constraint_int32_t constraint);
bool uki_input_int32_force(const char *msg, const char *repeat_msg, int32_t *value);
bool uki_input_int32_force_minmax(const char *msg, const char *repeat_msg, int32_t min, int32_t max, int32_t *value);
bool uki_input_int32_force_constraint(const char *msg, const char *repeat_msg, int32_t *value, uki_constraint_int32_t constraint);

bool uki_input_uint32(const char *msg, const char *err_msg, uint32_t *value);
bool uki_input_uint32_minmax(const char *msg, const char *err_msg, uint32_t min, uint32_t max, uint32_t *value);
bool uki_input_uint32_constraint(const char *msg, const char *err_msg, uint32_t *value, uki_constraint_uint32_t constraint);
bool uki_input_uint32_force(const char *msg, const char *repeat_msg, uint32_t *value);
bool uki_input_uint32_force_minmax(const char *msg, const char *repeat_msg, uint32_t min, uint32_t max, uint32_t *value);
bool uki_input_uint32_force_constraint(const char *msg, const char *repeat_msg, uint32_t *value, uki_constraint_uint32_t constraint);

bool uki_input_float(const char *msg, const char *err_msg, float *value);
bool uki_input_float_minmax(const char *msg, const char *err_msg, float min, float max, float *value);
bool uki_input_float_constraint(const char *msg, const char *err_msg, float *value, uki_constraint_float_t constraint);
bool uki_input_float_force(const char *msg, const char *repeat_msg, float *value);
bool uki_input_float_force_minmax(const char *msg, const char *repeat_msg, float min, float max, float *value);
bool uki_input_float_force_constraint(const char *msg, const char *repeat_msg, float *value, uki_constraint_float_t constraint);

bool uki_input_double(const char *msg, const char *err_msg, double *value);
bool uki_input_double_minmax(const char *msg, const char *err_msg, double min, double max, double *value);
bool uki_input_double_constraint(const char *msg, const char *err_msg, double *value, uki_constraint_double_t constraint);
bool uki_input_double_force(const char *msg, const char *repeat_msg, double *value);
bool uki_input_double_force_minmax(const char *msg, const char *repeat_msg, double min, double max, double *value);
bool uki_input_double_force_constraint(const char *msg, const char *repeat_msg, double *value, uki_constraint_double_t constraint);

// Для ввода строки в статический буфер
bool uki_input_str(const char *msg, const char *err_msg, char *value, uint8_t buf_size);
bool uki_input_str_force(const char *msg, const char *repeat_msg, char *value, uint8_t buf_size);
bool uki_input_str_constraint(const char *msg, const char *err_msg, char *value, uint8_t buf_size, uki_constraint_str_t constraint);
bool uki_input_str_force_constraint(const char *msg, const char *repeat_msg, char *value, uint8_t buf_size, uki_constraint_str_t constraint);

#endif
