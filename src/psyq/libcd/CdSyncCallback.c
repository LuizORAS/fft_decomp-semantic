#include "psx/libcd.h"

psyq_cd_result_callback_t CdSyncCallback(psyq_cd_result_callback_t callback) {
    psyq_cd_result_callback_t previous = g_psyq_cd_sync_callback;
    g_psyq_cd_sync_callback = callback;
    return previous;
}
