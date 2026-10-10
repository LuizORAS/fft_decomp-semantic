#include "fft/world.h"
#include "psx/types.h"

/* Frames the message/dialogue wait loop will hold before it fakes a confirm
 * press. */
void world_text_set_message_duration_frames(s32 frames) {
    g_world_text_message_duration_frames = frames;
}
