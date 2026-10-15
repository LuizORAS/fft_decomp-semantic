#include "fft/battle.h"
#include "psx/types.h"

/* Queue the Cancel sound (MAIN_SFX_CANCEL) for this frame: battle_script_process_pending_requests plays
 * g_battle_sound_pending_sfx_id after the threads have run. */
void battle_sound_set_effect_to_cancel(void) {
    g_battle_sound_pending_sfx_id = MAIN_SFX_CANCEL;
}
