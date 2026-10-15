#include "psx/libetc.h"

/* Start LIBETC's interrupt handling through the dispatch table's reset entry (startIntr) and return its
 * result. main_boot_run_startup, PadInit, ResetGraph, CD_init, CD_initintr, DecDCTReset and _SpuInit call it;
 * only the first call does anything. */
int ResetCallback(void) {
    return g_psyq_etc_dispatch->reset();
}
