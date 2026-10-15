#include "fft/event_bunit.h"

/* Opcode 0x0c (MENU_SCRIPT_OP_TEXTURE_PAGE): compute the texture page from bytes 2..4 (GetTPage)
 * and keep it for the following quads (g_bunit_gfx_texture_page). Returns the next command. */
const u8* bunit_menu_script_set_texture_page(const u8* data) {
    g_bunit_gfx_texture_page = GetTPage(data[4], data[2] >> 4, data[3] << 4, data[2] << 8);
    return data + data[1];
}
