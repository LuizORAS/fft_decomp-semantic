#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcodes 20 and 21: step over one operand. Opcodes 12 and 13 have their own
 * copies (wldcore_script_skip_1_operand_2 and _3). */
void wldcore_script_skip_1_operand(void) {
    u16* ip = &g_wldcore_script_state.ip;
    *ip += 1;
}
