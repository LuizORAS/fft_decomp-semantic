#include "psx/cpu_abi_inline.h"
#include "psx/libcard.h"

void StartCARD(void) {
    EnterCriticalSection();
    StartCARD2();
    /* Retail selects mode 0; SDK 4.7 documentation describes mode 1. */
    ChangeClearPAD(0);
    ExitCriticalSection();
}
