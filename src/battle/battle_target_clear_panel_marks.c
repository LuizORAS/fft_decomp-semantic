#include "fft/battle.h"
#include "psx/types.h"

/* Clear the mark of all 512 tile panels, from the last one down. */
void battle_target_clear_panel_marks(void) {
    s32 i;
    battle_target_panel_t* panel;

    i = MAP_TILE_SLOT_COUNT - 1;
    panel = &g_battle_target_panels[MAP_TILE_SLOT_COUNT - 1];
    do {
        panel->mark = 0;
        i -= 1;
        panel--;
    } while (i >= 0);
}
