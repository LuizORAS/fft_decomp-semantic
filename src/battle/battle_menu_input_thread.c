#include "fft/battle.h"
#include "fft/battle_script.h"
#include "fft/thread.h"

/* Thread entry of system function 0x28 (the silent move confirmation): with menu
 * sounds suppressed, handle confirm and cancel on the menu each frame until
 * battle_menu_should_close_thread asks it to stop. The thread exits only when its
 * second parameter is 0. */
void battle_menu_input_thread(void) {
    menu_entry_t* menu;

    menu = (menu_entry_t*)battle_thread_get_current_parameter_1();
    g_sound_suppressed = 1;
    while (1) {
        thread_yield();
        if (battle_menu_should_close_thread(&g_script_event_input) != 0) {
            break;
        }
        battle_menu_handle_action(menu, 0);
        battle_handle_menu_cancel_input(menu);
    }
    thread_yield();
    g_sound_suppressed = 0;
    if (battle_thread_get_current_parameter_2() == 0) {
        battle_thread_exit_current();
    }
}
