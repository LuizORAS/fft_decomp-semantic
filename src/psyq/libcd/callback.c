/* SCUS_942.21 0x80020760..0x8002083f. */
#include "psx/libcd.h"
static __inline__ void cd_dispatch_callbacks(void) {
    s32 events;
    u8 bank = *g_psyq_cd_index_status_register & 3;
    while ((events = getintr()) != 0) {
        if ((events & 4) && g_psyq_cd_ready_callback)
            ((psyq_cd_result_callback_t)g_psyq_cd_ready_callback)(g_psyq_cd_irq_state.ready, g_psyq_cd_ready_result);
        if ((events & 2) && g_psyq_cd_sync_callback)
            ((psyq_cd_result_callback_t)g_psyq_cd_sync_callback)(g_psyq_cd_irq_state.sync, g_psyq_cd_sync_result);
    }
    *g_psyq_cd_index_status_register = bank;
}
void callback(void) {
    cd_dispatch_callbacks();
}
