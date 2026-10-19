#include "psx/libcd.h"

/* Set the routine called when the drive has data ready and return the previous one. */
CdlCB CdReadyCallback(CdlCB callback) {
    CdlCB previous = CD_cbready;
    CD_cbready = callback;
    return previous;
}
