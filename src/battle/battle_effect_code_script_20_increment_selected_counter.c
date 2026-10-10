#include "fft/battle.h"

/* Opcode 20: add 1 to the counter selected by the top two bits of the
 * instruction's second byte. */
s32 battle_effect_code_script_20_increment_selected_counter(effect_record_t* record) {
    u8* instruction;

    instruction = record->script + record->pc;
    record->counters[instruction[1] >> 6] += 1;
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
