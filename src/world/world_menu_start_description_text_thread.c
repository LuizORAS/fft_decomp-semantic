#include "fft/world.h"
#include "psx/types.h"

/* Open the pending menu description in a Help dialog at the requested position.
 * Grid-menu ids above 0x1ffff are latched without opening a text thread. */
void world_menu_start_description_text_thread(s32 position) {
    s32 text_id = g_world_menu_description_text_id;

    if (text_id > 0x1ffff) {
        g_world_grid_menu_id = text_id;
        return;
    }
    if (text_id > 0) {
        world_thread_start(NATIVE_SCREEN_THREAD_SLOT_TEXT, world_text_character_handling_thread);
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_TEXT,
            position + (DIALOG_BOX_TYPE_HELP | DIALOG_OPTIONS_NO_TAIL), g_world_menu_description_text_id, 0);
        g_world_thread_task_active = 1;
    }
}
