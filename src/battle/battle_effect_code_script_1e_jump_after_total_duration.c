#include "fft/battle.h"
#include "psx/types.h"

/* Opcode 1e: jump to the offset in the second halfword once the record's
 * timeline frame reaches the end of the parent timeline (phase 1, a spawn
 * delay for each target after the first, the last child's duration and phase
 * 2); otherwise step over the 4-byte instruction. */
s32 battle_effect_code_script_1e_jump_after_total_duration(effect_record_t* record) {
    effect_timeline_t* timing;

    timing = g_battle_effect_timeline;
    if (record->timeline_frame.s >= (s32)(timing->phase1_duration + timing->phase2_duration
            + (g_battle_effect_context.target_count - 1) * timing->spawn_delay + timing->child_duration)) {
        battle_effect_script_instruction_t* instruction = battle_effect_get_current_script_instruction(record);

        record->pc = instruction->immediate.value;
    } else {
        record->pc += 4;
    }
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
