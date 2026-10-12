#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Spawn this frame's share of a particle group's effect slots, drawn with CLUT
 * clut_index of the weapons row.
 *
 * The group spawns while the secondary effect's timer is inside its spawn
 * frames. Its spawns_per_frame caps how many slots one frame may start; the
 * secondary effect's active_count counts the live slots and stops the
 * spawning at the group's total count. */
void battle_effect_start_group(s32 group, s32 clut_index) {
    battle_effect_secondary_data_t* effect;
    battle_effect_slot_t* slot;
    s32 spawned;
    s32 i;
    u8 id;

    if (g_battle_effect_trap_particle_configs[group].count > g_battle_effect_current_secondary->active_count
        && g_battle_effect_current_secondary->timer >= g_battle_effect_trap_particle_configs[group].spawn_start_frame
        && g_battle_effect_current_secondary->timer < g_battle_effect_trap_particle_configs[group].spawn_end_frame) {
        for (spawned = 0; spawned < g_battle_effect_trap_particle_configs[group].spawns_per_frame; spawned++) {
            for (i = 0; i < g_battle_effect_trap_particle_configs[group].count; i++) {
                if (g_battle_effect_current_secondary->slot_ids[i] == 0) {
                    break;
                }
            }
            id = battle_effect_alloc_slot();
            g_battle_effect_current_secondary->slot_ids[i] = id;
            if (id == 0) {
                break;
            }
            slot = &g_battle_effect_slots[g_battle_effect_current_secondary->slot_ids[i]];
            slot->life
                = battle_effect_spawn_particle_motion(&g_battle_effect_trap_particle_configs[group], &slot->motion);
            battle_effect_init_trap_animation(group, clut_index + MAP_CLUT(MAP_CLUT_ROW_WEAPONS, 0), slot);
            effect = g_battle_effect_current_secondary;
            effect->active_count++;
            if (effect->active_count == g_battle_effect_trap_particle_configs[group].count) {
                break;
            }
        }
    }
}
