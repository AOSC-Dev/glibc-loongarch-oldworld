#include<shlib-compat.h>
#include<signal.h>

#define sigaddset __nw_sigaddset
#undef libc_hidden_def
#define libc_hidden_def(name)
#include<signal/sigaddset.c>

#undef sigaddset
versioned_symbol(libc, __nw_sigaddset, sigaddset, GLIBC_2_36);
libc_hidden_ver(__nw_sigaddset, sigaddset)
