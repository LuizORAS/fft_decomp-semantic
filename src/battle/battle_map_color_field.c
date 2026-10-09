#include "fft/battle.h"
#include "psx/types.h"

void battle_map_color_field(s32 first, s32 second, s16 red, s16 green, s16 blue) {
    battle_map_modify_palette(first, second, 0, 0, 1, red, green, blue);
}
