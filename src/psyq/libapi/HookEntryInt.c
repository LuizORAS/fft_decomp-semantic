/* 0x80022024: BIOS 0xb0:0x19 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's HookEntryInt (table B, 0x19): make the BIOS's exception handler continue at the setjmp state
 * `jump_buffer` when it finishes (psx-spx). startIntr and restartIntr hook LIBETC's interrupt dispatcher
 * (trapIntr) this way. */
void HookEntryInt(void* jump_buffer) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_HOOK_ENTRY_INT);
}
