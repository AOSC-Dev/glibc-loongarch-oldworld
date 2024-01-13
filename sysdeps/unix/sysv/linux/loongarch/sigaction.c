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

#if IS_IN(libc) && (SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_36) || OTHER_SHLIB_COMPAT(libpthread, GLIBC_2_0, GLIBC_2_34))
#include <sys/mman.h>
#include <assert.h>
#if !IS_IN(rtld)
#include <libc-lock.h>
#endif
#include "ow_ucontext.h"

typedef void (*__linx_sighandler_t) (int, siginfo_t *, void *);

static void
custom_handler (int sig, siginfo_t *info, void *ucontext,
                __linx_sighandler_t real_handler)
{
    struct __ow_ucontext_t ow_ctx;

    struct ucontext_t *nw_ctx = ucontext;

    __copy_nw_ow(&ow_ctx, nw_ctx);
    int have_fpu = 0, have_lsx = 0, have_lasx = 0;
    void *fp_ctx_p = NULL;
    struct __nw_sctx_info *extinfo = (void *)&nw_ctx->uc_mcontext.__extcontext;
    while (extinfo->magic)
    {
        switch (extinfo->magic)
        {
            case FPU_CTX_MAGIC:
            {
                struct __nw_fpu_context *fp_ctx = (void *)(extinfo + 1);
                have_fpu = 1;
                fp_ctx_p = fp_ctx;
                ow_ctx.uc_mcontext.__fcsr = fp_ctx->fcsr;
                ow_ctx.uc_mcontext.__fcc = fp_ctx->fcc;
                for (int i = 0; i < 32; i++)
                {
                    ow_ctx.uc_mcontext.__fpregs[i].__val64[0] = fp_ctx->regs[i];
                }
                break;
            }
            case LSX_CTX_MAGIC:
            {
                struct __nw_lsx_context *fp_ctx = (void *)(extinfo + 1);
                have_lsx = 1;
                fp_ctx_p = fp_ctx;
                ow_ctx.uc_mcontext.__fcsr = fp_ctx->fcsr;
                ow_ctx.uc_mcontext.__fcc = fp_ctx->fcc;
                for (int i = 0; i < 32; i++)
                {
                    ow_ctx.uc_mcontext.__fpregs[i].__val64[0] = fp_ctx->regs[i][0];
                    ow_ctx.uc_mcontext.__fpregs[i].__val64[1] = fp_ctx->regs[i][1];
                }
                break;
            }
            case LASX_CTX_MAGIC:
            {
                struct __nw_lasx_context *fp_ctx = (void *)(extinfo + 1);
                have_lasx = 1;
                fp_ctx_p = fp_ctx;
                ow_ctx.uc_mcontext.__fcsr = fp_ctx->fcsr;
                ow_ctx.uc_mcontext.__fcc = fp_ctx->fcc;
                for (int i = 0; i < 32; i++)
                {
                    ow_ctx.uc_mcontext.__fpregs[i].__val64[0] = fp_ctx->regs[i][0];
                    ow_ctx.uc_mcontext.__fpregs[i].__val64[1] = fp_ctx->regs[i][1];
                    ow_ctx.uc_mcontext.__fpregs[i].__val64[2] = fp_ctx->regs[i][2];
                    ow_ctx.uc_mcontext.__fpregs[i].__val64[3] = fp_ctx->regs[i][3];
                }
                break;
            }
        }
        if (have_fpu || have_lsx || have_lasx)
        {
            break;
        }
        extinfo = (struct __nw_sctx_info *)((void *)extinfo + extinfo->size);
    }

    real_handler (sig, info, &ow_ctx);

    if (have_lasx)
    {
        struct __nw_lasx_context *fp_ctx = fp_ctx_p;
        for (int i = 0; i < 32; i++)
        {
            fp_ctx->regs[i][0] = ow_ctx.uc_mcontext.__fpregs[i].__val64[0];
            fp_ctx->regs[i][1] = ow_ctx.uc_mcontext.__fpregs[i].__val64[1];
            fp_ctx->regs[i][2] = ow_ctx.uc_mcontext.__fpregs[i].__val64[2];
            fp_ctx->regs[i][3] = ow_ctx.uc_mcontext.__fpregs[i].__val64[3];
        }
        fp_ctx->fcc = ow_ctx.uc_mcontext.__fcc;
        fp_ctx->fcsr = ow_ctx.uc_mcontext.__fcsr;
    }
    else if (have_lsx)
    {
        struct __nw_lsx_context *fp_ctx = fp_ctx_p;
        for (int i = 0; i < 32; i++)
        {
            fp_ctx->regs[i][0] = ow_ctx.uc_mcontext.__fpregs[i].__val64[0];
            fp_ctx->regs[i][1] = ow_ctx.uc_mcontext.__fpregs[i].__val64[1];
        }
        fp_ctx->fcc = ow_ctx.uc_mcontext.__fcc;
        fp_ctx->fcsr = ow_ctx.uc_mcontext.__fcsr;
    }
    else if (have_fpu)
    {
        struct __nw_fpu_context *fp_ctx = fp_ctx_p;
        for (int i = 0; i < 32; i++)
        {
            fp_ctx->regs[i] = ow_ctx.uc_mcontext.__fpregs[i].__val64[0];
        }
        fp_ctx->fcc = ow_ctx.uc_mcontext.__fcc;
        fp_ctx->fcsr = ow_ctx.uc_mcontext.__fcsr;
    }
    __copy_ow_nw (nw_ctx, &ow_ctx);
}

static inline int
is_fake_handler (__linx_sighandler_t handler)
{
  return handler == (__linx_sighandler_t)SIG_ERR
         || handler == (__linx_sighandler_t)SIG_DFL
         || handler == (__linx_sighandler_t)SIG_IGN
#ifdef SIG_HOLD
         || handler == (__linx_sighandler_t)SIG_HOLD
#endif
      ;
}


#define LA_INS_PCADDI 0x18000000u
#define LA_INS_LD_D 0x28C00000u
#define LA_INS_JIRL 0x4C000000u
#define LA_REG_ZERO 0
#define LA_REG_T0 12
#define LA_REG_RA 1
#define LA_REG_A0 4
#define LA_REG_A1 5
#define LA_REG_A2 6
#define LA_REG_A3 7
#define LA_REG_A4 8
#define LA_REG_A5 9

#define SIGHANDLER_PROG_NR_INS 4
struct __attribute__ ((__packed__, aligned (8))) sighandler_prog
{
  unsigned int magic[2];
  unsigned int prog[SIGHANDLER_PROG_NR_INS];

  struct sighandler_prog_data
  {
    unsigned long our_handler_addr;
    unsigned long orig_handler_addr;
  } data;
} static const prog_tmpl = {
  .magic = {U"开刀"},
  .prog = {
    /* pcaddi $t0, SIGHANDLER_PROG_NR_INS */
    LA_INS_PCADDI | LA_REG_T0 | (SIGHANDLER_PROG_NR_INS << 5),
    /* ld.d $a3, $t0, orig_handler_addr */
    LA_INS_LD_D | LA_REG_A3 | (LA_REG_T0 << 5) | (offsetof (struct sighandler_prog_data, orig_handler_addr) << 10),
    /* ld.d $t0, $t0, our_handler_addr */
    LA_INS_LD_D | LA_REG_T0 | (LA_REG_T0 << 5) | (offsetof (struct sighandler_prog_data, our_handler_addr) << 10),
    /* jirl $zero, $t0, 0 */
    LA_INS_JIRL | LA_REG_ZERO | (LA_REG_T0 << 5) | (0 << 10),
  },
  .data = {
    .our_handler_addr = (unsigned long)custom_handler,
  }
};


#define PROG_POOL_SIZE 2
#define LA_PAGE_SIZE (16 * 1024)

static_assert (sizeof (struct sighandler_prog) <= LA_PAGE_SIZE,
               "sighandler_prog too large");
static_assert (SIGHANDLER_PROG_NR_INS < (1u << 19), "prog too long");
static_assert (offsetof (struct sighandler_prog, data.our_handler_addr)
                   == offsetof (struct sighandler_prog, prog)
                          + sizeof (unsigned int) * SIGHANDLER_PROG_NR_INS,
               "data offset wrong");
static_assert (offsetof (struct sighandler_prog, data.our_handler_addr)
                       % sizeof (unsigned long)
                   == 0,
               "data offset not aligned");
static_assert (offsetof (struct sighandler_prog_data, orig_handler_addr)
                   < (1u << 11),
               "data too long");
static_assert (offsetof (struct sighandler_prog_data, our_handler_addr)
                   < (1u << 11),
               "data too long");


struct sighandler_prog_pool
{
  struct sighandler_prog *prog[PROG_POOL_SIZE];
  size_t pos;
#if !IS_IN(rtld)
  __libc_lock_define (, lock);
#endif
};

static void
destroy_handler (struct sighandler_prog *prog)
{
  __munmap (prog, LA_PAGE_SIZE);
}


static struct sighandler_prog *
alloc_handler (__linx_sighandler_t handler)
{
  struct sighandler_prog *prog
      = __mmap (NULL, LA_PAGE_SIZE, PROT_READ | PROT_WRITE,
                MAP_ANON | MAP_PRIVATE, -1, 0);
  if (__glibc_unlikely (prog == MAP_FAILED))
    {
      return NULL;
    }
  memcpy (prog, &prog_tmpl, sizeof (struct sighandler_prog));
  prog->data.orig_handler_addr = (unsigned long)handler;

  int rc = __mprotect (prog, LA_PAGE_SIZE, PROT_READ | PROT_EXEC);
  if (rc < 0)
    {
      destroy_handler (prog);
      prog = NULL;
    }
  return prog;
}


static void
store_handler (struct sighandler_prog_pool *pool, struct sighandler_prog *prog)
{
  struct sighandler_prog *orig_prog;
  orig_prog = pool->prog[pool->pos];
  pool->prog[pool->pos] = prog;
  pool->pos = (pool->pos + 1) % PROG_POOL_SIZE;
  if (orig_prog)
    {
      destroy_handler (orig_prog);
    }
}

int __ow___sigaction (int sig, const struct sigaction *act, struct sigaction *oact)
{
    static struct sighandler_prog_pool prog_pool[NSIG]
      = { [0 ... NSIG - 1] = {
              .prog = { NULL },
              .pos = 0,
#if !IS_IN(rtld)
              .lock = _LIBC_LOCK_INITIALIZER,
#endif
          } };

    sigset_t saveset, allset;
    struct sighandler_prog *old_prog = NULL;
    struct sighandler_prog *new_prog = NULL;
    int result = 0, result2 = 0;
    struct sigaction iact, ioact;

    if (sig <= 0 || sig >= __OW_NSIG || is_internal_signal (sig))
    {
        __set_errno (EINVAL);
        return -1;
    }

    if (sig >= NSIG)
    {
        if (oact)
        {
            memset (oact, 0, sizeof (struct sigaction));
            oact->sa_handler = SIG_IGN;
        }
        return 0;
    }

    if (act)
    {
        if (!is_fake_handler (act->sa_sigaction))
        {
            new_prog = alloc_handler (act->sa_sigaction);
            if (!new_prog)
            {
              return -1;
            }
            iact.sa_sigaction = (__linx_sighandler_t)new_prog;
        }
        else
        {
            iact.sa_sigaction = act->sa_sigaction;
        }
        memcpy (&iact.sa_mask, &act->sa_mask, sizeof (sigset_t));
        iact.sa_flags = act->sa_flags;
    }
    if (new_prog)
    {
        __sigfillset (&allset);
        result = INLINE_SYSCALL_CALL (rt_sigprocmask, SIG_BLOCK, &allset,
                                    &saveset, NSIG / 8);
        if (result < 0)
        {
            destroy_handler (new_prog);
            return result;
        }

#if !IS_IN(rtld)
        __libc_lock_lock (prog_pool[sig].lock);
#endif
    }
    result = __libc_sigaction (sig, act ? &iact : NULL, oact ? &ioact : NULL);
    if (result < 0)
    {
        if (new_prog)
        {
            destroy_handler (new_prog);
        }
        goto out_unlock;
    }
    if (oact)
    {
        oact->sa_sigaction = ioact.sa_sigaction;
        if (!is_fake_handler (ioact.sa_sigaction))
        {
            old_prog = (struct sighandler_prog *)ioact.sa_sigaction;
            if (memcmp (old_prog->magic, prog_tmpl.magic,
                      sizeof (prog_tmpl.magic))
              == 0)
            {
                oact->sa_sigaction
                    = (__linx_sighandler_t)old_prog->data.orig_handler_addr;
            }
        }
        memcpy (&oact->sa_mask, &ioact.sa_mask, NSIG / 8);
        __ow_clear_unused_bits (&oact->sa_mask);
        oact->sa_flags = ioact.sa_flags;
    }
    if (new_prog)
        store_handler (&prog_pool[sig], new_prog);

out_unlock:
    if (new_prog)
    {
#if !IS_IN(rtld)
        __libc_lock_unlock (prog_pool[sig].lock);
#endif

        result2 = INLINE_SYSCALL_CALL (rt_sigprocmask, SIG_SETMASK, &saveset,
                                       NULL, NSIG / 8);
    if (result2 < 0)
        result = result2;
    }
    return result;
}

#if SHLIB_COMPAT (libc, GLIBC_2_0, GLIBC_2_36)
compat_symbol (libc, __ow___sigaction, sigaction, GLIBC_2_0);
compat_symbol (libc, __ow___sigaction, __sigaction, GLIBC_2_0);
#endif

#if OTHER_SHLIB_COMPAT(libpthread, GLIBC_2_0, GLIBC_2_34)
compat_symbol (libpthread, __ow___sigaction, sigaction, GLIBC_2_0);
compat_symbol (libpthread, __ow___sigaction, __sigaction, GLIBC_2_0);
#endif
#endif
