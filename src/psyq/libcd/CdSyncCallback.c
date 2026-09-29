#include "psx/libcd.h"

void* CdSyncCallback(void* callback) {
    void* previous = g_psyq_cd_sync_callback;
    g_psyq_cd_sync_callback = callback;
    return previous;
}
