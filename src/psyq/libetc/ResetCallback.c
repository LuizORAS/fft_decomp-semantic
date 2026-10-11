#include "psx/libetc.h"

void ResetCallback(void) {
    g_psyq_etc_dispatch->reset();
}
