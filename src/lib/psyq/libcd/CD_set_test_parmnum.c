/* SCUS_942.21 0x80020750..0x8002075f. */
#include "psx/libcd.h"

/* Store a test parameter count that nothing reads. Nothing in the game calls it. */
void CD_set_test_parmnum(s32 parameter_count) {
    g_psyq_cd_command_attributes.parameter_counts[PSYQ_CD_COMMAND_TEST] = parameter_count;
}
