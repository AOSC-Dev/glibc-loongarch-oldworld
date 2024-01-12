ifeq ($(subdir),stdlib)
gen-as-const-headers += ucontext_i.sym
endif

ifeq ($(subdir),signal)
sysdep_routines += \
  ow_sigisempty \
  ow_sigismem \
  ow_sigaddset \
  ow_sigfillset \
  ow_sigempty \
  ow_sigdelset \
  # sysdep_routines
endif

abi-variants := lp64s lp64d

ifeq (,$(filter $(default-abi),$(abi-variants)))
$(error Unknown ABI $(default-abi), must be one of $(abi-variants))
endif

abi-lp64s-condition	:= __WORDSIZE == 64 && defined __loongarch_soft_float
abi-lp64d-condition	:= __WORDSIZE == 64 && defined __loongarch_double_float
