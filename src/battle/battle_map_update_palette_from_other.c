#include "fft/battle.h"
#include "psx/types.h"

/* Write one CLUT of a row from the colours at `source` with the red, green and
 * blue offsets added: battle_gfx_modify_vram_palette in MAP_COLOR_MODE_RESTORE, a
 * mode in which it neither halves nor greys the colours. */
void battle_map_update_palette_from_other(
    const rgba5551_t* source, s32 clut_row, s32 clut_index, s32 preset, s16 red, s16 green, s16 blue, s32 final_value) {
    battle_gfx_modify_vram_palette(
        MAP_COLOR_MODE_RESTORE, source, clut_row, clut_index, preset, red, green, blue, final_value);
}
