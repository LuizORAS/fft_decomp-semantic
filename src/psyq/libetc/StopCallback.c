#include "psx/libetc.h"

void StopCallback(void) {
    g_psyq_etc_dispatch->stop();
}
