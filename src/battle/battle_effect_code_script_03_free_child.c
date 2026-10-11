#include "fft/battle.h"

/* Opcode 03: free the child record in the child slot named by bits 4-5 of
 * the instruction's second byte. */
s32 battle_effect_code_script_03_free_child(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    s32 slot_offset = (instruction->header.bytes.packed_parameters >> 3) & 6;
    s16 child = record->child_indices[slot_offset >> 1];
    if (child != 0)
        battle_effect_free_state_record(child);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
