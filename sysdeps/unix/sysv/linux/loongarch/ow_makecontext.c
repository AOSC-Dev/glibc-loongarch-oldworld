#include <shlib-compat.h>

#if SHLIB_COMPAT (libc, GLIBC_2_1, GLIBC_2_36)
#include <sys/ucontext.h>
#include "ow_ucontext.h"

#undef versioned_symbol
#define versioned_symbol(a, b, c, d)
#define ucontext_t __ow_ucontext_t
#define __makecontext __ow___makecontext
#define __start_context __ow___start_context
#include "makecontext.c"

#undef __makecontext
compat_symbol (libc, __ow___makecontext, makecontext, GLIBC_2_1);

#endif
