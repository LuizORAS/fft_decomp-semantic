#include "fft/battle.h"

/* Opcode 06: put the native function whose id is the second halfword in the
 * callback slot named by the top six bits of the instruction's second byte.
 * A particle keyframe whose action names the slot binds it to the record. */
s32 battle_effect_code_script_06_load_callback_pointer(effect_record_t* record) {
    u8* command = record->pc + record->script;
    u32 idx = command[1] >> 2;
    g_battle_effect_callback_slots[idx] = (s32)battle_effect_get_file_pointer(*(s16*)(command + 2));
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
