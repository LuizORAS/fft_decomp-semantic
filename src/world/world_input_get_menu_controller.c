#include "fft/world.h"
#include "psx/types.h"

/* Return the menu's input word: the newly pressed buttons for `player` 0,
 * the held buttons otherwise. */
u32* world_input_get_menu_controller(s32 player) {
    if (player == 0) {
        return &g_world_menu_new_button_input;
    }
    return &g_world_menu_current_button_input;
}
