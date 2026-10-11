#include "psx/libcd.h"

void def_cbready(u8 unused_event, u8* unused_result) {
    DeliverEvent(HwCdRom, EvSpDR);
}
