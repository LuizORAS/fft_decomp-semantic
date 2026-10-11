#include "psx/libcd.h"

psyq_cd_result_callback_t CdReadyCallback(psyq_cd_result_callback_t callback) {
    psyq_cd_result_callback_t previous = g_psyq_cd_ready_callback;
    g_psyq_cd_ready_callback = callback;
    return previous;
}
