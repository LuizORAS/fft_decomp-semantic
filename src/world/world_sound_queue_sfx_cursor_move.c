#include "fft/world.h"

void world_sound_queue_sfx_cursor_move(void) {
    g_world_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
}
