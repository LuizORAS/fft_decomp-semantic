#include "psx/libcd.h"

void def_cbready(void) {
    DeliverEvent(HwCdRom, EvSpDR);
}
