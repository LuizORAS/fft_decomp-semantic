#include "fft/battle.h"

/* Opcode 00: jump to the script offset in the second halfword and end the
 * record's run for this frame. */
s32 battle_effect_code_script_00_jump_and_yield(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);

    record->pc = instruction->value.value.unsigned_value;
    return EFFECT_SCRIPT_RESULT_YIELD;
}
