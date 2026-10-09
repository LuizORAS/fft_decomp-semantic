#include "fft/battle.h"
#include "psx/types.h"

void battle_map_update_palette_from_other(
    const u16* source, s32 bank, s32 row, s32 preset, s16 red, s16 green, s16 blue, s32 final_value) {
    battle_gfx_modify_vram_palette(MAP_COLOR_MODE_RESTORE, source, bank, row, preset, red, green, blue, final_value);
}
