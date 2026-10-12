#include "fft/battle_effect.h"

/* Opcode 2b: clear the parent-timeline state (frame, spawn counters and
 * keyframe tracks) that opcode 29 steps. */
effect_script_result_e effect_code_script_2b_reset_parent_timeline(effect_record_t* record) {
    effect_reset_parent_timeline(record);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
