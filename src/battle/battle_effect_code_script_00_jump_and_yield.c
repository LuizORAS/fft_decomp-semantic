#include "fft/battle.h"

/* Opcode 00: jump to the script offset in the second 16-bit value and end the
 * record's run for this frame. */
effect_script_result_e battle_effect_code_script_00_jump_and_yield(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);

    record->pc = instruction->immediate.value;
    return EFFECT_SCRIPT_RESULT_YIELD;
}
