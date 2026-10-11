#include "fft/battle.h"

/* Opcode 21: subtract 1 from the counter selected by the top two bits of the
 * instruction's second byte. */
s32 battle_effect_code_script_21_decrement_selected_counter(effect_record_t* record) {
    battle_effect_script_header_t* instruction;

    instruction = (battle_effect_script_header_t*)(record->script + record->pc);
    record->counters[instruction->bytes.packed_parameters >> 6] -= 1;
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
