#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 12: step over one operand, as wldcore_script_skip_1_operand does. */
void wldcore_script_skip_1_operand_2(void) {
    u16* ip = &g_wldcore_script_state.ip;
    *ip += 1;
}
