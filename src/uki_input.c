#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "uki_input.h"

#define MAX_TEMP_STR_LEN 80

static bool imp__read_line(char *line)
{
    if (fgets(line, MAX_TEMP_STR_LEN, stdin) == NULL)
        return false;
    
    while (strlen(line) > 0 && isblank(line[strlen(line) - 1]))
        line[strlen(line) - 1] = '\0';

    return true;
}

UKI_API bool uki_input_int32(const char *msg, const char *err_msg, int32_t *value)
{
    char line[MAX_TEMP_STR_LEN];
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

UKI_API bool uki_input_constraint_int32(const char *msg, const char *err_msg, int32_t min, int32_t max, int32_t *value)
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

UKI_API bool uki_force_input_int32(const char *msg, const char *repeat_msg, int32_t *value)
{
    while (!uki_input_int32(msg, repeat_msg, value))
        if (feof(stdin))
            return false;
    return true;
}

UKI_API bool uki_force_constraint_input_int32(const char *msg, const char *repeat_msg, int32_t min, int32_t max, int32_t *value)
{
    while (!uki_input_constraint_int32(msg, repeat_msg, min, max, value))
        if (feof(stdin))
            return false;
    return true;
}
