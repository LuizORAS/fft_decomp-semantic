#include "fft/battle_effect.h"

/* Opcode 1f: jump to the offset in the second 16-bit value when the effect has as
 * many targets as the instruction's second byte shifted right by 2 says;
 * otherwise step over the 4-byte instruction. Retail scripts use it to end
 * at once when there is no target. */
effect_script_result_e battle_effect_code_script_1f_jump_if_target_count_equal(battle_effect_record_t* record) {
    const effect_script_instruction_t* instruction = effect_get_current_script_instruction(record);
    effect_script_parameters_t parameters = instruction->header.parameters;
    if (g_battle_effect_coord_data.target_count == parameters.targets.count) {
        record->pc = instruction->immediate.value;
    } else {
        record->pc += 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
