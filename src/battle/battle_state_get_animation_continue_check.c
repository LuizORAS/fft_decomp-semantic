#include "fft/battle.h"
#include "psx/types.h"

/* Return g_battle_state_animation_continue_check: 2 while an effect runs this frame, plus any
 * action target still off its tile centre. */
s32 battle_state_get_animation_continue_check(void) {
    return g_battle_state_animation_continue_check;
}
