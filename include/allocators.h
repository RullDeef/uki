#ifndef __UKI_ALLOCATORS_H_
#define __UKI_ALLOCATORS_H_

#if defined(UKI_MALLOC) && defined(UKI_FREE)
#define malloc(size) UKI_MALLOC(size)
#define free(ptr) UKI_FREE(ptr)
#elif defined(UKI_MALLOC) || defined(UKI_FREE)
#error you must define both UKI_MALLOC and UKI_FREE
#else
#include <stdlib.h>
#endif

#endif // __UKI_ALLOCATORS_H_
