#ifndef _OW_UCONTEXT_H
#define _OW_UCONTEXT_H 1
#include <sys/ucontext.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include "ow_sigop.h"

union __ow_loongarch_mc_fp_state {
    unsigned int   __val32[256 / 32];
    unsigned long long   __val64[256 / 64];
};

typedef struct __ow_mcontext_t {
    unsigned long long   __pc;
    unsigned long long   __gregs[32];
    unsigned int   __flags;

    unsigned int   __fcsr;
    unsigned int   __vcsr;
    unsigned long long   __fcc;
    union __ow_loongarch_mc_fp_state    __fpregs[32] __attribute__((__aligned__ (32)));

    unsigned int   __reserved;
} __ow_mcontext_t;

/* Userlevel context.  */
typedef struct __ow_ucontext_t
  {
    unsigned long int  __uc_flags;
    struct __ow_ucontext_t *uc_link;
    stack_t            uc_stack;
    __ow_mcontext_t    uc_mcontext;
    sigset_t           uc_sigmask;
  } __ow_ucontext_t;

struct __nw_sctx_info
  {
    unsigned int magic;
    unsigned int size;
    unsigned long long padding; /* padding to 16 bytes */
  };

/* FPU context */
#define FPU_CTX_MAGIC 0x46505501
#define FPU_CTX_ALIGN 8
struct __nw_fpu_context
  {
    unsigned long long regs[32];
    unsigned long long fcc;
    unsigned int fcsr;
  };
/* LSX context */
#define LSX_CTX_MAGIC 0x53580001
#define LSX_CTX_ALIGN 16
struct __nw_lsx_context
  {
    unsigned long long regs[32][2];
    unsigned long long fcc;
    unsigned long long fcsr;
  };
/* LASX context */
#define LASX_CTX_MAGIC 0x41535801
#define LASX_CTX_ALIGN 32
struct __nw_lasx_context
  {
    unsigned long long regs[32][4];
    unsigned long long fcc;
    unsigned long long fcsr;
  };

static inline void __copy_nw_ow (__ow_ucontext_t *ow_ctx, const ucontext_t *nw_ctx){
  ow_ctx->__uc_flags = nw_ctx->__uc_flags;
  ow_ctx->uc_link = NULL;
  ow_ctx->uc_stack = nw_ctx->uc_stack;
  ow_ctx->uc_sigmask = nw_ctx->uc_sigmask;
  __ow_clear_unused_bits (&ow_ctx->uc_sigmask);

  ow_ctx->uc_mcontext.__pc = nw_ctx->uc_mcontext.__pc;
  static_assert (sizeof (ow_ctx->uc_mcontext.__gregs) ==
                 sizeof (nw_ctx->uc_mcontext.__gregs),
                 "gregs size mismatch");
  memcpy (&ow_ctx->uc_mcontext.__gregs, &nw_ctx->uc_mcontext.__gregs,
          sizeof (ow_ctx->uc_mcontext.__gregs));
  ow_ctx->uc_mcontext.__flags = nw_ctx->uc_mcontext.__flags;
  ow_ctx->uc_mcontext.__fcsr = 0;
  ow_ctx->uc_mcontext.__vcsr = 0;
  ow_ctx->uc_mcontext.__fcc = 0;
  memset (&ow_ctx->uc_mcontext.__fpregs, 0,
          sizeof (ow_ctx->uc_mcontext.__fpregs));
  ow_ctx->uc_mcontext.__reserved = 0;
}

static inline void __copy_ow_nw (ucontext_t *nw_ctx, const __ow_ucontext_t *ow_ctx){
  nw_ctx->uc_mcontext.__flags = ow_ctx->uc_mcontext.__flags;
  memcpy (&nw_ctx->uc_mcontext.__gregs, &ow_ctx->uc_mcontext.__gregs,
          sizeof (nw_ctx->uc_mcontext.__gregs));
  nw_ctx->uc_mcontext.__pc = ow_ctx->uc_mcontext.__pc;
  nw_ctx->uc_sigmask = ow_ctx->uc_sigmask;
  __ow_clear_unused_bits (&nw_ctx->uc_sigmask);
  nw_ctx->uc_stack = ow_ctx->uc_stack;
  nw_ctx->__uc_flags = ow_ctx->__uc_flags;
}

#endif
