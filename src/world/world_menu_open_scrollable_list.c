#include "fft/world.h"
#include "psx/types.h"

/*
 * Copies the 0xFE-terminated byte list (at most 16 entries) into
 * g_world_name_entry_text and starts menu thread 2 for it.
 */
void world_menu_open_scrollable_list(u8* list) {
    s32 i;

    g_world_name_entry_original_name = list;
    g_world_name_entry_cursor_index = 0;
    g_world_name_entry_list_cursor = 1;
    world_thread_start(NATIVE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_MESSAGE, 0x21, 0xC015, 0);
    world_menu_init_scrollable_list_core(0, 0, 0);
    g_world_menu_cursor_position = 1;
    world_gfx_bind_data_pointer(WORLD_TEXT_DATA_NAME_ENTRY_KEYBOARD);
    g_world_text_section_pointers[TEXT_SECTION_NAME_ENTRY_KEYS] = g_world_text_name_entry_keys_data;
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
    world_menu_open_name_entry_key_list();
    world_script_set_vsync_mode_and_event_speed(2);
}
