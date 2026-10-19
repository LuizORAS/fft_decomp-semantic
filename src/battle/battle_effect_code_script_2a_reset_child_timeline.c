#include "fft/battle_effect.h"

/* Opcode 2a: clear the child-timeline state (target, frame and keyframe
 * tracks) that opcode 28 steps. */
effect_script_result_e battle_effect_code_script_2a_reset_child_timeline(battle_effect_record_t* record) {
    battle_effect_reset_child_timeline(record, 0);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
