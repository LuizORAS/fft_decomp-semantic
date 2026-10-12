#include "fft/event_helpmenu.h"

/* Thread tail of the help menu: restore the 0x80 bytes of text section
 * pointers saved when it opened, free the overlay reservation and exit. */
void helpmenu_menu_close(void) {
    battle_copy_bytes(g_battle_text_section_pointers, g_helpmenu_text_saved_pointers, 0x80);
    main_heap_free(g_main_heap_high_overlay_load_address);
    g_battle_menu_help_open = 0;
    battle_thread_exit_current();
}
