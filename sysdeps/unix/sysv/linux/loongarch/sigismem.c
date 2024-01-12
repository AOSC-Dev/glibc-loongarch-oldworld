#include<shlib-compat.h>

#define sigismember __nw_sigismember
#include<signal/sigismem.c>

#undef sigismember
versioned_symbol(libc, __nw_sigismember, sigismember, GLIBC_2_36);
