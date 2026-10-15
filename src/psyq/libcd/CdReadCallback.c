#include "psx/libcd.h"

/* Set the routine called when a CdRead ends (with CdlComplete or CdlDiskError) and return the previous
 * one. */
void* CdReadCallback(void* callback) {
    void* previous = g_psyq_cd_read_callback;
    g_psyq_cd_read_callback = callback;
    return previous;
}
