#include "psx/libetc.h"

s32 RestartCallback(void) {
    return g_psyq_etc_dispatch->restart();
}
