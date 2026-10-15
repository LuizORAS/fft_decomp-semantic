#include "psx/libcd.h"

/* Default sync callback that CdInit installs: deliver the CD-ROM event EvSpCOMP (a command completed). */
void def_cbsync(void) {
    DeliverEvent(HwCdRom, EvSpCOMP);
}
