#include "psx/libcard.h"

/* Stop the memory card library (the BIOS's StopCARD2), swap the pad-clear patch back and restore the
 * exception handler's instructions. Nothing in the game calls it. */
s32 StopCARD(void) {
    StopCARD2();
    _patch_card2();
    psyq_card_restore_exception_prefix();
}
