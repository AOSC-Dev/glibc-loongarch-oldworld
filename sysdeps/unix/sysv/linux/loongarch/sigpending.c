#include<shlib-compat.h>

#include<signal.h>

#define sigpending __sigpending
#include<sysdeps/unix/sysv/linux/sigpending.c>
#undef sigpending

versioned_symbol (libc, __sigpending, sigpending, GLIBC_2_36);

#if SHLIB_COMPAT(libc, GLIBC_2_0, GLIBC_2_36)
#include "ow_sigop.h"
int __ow_sigpending (sigset_t *set)
{
    int rc = __sigpending (set);
    if (rc >= 0){
        __ow_clear_unused_bits(set);
    }
    return rc;
}
compat_symbol (libc, __ow_sigpending, sigpending, GLIBC_2_0);
#endif
