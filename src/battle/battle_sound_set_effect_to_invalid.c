#include "fft/battle.h"
#include "psx/types.h"

/* Queue the Invalid sound (MAIN_SFX_INVALID) for this frame, as battle_sound_set_effect_to_cancel
 * does. */
void battle_sound_set_effect_to_invalid(void) {
    g_battle_sound_pending_sfx_id = MAIN_SFX_INVALID;
}
