#include "fft/battle.h"

/* Opcode 19: jump to the offset in the third halfword when the number of
 * occupied child slots equals the second halfword; otherwise step over the
 * 6-byte instruction. */
effect_script_result_e battle_effect_code_script_19_jump_if_child_count_equal(effect_record_t* record) {
    s16 i;
    s16 count;
    s16 target;

    target = (u16)battle_effect_get_current_script_instruction(record)->branch.comparison;
    count = 0;
    for (i = 0; i < EFFECT_SCRIPT_CHILD_SLOT_COUNT; i++) {
        if (record->child_indices[i] != 0) {
            count++;
        }
    }
    if (count == target) {
        record->pc = battle_effect_get_current_script_instruction(record)->branch.target_offset;
    } else {
        record->pc += 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
