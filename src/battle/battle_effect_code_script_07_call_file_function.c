#include "fft/battle.h"

typedef void (*battle_effect_function_t)(s16, u32, s32, s32);

/* Opcode 07: call the native function whose id is the second halfword with
 * the record index and the top six bits of the instruction's second byte as
 * its callback slot, and remember it in the record. */
s32 battle_effect_code_script_07_call_file_function(effect_record_t* record) {
    u8* command = record->pc + record->script;
    u32 slot = command[1] >> 2;
    battle_effect_function_t handler = battle_effect_get_file_pointer(*(s16*)(command + 2));
    record->file_function = handler;
    handler(record->record_index, slot, 0, 0);
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
