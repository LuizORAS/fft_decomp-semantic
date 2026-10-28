#include "psx/libcd.h"

/* Default sync callback that CdInit installs: deliver the CD-ROM event EvSpCOMP (a command completed). */
void def_cbsync(u8 unused_event, u8* unused_result) {
    DeliverEvent(HwCdRom, EvSpCOMP);
}
