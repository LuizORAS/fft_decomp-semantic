#include "fft/battle.h"

/* Opcode 26: spawn this frame's particles of the emitter named by bits 10-13
 * of the instruction word, at the record's frame counter. */
s32 battle_effect_code_script_26_render_emitter(effect_record_t* record) {
    battle_effect_spawn_emitter_particles(
        record->record_index, record->frame_counter, (battle_effect_get_current_script_header(record) >> 10) & 0xf, 0);
    record->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
