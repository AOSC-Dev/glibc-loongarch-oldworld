#include<shlib-compat.h>
#include<signal.h>

#define sigemptyset __nw_sigemptyset
#undef libc_hidden_def
#define libc_hidden_def(name)
#include<signal/sigempty.c>

#undef sigemptyset
versioned_symbol(libc, __nw_sigemptyset, sigemptyset, GLIBC_2_36);
libc_hidden_ver(__nw_sigemptyset, sigemptyset)
