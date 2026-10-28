#include "fft/wldcore.h"

/* The camera origin that centers a map position: the position negated and clamped to the scroll
 * limits (WLDCORE_SCROLL_MIN/MAX_X and _Y). */
void wldcore_map_negate_and_clamp_coordinates(const SVECTOR* input, VECTOR* output) {
    s32 x;
    s32 y;

    x = -input->vx;
    y = -input->vy;
    if (x < WLDCORE_SCROLL_MIN_X) {
        x = WLDCORE_SCROLL_MIN_X;
    }
    if (x > WLDCORE_SCROLL_MAX_X) {
        x = WLDCORE_SCROLL_MAX_X;
    }
    if (y < WLDCORE_SCROLL_MIN_Y) {
        y = WLDCORE_SCROLL_MIN_Y;
    }
    if (y > WLDCORE_SCROLL_MAX_Y) {
        y = WLDCORE_SCROLL_MAX_Y;
    }
    output->vx = x;
    output->vy = y;
}
