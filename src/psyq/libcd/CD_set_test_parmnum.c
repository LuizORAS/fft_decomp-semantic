/* SCUS_942.21 0x80020750..0x8002075f. */
#include "psx/libcd.h"

void CD_set_test_parmnum(s32 parameter_count) {
    g_psyq_cd_command_attributes.parameter_counts[PSYQ_CD_COMMAND_TEST] = parameter_count;
}
