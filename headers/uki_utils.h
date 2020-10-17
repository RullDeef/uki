/*
    Данный модуль содержит вспомогательные функции.

    (C) 2020 - Rull Deef
*/

#ifndef __UKI_UTILS_H_
#define __UKI_UTILS_H_

#include <sys/types.h>

void uki__safe_strcpy(char *dst, const char *src, uint8_t buf_size);
void uki__center_str(char *str, uint8_t size);

void uki_clear_console(void);

#endif
