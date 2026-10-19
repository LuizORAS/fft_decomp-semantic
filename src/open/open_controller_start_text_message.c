#include "fft/open.h"
#include "fft/thread.h"

/* Pushes the text-message controller (handler 8) on thread 2; its record
 * keeps run_followup and the formation entry mask to restore. */
void open_controller_start_text_message(s32 parameter, s32 run_followup) {
    {
        s32 controller = g_open_current_controller_index;

        g_open_controller_records_next[controller].text_message.run_followup = run_followup;
    }
    if (run_followup != 0) {
        world_text_save_section_pointers();
        world_text_relocate_pointer_table(g_world_text_wldhelp_section_offsets);
    }

    thread_start(THREAD_SCREEN_SLOT_MESSAGE, world_text_character_handling_thread);
    thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x33, parameter, 0);

    {
        s32 controller = g_open_current_controller_index;
        s32 saved_blocked_buttons = g_open_menu_blocked_buttons;

        g_open_menu_blocked_buttons = -1;
        g_open_controller_records_next[controller].text_message.saved_blocked_buttons = saved_blocked_buttons;
    }
    main_sound_play_sfx(MAIN_SFX_WINDOW_OPEN);

    {
        s32 controller = g_open_current_controller_index;
        s32 next_controller = controller + 1;

        g_open_controller_handler_indices[controller] = OPEN_CONTROLLER_TEXT_MESSAGE;
        g_open_current_controller_index = next_controller;
    }
}
