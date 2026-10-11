#include "fft/battle.h"
#include "psx/types.h"

/* Opcode 1f: jump to the offset in the second 16-bit value when the effect has as
 * many targets as the instruction's second byte shifted right by 2 says;
 * otherwise step over the 4-byte instruction. Retail scripts use it to end
 * at once when there is no target. */
effect_script_result_e battle_effect_code_script_1f_jump_if_hit_counter_equal(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);
    if (g_battle_effect_context.target_count == (instruction->header.packed_parameters >> 2)) {
        record->pc = instruction->immediate.value;
    } else {
        record->pc += 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
