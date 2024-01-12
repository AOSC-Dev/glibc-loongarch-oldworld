#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigismember __ow_sigismember
#include<signal/sigismem.c>

#undef sigismember
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigismember, sigismember, GLIBC_2_0);
#endif
