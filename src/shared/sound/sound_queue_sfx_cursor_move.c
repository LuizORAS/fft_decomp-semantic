#include "fft/main.h"
#include "fft/sound.h"

/* Queue the Cursor Move sound (MAIN_SFX_CURSOR_MOVE) for this frame, as
 * sound_queue_sfx_cancel does. */
void sound_queue_sfx_cursor_move(void) {
    g_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
}
