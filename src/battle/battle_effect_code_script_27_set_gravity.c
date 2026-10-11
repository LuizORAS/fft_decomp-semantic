#include "fft/battle.h"

/* Opcode 27: take the gravity vector and the inertia threshold from the
 * effect file's particle-system section. */
s32 battle_effect_code_script_27_set_gravity(effect_record_t* record) {
    battle_effect_set_gravity((battle_effect_gravity_t*)g_effect_geometry_table->gravity);
    battle_effect_set_inertia_threshold(g_effect_geometry_table->inertia_threshold);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
