#include "fft/battle_unit.h"
#include "psx/types.h"

/* Clear rotation, delay and pending animation for 21 slots; capture facing or 0xff when absent. */
void unit_reset_animation_states(void) {
    unit_animation_state_t* state;
    s32 i;

    i = 0;
    state = g_unit_animation_states;
    for (; i < BATTLE_UNIT_SLOT_COUNT; i++, state++) {
        state->rotating = 0;
        state->delay = 0;
        if (battle_unit_has_runtime_id(i) != 0) {
            state->initial_facing = battle_unit_get_facing_nibble_by_runtime_id(i);
        } else {
            state->initial_facing = 0xff;
        }
        g_unit_pending_animation_ids[i] = 0;
        g_unit_was_moving_latch[i] = 0;
    }
}
