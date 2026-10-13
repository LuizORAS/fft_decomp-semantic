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
void effect_start_group(s32 group, s32 clut_index) {
    effect_slot_t* slot;
    s32 spawned;
    s32 i;
    u8 id;

    if (g_effect_trap_particle_configs[group].count > g_effect_current_secondary->active_count
        && g_effect_current_secondary->timer >= g_effect_trap_particle_configs[group].spawn_start_frame
        && g_effect_current_secondary->timer < g_effect_trap_particle_configs[group].spawn_end_frame) {
        for (spawned = 0; spawned < g_effect_trap_particle_configs[group].spawns_per_frame; spawned++) {
            for (i = 0; i < g_effect_trap_particle_configs[group].count; i++) {
                if (g_effect_current_secondary->slot_ids[i] == 0) {
                    break;
                }
            }
            id = effect_alloc_slot();
            g_effect_current_secondary->slot_ids[i] = id;
            if (id == 0) {
                break;
            }
            slot = &g_effect_slots[g_effect_current_secondary->slot_ids[i]];
            slot->life = effect_spawn_particle_motion(&g_effect_trap_particle_configs[group], &slot->motion);
            effect_init_trap_animation(group, clut_index + MAP_CLUT(MAP_CLUT_ROW_WEAPONS, 0), slot);
            g_effect_current_secondary->active_count++;
            if (g_effect_current_secondary->active_count == g_effect_trap_particle_configs[group].count) {
                break;
            }
        }
    }
}
