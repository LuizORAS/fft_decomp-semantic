#include "fft/world.h"
#include "psx/types.h"

/* Queue the Cancel sound (MAIN_SFX_CANCEL) in g_world_sound_pending_sfx_id, which
 * world_sound_dispatch_effect plays. */
void world_sound_set_effect_to_cancel(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CANCEL;
}
