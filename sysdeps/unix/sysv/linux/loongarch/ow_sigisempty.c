#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigisemptyset __ow_sigisemptyset
#include<signal/sigisempty.c>

#undef sigisemptyset
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigisemptyset, sigisemptyset, GLIBC_2_0);
#endif
