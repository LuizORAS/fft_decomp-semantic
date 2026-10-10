#include "fft/battle.h"

/* Opcode 19: jump to the offset in the third halfword when the number of
 * occupied child slots equals the second halfword; otherwise step over the
 * 6-byte instruction. */
s32 battle_effect_code_script_19_jump_if_child_count_equal(effect_record_t* record) {
    s16 i;
    s16 count;
    s16 target;

    target = *(u16*)(record->pc + (s32)record->script + 2);
    count = 0;
    for (i = 0; i < 4; i++) {
        if (record->child_indices[i] != 0) {
            count++;
        }
    }
    if (count == target) {
        record->pc = *(u16*)(record->pc + (s32)record->script + 4);
    } else {
        record->pc += 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
