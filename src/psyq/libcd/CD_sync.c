/* SCUS_942.21 0x8001f6b8..0x8001f937. */
#include "psx/libc.h"
#include "psx/libcd.h"
#include "psx/libetc.h"

/* Removing volatile from the typed SDK timeout view preserves the original scheduling. */
#define CD_ALARM (*(psyq_cd_timeout_state_t*)&g_psyq_cd_timeout)
static __inline__ void cd_set_timeout(char* description) {
    CD_ALARM.deadline = VSync(-1) + 960;
    CD_ALARM.counter = 0;
    CD_ALARM.description = description;
}
static __inline__ s32 cd_check_timeout(void) {
    if (CD_ALARM.deadline < VSync(-1) || CD_ALARM.counter++ > 0x3c0000) {
        puts(g_psyq_cd_timeout_message);
        printf(g_psyq_cd_timeout_format, CD_ALARM.description, g_psyq_cd_command_names[g_psyq_cd_last_command],
            g_psyq_cd_interrupt_names[g_psyq_cd_irq_state.sync], g_psyq_cd_interrupt_names[g_psyq_cd_irq_state.ready]);
        CD_flush();
        return -1;
    }
    return 0;
}
static __inline__ void cd_copy_result(void* destination, void* source, u32 size) {
    u8* output = destination;
    u8* input = source;
    if (!output)
        return;
    while (size--)
        *output++ = *input++;
}
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
s32 CD_sync(s32 mode, u8* result) {
    s32 event;
    cd_set_timeout(g_psyq_cd_sync_timeout_name);
    while (1) {
        if (cd_check_timeout())
            return -1;
        if (CheckCallback())
            cd_dispatch_callbacks();
        event = g_psyq_cd_irq_state.sync;
        if (event == CdlComplete || event == CdlDiskError) {
            g_psyq_cd_irq_state.sync = CdlComplete;
            cd_copy_result(result, g_psyq_cd_sync_result, 8);
            return event;
        }
        if (mode)
            return 0;
    }
}
