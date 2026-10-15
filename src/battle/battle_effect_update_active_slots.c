#include "fft/battle.h"

/* Advance the current secondary effect's particles: every effect slot in the
 * first count entries of slot_ids, count being the particle group's. A slot that
 * has finished is freed and active_count drops by one. */
void battle_effect_update_active_slots(s32 group) {
    s32 i;
    s32 id;

    for (i = 0; i < g_battle_effect_particle_groups[group].count; i++) {
        id = g_battle_effect_current_secondary->slot_ids[i];
        if (id != 0 && battle_effect_update_slot(id) == 0) {
            battle_effect_free_slot(id);
            g_battle_effect_current_secondary->slot_ids[i] = 0;
            g_battle_effect_current_secondary->active_count--;
        }
    }
}
