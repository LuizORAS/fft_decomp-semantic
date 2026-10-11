#include "fft/battle.h"

/* Opcode 16: jump to the offset in the third halfword when the record has as
 * many particles as the second halfword says; otherwise step over the 6-byte
 * instruction. Retail scripts end with it, waiting for 0. */
s32 battle_effect_code_script_16_jump_if_work_count_equal(effect_record_t* record) {
    battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);
    if (record->work_count == instruction->branch.comparison) {
        record->pc = (s16)instruction->branch.target_offset;
    } else {
        record->pc += 6;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
