#include "fft/wldcore.h"

void wldcore_opcode_load_background_set(void) {
    s32 value;

    value = g_wldcore_active_saved_record.instruction.bytes.operand_0;
    if (g_wldcore_active_saved_record.background_set != value) {
        DrawSync(0);
        g_wldcore_active_saved_record.background_set = value;
        wldcore_gfx_load_background_set(value);
        wldcore_wait_for_file_load();
    }
    g_wldcore_active_saved_record.state_flags |= 4;
}
