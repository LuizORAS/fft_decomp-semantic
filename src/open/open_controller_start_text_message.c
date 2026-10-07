#include "fft/open.h"

/* Pushes the text-message controller (handler 8) on thread 2; its record
 * keeps run_followup and the formation entry mask to restore. */
void open_controller_start_text_message(s32 parameter, s32 run_followup) {
    {
        s32 controller = g_open_current_controller_index;

        g_open_controller_stream_start[controller].text_message.run_followup = run_followup;
    }
    if (run_followup != 0) {
        world_text_save_section_pointers();
        world_text_init_format_section_pointers(g_open_text_section_offsets);
    }

    world_thread_start(2, world_text_character_handling_thread);
    world_thread_set_parameters(2, 0x33, parameter, 0);

    {
        s32 controller = g_open_current_controller_index;
        s32 previous_mask = g_open_menu_formation_entry_mask;

        g_open_menu_formation_entry_mask = -1;
        g_open_controller_stream_start[controller].text_message.saved_formation_entry_mask = previous_mask;
    }
    main_sound_play_sfx(MAIN_SFX_WINDOW_OPEN);

    {
        s32 controller = g_open_current_controller_index;
        s32 next_controller = controller + 1;

        g_open_controller_handler_indices[controller] = 8;
        g_open_current_controller_index = next_controller;
    }
}
