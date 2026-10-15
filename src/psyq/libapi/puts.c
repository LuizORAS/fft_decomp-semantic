/* 0x8002228c: BIOS 0xb0:0x3f tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's puts (table B, 0x3f): print `text` on the BIOS's debug console. LIBCD's traces and timeouts and
 * v_wait's timeout message use it. */
s32 puts(const char* text) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_PUTS);
}
