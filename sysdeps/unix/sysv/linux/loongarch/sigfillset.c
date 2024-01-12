#include<shlib-compat.h>
#include<signal.h>

#define sigfillset __nw_sigfillset
#undef libc_hidden_def
#define libc_hidden_def(name)
#include<signal/sigfillset.c>

#undef sigfillset
versioned_symbol(libc, __nw_sigfillset, sigfillset, GLIBC_2_36);
libc_hidden_ver(__nw_sigfillset, sigfillset)
