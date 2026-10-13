#include "fft/battle_sound.h"

void sound_queue_sfx_cursor_move(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
}
