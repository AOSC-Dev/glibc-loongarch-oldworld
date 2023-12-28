#include <signal.h>

#undef _NSIG
#define _NSIG _NW_NSIG

#include <sysdeps/unix/sysv/linux/epoll_pwait.c>
