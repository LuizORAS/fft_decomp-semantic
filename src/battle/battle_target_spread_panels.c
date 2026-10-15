#include "fft/battle.h"
#include "psx/types.h"

/* Spread the range from the frontier panels up to `passes` times: each pass visits every panel
 * whose mark is set, clears it and spreads to its neighbours (battle_target_spread_panel_to_neighbors),
 * and the passes stop once none spreads. A zero count only clears the marks. Every caller passes 0
 * as the unused second argument. */
void battle_target_spread_panels(u8 passes, s32 unused) {
    s32 y;
    s32 x;
    s32 changed;
    s32 pass;
    battle_target_panel_t* panel;

    if (passes == 0) {
        battle_target_clear_panel_marks();
        return;
    }
    changed = 1;
    for (pass = 0; pass < passes; pass++) {
        if (changed == 0) {
            break;
        }
        changed = 0;
        for (y = 0; y < g_battle_map_max_y; y++) {
            for (x = 0; x < g_battle_map_max_x; x++) {
                panel = &g_battle_target_panels[y * g_battle_map_max_x + x];
                if (panel->mark != 0) {
                    panel->mark = 0;
                    changed += battle_target_spread_panel_to_neighbors(y, x);
                }
            }
        }
    }
}
