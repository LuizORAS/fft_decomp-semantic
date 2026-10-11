#include "fft/battle.h"

/* Opcode 23: subtract the second halfword from the counter selected by the
 * top two bits of the instruction's second byte. */
s32 battle_effect_code_script_23_subtract_immediate_from_selected_counter(effect_record_t* record) {
    battle_effect_script_value_instruction_t* instruction;
    u16 amount;

    instruction = (battle_effect_script_value_instruction_t*)(record->script + record->pc);
    amount = instruction->value.unsigned_value;
    record->counters[instruction->header.bytes.packed_parameters >> 6] -= amount;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
