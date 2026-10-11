#include "fft/battle.h"

/* Opcode 00: jump to the script offset in the second halfword and end the
 * record's run for this frame. */
s32 battle_effect_code_script_00_jump_and_yield(effect_record_t* record) {
    s32 pc = record->pc;
    s32 base = (s32)record->script;
    record->pc = ((battle_effect_script_value_instruction_t*)(pc + base))->value.unsigned_value;
    return EFFECT_SCRIPT_RESULT_YIELD;
}
