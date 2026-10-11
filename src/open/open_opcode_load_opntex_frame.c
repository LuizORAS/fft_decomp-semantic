#include "fft/open.h"

/* Each OPNTEX frame occupies 23 * 0x800 bytes of the loaded data. */
#define OPEN_OPNTEX_FRAME_SECTORS 23

/* Opcode 11: upload frame command[1] of the loaded OPNTEX data as the picture and
 * set its fade intensity to 0, which hides it. Four bytes. */
void open_opcode_load_opntex_frame(s16* command) {
    open_gfx_load_opntex_into_frame_buffer(g_open_gfx_opntex_data + command[1] * OPEN_OPNTEX_FRAME_SECTORS * 0x800);
    g_open_script_state.opntex.fade_intensity = 0;
    g_open_script_state.dispatch.byte_offset += 4;
}
