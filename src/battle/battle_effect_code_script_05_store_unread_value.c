#include "fft/battle_effect.h"

/* Opcode 05: store the high nibble of the instruction's second byte in
 * D_801BF000, which retail clears before the script runs
 * (battle_effect_update_stage) and never reads. Every retail script begins with it. */
effect_script_result_e battle_effect_code_script_05_store_unread_value(battle_effect_record_t* record) {
    const effect_script_instruction_t* instruction = effect_get_current_script_instruction(record);

    effect_script_parameters_t parameters = instruction->header.parameters;
    D_801BF000 = parameters.opcode_05.high_nibble;
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
