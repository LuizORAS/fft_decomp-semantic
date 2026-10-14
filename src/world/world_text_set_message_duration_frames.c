#include "fft/world.h"
#include "psx/types.h"

/* Set how long the next timed message (world_menu_icon_timed_thread) stays up before it closes by
 * itself; same as battle_text_set_message_duration_frames. */
void world_text_set_message_duration_frames(s32 frames) {
    g_world_text_message_duration_frames = frames;
}
