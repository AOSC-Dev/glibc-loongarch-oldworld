/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1999 by Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#ifndef _BITS_SIGCONTEXT_H
#define _BITS_SIGCONTEXT_H

#include <asm/sgidefs.h>

/*
 * Keep this struct definition in sync with the sigcontext fragment
 * in arch/mips/kernel/asm-offsets.c
 *
 * Warning: this structure illdefined with sc_badvaddr being just an unsigned
 * int so it was changed to unsigned long in 2.6.0-test1.  This may break
 * binary compatibility - no prisoners.
 * DSP ASE in 2.6.12-rc4.  Turn sc_mdhi and sc_mdlo into an array of four
 * entries, add sc_dsp and sc_reserved for padding.  No prisoners.
 */
union fpureg {
    unsigned int   sc_val32[256 / 32];
    unsigned long long    sc_val64[256 / 64];
};

struct sigcontext {
    unsigned long long   sc_pc;
    unsigned long long    sc_regs[32];
    unsigned int   sc_flags;

    unsigned int   sc_fcsr;
    unsigned int   sc_vcsr;
    unsigned long long    sc_fcc;
    union fpureg    sc_fpregs[32] __attribute__((__aligned__ (32)));

    unsigned int   sc_reserved;
};


#endif /* _BITS_SIGCONTEXT_H */
