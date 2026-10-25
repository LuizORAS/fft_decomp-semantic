#include "fft/battle_map.h"
#include "psx/types.h"

/* Return the map background color (red, green, blue bytes) that battle_map_set_background_color
 * and its siblings load as the GTE back color. No retail code calls it. */
rgb24_t* battle_map_get_background_color(void) {
    return &g_battle_map_background_color;
}
