#include "fft/world.h"
#include "psx/types.h"

/* Queue the Confirm sound (MAIN_SFX_CONFIRM), as world_sound_set_effect_to_cancel does. */
void world_sound_set_effect_to_confirm(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CONFIRM;
}
