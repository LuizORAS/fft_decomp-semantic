#include "fft/world.h"
#include "psx/types.h"

/* Open the name entry on the name `list` (0xFE-terminated, at most 16 bytes; the confirmation falls back
 * to it when the entry is left empty): start the message box with prompt 0xC015, bind the keyboard text and
 * its key rows (WORLD_TEXT_SECTION_NAME_ENTRY_KEYS), copy the name into g_world_name_entry_text with the cursor
 * at its start, open the key rows (world_name_open_key_rows) and set the screen's event speed. */
void world_name_open_entry(u8* list) {
    s32 i;

    g_world_name_entry_original_name = list;
    g_world_name_entry_cursor_index = 0;
    g_world_name_entry_list_cursor = 1;
    world_thread_start(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
    world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x21, 0xC015, 0);
    world_menu_init_scrollable_list_core(0, 0, 0);
    g_world_menu_cursor_position = 1;
    world_text_bind_section_data(WORLD_TEXT_DATA_NAME_ENTRY_KEYBOARD);
    g_world_text_section_pointers[WORLD_TEXT_SECTION_NAME_ENTRY_KEYS] = g_world_text_name_entry_keys_data;
    g_world_name_entry_cursor = 0;
    i = 0;
    while ((g_world_name_entry_text[i] = list[i]) != TEXT_END_WAIT_FOR_CONFIRM) {
        if (i >= 16) {
            g_world_name_entry_text[16] = TEXT_END_WAIT_FOR_CONFIRM;
            break;
        }
        i++;
    }
    world_name_apply_entry_key(WORLD_NAME_ENTRY_KEY_CURSOR_LEFT);
    g_world_menu_sound_effect_id = MAIN_SFX_NONE;
    g_world_name_entry_state = 0;
    world_name_open_key_rows();
    world_script_set_vsync_mode_and_event_speed(2);
}
