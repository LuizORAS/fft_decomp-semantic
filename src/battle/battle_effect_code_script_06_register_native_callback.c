#include "fft/battle_effect.h"

/* Opcode 06: put the native callback whose catalog id is the second 16-bit value in the
 * callback slot named by the top six bits of the instruction's second byte.
 * A particle keyframe whose action names the slot binds it to the record. */
effect_script_result_e battle_effect_code_script_06_register_native_callback(effect_record_t* record) {
    battle_effect_script_instruction_t* command = battle_effect_get_current_script_instruction(record);
    battle_effect_script_parameters_t parameters = command->header.parameters;
    u32 slot = parameters.callback.slot;
    g_battle_effect_callback_slots[slot] = battle_effect_get_native_callback(command->immediate.value);
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
