#include "fft/battle.h"

/* Opcode 13: jump to the offset in the third halfword when the counter
 * selected by the top two bits of the second byte exceeds the second
 * halfword; otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_13_jump_if_counter_greater(effect_record_t* record) {
    battle_effect_script_instruction_t* cur = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    s16* counter = &record->counters[cur->branch.header.bytes.packed_parameters >> 6];
    if (*counter > cur->branch.comparison.signed_value) {
        record->pc = cur->branch.target_offset.unsigned_value;
    } else {
        record->pc += 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
