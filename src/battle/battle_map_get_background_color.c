#include "fft/battle.h"
#include "psx/types.h"

/* Return the map background colour (red, green, blue bytes) that battle_map_set_background_color
 * and its siblings load as the GTE back colour. No retail code calls it. */
u8* battle_map_get_background_color(void) {
    return g_battle_map_background_color;
}
