#include "fft/open.h"
#include "psx/types.h"

/* Opcode 12: fade the OPNTEX picture in over command[1] frames (intensity 0 to
 * 0x80). Four bytes. */
void open_opcode_start_fade_intensity_increase(s16* command) {
    s32 value = command[1];

    g_open_script_state.dispatch.flags |= OPEN_SCRIPT_DISPATCH_INCREASE_FADE_INTENSITY;
    g_open_script_state.opntex.fade_elapsed_frames = 0;
    g_open_script_state.opntex.fade_intensity = 0;
    g_open_script_state.opntex.fade_duration_frames = value;
    g_open_script_state.dispatch.byte_offset += 4;
}
