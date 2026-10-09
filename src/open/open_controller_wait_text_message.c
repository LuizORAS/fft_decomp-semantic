#include "fft/open.h"

/* The thread-2 controller's view of its open_controller_record_t. */
typedef struct open_controller_thread_completion {
    s32 run_followup;
    s32 saved_blocked_buttons; /* g_open_menu_blocked_buttons when the message started */
} open_controller_thread_completion_t;

void open_controller_wait_text_message(open_controller_thread_completion_t* completion) {
    if (world_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_MESSAGE) == 0) {
        g_open_menu_blocked_buttons = completion->saved_blocked_buttons;
        g_open_current_controller_index--;
        if (completion->run_followup != 0) {
            world_text_restore_section_pointers();
        }
    }
}
