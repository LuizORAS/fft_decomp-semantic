#include "fft/battle.h"

s32 battle_script_add_ghost_unit_event_instruction(
    s32 map_x, s32 map_y, s32 map_level, u16 map_height, s32 portrait_id, s32 misc_id, s32 flags) {
    s32 ret;

    if (battle_unit_get_misc_data_by_misc_id((u16)misc_id) != 0) {
        ret = 0;
    } else {
        g_main_gfx_unit_graphics_load_queue.count = 0;
        g_battle_gfx_unit_graphics_load_state[0] = BATTLE_GFX_UNIT_LOAD_KIND_FIRST;
        g_battle_gfx_unit_graphics_load_state[1] = BATTLE_GFX_UNIT_LOAD_PHASE_START_READ;
        g_battle_gfx_unit_graphics_load_state[2] = 0;
        g_battle_gfx_unit_graphics_load_state[3] = 0;
        battle_gfx_append_unit_graphics_load_descriptor(
            map_x, map_y, map_level, (s16)map_height, (s16)portrait_id, 0, (s16)misc_id, 0, flags);
        ret = 1;
    }
    return ret;
}
