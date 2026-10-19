#include "psx/libcard.h"

/* Start the memory card interrupts (the BIOS's StartCARD2) with the pad-clear setting off, in a critical
 * section. main_card_init_events calls it. */
s32 StartCARD(void) {
    EnterCriticalSection();
    StartCARD2();
    /* Retail selects mode 0; SDK 4.7 documentation describes mode 1. */
    ChangeClearPAD(0);
    ExitCriticalSection();
}
