#include "fft/battle.h"
#include "psx/types.h"

/* Thread entry of system function 0x28 (the silent move confirmation): with menu
 * sounds suppressed, handle confirm and cancel on the menu each frame until
 * battle_menu_should_close_thread asks it to stop. The thread exits only when its
 * second parameter is 0. */
void battle_menu_input_loop_thread(void) {
    void* menu;

    menu = (void*)battle_thread_get_current_parameter_1();
    g_battle_sound_suppressed = 1;
    while (1) {
        battle_thread_yield();
        if (battle_menu_should_close_thread(&g_battle_script_event_input) != 0) {
            break;
        }
        battle_menu_handle_entry_confirm(menu, 0);
        battle_menu_handle_entry_cancel(menu);
    }
    battle_thread_yield();
    g_battle_sound_suppressed = 0;
    if (battle_thread_get_current_parameter_2() == 0) {
        battle_thread_exit_current();
    }
}
