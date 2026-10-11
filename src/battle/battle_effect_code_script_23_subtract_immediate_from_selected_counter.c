#include "fft/battle.h"

/* Opcode 23: subtract the second 16-bit value from the counter selected by the
 * top two bits of the instruction's second byte. */
effect_script_result_e battle_effect_code_script_23_subtract_immediate_from_selected_counter(effect_record_t* record) {
    battle_effect_script_parameters_t parameters;
    battle_effect_script_instruction_t* instruction;
    u16 amount;

    instruction = battle_effect_get_current_script_instruction(record);
    parameters = instruction->header.parameters;
    amount = instruction->immediate.value;
    record->counters[parameters.counter.slot] -= amount;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
