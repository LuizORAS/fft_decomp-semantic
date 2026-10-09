#include "fft/open.h"
#include "psx/types.h"

void open_opcode_start_fade_intensity_increase(s16* command) {
    s32 value = command[1];

    g_open_script_state.dispatch.flags |= 0x40;
    g_open_script_state.opntex.fade_elapsed_frames = 0;
    g_open_script_state.opntex.fade_intensity = 0;
    g_open_script_state.opntex.fade_duration_frames = value;
    g_open_script_state.dispatch.byte_offset += 4;
}
