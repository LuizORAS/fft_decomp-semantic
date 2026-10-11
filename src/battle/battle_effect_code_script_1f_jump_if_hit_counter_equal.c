#include "fft/battle.h"
#include "psx/types.h"

/* Opcode 1f: jump to the offset in the second halfword when the effect has as
 * many targets as the instruction's second byte shifted right by 2 says;
 * otherwise step over the 4-byte instruction. Retail scripts use it to end
 * at once when there is no target. */
s32 battle_effect_code_script_1f_jump_if_hit_counter_equal(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    if (g_battle_effect_context.target_count == (instruction->value.header.bytes.packed_parameters >> 2)) {
        record->pc = instruction->value.value.unsigned_value;
    } else {
        record->pc += 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
