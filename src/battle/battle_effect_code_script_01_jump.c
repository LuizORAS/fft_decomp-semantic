#include "fft/battle.h"

/* Opcode 01: jump to the script offset in the second 16-bit value. */
effect_script_result_e battle_effect_code_script_01_jump(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);

    record->pc = instruction->immediate.value;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
