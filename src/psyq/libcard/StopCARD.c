#include "psx/cpu_abi_inline.h"
#include "psx/libcard.h"

void StopCARD(void) {
    StopCARD2();
    _patch_card2();
    psyq_card_restore_exception_prefix();
}
