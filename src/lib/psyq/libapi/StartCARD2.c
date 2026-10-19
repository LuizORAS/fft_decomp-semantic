/* 0x800288a4: BIOS 0xb0:0x4b tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's StartCARD2 (table B, 0x4b): start the BIOS's memory card handling. StartCARD calls it. */
s32 StartCARD2(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_START_CARD2);
}
