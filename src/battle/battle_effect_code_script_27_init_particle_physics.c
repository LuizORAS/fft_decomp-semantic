#include "fft/battle_effect.h"

/* Opcode 27: take the gravity vector and the inertia threshold from the
 * effect file's particle-system section. */
effect_script_result_e battle_effect_code_script_27_init_particle_physics(battle_effect_record_t* record) {
    battle_effect_set_gravity(&g_battle_effect_geometry_table->gravity);
    battle_effect_set_inertia_threshold(g_battle_effect_geometry_table->inertia_threshold);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
