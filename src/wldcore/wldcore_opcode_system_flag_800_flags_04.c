#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_opcode_system_flag_800_flags_04(void) {
    u16* flags;
    u32 sys;
    u16 value;

    flags = &g_wldcore_active_saved_record.state_flags;
    sys = g_main_system_flags;
    value = *flags;
    g_main_system_flags = sys | MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
    *flags = (value & ~WLDCORE_OPCODE_STATE_VALUE_DISPLAY) | WLDCORE_OPCODE_STATE_FETCH;
}
