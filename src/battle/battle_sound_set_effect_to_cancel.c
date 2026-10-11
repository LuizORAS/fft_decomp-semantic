#include "fft/battle.h"
#include "psx/types.h"

void battle_sound_set_effect_to_cancel(void) {
    g_battle_sound_pending_sfx_id = MAIN_SFX_CANCEL;
}
