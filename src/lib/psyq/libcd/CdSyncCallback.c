#include "psx/libcd.h"

/* Set the routine called when a command completes and return the previous one. */
CdlCB CdSyncCallback(CdlCB callback) {
    CdlCB previous = CD_cbsync;
    CD_cbsync = callback;
    return previous;
}
