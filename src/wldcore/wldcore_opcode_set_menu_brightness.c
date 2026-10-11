#include "fft/wldcore.h"

void wldcore_opcode_set_menu_brightness(void) {
    world_menu_set_brightness((u8)(g_wldcore_active_saved_record.instruction >> 8),
        (u8)(g_wldcore_active_saved_record.instruction >> 16), (u8)(g_wldcore_active_saved_record.instruction >> 24));
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
