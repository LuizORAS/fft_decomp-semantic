#ifndef PSX_CPU_RETURN_ADDRESS_ABI_H
#define PSX_CPU_RETURN_ADDRESS_ABI_H
/* These handwritten entries return through architectural RA without a C frame. */
register void* psyq_cpu_return_address __asm__("$31");
#endif
