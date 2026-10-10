#include "fft/battle.h"
#include "psx/types.h"

/* Nonzero while an effect cycle or a unit step is still running; the event script
 * polls it to wait for an effect to end. */
s32 battle_state_get_animation_continue_check(void) {
    return g_battle_state_animation_continue_check;
}
