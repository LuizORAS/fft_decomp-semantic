#include "psx/libetc.h"

/* Stop LIBETC's interrupt handling (stopIntr, through the dispatch table). main calls it in its
 * unreachable tail. */
void StopCallback(void) {
    g_psyq_etc_dispatch->stop();
}
