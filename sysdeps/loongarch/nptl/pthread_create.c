#include <signal.h>

#undef _NSIG
#define _NSIG _NW_NSIG

#include_next <nptl/pthread_create.c>
