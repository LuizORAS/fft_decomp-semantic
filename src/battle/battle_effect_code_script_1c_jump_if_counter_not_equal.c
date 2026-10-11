#include "fft/battle.h"

/* Opcode 1c: jump to the offset in the third halfword when the counter
 * selected by the second byte's top two bits differs from the second
 * halfword; otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_1c_jump_if_counter_not_equal(effect_record_t* record) {
    battle_effect_script_branch_instruction_t* cur;
    u32 slot;
    s16 pc;

    pc = record->pc;
    cur = (battle_effect_script_branch_instruction_t*)(pc + record->script);
    slot = cur->header.bytes.packed_parameters >> 6;
    if (record->counters[slot] != cur->comparison.signed_value) {
        record->pc = cur->target_offset.unsigned_value;
    } else {
        record->pc = pc + 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
