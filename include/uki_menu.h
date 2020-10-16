/*

    uki - библиотека для рутинных функций по работе с консолью, и псевдоинтерфейсом.

    Данный модуль содержит функции для работы с консольным меню.

    (C) 2020 - Rull Deef

*/

#ifndef __UKI_MENU_H_
#define __UNI_MENU_H_

#include <sys/types.h>
#include "uki_defines.h"

// Функция обработчик пунктов меню
// Должна вернуть UKI_MENU_EXIT чтобы выйти из меню корректно
typedef int (*uki_menu_opt_fn_t)(void *);

typedef struct
{
    char name[UKI_MENU_OPT_NAME_BUF_LEN];
    uki_menu_opt_fn_t func;
} uki_menu_opt_t;

typedef struct
{
    uint8_t opts_amount;
    char title[UKI_MENU_TITLE_BUF_LEN];
    uki_menu_opt_t opts[UKI_MENU_MAX_OPTS];
} uki_menu_t;

uki_menu_t uki_menu_create(const char *title, uint8_t opts_amount, ...);

// Функция для выхода из меню
int uki_menu_opt_exit(void *data);

// Возвращает UKI_OK, если не было никаких ошибок
int uki_menu_run(const uki_menu_t *menu, void* data);

#endif
