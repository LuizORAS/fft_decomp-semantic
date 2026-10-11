#include "fft/battle.h"

/* Opcode 10: set the counter selected by the top two bits of the
 * instruction's second byte to the second 16-bit value. */
effect_script_result_e battle_effect_code_script_10_set_selected_counter(effect_record_t* record) {
    battle_effect_script_parameters_t parameters;
    battle_effect_script_instruction_t* instruction;
    u32 slot;

    instruction = battle_effect_get_current_script_instruction(record);
    parameters = instruction->header.parameters;
    slot = parameters.counter.slot;
    record->counters[slot] = instruction->immediate.value;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
