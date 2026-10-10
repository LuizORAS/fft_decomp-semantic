#include "fft/event_equip.h"
#include "psx/types.h"

/* Start the help text thread for the current help message, if one is set:
 * save the text section pointers, bind the help text table and lock the menu
 * input. The dialog type is `thread_data` + 0x38. */
void equip_text_start_help_thread(s32 thread_data) {
    if (g_equip_text_help_message_id > 0) {
        battle_text_save_section_pointers();
        battle_text_relocate_pointer_table(g_equip_help_text_table);
        battle_thread_start(NATIVE_SCREEN_THREAD_SLOT_TEXT, battle_text_character_handling_thread);
        battle_thread_set_parameters(
            NATIVE_SCREEN_THREAD_SLOT_TEXT, thread_data + 0x38, g_equip_text_help_message_id, 0);
        g_battle_menu_input_lock_mode = 1;
    }
}
