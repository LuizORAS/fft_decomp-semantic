#include "fft/battle_effect.h"

/* Advance one effect slot (a particle of a secondary effect) by a frame: step its
 * motion, advance its frame script, in which each step shows a frame for hold
 * calls and the last step wraps to the first, and draw it as a billboard.
 *
 * Returns 0 when the slot has finished: a positive life counts down and ends the
 * slot at 0; a negative life ends it on the frame the script wraps. */
s32 battle_effect_update_slot(s32 id) {
    battle_effect_slot_t* slot = &g_battle_effect_slots[id];
    s32 wrapped;
    s32 result;

    battle_effect_step_motion(&slot->motion);
    wrapped = 0;
    if (--slot->hold <= 0) {
        slot->frame = slot->script->step[slot->step_index].frame;
        slot->hold = slot->script->step[slot->step_index].hold;
        slot->step_index++;
        if (slot->step_index >= slot->script->count) {
            wrapped = 1;
            slot->step_index = 0;
        }
    }
    battle_effect_draw_billboard_sprite(slot);
    result = 1;
    if (slot->life < 0) {
        result = wrapped == 0;
    } else if (--slot->life == 0) {
        result = 0;
    }
    return result;
}
