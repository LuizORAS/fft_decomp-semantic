#include "fft/wldcore.h"
#include "psx/libgpu.h"

/* Point render record `index` at the formation portrait of party member
 * `party_index`: its texture page, cell rectangle and CLUT (16 colours at
 * x 0x280 + 0x40 per 40 portraits + 16 * (code % 4), y 0x1F0 +
 * (code % 40) / 4), at full brightness, and show the record. The first
 * argument is not read. */
void wldcore_window_load_party_unit_sprite_record(wldcore_menu_send_unit_level_t* unused, s32 party_index, s32 index) {
    POLY_FT4 poly;
    s32 code;
    s32 cell;
    CVECTOR* color;
    wldcore_window_render_bounds16_t* bounds;

    code = world_unit_get_formation_sprite(party_index);
    cell = code;
    world_gfx_set_portrait_poly_texture(&poly, code);
    g_wldcore_window_render_records[index].clut.x = (code / 40 << 6) + 0x280 + cell % 4 * 16;
    g_wldcore_window_render_records[index].clut.y = cell % 40 / 4 + 0x1F0;
    g_wldcore_window_render_records[index].tpage = poly.tpage;
    bounds = &g_wldcore_window_render_records[index].bounds;
    bounds->position.x = poly.u0;
    bounds->position.y = poly.v0;
    bounds->dimensions.x = poly.u1 - poly.u0;
    bounds->dimensions.y = poly.v2 - poly.v0;
    color = &g_wldcore_window_render_records[index].color;
    color->r = 0x80;
    color->g = 0x80;
    color->b = 0x80;
    g_wldcore_window_render_records[index].anim_counter = 0;
    g_wldcore_window_render_records[index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
}
