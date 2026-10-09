#include "fft/open.h"
#include "psx/types.h"

void open_opcode_start_opntex_sequence(s16* command) {
    command++;
    g_open_script_state.opntex.first_frame = *command++;
    g_open_script_state.opntex.last_frame = *command++;
    g_open_script_state.opntex.frame_duration = *command++;
    g_open_script_state.opntex_control_value = *command++;
    g_open_script_state.opntex.frame_countdown = 1;
    g_open_script_state.dispatch.byte_offset += 10;
}
