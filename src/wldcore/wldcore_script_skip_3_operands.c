#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 23: step over three operands. */
void wldcore_script_skip_3_operands(void) {
    u16* ip = &g_wldcore_script_state.ip;
    *ip += 3;
}
