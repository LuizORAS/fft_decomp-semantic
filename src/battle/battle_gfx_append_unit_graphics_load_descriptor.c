#include "fft/battle.h"

/* clang-format off */
void battle_gfx_append_unit_graphics_load_descriptor(map_x, map_y, map_level, map_height, portrait_id, palette_id,
    misc_id, unit, flags)
    u8 map_x;
    u8 map_y;
    u8 map_level;
    u16 map_height;
    s32 portrait_id;
    u16 palette_id;
    u16 misc_id;
    battle_stats_t* unit;
    u32 flags;
/* clang-format on */
{
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].map_x = map_x;
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].map_y = map_y;
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].map_level = map_level;
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].map_height = map_height;
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].portrait_id
        = portrait_id;
    if ((s16)portrait_id >= BATTLE_SPRITESHEET_ID_COUNT) {
        g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].portrait_id
            = BATTLE_SPRITESHEET_ID_RAMZA_CHAPTER_1;
    }
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].palette_id = palette_id;
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].misc_id = misc_id;
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].battle_stats = unit;
    g_main_gfx_unit_graphics_load_queue.descriptors[g_main_gfx_unit_graphics_load_queue.count].flags = flags;
    if (g_main_gfx_unit_graphics_load_queue.count < 16) {
        g_main_gfx_unit_graphics_load_queue.count++;
    }
}
