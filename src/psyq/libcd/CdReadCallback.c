#include "psx/libcd.h"

psyq_cd_result_callback_t CdReadCallback(psyq_cd_result_callback_t callback) {
    psyq_cd_result_callback_t previous = g_psyq_cd_read_callback;
    g_psyq_cd_read_callback = callback;
    return previous;
}
