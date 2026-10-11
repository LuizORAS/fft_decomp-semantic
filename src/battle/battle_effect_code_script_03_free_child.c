#include "fft/battle.h"

/* Opcode 03: free the child record in the child slot named by bits 4-5 of
 * the instruction's second byte. */
effect_script_result_e battle_effect_code_script_03_free_child(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);
    s32 slot = (instruction->header.packed_parameters >> 4) & 0x3;
    s16 child = record->child_indices[slot];
    if (child != 0)
        battle_effect_free_state_record(child);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
