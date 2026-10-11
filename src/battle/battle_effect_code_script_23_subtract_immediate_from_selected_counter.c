#include "fft/battle.h"

/* Opcode 23: subtract the second halfword from the counter selected by the
 * top two bits of the instruction's second byte. */
s32 battle_effect_code_script_23_subtract_immediate_from_selected_counter(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction;
    u16 amount;

    instruction = battle_effect_get_current_script_instruction(record);
    amount = (u16)instruction->immediate.value;
    record->counters[instruction->header.packed_parameters >> 6] -= amount;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
