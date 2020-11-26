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

#endif // __UKI_DEFINES_H_
