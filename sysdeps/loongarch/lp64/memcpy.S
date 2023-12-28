#ifdef _LIBC
#include <sysdep.h>
#include <sys/regdef.h>
#include <sys/asm.h>
#else
#include <regdef.h>
#include <sys/asm.h>
#endif

/* Allow the routine to be named something else if desired.  */
#ifndef MEMCPY_NAME
#define MEMCPY_NAME memcpy
#endif

#define LD_64(reg, n) \
	ld.d	t0, reg, n;    \
	ld.d	t1, reg, n+8;  \
	ld.d	t2, reg, n+16; \
	ld.d	t3, reg, n+24; \
	ld.d	t4, reg, n+32; \
	ld.d	t5, reg, n+40; \
	ld.d	t6, reg, n+48; \
	ld.d	t7, reg, n+56; 


#define ST_64(reg, n) \
	st.d	t0, reg, n;    \
	st.d	t1, reg, n+8;  \
	st.d	t2, reg, n+16; \
	st.d	t3, reg, n+24; \
	st.d	t4, reg, n+32; \
	st.d	t5, reg, n+40; \
	st.d	t6, reg, n+48; \
	st.d	t7, reg, n+56; 
	

#ifdef ANDROID_CHANGES
LEAF(MEMCPY_NAME, 0)
#else
LEAF(MEMCPY_NAME)
#endif

//1st var: dest ptr: void *str1 $r4
//2nd var: src  ptr: void *str2 $r5
//3rd var: size_t num
//t0~t9 registers as temp

	add.d	a4, a1, a2
	add.d	a3, a0, a2
	move	t8, a0
	srai.d	a6, a2, 4  		#num/16
	beqz	a6, less_16bytes        #num<16
	nop
	srai.d	a6, a2, 8  		#num/256
	bnez	a6, more_256bytes       #num>256
	nop
	srai.d	a6, a2, 7  
	beqz	a6, less_128bytes	#num<128
	nop

	LD_64(a1, 0)			#128<num<256
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)
	LD_64(a4, -128)
	ST_64(a3, -128)
	LD_64(a4, -64)
	ST_64(a3, -64)

	jr	ra
	nop

less_128bytes:
	srai.d	a6, a2, 6  #num/64
	beqz	a6, less_64bytes
	nop

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a4, -64)
	ST_64(a3, -64)
	
	jr	ra
	nop

less_64bytes:
	srai.d	a6, a2, 5 #num/32
	beqz	a6, less_32bytes
	nop
	
	ld.d	t0, a1, 0
	ld.d	t1, a1, 8
	ld.d	t2, a1, 16
	ld.d	t3, a1, 24
	ld.d	t4, a4, -32
	ld.d	t5, a4, -24
	ld.d	t6, a4, -16
	ld.d	t7, a4, -8
	st.d	t0, a0, 0
	st.d	t1, a0, 8
	st.d	t2, a0, 16
	st.d	t3, a0, 24
	st.d	t4, a3, -32
	st.d	t5, a3, -24
	st.d	t6, a3, -16
	st.d	t7, a3, -8

	jr	ra
	nop

less_32bytes:
	ld.d	t0, a1, 0
	ld.d	t1, a1, 8
	ld.d	t2, a4, -16
	ld.d	t3, a4, -8
	st.d	t0, a0, 0
	st.d	t1, a0, 8
	st.d	t2, a3, -16
	st.d	t3, a3, -8

	jr	ra
	nop

less_16bytes:
	srai.d	a6, a2, 3 #num/8
	beqz	a6, less_8bytes
	nop
	
	ld.d	t0, a1, 0
	ld.d	t1, a4, -8
	st.d	t0, a0, 0
	st.d	t1, a3, -8

	jr	ra
	nop

less_8bytes:
	srai.d	a6, a2, 2
	beqz	a6, less_4bytes
	nop

	ld.w	t0, a1, 0
	ld.w	t1, a4, -4
	st.w	t0, a0, 0
	st.w	t1, a3, -4

	jr	ra
	nop
	
less_4bytes:
	srai.d	a6, a2, 1
	beqz	a6, less_2bytes
	nop
	
	ld.h	t0, a1, 0
	ld.h	t1, a4, -2
	st.h	t0, a0, 0
	st.h	t1, a3, -2

	jr	ra
	nop

less_2bytes:
	beqz	a2, less_1bytes
	nop

	ld.b	t0, a1, 0
	st.b	t0, a0, 0

	jr	ra
	nop

less_1bytes:
	jr	ra
	nop

more_256bytes:
	srli.d	a0, a0, 3
	slli.d	a0, a0, 3
	addi.d	a0, a0,  0x8
	sub.d	a7, t8,  a0
	ld.d	t0, a1, 0
	sub.d	a1,  a1,  a7
	st.d	t0, t8, 0

	LD_64(a4, -128)
	ST_64(a3, -128)
	LD_64(a4, -64)
	ST_64(a3, -64)

	add.d	a7, a7, a2
	addi.d	a7, a7, -0x80     
	srai.d	a6, a7, 22    
	bnez	a6, loop_most
	nop
	srai.d	a6, a7, 12
	beqz	a6, loop_less
	nop

loop_more:
	addi.d	a5, zero, 4
loop_in_1:
	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#1
	addi.d	a1, a1, 128	#1

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#2
	addi.d	a1, a1, 128	#2

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#3
	addi.d	a1, a1, 128	#3

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#4
	addi.d	a1, a1, 128	#4

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#5
	addi.d	a1, a1, 128	#5

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#6
	addi.d	a1, a1, 128	#6

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#7
	addi.d	a1, a1, 128	#7

	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)

	addi.d	a0, a0, 128	#8
	addi.d	a1, a1, 128	#8

	addi.d	a5, a5, -1
	bnez	a5, loop_in_1
	nop
	addi.d	a6, a6, -1
	bnez	a6, loop_more
	nop

 	lu12i.w t4, 1
  	addi.d 	t4, t4, -1 
  	and 	a7, a7, t4
	
	b	loop_less
	nop

loop_most:
	srai.d	a6, a7, 12
	lu12i.w	a3, 1
	add.d	a2, a3, a0
	add.d	a3, a3, a1

loop_most_loop:
	addi.d	a5, zero, 4
loop_in_2:
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #1
	addi.d	a1,  a1,   128   #1
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #2
	addi.d	a1,  a1,   128   #2
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #3
	addi.d	a1,  a1,   128   #3
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #4
	addi.d	a1,  a1,   128   #4
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #5
	addi.d	a1,  a1,   128   #5
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #6
	addi.d	a1,  a1,   128   #6
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #7
	addi.d	a1,  a1,   128   #7
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	LD_64(a1, 0)
	preld	0, a3, 0
	ST_64(a0, 0)
	preld	8, a2, 0
	LD_64(a1, 64)
	preld	0, a3, 64
	ST_64(a0, 64)
	preld	8, a2, 64
	addi.d	a0,  a0,   128   #8
	addi.d	a1,  a1,   128   #8
	addi.d  a2,  a2,   128
   	addi.d  a3,  a3,   128
	addi.d	a5, a5, -1
	bnez	a5, loop_in_2
	nop
	addi.d	a6, a6, -1
	bnez	a6, loop_most_loop
	nop

 	lu12i.w t4, 1
  	addi.d 	t4, t4, -1 
  	and 	a7, a7, t4

loop_less:
	LD_64(a1, 0)
	ST_64(a0, 0)
	LD_64(a1, 64)
	ST_64(a0, 64)
	addi.d	a0,  a0,   0x80
	addi.d	a1,  a1,   0x80
	addi.d	a7,  a7, -0x80
	slt	a6, a7, zero
	beqz	a6, loop_less
	nop
	move	v0, t8
	jr	ra
	nop

END(MEMCPY_NAME)
#ifndef ANDROID_CHANGES
#ifdef _LIBC
libc_hidden_builtin_def (MEMCPY_NAME)
#endif
#endif
