#include "psx/libetc.h"

/* Stop LIBETC's interrupt handling (stopIntr, through the dispatch table). main calls it in its
 * unreachable tail. */
s32 StopCallback(void) {
    /* Return the interrupt-state address as a 32-bit integer. */
    return (s32)g_psyq_etc_dispatch->stop();
}
