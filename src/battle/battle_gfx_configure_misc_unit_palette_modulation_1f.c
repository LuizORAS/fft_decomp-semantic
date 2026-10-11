#include "fft/battle.h"
#include "psx/types.h"

/* Start a unit's fade-out removal with a red flash: fade step 4 and a red offset
 * of 0x1f. */
void battle_gfx_configure_misc_unit_palette_modulation_1f(s32 misc_id) {
    battle_gfx_configure_misc_unit_palette_modulation(misc_id, 4, 0x1F, 0, 0);
}
