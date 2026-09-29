#include "psx/libcd.h"

void* CdReadCallback(void* callback) {
    void* previous = g_psyq_cd_read_callback;
    g_psyq_cd_read_callback = callback;
    return previous;
}
