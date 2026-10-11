#include "psx/libcd.h"

CdlCB CdSyncCallback(CdlCB callback) {
    CdlCB previous = g_psyq_cd_sync_callback;
    g_psyq_cd_sync_callback = callback;
    return previous;
}
