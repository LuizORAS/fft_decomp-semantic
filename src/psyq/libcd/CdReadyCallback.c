#include "psx/libcd.h"

CdlCB CdReadyCallback(CdlCB callback) {
    CdlCB previous = g_psyq_cd_ready_callback;
    g_psyq_cd_ready_callback = callback;
    return previous;
}
