#include "psx/libcd.h"

/* Default read callback that CdInit installs: deliver EvSpDR, as def_cbready does. */
void def_cbread(u8 unused_event, u8* unused_result) {
    DeliverEvent(HwCdRom, EvSpDR);
}
