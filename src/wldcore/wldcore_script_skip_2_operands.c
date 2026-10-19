#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcodes 5 to 11 and 27: step over two operands. */
void wldcore_script_skip_2_operands(void) {
    u16* ip = &g_wldcore_script_state.ip;
    *ip += 2;
}
