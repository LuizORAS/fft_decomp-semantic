#include "psx/cpu_abi_inline.h"
#include "psx/libcard.h"

/* Start the memory card library in mode `mode` (the BIOS's InitCARD2) with the pad-clear setting off,
 * and apply the two BIOS patches (_patch_card, _patch_card2), in a critical section. main_card_init_events
 * calls it. */
void InitCARD(s32 mode) {
    ChangeClearPAD(0);
    EnterCriticalSection();
    InitCARD2(mode);
    _patch_card();
    _patch_card2();
    ExitCriticalSection();
}
