#include "psx/libcd.h"

void def_cbsync(u8 unused_event, u8* unused_result) {
    DeliverEvent(HwCdRom, EvSpCOMP);
}
