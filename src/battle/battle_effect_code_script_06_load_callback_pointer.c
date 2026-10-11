#include "fft/battle.h"

/* Opcode 06: put the native function whose id is the second 16-bit value in the
 * callback slot named by the top six bits of the instruction's second byte.
 * A particle keyframe whose action names the slot binds it to the record. */
effect_script_result_e battle_effect_code_script_06_load_callback_pointer(effect_record_t* record) {
    battle_effect_script_instruction_t* command = battle_effect_get_current_script_instruction(record);
    u32 idx = command->header.packed_parameters >> 2;
    g_battle_effect_callback_slots[idx] = battle_effect_get_file_function(command->immediate.value);
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
