#include "fft/event_bunit.h"
#include "psx/types.h"

/* Stack view of world_oriented_quad_t whose clut/tpage slots receive the
 * sprite query's palette and spritesheet IDs directly; the query's last two
 * halfwords extend it to 0x18 bytes. */
typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 u;
    s16 v;
    s16 uw;
    s16 vh;
    battle_unit_sprite_query_t query; /* 0x10 */
} bunit_sprite_quad_t;

/* battle_gfx_source_part_t with its attributes as bit-fields: the part's
 * place, the 8x8 tile its picture starts at (32 tiles per row) and the size
 * that indexes g_bunit_sprite_cell_sizes, this overlay's copy of
 * g_battle_gfx_part_sizes in pixels. The flip bits are not read: the facing
 * quadrant alone mirrors the sprite. */
typedef struct {
    s8 x_shift;
    s8 y_shift;
    u16 tile : 10;
    u16 size : 4;
    u16 flip : 2;
} bunit_sprite_cell_t;

/* Draw a unit's current SHP frame at (x, y), then its two name-plate quads.
 *
 * The frame's first byte is taken whole as the part count minus one, where
 * BATTLE keeps only its low three bits; no unit frame on the disc sets the
 * others.
 *
 * Brightness falls off with distance from the animated focal point; the
 * selected unit's plate also pulses. The target keeps two short-lived values
 * in the loop counter and OT-index registers (the falloff result in `i`, the
 * facing quadrant in `value`), so the source reuses those variables. */
void bunit_gfx_draw_unit_sprite_and_name_plate(s16 unit, s32 x, s32 y) {
    rgb24_t rgb;
    bunit_sprite_quad_t quad;
    u8* frame;
    s32 base_y;
    s32 i;
    bunit_sprite_cell_t* cells;
    s32 dx;
    s32 orientation;
    s32 value;
    s32 size;

    i = bunit_gfx_calculate_distance_falloff(x, y, 0xC8, 0x50);
    rgb.r = rgb.g = rgb.b = i > 0x80 ? 0x80 : i;
    frame = battle_gfx_get_unit_sprite_frame_and_vram_ids(g_bunit_unit_data[unit]->misc_unit_id, &quad.query);
    base_y = (quad.query.graphic_height >> 1) + 0x16;
    if (frame == (u8*)-1) {
        return;
    }
    cells = (bunit_sprite_cell_t*)(frame + 2);
    for (i = 0; i <= frame[0]; i++) {
        size = cells[i].size;
        quad.uw = quad.w = g_bunit_sprite_cell_sizes[size][0];
        quad.vh = quad.h = g_bunit_sprite_cell_sizes[size][1];
        value = battle_unit_get_camera_facing_quadrant_by_battle_id(g_bunit_unit_data[unit]->battle_id);
        dx = cells[i].x_shift;
        orientation = TEXTURE_UV_FLIP_NONE;
        if (value >= 2) {
            orientation = TEXTURE_UV_FLIP_X;
            dx = -dx - quad.w;
        }
        /* The (s16) keeps the 0x1C offset on dx instead of letting fold
         * reassociate it onto x. */
        quad.x = x + (s16)(dx + 0x1C);
        quad.y = cells[i].y_shift + (y + base_y);
        if (g_bunit_unit_data[unit]->mount_info & BATTLE_MOUNT_INFO_FLAG_RIDER) {
            quad.y -= 8;
        }
        quad.u = cells[i].tile % 32 * 8;
        quad.v = cells[i].tile / 32 * 8 + quad.query.graphic_y_offset;
        value = 7;
        if (g_bunit_unit_data[unit]->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
            value = 8;
            if (i >= 3) {
                value = 6;
            }
        }
        bunit_gfx_enqueue_oriented_textured_quad((world_oriented_quad_t*)&quad, &rgb, orientation, 0, value);
    }
    if (g_bunit_unit_data[unit]->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
        return;
    }
    g_bunit_unit_shadow_sprite.x = x + 0x12;
    g_bunit_unit_shadow_sprite.y = y + base_y - 4;
    g_bunit_unit_shadow_sprite.tpage = GetTPage(0, 2, 0x3C0, 0x100);
    bunit_gfx_enqueue_oriented_textured_quad(&g_bunit_unit_shadow_sprite, 0, TEXTURE_UV_FLIP_NONE, 1, 5);
    g_bunit_unit_dot_sprite.x = x;
    g_bunit_unit_dot_sprite.y = y;
    if (g_bunit_unit_data[unit]->team_kind == 1) {
        g_bunit_unit_dot_sprite.clut = 0x3F12;
    } else {
        g_bunit_unit_dot_sprite.clut = 0x3F13;
    }
    g_bunit_unit_dot_sprite.tpage = GetTPage(0, 1, 0x3C0, 0x100);
    if (unit == g_bunit_unit_selected_index) {
        g_bunit_unit_dot_glow_timer += g_bunit_unit_dot_glow_direction * 2;
        if (g_bunit_unit_dot_glow_timer > 0x28) {
            g_bunit_unit_dot_glow_direction = -1;
        }
        if (g_bunit_unit_dot_glow_timer < -0x28) {
            g_bunit_unit_dot_glow_direction = 1;
        }
        rgb.r += g_bunit_unit_dot_glow_timer;
        rgb.g += g_bunit_unit_dot_glow_timer;
        rgb.b += g_bunit_unit_dot_glow_timer;
    }
    bunit_gfx_enqueue_oriented_textured_quad(&g_bunit_unit_dot_sprite, &rgb, TEXTURE_UV_FLIP_NONE, 1, 8);
}
