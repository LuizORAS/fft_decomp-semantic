#include "fft/battle.h"

/* Opcode 07: call the native function whose id is the second halfword with
 * the record index and the top six bits of the instruction's second byte as
 * its callback slot, and remember it in the record. */
s32 battle_effect_code_script_07_call_file_function(effect_record_t* record) {
    battle_effect_script_instruction_t* command = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    u32 slot = command->value.header.bytes.packed_parameters >> 2;
    battle_effect_callback_t handler = battle_effect_get_file_function(command->value.value.signed_value);
    record->file_function = handler;
    handler(record->record_index, slot, 0, 0);
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
