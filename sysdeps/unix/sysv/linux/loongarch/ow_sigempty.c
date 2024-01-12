#define LA_OW_COMPACT

#include<shlib-compat.h>

#define sigemptyset __ow_sigemptyset
#include<signal/sigempty.c>

#undef sigemptyset
#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol(libc, __ow_sigemptyset, sigemptyset, GLIBC_2_0);
#endif
