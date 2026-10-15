#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 24: step over four operands. */
void wldcore_script_skip_4_operands(void) {
    u16* ip = &g_wldcore_script_state.ip;
    *ip += 4;
}
