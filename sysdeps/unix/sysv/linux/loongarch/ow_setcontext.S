#include <shlib-compat.h>
#include "ow_ucontext-macros.h"

#if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_36)
#undef versioned_symbol
#define versioned_symbol(a, b, c, d)
#define __setcontext __ow___setcontext
#define __start_context __ow___start_context
#include "setcontext.S"

#undef __setcontext
compat_symbol (libc, __ow___setcontext, setcontext, GLIBC_2_0);

#endif
