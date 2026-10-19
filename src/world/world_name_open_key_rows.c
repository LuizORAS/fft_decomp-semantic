#include "fft/world.h"

/* Open the keyboard's nine rows as a scrollable list (g_world_name_entry_key_ids 0-8 and a -1
 * terminator), drawn with menu script 7 and the keyboard display script: row 0 holds the special keys, the
 * others eleven characters each. */
void world_name_open_key_rows(void) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 8; i >= 0; i--) {
        g_world_name_entry_key_ids[i] = i;
        count++;
    }
    g_world_name_entry_key_ids[count] = -1;
    world_menu_init_and_load_scrollable_list(g_world_name_entry_key_ids, g_world_menu_cursor_position,
        world_menu_get_script(WORLD_TEXT_DATA_NAME_ENTRY_KEYBOARD), &g_world_name_entry_keyboard_script);
}
