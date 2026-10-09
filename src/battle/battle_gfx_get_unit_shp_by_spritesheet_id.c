#include "fft/battle.h"

battle_gfx_unit_shp_frame_tables_t* battle_gfx_get_unit_shp_by_spritesheet_id(u32 index) {
    s32 value;

    value = g_battle_gfx_spritesheet_data[index & 0xffff].shp_id;
    if (value < 5) {
        return &g_battle_gfx_unit_shp_frame_tables[value];
    }
    return &g_battle_gfx_boss_shp_frame_tables;
}
