#include "psx/cpu_abi_inline.h"
#include "psx/libcard.h"

/* Leaves the return value unspecified. */
s32 StartCARD(void) {
    EnterCriticalSection();
    StartCARD2();
    ChangeClearPAD(0);
    ExitCriticalSection();
}
