#include "psx/libetc.h"

/* Set the callback of interrupt `channel` and return the previous one (setIntr, through the dispatch
 * table). LIBCD installs its drive handler on interrupt 2 with it, and startIntrVSync and startIntrDMA their
 * dispatchers on 0 and 3. */
void* InterruptCallback(s32 channel, psyq_interrupt_callback_t callback) {
    return g_psyq_etc_dispatch->irq_callback(channel, callback);
}
