#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "uki_defines.h"
#include "uki_utils.h"

void uki__safe_strcpy(char *dst, const char *src, uint32_t buf_size)
{
    if (strlen(src) + 1 > buf_size)
    {
        strncpy(dst, src, buf_size - 4);
        strcpy(dst + buf_size - 4, "...");
    }
    else
        strcpy(dst, src);
}

void uki__center_str(char *str, uint32_t size)
{
    uint32_t len = strlen(str);
    if (len == 0)
    {
        sprintf(str, "%*s", size - 1, "");
    }
    else if (strlen(str) < size)
    {
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

void uki_wait(const char *msg)
{
    char temp[UKI_TEMP_STR_BUF_LEN];
    if (msg != NULL)
        printf("%s\n", msg);
    fgets(temp, UKI_TEMP_STR_BUF_LEN, stdin);
}
