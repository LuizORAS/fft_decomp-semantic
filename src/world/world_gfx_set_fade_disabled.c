#include "fft/world.h"
#include "psx/types.h"

/* Turn the screen fade helpers (world_gfx_get_fade_state, the two fade starts and the two fade tiles)
 * off or on: while disabled they do nothing and g_world_gfx_fade_intensity keeps its value. The name entry
 * screen sets it (world_name_run_entry_screen). */
void world_gfx_set_fade_disabled(u8 disabled) {
    g_world_gfx_fade_disabled = disabled;
}
