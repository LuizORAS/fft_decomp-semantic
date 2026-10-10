#include "fft/open.h"
#include "psx/types.h"

/* Opcode 15: start a slideshow of OPNTEX frames command[1] to command[2], each on
 * screen for command[3] frames with fades in and out of command[4] frames. The
 * first frame is loaded on the next update. Ten bytes. */
void open_opcode_start_opntex_sequence(s16* command) {
    command++;
    g_open_script_state.opntex.first_frame = *command++;
    g_open_script_state.opntex.last_frame = *command++;
    g_open_script_state.opntex.frame_duration = *command++;
    g_open_script_state.opntex_control_value = *command++;
    g_open_script_state.opntex.frame_countdown = 1;
    g_open_script_state.dispatch.byte_offset += 10;
}
