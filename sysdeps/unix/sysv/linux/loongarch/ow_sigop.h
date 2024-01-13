#include <signal.h>
#include <string.h>

extern int __ow_sigaddset(sigset_t *, int);
extern int __ow_sigdelset(sigset_t *, int);
extern int __ow_sigemptyset(sigset_t *);
extern int __ow_sigfillset(sigset_t *);
extern int __ow_sigandset (sigset_t *__set, const sigset_t *__left, const sigset_t *__right);
extern int __ow_sigorset (sigset_t *__set, const sigset_t *__left, const sigset_t *__right);

#define __OW_NSIG 128

static inline void __ow_clear_unused_bits(sigset_t *set)
{
    memset ((void *)set + (NSIG / 8), 0, (__OW_NSIG - NSIG) / 8);
}
