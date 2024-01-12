#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigandset __ow_sigandset
#include<signal/sigandset.c>

#undef sigandset
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigandset, sigandset, GLIBC_2_0);
#endif
