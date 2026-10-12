#include "fft/world.h"
#include "psx/types.h"

/* Set the sixteen menu results (g_world_menu_selection_results) to -1, nothing chosen, before a
 * screen or a menu runs.
 *
 * Built at -O1 like world_formation_init_menu_state: -O1 does no strength reduction, which is why the
 * target recomputes the index inside the loop. */
void world_menu_reset_selection_results(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        g_world_menu_selection_results[i] = -1;
    }
}
