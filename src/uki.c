#include "uki.h"

#ifdef UKI_IMPL

#ifndef __UKI_IMPL_H_
#define __UKI_IMPL_H_

#include "table.c"
#include "menu.c"

int uki_init(void)
{
    return 0;
}

void uki_destroy(void)
{
    uki_table_cleanup();
}

#endif // __UKI_IMPL_H_

#endif // UKI_IMPL
