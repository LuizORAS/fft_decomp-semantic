#include "fft/battle.h"

s32 battle_script_add_ghost_unit_event_instruction(
    s32 map_x, s32 map_y, s32 map_level, u16 map_height, s32 portrait_id, s32 misc_id, s32 flags) {
    s32 ret;

    if (battle_unit_get_misc_data_by_misc_id((u16)misc_id) != 0) {
        ret = 0;
    } else {
        g_unit_graphics_load_count = 0;
        g_battle_gfx_state_words[0] = 0;
        g_battle_gfx_state_words[1] = 0;
        g_battle_gfx_state_words[2] = 0;
        g_battle_gfx_state_words[3] = 0;
        battle_gfx_append_unit_graphics_load_descriptor(
            map_x, map_y, map_level, (s16)map_height, (s16)portrait_id, 0, (s16)misc_id, 0, flags);
        ret = 1;
    }
    return ret;
}
