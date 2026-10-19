#include "fft/battle_map.h"
#include "psx/types.h"

/* Fade the ambient light to `color` and make it the base colour that the
 * MAP_COLOR_MODE_BASE modes and RESTORE start from. */
void battle_map_transition_and_store_ambient_light_color(s32 frame_duration, const CVECTOR* color) {
    battle_map_transition_ambient_light_color(frame_duration, color);
    g_battle_map_ambient_light_color = *color;
}
