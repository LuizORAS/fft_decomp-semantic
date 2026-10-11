#include "fft/battle.h"

struct battle_effect_parent_timeline_state;
extern void battle_effect_reset_parent_timeline(struct battle_effect_parent_timeline_state* data);

/* Opcode 2b: clear the parent-timeline state (frame, spawn counters and
 * keyframe tracks) that opcode 29 steps. */
s32 battle_effect_code_script_2b_reset_parent_timeline(effect_record_t* record) {
    battle_effect_reset_parent_timeline((struct battle_effect_parent_timeline_state*)record);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
