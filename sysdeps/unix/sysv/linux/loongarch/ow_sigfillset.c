#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigfillset __ow_sigfillset
#include<signal/sigfillset.c>

#undef sigfillset
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigfillset, sigfillset, GLIBC_2_0);
#endif
