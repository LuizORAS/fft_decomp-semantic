#include "fft/battle_effect.h"
#include "psx/libgte.h"

/* The subset of effect_init_render_state that does not touch the
 * prim buffer or the sprite counters. */
void effect_reset_render_state(void) {
    g_effect_matrix.m[2][2] = ONE;
    g_effect_matrix.m[2][1] = 0;
    g_effect_matrix.m[2][0] = 0;
    g_effect_matrix.m[1][2] = 0;
    g_effect_matrix.m[0][2] = 0;
    g_effect_matrix.t[2] = 0;
    g_effect_corner_bottom_right.vz = EFFECT_CORNER_Z_DEFAULT;
    g_effect_corner_bottom_left.vz = EFFECT_CORNER_Z_DEFAULT;
    g_effect_corner_top_right.vz = EFFECT_CORNER_Z_DEFAULT;
    g_effect_corner_top_left.vz = EFFECT_CORNER_Z_DEFAULT;
}
