#include "fft/battle_effect.h"
#include "psx/libgte.h"

/* Begin one effect-rendering frame.
 *
 * The renderer alternates packet storage and remembers the arena offset so
 * the frame's transient primitives can be reclaimed together. */
void effect_start_render_frame(void) {
    SetGeomOffset(0, 0);
    g_effect_frame_parity = 1 - g_effect_frame_parity;
    g_effect_frame_start_prim_buffer_offset = g_effect_prim_buffer_offset;
}
