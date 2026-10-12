#include "fft/battle_map.h"
#include "psx/types.h"

rgb24_t* battle_map_get_background_color(void) {
    return &g_battle_map_background_color;
}
