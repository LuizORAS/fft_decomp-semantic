#include "fft/battle.h"

/* Opcode 13: jump to the offset in the third halfword when the counter
 * selected by the top two bits of the second byte exceeds the second
 * halfword; otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_13_jump_if_counter_greater(effect_record_t* record) {
    s16 pc = record->pc;
    u8* cur = pc + record->script;
    s16* counter = &record->counters[cur[1] >> 6];
    if (*counter > *(s16*)(cur + 2)) {
        record->pc = *(u16*)(cur + 4);
    } else {
        record->pc = pc + 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
