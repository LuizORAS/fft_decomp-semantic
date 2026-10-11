#include "fft/battle.h"

enum { EFFECT_SCRIPT_CHILD_SLOT_COUNT = 4 };

/* Opcode 02: start a child record on this script at the offset in the second
 * halfword and keep its index in the child slot named by the high nibble of
 * the instruction's second byte (slots 4 and up keep nothing). */
s32 battle_effect_code_script_02_spawn_child(effect_record_t* record) {
    s16 slot;
    s16 child;
    battle_effect_script_instruction_t* instruction;

    instruction = battle_effect_get_current_script_instruction(record);
    slot = instruction->header.packed_parameters >> 4;
    child = battle_effect_start_script_record(
        record->script, instruction->immediate.value, g_battle_effect_current_record_index);
    if (slot < EFFECT_SCRIPT_CHILD_SLOT_COUNT) {
        record->child_indices[slot] = child;
    }
    record->pc += 4;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
