#include "psx/libcd.h"

/* Default ready callback that CdInit installs: deliver the CD-ROM event EvSpDR (data ready). */
void def_cbready(u8 unused_event, u8* unused_result) {
    DeliverEvent(HwCdRom, EvSpDR);
}
