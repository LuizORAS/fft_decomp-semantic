#include "fft/battle.h"
#include "psx/types.h"

/* Opcode 1f: jump to the offset in the second halfword when the effect has as
 * many targets as the instruction's second byte shifted right by 2 says;
 * otherwise step over the 4-byte instruction. Retail scripts use it to end
 * at once when there is no target. */
s32 battle_effect_code_script_1f_jump_if_hit_counter_equal(effect_record_t* record) {
    s16 pc = record->pc;
    s32 script = (s32)record->script;
    battle_effect_script_value_instruction_t* instruction = (battle_effect_script_value_instruction_t*)((pc + script));
    if (g_battle_effect_coord_data.target_count == (instruction->header.bytes.packed_parameters >> 2)) {
        record->pc = instruction->value.unsigned_value;
    } else {
        record->pc = pc + 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
