#include "fft/battle.h"
#include "psx/types.h"

/* Calls battle_gfx_configure_misc_unit_palette_modulation with the same
 * arguments; the event instructions that remove a unit start its fade-out here. */
void battle_gfx_init_misc_unit_palette_modulation(s32 misc_id, s32 mode, s16 red, s16 green, s16 blue) {
    battle_gfx_configure_misc_unit_palette_modulation(misc_id, mode, red, green, blue);
}
