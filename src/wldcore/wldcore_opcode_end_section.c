#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: end the section; the script branches to the recorded
 * section after yielding this frame. */
void wldcore_opcode_end_section(void) {
    u16* flags;

    flags = &g_wldcore_active_saved_record.state_flags;
    *flags |= WLDCORE_OPCODE_STATE_BRANCH | WLDCORE_OPCODE_STATE_YIELD;
}
