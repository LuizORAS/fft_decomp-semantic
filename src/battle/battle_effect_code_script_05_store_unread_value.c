#include "fft/battle.h"

/* Opcode 05: store the high nibble of the instruction's second byte in
 * D_801BF000, which nothing reads. Every retail script begins with it. */
s32 battle_effect_code_script_05_store_unread_value(effect_record_t* record) {
    u8* instruction = record->script + record->pc;

    D_801BF000 = instruction[1] >> 4;
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
