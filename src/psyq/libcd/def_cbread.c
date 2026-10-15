#include "psx/libcd.h"

/* Default read callback that CdInit installs: deliver EvSpDR, as def_cbready does. */
void def_cbread(void) {
    DeliverEvent(HwCdRom, EvSpDR);
}
