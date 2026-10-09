#include "fft/battle.h"

/* Expand a unit graphic frame into the renderer's sprite-part records. */
void battle_gfx_load_unit_frame_parts(battle_unit_misc_data_t* unit, battle_gfx_sprite_display_data_t* display,
    battle_gfx_source_frame_t* src, s32 sheet_flags) {
    battle_gfx_source_part_t* part;
    u32 i;
    u32 count;
    u8 flags;
    u16 attr;
    u16 size;
    u32 tile;
    s32 u;
    s32 v;
    s32 width;
    s32 height;

    flags = src->flags;
    count = display->part_count;
    display->tpage = (flags & 0x60) | sheet_flags;
    display->clut = unit->vram_palette_id;
    for (i = 0; i < count; i++) {
        part = &src->parts[i];
        attr = part->attributes;
        size = (attr & 0x3c00) >> 10;
        width = g_battle_gfx_part_sizes[size].width;
        height = g_battle_gfx_part_sizes[size].height;
        tile = attr & 0x3ff;
        u = (tile & 0x1f) * 8;
        v = (tile >> 5) * 8;
        battle_gfx_store_sprite_display_data(display, i, part->x_shift, part->y_shift, u, v, (s16)(width * 8),
            (s16)(height * 8), (flags & 0x61) | ((attr & 0xc000) >> 13));
    }
}
