#include "fft/wldcore.h"

/* The y counterpart of wldcore_map_apply_clamped_horizontal_scroll, within
 * WLDCORE_SCROLL_MIN_Y..WLDCORE_SCROLL_MAX_Y, for the y members of both points. */
void wldcore_map_apply_clamped_vertical_scroll(wldcore_point32_t* amount, wldcore_point32_t* position) {
    s32 delta;
    s32 old_position;
    s32 value;

    delta = -amount->y;
    amount->y = delta;
    if (delta < 0) {
        old_position = position->y;
        value = old_position + delta;
        if (value < WLDCORE_SCROLL_MIN_Y) {
            value = WLDCORE_SCROLL_MIN_Y;
        }
        amount->y = delta - (value - old_position);
        position->y = value;
    }

    if (amount->y > 0) {
        old_position = position->y;
        value = old_position + amount->y;
        if (value >= (WLDCORE_SCROLL_MAX_Y + 1)) {
            value = WLDCORE_SCROLL_MAX_Y;
        }
        amount->y -= value - old_position;
        position->y = value;
    }

    amount->y = -amount->y;
}
