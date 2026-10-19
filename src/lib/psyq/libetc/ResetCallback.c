#include "psx/libetc.h"

/* Start LIBETC's interrupt handling through the dispatch table's reset entry (startIntr) and return its
 * result. main_boot_run_startup, PadInit, ResetGraph, CD_init, CD_initintr, DecDCTReset and _SpuInit call it;
 * only the first call does anything. */
s32 ResetCallback(void) {
    /* Return the interrupt-state address as a 32-bit integer. */
    return (s32)g_psyq_etc_dispatch->reset();
}
