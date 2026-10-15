#include "psx/libcd.h"

/* Default ready callback that CdInit installs: deliver the CD-ROM event EvSpDR (data ready). */
void def_cbready(void) {
    DeliverEvent(HwCdRom, EvSpDR);
}
