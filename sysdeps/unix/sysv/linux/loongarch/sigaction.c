#include <shlib-compat.h>
#include <signal.h>
#include <internal-signals.h>

#undef weak_alias
#define weak_alias(name, aliasname)
#undef libc_hidden_def
#define libc_hidden_def(name)
#define __sigaction __nw_sigaction

#include <signal/sigaction.c>

#undef __sigaction

versioned_symbol (libc, __nw_sigaction, sigaction, GLIBC_2_36);
versioned_symbol (libc, __nw_sigaction, __sigaction, GLIBC_2_36);
libc_hidden_ver (__nw_sigaction, __sigaction);

#if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_36)
#include "ow_sigop.h"
int __ow___sigaction (int sig, const struct sigaction *act, struct sigaction *oact)
{
    return __nw_sigaction(sig, act, oact);
}
compat_symbol (libc, __ow___sigaction, sigaction, GLIBC_2_0);
compat_symbol (libc, __ow___sigaction, __sigaction, GLIBC_2_0);
#endif
