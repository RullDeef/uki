#ifndef __UKI_DEFINES_H_
#define __UKI_DEFINES_H_

#define UKI_MIN(a, b) ((a) < (b) ? (a) : (b))
#define UKI_MAX(a, b) ((a) > (b) ? (a) : (b))

#ifndef UKI_TABLE_PRINT_MAX_WIDTH
#define UKI_TABLE_PRINT_MAX_WIDTH 80U
#endif

#ifndef UKI_TABLE_STRBUF_SIZE
#define UKI_TABLE_STRBUF_SIZE 64U
#endif

#ifndef UKI_TABLE_MAX_TABLES_COUNT
#define UKI_TABLE_MAX_TABLES_COUNT 255U
#endif

#ifndef UKI_MENU_INFO_SIZE
#define UKI_MENU_INFO_SIZE 64U
#endif

#ifndef UKI_MENU_PROMPT_SIZE
#define UKI_MENU_PROMPT_SIZE 32U
#endif

#ifndef UKI_MENU_MAX_MENUS_COUNT
#define UKI_MENU_MAX_MENUS_COUNT 31U
#endif

#endif // __UKI_DEFINES_H_
