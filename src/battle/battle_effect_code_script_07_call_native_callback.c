#include "fft/battle_effect.h"

/* Opcode 07: call native callback `id` (the second 16-bit value) with the record index and the top six bits of
 * the instruction's second byte as its callback slot, and keep it in the record's last_native_callback. */
effect_script_result_e battle_effect_code_script_07_call_native_callback(battle_effect_record_t* record) {
    const effect_script_instruction_t* command = effect_get_current_script_instruction(record);
    effect_script_parameters_t parameters = command->header.parameters;
    u32 slot = parameters.callback.slot;
    battle_effect_callback_t handler = battle_effect_get_native_callback(command->immediate.value);
    record->last_native_callback = handler;
    handler(record->record_index, slot, 0, 0);
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
