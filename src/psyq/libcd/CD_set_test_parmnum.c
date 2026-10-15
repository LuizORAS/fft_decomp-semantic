/* SCUS_942.21 0x80020750..0x8002075f. */
#include "psx/libcd.h"

/* Store a test parameter count that nothing reads. Nothing in the game calls it. */
void CD_set_test_parmnum(s32 parameter_count) {
    g_psyq_cd_test_parameter_count = parameter_count;
}
