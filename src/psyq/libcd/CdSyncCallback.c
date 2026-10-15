#include "psx/libcd.h"

/* Set the routine called when a command completes and return the previous one. */
void* CdSyncCallback(void* callback) {
    void* previous = g_psyq_cd_sync_callback;
    g_psyq_cd_sync_callback = callback;
    return previous;
}
