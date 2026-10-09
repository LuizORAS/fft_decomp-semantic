#include "fft/battle.h"
#include "psx/types.h"

void battle_map_update_all_palettes_from_color_data(s32 mode, const void* color_data) {
    battle_map_update_palette_colors(mode, MAP_PALETTE_BANK_MAP, 0, 1, color_data);
}
