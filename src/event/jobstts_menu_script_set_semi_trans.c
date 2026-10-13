#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Opcode 0x0e (MENU_SCRIPT_OP_SEMI_TRANS): byte 3 turns semi-transparency on or off for the
 * primitives the following commands queue (g_jobstts_gfx_semitransparency). Returns the next command. */
const u8* jobstts_menu_script_set_semi_trans(const u8* data) {
    g_jobstts_gfx_semitransparency = data[3];
    return data + data[1];
}
