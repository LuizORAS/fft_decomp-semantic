#include "fft/battle.h"

/* Opcode 10: set the counter selected by the top two bits of the
 * instruction's second byte to the second halfword. */
s32 battle_effect_code_script_10_set_selected_counter(effect_record_t* record) {
    battle_effect_script_value_instruction_t* cur;
    u32 slot;

    cur = (battle_effect_script_value_instruction_t*)(record->pc + record->script);
    slot = cur->header.bytes.packed_parameters >> 6;
    record->counters[slot] = cur->value.unsigned_value;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
