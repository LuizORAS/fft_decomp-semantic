#include "psx/libetc.h"

/* Resume LIBETC's interrupt handling after StopCallback (restartIntr, through the dispatch table). No code
 * refers to it. */
s32 RestartCallback(void) {
    /* Return the interrupt-state address as a 32-bit integer. */
    return (s32)g_psyq_etc_dispatch->restart();
}
