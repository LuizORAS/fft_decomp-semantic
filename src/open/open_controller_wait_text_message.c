#include "fft/open.h"

/* Handler 8: once the message thread has ended, give back the blocked buttons,
 * pop the controller and, with run_followup, restore the text section pointers
 * open_controller_start_text_message replaced. */
void open_controller_wait_text_message(open_controller_record_t* record) {
    open_controller_thread_completion_t* completion = &record->text_message;
    if (world_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_MESSAGE) == 0) {
        g_open_menu_blocked_buttons = completion->saved_blocked_buttons;
        g_open_current_controller_index--;
        if (completion->run_followup != 0) {
            world_text_restore_section_pointers();
        }
    }
}
