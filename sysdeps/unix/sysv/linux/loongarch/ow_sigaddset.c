#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigaddset __ow_sigaddset
#include<signal/sigaddset.c>

#undef sigaddset
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigaddset, sigaddset, GLIBC_2_0);
#endif
