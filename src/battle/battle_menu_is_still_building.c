#include "fft/battle.h"
#include "psx/types.h"

s32 battle_menu_is_still_building(void) {
    s32 i;
    s32 result;

    if (g_battle_menu_transition_state != 0) {
        return g_battle_menu_transition_state;
    }
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_BUILDING) != 0) {
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
    if (g_battle_menu_input_mode != MENU_INPUT_BLOCKED) {
        result = i != 0xA;
        return result * 4;
    }
    return 2;
}
