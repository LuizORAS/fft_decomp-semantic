#include "fft/battle.h"

/* Opcode 15: jump to the offset in the third halfword when the counter
 * selected by the top two bits of the second byte is below the second
 * halfword; otherwise step over the 6-byte instruction. */
s32 battle_effect_code_script_15_jump_if_counter_less(effect_record_t* record) {
    s16 pc = record->pc;
    u8* instruction = record->script + pc;
    if (record->counters[instruction[1] >> 6] < *(s16*)(instruction + 2)) {
        record->pc = *(u16*)(instruction + 4);
    } else {
        record->pc = pc + 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
