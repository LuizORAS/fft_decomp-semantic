#include "fft/wldcore.h"
#include "psx/types.h"

/* Show `text` in the message box as menu level MESSAGE: WORLD's text thread runs it in the message slot
 * (mode 0x3b while the unit panel threads are in use, else 0x33), reading the text sections at
 * g_world_text_wldhelp_section_offsets when `reload_text` is set. Every button is withheld from WORLD's menus
 * until wldcore_menu_finish_close_request pops the level, and the window-open sound plays. */
void wldcore_menu_push_message_level(s32 text, s32 reload_text) {
    s32 index;

    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].message.reload_text = reload_text;
    if (reload_text != 0) {
        world_text_save_section_pointers();
        world_text_relocate_pointer_table(g_world_text_wldhelp_section_offsets);
    }
    world_thread_start(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, world_text_character_handling_thread);
    if (g_main_system_flags & MAIN_SYSTEM_FLAG_UNIT_PANEL_THREADS) {
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x3B, text, 0);
    } else {
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x33, text, 0);
    }
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].message.saved_blocked_buttons
        = g_wldcore_menu_blocked_buttons;
    g_wldcore_menu_blocked_buttons = -1;
    wldcore_sound_play_effect(MAIN_SFX_WINDOW_OPEN);
    index = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[index].message._unknown_0c = 8;
    g_wldcore_menu_stack_records_next[index].message.window_index = 0;
    g_wldcore_menu_stack_types[index + 1] = WLDCORE_MENU_LEVEL_MESSAGE;
    g_wldcore_menu_stack_depth = index + 1;
}
