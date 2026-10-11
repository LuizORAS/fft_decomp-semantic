#include "fft/battle.h"
#include "psx/types.h"

/* Restarts the unit graphics loader with one descriptor, for a unit an event adds
 * during the battle: the unit is put on the field
 * (battle_unit_set_enemy_level_data_by_battle_id) and queued at its map position
 * and facing with its sheet and palette, the caller's flags and misc id 0xff,
 * which takes the first free misc slot. Always returns 1. */
s32 battle_gfx_load_unit_graphics_by_battle_id(s32 battle_id, s32 flags) {
    battle_stats_t* unit;

    g_main_gfx_unit_graphics_load_queue.count = 0;
    g_battle_gfx_unit_graphics_load_state[0] = BATTLE_GFX_UNIT_LOAD_KIND_FIRST;
    g_battle_gfx_unit_graphics_load_state[1] = BATTLE_GFX_UNIT_LOAD_PHASE_START_READ;
    g_battle_gfx_unit_graphics_load_state[2] = 0;
    g_battle_gfx_unit_graphics_load_state[3] = 0;
    unit = battle_unit_get_stats_from_battle_id(battle_id);
    battle_unit_set_enemy_level_data_by_battle_id(battle_id);
    battle_gfx_append_unit_graphics_load_descriptor(unit->x, unit->position.bits.y,
        unit->position.bits.higher_elevation, unit->position.bits.facing, unit->spritesheet_id,
        unit->job_portrait_palette, 0xFF, unit, flags);
    return 1;
}
