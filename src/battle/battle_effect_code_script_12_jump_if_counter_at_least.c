#include "fft/battle.h"

/* Opcode 12: jump to the offset in the third halfword when the counter
 * selected by the top two bits of the second byte is at least the second
 * halfword; otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_12_jump_if_counter_at_least(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    if (record->counters[instruction->branch.header.bytes.packed_parameters >> 6]
        >= instruction->branch.comparison.signed_value) {
        record->pc = instruction->branch.target_offset.unsigned_value;
    } else {
        record->pc += 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
