#include "psx/libcd.h"

void def_cbread(u8 unused_event, u8* unused_result) {
    DeliverEvent(HwCdRom, EvSpDR);
}
