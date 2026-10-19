#include "fft/battle_effect.h"

/* Opcode 10: set the counter selected by the top two bits of the
 * instruction's second byte to the second 16-bit value. */
effect_script_result_e battle_effect_code_script_10_set_selected_counter(battle_effect_record_t* record) {
    effect_script_parameters_t parameters;
    const effect_script_instruction_t* instruction;

    instruction = effect_get_current_script_instruction(record);
    parameters = instruction->header.parameters;
    record->counters[parameters.counter.slot] = instruction->immediate.value;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
