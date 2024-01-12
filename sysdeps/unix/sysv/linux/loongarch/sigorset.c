#include<shlib-compat.h>

#define sigorset __nw_sigorset
#include<signal/sigorset.c>

#undef sigorset
versioned_symbol(libc, __nw_sigorset, sigorset, GLIBC_2_36);
