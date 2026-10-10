#include "fft/battle.h"

/* Opcode 18: jump to the offset in the third halfword when the record has
 * fewer particles than the second halfword says; otherwise step over the
 * 6-byte instruction. */
s32 battle_effect_code_script_18_jump_if_work_count_less(effect_record_t* record) {
    s16 pc = record->pc;
    s16* instruction = (s16*)(pc + record->script);
    if (record->work_count < instruction[1]) {
        record->pc = *(instruction + 2);
    } else {
        record->pc = pc + 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
