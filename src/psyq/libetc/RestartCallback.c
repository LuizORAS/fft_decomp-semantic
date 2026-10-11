#include "psx/libetc.h"

s32 RestartCallback(void) {
    /* The SDK exposes this system-only state pointer as an integer result. */
    return (s32)g_psyq_etc_dispatch->restart();
}
