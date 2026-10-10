#include "fft/battle.h"

/* Opcode 22: add the second halfword to the counter selected by the top two
 * bits of the instruction's second byte. */
s32 battle_effect_code_script_22_add_immediate_to_selected_counter(effect_record_t* record) {
    u8* instruction;
    u16 amount;

    instruction = record->script + record->pc;
    amount = *(u16*)(instruction + 2);
    record->counters[instruction[1] >> 6] += amount;
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
