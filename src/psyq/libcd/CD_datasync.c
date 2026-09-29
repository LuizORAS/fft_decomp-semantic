/* SCUS_942.21 0x800204e4..0x8002064f. */
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
s32 CD_datasync(s32 mode) {
    s32 result;
    cd_set_timeout(g_psyq_cd_data_sync_timeout_name);
    while (1) {
        if (cd_check_timeout()) {
            result = -1;
            break;
        }
        if (!(*g_psyq_cd_dma_control_register & 0x01000000)) {
            result = 0;
            break;
        }
        if (mode) {
            result = 1;
            break;
        }
    }
    return result;
}
