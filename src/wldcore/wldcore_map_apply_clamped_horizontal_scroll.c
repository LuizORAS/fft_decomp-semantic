#include "fft/wldcore.h"

/* Scroll the map by the x of a cursor move: the camera origin `position` moves by -amount, clamped
 * to WLDCORE_SCROLL_MIN_X..WLDCORE_SCROLL_MAX_X (the range wldcore_map_negate_and_clamp_coordinates
 * uses), and the part the limit stops is left in `amount`, so the cursor itself moves by it. */
void wldcore_map_apply_clamped_horizontal_scroll(s32* amount, s32* position) {
    s32 delta;
    s32 old_position;
    s32 value;

    delta = -(*amount);
    (*amount) = delta;
    if (delta < 0) {
        old_position = *position;
        value = old_position + delta;
        if (value < WLDCORE_SCROLL_MIN_X) {
            value = WLDCORE_SCROLL_MIN_X;
        }
        (*amount) = delta - (value - old_position);
        (*position) = value;
    }

    if ((*amount) > 0) {
        old_position = *position;
        value = old_position + (*amount);
        if (value >= (WLDCORE_SCROLL_MAX_X + 1)) {
            value = WLDCORE_SCROLL_MAX_X;
        }
        (*amount) -= value - old_position;
        (*position) = value;
    }

    (*amount) = -(*amount);
}
