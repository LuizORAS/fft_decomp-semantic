#include "fft/wldcore.h"

/* Scroll the map by a cursor move on both axes (wldcore_map_apply_clamped_horizontal_scroll, then
 * the vertical one): `position` moves the opposite way within the scroll limits, and what the limits
 * stop stays in `amount`. */
void wldcore_map_apply_clamped_scroll(open_point32_t* amount, VECTOR* position) {
    wldcore_map_apply_clamped_horizontal_scroll(&amount->x, &position->vx);
    wldcore_map_apply_clamped_vertical_scroll(amount, position);
}
