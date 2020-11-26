#ifndef __UKI_ALLOCATORS_H_
#define __UKI_ALLOCATORS_H_

#ifndef UKI_MALLOC
#include <stdlib.h>
#define UKI_MALLOC(size) malloc(size)
#endif

#ifndef UKI_FREE
#include <stdlib.h>
#define UKI_FREE(ptr) free(ptr)
#endif

#endif // __UKI_ALLOCATORS_H_
