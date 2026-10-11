#include "fft/battle.h"

/* Opcode 10: set the counter selected by the top two bits of the
 * instruction's second byte to the second halfword. */
s32 battle_effect_code_script_10_set_selected_counter(effect_record_t* record) {
    u8* cur;
    u32 slot;

    cur = record->pc + record->script;
    slot = cur[1] >> 6;
    record->counters[slot] = *(u16*)(cur + 2);
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
