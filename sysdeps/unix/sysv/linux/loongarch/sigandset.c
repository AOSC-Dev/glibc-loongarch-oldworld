#include<shlib-compat.h>

#define sigandset __nw_sigandset
#include<signal/sigandset.c>

#undef sigandset
versioned_symbol(libc, __nw_sigandset, sigandset, GLIBC_2_36);
