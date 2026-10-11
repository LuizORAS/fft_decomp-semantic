#include "fft/battle.h"
#include "psx/types.h"

/* Opcode 1d: jump to the offset in the second halfword once the record's
 * timeline frame reaches the child timeline's duration; otherwise step over
 * the 4-byte instruction. */
s32 battle_effect_code_script_1d_jump_after_effect_length(effect_record_t* record) {
    if (record->timeline_frame.s >= g_battle_effect_timing_channels->duration) {
        battle_effect_script_instruction_t* instruction
            = BATTLE_EFFECT_SCRIPT_INSTRUCTION_AT(record->script, record->pc);

        record->pc = instruction->value.value.unsigned_value;
    } else {
        record->pc = record->pc + 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
