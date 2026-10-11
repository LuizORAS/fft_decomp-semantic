#include "fft/battle.h"

/* Opcode 02: start a child record on this script at the offset in the second
 * 16-bit value and keep its index in the child slot named by the high nibble of
 * the instruction's second byte (slots 4 and up keep nothing). */
effect_script_result_e battle_effect_code_script_02_spawn_child(effect_record_t* record) {
    battle_effect_script_parameters_t parameters;
    s16 slot;
    s16 child;
    battle_effect_script_instruction_t* instruction;

    instruction = battle_effect_get_current_script_instruction(record);
    parameters = instruction->header.parameters;
    slot = parameters.child.slot;
    child = battle_effect_start_script_record(
        record->script, instruction->immediate.value, g_battle_effect_current_record_index);
    if (slot < EFFECT_SCRIPT_CHILD_SLOT_COUNT) {
        record->child_indices[slot] = child;
    }
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
