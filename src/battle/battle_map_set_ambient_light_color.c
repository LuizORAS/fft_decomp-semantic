#include "fft/battle.h"
#include "psx/types.h"

/* Fade the ambient light to `color` without changing the base colour, unlike
 * battle_map_transition_and_store_ambient_light_color. */
void battle_map_set_ambient_light_color(s32 frame_duration, const map_color_t* color) {
    battle_map_transition_ambient_light_color(frame_duration, color);
}
