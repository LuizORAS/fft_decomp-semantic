#include "fft/event_bunit.h"
#include "psx/types.h"

/* Opcode 0x0e (MENU_SCRIPT_OP_SEMI_TRANS): byte 3 turns semi-transparency on or off for the
 * primitives the following commands queue (g_bunit_gfx_semitrans_enabled). Returns the next command. */
u8* bunit_menu_script_set_semi_trans(u8* data) {
    g_bunit_gfx_semitrans_enabled = data[3];
    return data + data[1];
}
