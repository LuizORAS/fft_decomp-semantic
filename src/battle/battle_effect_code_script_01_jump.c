#include "fft/battle.h"

/* Opcode 01: jump to the script offset in the second halfword. */
s32 battle_effect_code_script_01_jump(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);

    record->pc = instruction->value.value.unsigned_value;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
