#ifndef __UKI_MENU_H_
#define __UKI_MENU_H_

#include "allocators.h"
#include <stdbool.h>

typedef unsigned int uki_menu_t;

uki_menu_t uki_menu_create_ctx(void);
uki_menu_t uki_menu_create_cmd(void);
void uki_menu_destroy(uki_menu_t id);

bool uki_menu_is_valid(uki_menu_t id);

int uki_menu_ctx_info_set(uki_menu_t id, const char *info);
int uki_menu_ctx_opt_add(uki_menu_t id, const char *opt_str, void *data);
int uki_menu_ctx_run(uki_menu_t id, void **data);

// frees all(!) allocated menus
void uki_menu_cleanup(void);

#endif // __UKI_MENU_H_
