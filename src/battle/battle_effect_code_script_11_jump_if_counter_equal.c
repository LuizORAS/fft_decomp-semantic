#include "fft/battle.h"

/* Opcode 11: jump to the offset in the third halfword when the counter
 * selected by the top two bits of the second byte equals the second halfword;
 * otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_11_jump_if_counter_equal(effect_record_t* record) {
    s16 pc = record->pc;
    battle_effect_script_branch_instruction_t* instruction
        = (battle_effect_script_branch_instruction_t*)(pc + record->script);
    if (record->counters[instruction->header.bytes.packed_parameters >> 6] == instruction->comparison.signed_value) {
        record->pc = instruction->target_offset.unsigned_value;
    } else {
        record->pc = pc + 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
