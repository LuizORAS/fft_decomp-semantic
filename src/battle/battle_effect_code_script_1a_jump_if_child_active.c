#include "fft/battle.h"

/* Opcode 1a: jump to the offset in the second halfword when the child slot
 * named by the second byte's high nibble holds a record; otherwise step over
 * the 4-byte instruction. */
s32 battle_effect_code_script_1a_jump_if_child_active(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction;
    u32 slot;

    instruction = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);
    slot = instruction->value.header.bytes.packed_parameters >> 4;
    if (record->child_indices[slot] != 0) {
        record->pc = instruction->value.value.unsigned_value;
    } else {
        record->pc = record->pc + 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
