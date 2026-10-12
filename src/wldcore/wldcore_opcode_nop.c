#include "fft/wldcore.h"

void wldcore_opcode_nop(void) {
    u16* flags;

    flags = &g_wldcore_active_saved_record.state_flags;
    *flags |= WLDCORE_OPCODE_STATE_FETCH;
}
