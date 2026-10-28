/* 0x80022004: BIOS 0xb0:0x17 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's ReturnFromException (table B, 0x17): restore the registers the exception saved and resume the
 * interrupted code. trapIntr ends with it. */
void ReturnFromException(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_RETURN_FROM_EXCEPTION);
}
