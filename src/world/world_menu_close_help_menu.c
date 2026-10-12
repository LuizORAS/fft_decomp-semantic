#include "fft/world.h"

/* Thread tail of the help menu: restore the text section pointers saved
 * when it opened, free the overlay reservation and exit. */
void world_menu_close_help_menu(void) {
    world_script_copy_bytes(
        g_text_section_pointers, g_world_text_section_pointer_backup, sizeof(g_text_section_pointers));
    main_heap_free(g_main_heap_high_overlay_load_address);
    g_world_text_overlay_active = 0;
    world_thread_exit_current();
}
