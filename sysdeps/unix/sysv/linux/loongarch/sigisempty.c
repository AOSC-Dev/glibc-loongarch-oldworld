#include<shlib-compat.h>

#define sigisemptyset __nw_sigisemptyset
#include<signal/sigisempty.c>

#undef sigisemptyset
versioned_symbol(libc, __nw_sigisemptyset, sigisemptyset, GLIBC_2_36);
