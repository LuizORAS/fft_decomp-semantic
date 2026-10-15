#include "fft/world.h"

/* Set the pad mask world_menu_run_frame applies to each frame's new presses; WLDCORE and OPEN
 * pass the complement of the buttons they block. */
void world_menu_set_allowed_buttons(u32 mask) {
    g_world_menu_allowed_buttons = mask;
}
