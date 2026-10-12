#include "fft/world.h"
#include "psx/types.h"

void world_sound_set_scroll_effect(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
}
