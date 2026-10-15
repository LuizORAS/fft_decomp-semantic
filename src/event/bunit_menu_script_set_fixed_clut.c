#include "fft/event_bunit.h"
#include "psx/libgpu.h"

/* Opcode 9 (MENU_SCRIPT_OP_FIXED_CLUT): the CLUT at x = byte 3 * 16, y = byte 4 + byte 2 * 256
 * becomes the current one (g_bunit_gfx_clut_id). Returns the next command. */
const u8* bunit_menu_script_set_fixed_clut(const u8* data) {
    g_bunit_gfx_clut_id = GetClut(data[3] << 4, data[4] | (data[2] << 8));
    return data + data[1];
}
