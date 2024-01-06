ifeq ($(subdir),elf)
sysdep_routines	+= dl-vdso
ifeq ($(build-shared),yes)
# This is needed for DSO loading from static binaries.
sysdep-dl-routines += dl-static
endif
endif

#ifeq ($(subdir),misc)
#sysdep_headers += sys/cachectl.h
#sysdep_routines += flush-icache
#endif

ifeq ($(subdir),stdlib)
gen-as-const-headers += ucontext_i.sym
endif

ifeq ($(subdir),io)
sysdep_routines += statx_cp
endif
