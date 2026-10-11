#include "fft/battle.h"

/* Opcode 1a: jump to the offset in the second halfword when the child slot
 * named by the second byte's high nibble holds a record; otherwise step over
 * the 4-byte instruction. */
s32 battle_effect_code_script_1a_jump_if_child_active(effect_record_t* record) {
    battle_effect_script_value_instruction_t* cur;
    u32 slot;

    cur = (battle_effect_script_value_instruction_t*)(record->pc + record->script);
    slot = cur->header.bytes.packed_parameters >> 4;
    if (record->child_indices[slot] != 0) {
        record->pc = cur->value.unsigned_value;
    } else {
        record->pc = record->pc + 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
