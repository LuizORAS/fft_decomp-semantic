#include "fft/battle.h"
#include "psx/types.h"

/* VERTICAL_FIXED abilities: mark as choices, on both layers, the tiles in range in the unit's row
 * and column, up to 31 steps each way.
 *
 * The target tests the lower layer's range byte and marks both layers when it is nonzero. Sharing
 * the loop index and panel temporaries across the sweeps preserves the target register
 * allocation. */
void battle_target_apply_vertical_fixed(s32 x, s32 y) {
    s32 shared;
    s32 vertical;
    s32 tile_index;
    battle_target_panel_t* panel;
    s32 i;

    x &= 0xFF;
    y &= 0xFF;
    {
        vertical = y;
        for (i = -0x1F; i < 0x20; i++) {
            s32 mx;
            battle_target_panel_t* base = g_battle_target_panels;
            battle_target_panel_t* lvl1 = base + 0x100;
            shared = x + i;
            mx = g_battle_map_max_x;
            tile_index = (vertical * mx) + shared;
            panel = &base[tile_index];
            if ((shared >= 0) && (shared < mx) && ((u8)panel->remaining_range != 0)) {
                panel->mark = 1;
                lvl1[tile_index].mark = 1;
            }
        }
    }
    {
        shared = x;
        for (i = -0x1F; i < 0x20; i++) {
            battle_target_panel_t* base = g_battle_target_panels;
            battle_target_panel_t* lvl1 = base + 0x100;
            vertical = y + i;
            tile_index = (vertical * g_battle_map_max_x) + shared;
            panel = &base[tile_index];
            if ((vertical >= 0) && (vertical < g_battle_map_max_y) && ((u8)panel->remaining_range != 0)) {
                panel->mark = 1;
                lvl1[tile_index].mark = 1;
            }
        }
    }
}
