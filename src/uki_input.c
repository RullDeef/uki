#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "uki_input.h"

static bool imp__read_line(char *line)
{
    if (fgets(line, UKI_TEMP_STR_BUF_LEN, stdin) == NULL)
        return false;
    
    while (strlen(line) > 0 && isblank(line[strlen(line) - 1]))
        line[strlen(line) - 1] = '\0';

    return true;
}

bool uki_input_int32(const char *msg, const char *err_msg, int32_t *value)
{
    char line[UKI_TEMP_STR_BUF_LEN];
    printf("%s", msg);
    if (imp__read_line(line))
    {
        char *end = NULL;
        long num = strtol(line, &end, 10);
        if (num != 0 || (line != end))
        {
            if (-2147483648L < num && num < 2147483648L)
            {
                *value = (int32_t)num;
                return true;
            }
        }
    }
    printf("%s", err_msg);
    return false;
}

bool uki_input_int32_minmax(const char *msg, const char *err_msg, int32_t min, int32_t max, int32_t *value)
{
    int32_t num;
    if (uki_input_int32(msg, err_msg, &num))
    {
        if (min <= num && num < max)
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
    }
    return false;
}

bool uki_input_int32_constraint(const char *msg, const char *err_msg, int32_t *value, uki_constraint_int32_t constraint)
{
    int32_t num;
    if (uki_input_int32(msg, err_msg, &num))
    {
        if (constraint(num))
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
    }
    return false;
}

bool uki_input_int32_force(const char *msg, const char *repeat_msg, int32_t *value)
{
    while (!uki_input_int32(msg, repeat_msg, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_int32_force_minmax(const char *msg, const char *repeat_msg, int32_t min, int32_t max, int32_t *value)
{
    while (!uki_input_int32_minmax(msg, repeat_msg, min, max, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_int32_force_constraint(const char *msg, const char *repeat_msg, int32_t *value, uki_constraint_int32_t constraint)
{
    while (!uki_input_int32_constraint(msg, repeat_msg, value, constraint))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_uint32(const char *msg, const char *err_msg, uint32_t *value)
{
    char line[UKI_TEMP_STR_BUF_LEN];
    printf("%s", msg);
    if (imp__read_line(line))
    {
        char *end = NULL;
        unsigned long num = strtoul(line, &end, 10);
        if (num != 0 || (line != end))
        {
            *value = (uint32_t)num;
            return true;
        }
    }
    printf("%s", err_msg);
    return false;
}

bool uki_input_uint32_minmax(const char *msg, const char *err_msg, uint32_t min, uint32_t max, uint32_t *value)
{
    uint32_t num;
    if (uki_input_uint32(msg, err_msg, &num))
    {
        if (min <= num && num < max)
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
    }
    return false;
}

bool uki_input_uint32_constraint(const char *msg, const char *err_msg, uint32_t *value, uki_constraint_uint32_t constraint)
{
    uint32_t num;
    if (uki_input_uint32(msg, err_msg, &num))
    {
        if (constraint(num))
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
    }
    return false;
}

bool uki_input_uint32_force(const char *msg, const char *repeat_msg, uint32_t *value)
{
    while (!uki_input_uint32(msg, repeat_msg, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_uint32_force_minmax(const char *msg, const char *repeat_msg, uint32_t min, uint32_t max, uint32_t *value)
{
    while (!uki_input_uint32_minmax(msg, repeat_msg, min, max, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_uint32_force_constraint(const char *msg, const char *repeat_msg, uint32_t *value, uki_constraint_uint32_t constraint)
{
    while (!uki_input_uint32_constraint(msg, repeat_msg, value, constraint))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_float(const char *msg, const char *err_msg, float *value)
{
    char line[UKI_TEMP_STR_BUF_LEN];
    printf("%s", msg);
    if (imp__read_line(line))
    {
        char *end = NULL;
        float num = strtof(line, &end);
        if (num != 0.0f || (line != end))
        {
            *value = num;
            return true;
        }
    }
    printf("%s", err_msg);
    return false;
}

bool uki_input_float_minmax(const char *msg, const char *err_msg, float min, float max, float *value)
{
    float num;
    if (uki_input_float(msg, err_msg, &num))
    {
        if (min <= num && num < max)
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
        return false;
    }
    return false;
}

bool uki_input_float_constraint(const char *msg, const char *err_msg, float *value, uki_constraint_float_t constraint)
{
    float num;
    if (uki_input_float(msg, err_msg, &num))
    {
        if (constraint(num))
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
        return false;
    }
    return false;
}

bool uki_input_float_force(const char *msg, const char *repeat_msg, float *value)
{
    while (!uki_input_float(msg, repeat_msg, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_float_force_minmax(const char *msg, const char *repeat_msg, float min, float max, float *value)
{
    while (!uki_input_float_minmax(msg, repeat_msg, min, max, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_float_force_constraint(const char *msg, const char *repeat_msg, float *value, uki_constraint_float_t constraint)
{
    while (!uki_input_float_constraint(msg, repeat_msg, value, constraint))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_double(const char *msg, const char *err_msg, double *value)
{
    char line[UKI_TEMP_STR_BUF_LEN];
    printf("%s", msg);
    if (imp__read_line(line))
    {
        char *end = NULL;
        double num = strtod(line, &end);
        if (num != 0.0 || (line != end))
        {
            *value = num;
            return true;
        }
    }
    printf("%s", err_msg);
    return false;
}

bool uki_input_double_minmax(const char *msg, const char *err_msg, double min, double max, double *value)
{
    double num;
    if (uki_input_double(msg, err_msg, &num))
    {
        if (min <= num && num < max)
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
        return false;
    }
    return false;
}

bool uki_input_double_constraint(const char *msg, const char *err_msg, double *value, uki_constraint_double_t constraint)
{
    double num;
    if (uki_input_double(msg, err_msg, &num))
    {
        if (constraint(num))
        {
            *value = num;
            return true;
        }
        printf("%s", err_msg);
        return false;
    }
    return false;
}

bool uki_input_double_force(const char *msg, const char *repeat_msg, double *value)
{
    while (!uki_input_double(msg, repeat_msg, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_double_force_minmax(const char *msg, const char *repeat_msg, double min, double max, double *value)
{
    while (!uki_input_double_minmax(msg, repeat_msg, min, max, value))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_double_force_constraint(const char *msg, const char *repeat_msg, double *value, uki_constraint_double_t constraint)
{
    while (!uki_input_double_constraint(msg, repeat_msg, value, constraint))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_str(const char *msg, const char *err_msg, char *value, uint32_t buf_size)
{
    char line[UKI_TEMP_STR_BUF_LEN];
    printf("%s", msg);
    if (imp__read_line(line))
    {
        if (strlen(line) + 1 < buf_size)
        {
            strcpy(value, line);
            return true;
        }
    }
    printf("%s", err_msg);
    return false;
}

bool uki_input_str_force(const char *msg, const char *repeat_msg, char *value, uint32_t buf_size)
{
    while (!uki_input_str(msg, repeat_msg, value, buf_size))
        if (feof(stdin))
            return false;
    return true;
}

bool uki_input_str_constraint(const char *msg, const char *err_msg, char *value, uint32_t buf_size, uki_constraint_str_t constraint)
{
    char line[UKI_TEMP_STR_BUF_LEN];
    printf("%s", msg);
    if (imp__read_line(line))
    {
        if (strlen(line) + 1 < buf_size && constraint(line))
        {
            strcpy(value, line);
            return true;
        }
    }
    printf("%s", err_msg);
    return false;
}

bool uki_input_str_force_constraint(const char *msg, const char *repeat_msg, char *value, uint32_t buf_size, uki_constraint_str_t constraint)
{
    while (!uki_input_str_constraint(msg, repeat_msg, value, buf_size, constraint))
        if (feof(stdin))
            return false;
    return true;
}

int uki_input_scanf_line(const char *msg, const char *fmt, ...)
{
    char line[UKI_TEMP_STR_BUF_LEN];
    va_list arglist;

    int result = EOF;
    printf("%s", msg);
    if (imp__read_line(line))
    {
        va_start(arglist, fmt);
        result = vsscanf(line, fmt, arglist);
        va_end(arglist);
    }
    return result;
}
