#include "fft/battle.h"

/* Opcode 14: jump to the offset in the third halfword when the counter
 * selected by the top two bits of the second byte is at most the second
 * halfword; otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_14_jump_if_counter_at_most(effect_record_t* record) {
    battle_effect_script_instruction_t* cur = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    s16* counter = &record->counters[cur->branch.header.bytes.packed_parameters >> 6];
    if (*counter <= cur->branch.comparison.signed_value) {
        record->pc = cur->branch.target_offset.unsigned_value;
    } else {
        record->pc += 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
