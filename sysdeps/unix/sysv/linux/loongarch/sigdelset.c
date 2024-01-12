#include<shlib-compat.h>
#include<signal.h>

#define sigdelset __nw_sigdelset
#undef libc_hidden_def
#define libc_hidden_def(name)
#include<signal/sigdelset.c>

#undef sigdelset
versioned_symbol(libc, __nw_sigdelset, sigdelset, GLIBC_2_36);
libc_hidden_ver(__nw_sigdelset, sigdelset)
