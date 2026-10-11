#include "fft/battle.h"

/* Opcode 22: add the second 16-bit value to the counter selected by the top two
 * bits of the instruction's second byte. */
effect_script_result_e battle_effect_code_script_22_add_immediate_to_selected_counter(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction;
    u16 amount;

    instruction = battle_effect_get_current_script_instruction(record);
    amount = instruction->immediate.value;
    record->counters[instruction->header.packed_parameters >> 6] += amount;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
