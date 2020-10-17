#include <stdlib.h>
#include <string.h>
#include "uki_defines.h"
#include "uki_utils.h"

void uki__safe_strcpy(char *dst, const char *src, uint8_t buf_size)
{
    if (strlen(src) + 1 > buf_size)
    {
        strncpy(dst, src, buf_size - 4);
        strcpy(dst + buf_size - 4, "...");
    }
    else
        strcpy(dst, src);
}

void uki__center_str(char *str, uint8_t size)
{
    if (strlen(str) < size)
    {
        uint8_t len = strlen(str);
        memmove(str + (size - len) / 2, str, (len + 1) * sizeof(char));
        memset(str, ' ', (size - len) / 2);
        memset(str + (size - len) / 2 + len, ' ', size - len);
        str[size] = '\0';
    }
}

void uki_clear_console(void)
{
    system("clear");
}
