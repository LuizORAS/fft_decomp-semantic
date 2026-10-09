#include "fft/battle.h"

/*
 * Render the emitter selected by effect-script opcode 0x26.
 *
 * Bits 10–13 select the emitter. The command then advances by one halfword and
 * returns 1 so effect-script execution can continue.
 */
s32 battle_effect_code_script_26_render_emitter(effect_record_t* effect) {
    battle_effect_spawn_emitter_particles(
        effect->record_index, effect->emitter_index, (*(u16*)(effect->script + effect->pc) >> 10) & 0xf, 0);
    effect->pc += 2;
    return EFFECT_SCRIPT_RESULT_CONTINUE;
}
