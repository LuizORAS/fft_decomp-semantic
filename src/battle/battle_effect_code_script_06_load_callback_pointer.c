#include "fft/battle.h"

/* Opcode 06: put the native function whose id is the second halfword in the
 * callback slot named by the top six bits of the instruction's second byte.
 * A particle keyframe whose action names the slot binds it to the record. */
s32 battle_effect_code_script_06_load_callback_pointer(effect_record_t* record) {
    battle_effect_script_value_instruction_t* command
        = (battle_effect_script_value_instruction_t*)(record->pc + record->script);
    u32 idx = command->header.bytes.packed_parameters >> 2;
    g_battle_effect_callback_slots[idx] = (s32)battle_effect_get_file_pointer(command->value.signed_value);
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
