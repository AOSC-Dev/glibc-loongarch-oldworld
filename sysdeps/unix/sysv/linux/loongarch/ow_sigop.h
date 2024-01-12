#include <signal.h>

extern int __ow_sigaddset(sigset_t *, int);
extern int __ow_sigdelset(sigset_t *, int);
extern int __ow_sigemptyset(sigset_t *);
extern int __ow_sigfillset(sigset_t *);
extern int __ow_sigandset (sigset_t *__set, const sigset_t *__left, const sigset_t *__right);
extern int __ow_sigorset (sigset_t *__set, const sigset_t *__left, const sigset_t *__right);
