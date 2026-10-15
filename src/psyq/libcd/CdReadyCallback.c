#include "psx/libcd.h"

/* Set the routine called when the drive has data ready and return the previous one. */
void* CdReadyCallback(void* callback) {
    void* previous = g_psyq_cd_ready_callback;
    g_psyq_cd_ready_callback = callback;
    return previous;
}
