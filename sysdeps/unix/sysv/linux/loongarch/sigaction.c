/* Copyright (C) 1997-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <signal.h>
#include <string.h>

#include <sys/syscall.h>
#include <sysdep.h>

/* New ports should not define the obsolete SA_RESTORER, however some
   architecture requires for compat mode and/or due old ABI.  */
#include <kernel_sigaction.h>

#ifndef SA_RESTORER
#define SET_SA_RESTORER(kact, act)
#define RESET_SA_RESTORER(act, kact)
#endif

/* SPARC passes the restore function as an argument to rt_sigaction.  */
#ifndef STUB
#define STUB(act)
#endif

#include <sys/ucontext.h>

typedef void (*__linx_sighandler_t) (int, siginfo_t *, void *);

static volatile __linx_sighandler_t saved_handlers[NSIG] = { 0 };

static void
custom_handler (int sig, siginfo_t *info, void *ucontext)
{

  struct nw_sigcontext
  {
    unsigned long long sc_pc;
    unsigned long long sc_regs[32];
    unsigned int sc_flags;
    unsigned long long sc_extcontext[0] __attribute__ ((__aligned__ (16)));
  };

  struct nw_ucontext
  {
    unsigned long uc_flags;
    struct nw_ucontext *uc_link;
    stack_t uc_stack;
    sigset_t uc_sigmask;
    unsigned char __unused[1024 / 8 - sizeof (sigset_t)];
    struct nw_sigcontext uc_mcontext;
  };

  struct nw_sctx_info
  {
    unsigned int magic;
    unsigned int size;
    unsigned long long padding; /* padding to 16 bytes */
  };

/* FPU context */
#define FPU_CTX_MAGIC 0x46505501
#define FPU_CTX_ALIGN 8
  struct fpu_context
  {
    unsigned long long regs[32];
    unsigned long long fcc;
    unsigned int fcsr;
  };
/* LSX context */
#define LSX_CTX_MAGIC 0x53580001
#define LSX_CTX_ALIGN 16
  struct lsx_context
  {
    unsigned long long regs[32][2];
    unsigned long long fcc;
    unsigned long long fcsr;
  };
/* LASX context */
#define LASX_CTX_MAGIC 0x41535801
#define LASX_CTX_ALIGN 32
  struct lasx_context
  {
    unsigned long long regs[32][4];
    unsigned long long fcc;
    unsigned long long fcsr;
  };

  struct ucontext_t ow_ctx;

  struct nw_ucontext *nw_ctx = ucontext;

  ow_ctx.__uc_flags = nw_ctx->uc_flags;
  ow_ctx.uc_link = NULL;
  ow_ctx.uc_stack = nw_ctx->uc_stack;
  ow_ctx.uc_sigmask = nw_ctx->uc_sigmask;

  ow_ctx.uc_mcontext.__pc = nw_ctx->uc_mcontext.sc_pc;
  memcpy (&ow_ctx.uc_mcontext.__gregs, &nw_ctx->uc_mcontext.sc_regs,
          sizeof (ow_ctx.uc_mcontext.__gregs));
  ow_ctx.uc_mcontext.__flags = nw_ctx->uc_mcontext.sc_flags;
  ow_ctx.uc_mcontext.__fcsr = 0;
  ow_ctx.uc_mcontext.__vcsr = 0;
  ow_ctx.uc_mcontext.__fcc = 0;
  memset (&ow_ctx.uc_mcontext.__fpregs, 0,
          sizeof (ow_ctx.uc_mcontext.__fpregs));

  int have_fpu = 0, have_lsx = 0, have_lasx = 0;
  void *fp_ctx_p = NULL;
  struct nw_sctx_info *extinfo = (void *)&nw_ctx->uc_mcontext.sc_extcontext;
  while (extinfo->magic)
    {
      switch (extinfo->magic)
        {
        case FPU_CTX_MAGIC:
          {
            struct fpu_context *fp_ctx = (void *)(extinfo + 1);
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
            struct lsx_context *fp_ctx = (void *)(extinfo + 1);
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
            struct lasx_context *fp_ctx = (void *)(extinfo + 1);
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
      extinfo = (struct nw_sctx_info *)((void *)extinfo + extinfo->size);
    }

  saved_handlers[sig](sig, info, &ow_ctx);

  if (have_lasx)
    {
      struct lasx_context *fp_ctx = fp_ctx_p;
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
      struct lsx_context *fp_ctx = fp_ctx_p;
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
      struct fpu_context *fp_ctx = fp_ctx_p;
      for (int i = 0; i < 32; i++)
        {
          fp_ctx->regs[i] = ow_ctx.uc_mcontext.__fpregs[i].__val64[0];
        }
      fp_ctx->fcc = ow_ctx.uc_mcontext.__fcc;
      fp_ctx->fcsr = ow_ctx.uc_mcontext.__fcsr;
    }
  nw_ctx->uc_mcontext.sc_flags = ow_ctx.uc_mcontext.__flags;
  memcpy (&nw_ctx->uc_mcontext.sc_regs, &ow_ctx.uc_mcontext.__gregs,
          sizeof (nw_ctx->uc_mcontext.sc_regs));
  nw_ctx->uc_mcontext.sc_pc = ow_ctx.uc_mcontext.__pc;
  nw_ctx->uc_sigmask = ow_ctx.uc_sigmask;
  nw_ctx->uc_stack = ow_ctx.uc_stack;
  nw_ctx->uc_flags = ow_ctx.__uc_flags;
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

/* If ACT is not NULL, change the action for SIG to *ACT.
   If OACT is not NULL, put the old action for SIG in *OACT.  */
int
__libc_sigaction (int sig, const struct sigaction *act, struct sigaction *oact)
{
  int result;

  struct kernel_sigaction kact, koact;

  __linx_sighandler_t orig_handler = saved_handlers[sig];

  if (act)
    {
      if (!is_fake_handler ((__linx_sighandler_t)(act)->sa_handler))
        {
          orig_handler = atomic_exchange_acq (
              &saved_handlers[sig], (__linx_sighandler_t)act->sa_handler);
          kact.k_sa_handler = (__sighandler_t)custom_handler;
        }
      else
        {
          kact.k_sa_handler = act->sa_handler;
        }
      memcpy (&kact.sa_mask, &act->sa_mask, sizeof (sigset_t));
      kact.sa_flags = act->sa_flags;
      SET_SA_RESTORER (&kact, act);
    }

  /* XXX The size argument hopefully will have to be changed to the
     real size of the user-level sigset_t.  */
  result = INLINE_SYSCALL_CALL (rt_sigaction, sig, act ? &kact : NULL,
                                oact ? &koact : NULL, STUB (act) _NW_NSIG / 8);

  if (oact && result >= 0)
    {
      if (koact.k_sa_handler == (__sighandler_t)custom_handler)
        {
          oact->sa_handler = (__sighandler_t)orig_handler;
        }
      else
        {
          oact->sa_handler = koact.k_sa_handler;
        }
      memcpy (&oact->sa_mask, &koact.sa_mask, _NW_NSIG / 8);
      memset ((void *)&oact->sa_mask + (_NW_NSIG / 8), 0,
              (_NSIG - _NW_NSIG) / 8);
      oact->sa_flags = koact.sa_flags;
      RESET_SA_RESTORER (oact, &koact);
    }
  else if (result < 0)
    {
      atomic_exchange_acq (&saved_handlers[sig], orig_handler);
    }
  return result;
}
libc_hidden_def (__libc_sigaction)

#include <nptl/sigaction.c>
