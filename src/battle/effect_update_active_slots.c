#include "fft/battle_effect.h"

/* Advance the current secondary effect's particles: every effect slot in the
 * first count entries of slot_ids, count being the particle group's. A slot that
 * has finished is freed and active_count drops by one. */
void effect_update_active_slots(s32 group) {
    s32 i;
    s32 id;

    for (i = 0; i < g_effect_trap_particle_configs[group].count; i++) {
        id = g_effect_current_secondary->slot_ids[i];
        if (id != 0 && effect_update_slot(id) == 0) {
            effect_free_slot(id);
            g_effect_current_secondary->slot_ids[i] = 0;
            g_effect_current_secondary->active_count--;
        }
    }
}
