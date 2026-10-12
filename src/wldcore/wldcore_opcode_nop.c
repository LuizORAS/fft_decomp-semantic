#include "fft/wldcore.h"

void wldcore_opcode_nop(void) {
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
