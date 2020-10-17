#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "uki_input.h"

static bool imp__read_line(char *line)
{
    if (fgets(line, UKI_MAX_TEMP_STR_LEN, stdin) == NULL)
        return false;
    
    while (strlen(line) > 0 && isblank(line[strlen(line) - 1]))
        line[strlen(line) - 1] = '\0';

    return true;
}

bool uki_input_int32(const char *msg, const char *err_msg, int32_t *value)
{
    char line[UKI_MAX_TEMP_STR_LEN];
    printf("%s", msg);
    if (imp__read_line(line))
    {
        char *end = NULL;
        long num = strtol(line, &end, 10);
        if (num != 0 || (line != end))
        {
            if (-1L << 31 < num && num < 1L << 31)
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

bool uki_input_float(const char *msg, const char *err_msg, float *value)
{
    char line[UKI_MAX_TEMP_STR_LEN];
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
