#include "psx/libcd.h"

void def_cbread(void) {
    DeliverEvent(HwCdRom, EvSpDR);
}
