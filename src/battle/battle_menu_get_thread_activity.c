#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Report how busy the menu layer is: the transition state while one runs (4 during the unit list's
 * fade-out), 2 while the menu-building slot runs or menu input is blocked, 4 while a thread in slots 4
 * to 9 runs, else 0. The `i != 3` test inside the scan always passes. */
s32 battle_menu_get_thread_activity(void) {
    s32 i;
    s32 result;

    if (g_battle_menu_transition_state != 0) {
        return g_battle_menu_transition_state;
    }
    if (battle_thread_is_running_asm(THREAD_SLOT_MENU_BUILDING) != 0) {
        return 2;
    }
    i = 4;
    do {
        if (i != 3) {
            if (battle_thread_is_running_asm(i) != 0) {
                break;
            }
        }
        i += 1;
    } while (i < 0xA);
    if (g_menu_input_mode != MENU_INPUT_BLOCKED) {
        result = i != 0xA;
        return result * 4;
    }
    return 2;
}
