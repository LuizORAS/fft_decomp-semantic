#include "psx/libcd.h"

void def_cbsync(void) {
    DeliverEvent(HwCdRom, EvSpCOMP);
}
