#include "psx/abi_inline.h"

/* BIOS consumes the original argument registers and caller stack unchanged.
 * Its public variadic prototype in psx/libc.h is intentionally omitted here:
 * a variadic C definition adds four stores absent from this ABI tail veneer. */
/* The BIOS's printf (table A, 0x3f): print a formatted message on the BIOS's debug console (psx-spx). Only
 * the libraries' error and trace messages use it (LIBCD, LIBETC, LIBGPU, LIBGS, LIBSPU, the MDEC reset).
 *
 * The BIOS consumes the original argument registers and caller stack unchanged.
 * Its public variadic prototype in psx/libc.h is intentionally omitted here:
 * a variadic C definition adds four stores absent from this ABI tail veneer. */
int printf() {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_PRINTF);
}
