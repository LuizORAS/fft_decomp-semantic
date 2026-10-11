#include "fft/battle.h"

/* Opcode 10: set the counter selected by the top two bits of the
 * instruction's second byte to the second halfword. */
s32 battle_effect_code_script_10_set_selected_counter(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction;
    u32 slot;

    instruction = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    slot = instruction->value.header.bytes.packed_parameters >> 6;
    record->counters[slot] = instruction->value.value.unsigned_value;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
