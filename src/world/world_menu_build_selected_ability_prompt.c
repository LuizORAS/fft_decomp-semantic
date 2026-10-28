#include "fft/menu.h"
#include "fft/world.h"

/* Thread entry of g_world_learn_confirm_menu, the Learn list's confirmation: put the name of the
 * ability under the cursor in text substitution value 0, then run the linked-entry thread. */
void world_menu_build_selected_ability_prompt(void) {
    g_text_substitution_values[0] = g_menu_ability_entries[g_world_menu_cursor_position] + TEXT_ID_ABILITY_NAME_BASE;
    world_menu_icon_linked_entry_thread();
}
