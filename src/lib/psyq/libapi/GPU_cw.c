/* 0x80026f84: BIOS 0xa0:0x49 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's GPU_cw (table A, 0x49): wait until the GPU is ready, then write `command` as one GP0 command word
 * (psx-spx). ResetGraph sends the GPU dispatch table's address masked to 24 bits, so the command byte is 0. */
void GPU_cw(u32 command) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_GPU_CW);
}
