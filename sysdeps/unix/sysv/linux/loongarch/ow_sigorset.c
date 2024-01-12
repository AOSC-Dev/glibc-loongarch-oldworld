#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigorset __ow_sigorset
#include<signal/sigorset.c>

#undef sigorset
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigorset, sigorset, GLIBC_2_0);
#endif
