#include "psx/libcd.h"

void* CdReadyCallback(void* callback) {
    void* previous = g_psyq_cd_ready_callback;
    g_psyq_cd_ready_callback = callback;
    return previous;
}
