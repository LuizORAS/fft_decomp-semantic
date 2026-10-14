#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_map_blend_darkness_color with the colour offsets narrowed to s16: the entry the
 * effect timelines call. */
void battle_map_start_darkness_blend(s32 mode, s32 frame_duration, s16 red, s16 green, s16 blue) {
    battle_map_blend_darkness_color(mode, frame_duration, red, green, blue);
}
