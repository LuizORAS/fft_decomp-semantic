/* SCUS_942.21 0x800201b4..0x800202a7. */
#include "psx/libcd.h"

s32 CD_initvol(void) {
    CdlATV volume;
    if (g_psyq_cd_spu_registers->current_main_volume_left == 0
        && g_psyq_cd_spu_registers->current_main_volume_right == 0) {
        g_psyq_cd_spu_registers->main_volume_left = 0x3fff;
        g_psyq_cd_spu_registers->main_volume_right = 0x3fff;
    }
    g_psyq_cd_spu_registers->cd_volume_left = 0x3fff;
    g_psyq_cd_spu_registers->cd_volume_right = 0x3fff;
    g_psyq_cd_spu_registers->control = 0xc001;
    volume.val0 = volume.val2 = 0x80;
    volume.val1 = volume.val3 = 0;
    *g_psyq_cd_index_status_register = 2;
    *g_psyq_cd_parameter_data_register = volume.val0;
    *g_psyq_cd_request_interrupt_register = volume.val1;
    *g_psyq_cd_index_status_register = 3;
    *g_psyq_cd_command_response_register = volume.val2;
    *g_psyq_cd_parameter_data_register = volume.val3;
    *g_psyq_cd_request_interrupt_register = 0x20;
    return 0;
}
