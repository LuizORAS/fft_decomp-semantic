/* SCUS_942.21 0x800202f8..0x800204e3. */
#include "psx/libc.h"
#include "psx/libcd.h"
#include "psx/libetc.h"

s32 CD_init(void) {
    puts(g_psyq_cd_init_message);
    printf(g_psyq_cd_init_address_format, &g_psyq_cd_init_irq_state_pointer);
    g_psyq_cd_last_command = 0;
    g_psyq_cd_mode = 0;
    g_psyq_cd_ready_callback = 0;
    g_psyq_cd_sync_callback = 0;
    g_psyq_cd_status_detail = 0;
    /* Retail resets all four status bytes with a word store. */
    *(u32*)&g_psyq_cd_status = 0;
    ResetCallback();
    InterruptCallback(2, callback);
    *g_psyq_cd_index_status_register = 1;
    while (*g_psyq_cd_request_interrupt_register & 7) {
        *g_psyq_cd_index_status_register = 1;
        *g_psyq_cd_request_interrupt_register = 7;
        *g_psyq_cd_parameter_data_register = 7;
    }
    g_psyq_cd_irq_state.ready = g_psyq_cd_irq_state.second_ready = 0;
    g_psyq_cd_irq_state.sync = CdlComplete;
    *g_psyq_cd_index_status_register = 0;
    *g_psyq_cd_request_interrupt_register = 0;
    *g_psyq_cd_common_delay_register = 0x1325;
    CD_cw(CdlNop, 0, 0, 0);
    /* The device status is stored and tested as a full word in this routine. */
    if (*(u32*)&g_psyq_cd_status & CdlStatShellOpen)
        CD_cw(CdlNop, 0, 0, 0);
    if (CD_cw(CdlReset, 0, 0, 0))
        return -1;
    if (CD_cw(CdlDemute, 0, 0, 0))
        return -1;
    return CD_sync(0, 0) == CdlComplete ? 0 : -1;
}
