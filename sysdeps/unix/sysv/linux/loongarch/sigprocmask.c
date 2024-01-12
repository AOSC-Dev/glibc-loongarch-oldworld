#include <shlib-compat.h>

#undef weak_alias
#define weak_alias(name, aliasname)
#include <sysdeps/unix/sysv/linux/sigprocmask.c>

versioned_symbol (libc, __sigprocmask, sigprocmask, GLIBC_2_36);

#if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_36)
#include "ow_sigop.h"
int __ow_sigprocmask (int how, const sigset_t *set, sigset_t *oldset)
{
    int rc = __sigprocmask (how, set, oldset);
    if (rc >= 0 && oldset != NULL){
        __ow_clear_unused_bits(oldset);
    }
    return rc;
}
compat_symbol (libc, __ow_sigprocmask, sigprocmask, GLIBC_2_0);
#endif
