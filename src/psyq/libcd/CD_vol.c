/* SCUS_942.21 0x8002004c..0x800200d3. */
#include "psx/libcd.h"

s32 CD_vol(CdlATV* volume) {
    *g_psyq_cd_index_status_register = 2;
    *g_psyq_cd_parameter_data_register = volume->val0;
    *g_psyq_cd_request_interrupt_register = volume->val1;
    *g_psyq_cd_index_status_register = 3;
    *g_psyq_cd_command_response_register = volume->val2;
    *g_psyq_cd_parameter_data_register = volume->val3;
    *g_psyq_cd_request_interrupt_register = 0x20;
    return 0;
}
