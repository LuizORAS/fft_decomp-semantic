#include "fft/wldcore.h"

void wldcore_opcode_branch_if_savedata_bit_clear(void) {
    wldcore_opcode_branch_if_savedata_bit(0);
}
