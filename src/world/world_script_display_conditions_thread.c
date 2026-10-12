#include "fft/event_require.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_display_conditions_thread (DisplayConditions).
 *
 * The no-op immediately before it is a separate function. */
void world_script_display_conditions_thread(void) {
    g_world_menu_hide_numeric_values = 0;
    require_condition_dispatch();
}
