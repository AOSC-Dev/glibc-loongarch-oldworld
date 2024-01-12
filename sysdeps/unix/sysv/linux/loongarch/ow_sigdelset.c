#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigdelset __ow_sigdelset
#include<signal/sigdelset.c>

#undef sigdelset
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigdelset, sigdelset, GLIBC_2_0);
#endif
