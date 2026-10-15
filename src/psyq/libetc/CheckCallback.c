#include "psx/libetc.h"

/* Return 1 while trapIntr is running interrupt callbacks, else 0. LIBCD's waits (CD_sync, CD_ready, CD_cw)
 * run the CD callbacks themselves while it is 1, since the interrupt cannot come then. */
s32 CheckCallback(void) {
    return g_psyq_etc_interrupt_state.in_callback;
}
