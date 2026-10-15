#include "psx/cpu_abi_inline.h"
#include "psx/libcard.h"

/* Stop the memory card library (the BIOS's StopCARD2), swap the pad-clear patch back and restore the
 * exception handler's words. Nothing in the game calls it. */
void StopCARD(void) {
    StopCARD2();
    _patch_card2();
    psyq_card_restore_exception_prefix();
}
