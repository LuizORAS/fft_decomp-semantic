#include "fft/battle.h"

/* Return 1 when one of the nine spritesheet slots holds `spritesheet_id`: the queued unit graphics load
 * then skips reading the sheet's SPR file and goes straight to installing it. */
s32 battle_gfx_is_spritesheet_id_loaded(s16 spritesheet_id) {
    s32 slot;

    for (slot = 0; slot < 9; slot++) {
        if (g_battle_gfx_spritesheet_slots[slot].spritesheet_id == spritesheet_id) {
            return 1;
        }
    }
    return 0;
}
