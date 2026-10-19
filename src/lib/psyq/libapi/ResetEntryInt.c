/* 0x80022014: BIOS 0xb0:0x18 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's ResetEntryInt (table B, 0x18): put back the BIOS's default exit from its exception handler,
 * removing the hook HookEntryInt installed. stopIntr calls it. */
void ResetEntryInt(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_RESET_ENTRY_INT);
}
