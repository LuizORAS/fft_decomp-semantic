#include "fft/battle.h"
#include "psx/types.h"

/* Opcode 1f: jump to the offset in the second halfword when the effect has as
 * many targets as the instruction's second byte shifted right by 2 says;
 * otherwise step over the 4-byte instruction. Retail scripts use it to end
 * at once when there is no target. */
s32 battle_effect_code_script_1f_jump_if_target_count_equal(effect_record_t* record) {
    s16 pc = record->pc;
    s32 script = (s32)record->script;
    u8* instruction = (u8*)(pc + script);
    if (g_battle_effect_coord_data.target_count == (instruction[1] >> 2)) {
        record->pc = *(u16*)(instruction + 2);
    } else {
        record->pc = pc + 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
