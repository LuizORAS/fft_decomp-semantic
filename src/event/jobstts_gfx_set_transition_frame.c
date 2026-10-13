#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Set the window-opening step (g_jobstts_gfx_transition_frame); the screen sets 10 to show its windows at full size at
 * once. */
void jobstts_gfx_set_transition_frame(s32 frame) {
    g_jobstts_gfx_transition_frame = frame;
}
