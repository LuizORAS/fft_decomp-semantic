#include "fft/battle.h"

/* Opcode 13: jump to the offset in the third halfword when the counter
 * selected by the top two bits of the second byte exceeds the second
 * halfword; otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_13_jump_if_counter_greater(effect_record_t* record) {
    s16 pc = record->pc;
    battle_effect_script_branch_instruction_t* cur = (battle_effect_script_branch_instruction_t*)(pc + record->script);
    s16* counter = &record->counters[cur->header.bytes.packed_parameters >> 6];
    if (*counter > cur->comparison.signed_value) {
        record->pc = cur->target_offset.unsigned_value;
    } else {
        record->pc = pc + 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
