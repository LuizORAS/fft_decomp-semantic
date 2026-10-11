#include "fft/event_equip.h"
#include "psx/types.h"

/* Start-or-poll helper for a menu thread: the first call starts `thread_id`
 * on the descriptor's thread function with the descriptor as parameter 1
 * (unless the slot is still busy) and returns 1; later calls return whether
 * the thread is still running, 0 once it has ended. */
s32 equip_thread_start_managed(s32 thread_id, menu_entry_t* desc) {
    if (g_equip_thread_state == 0) {
        if (battle_thread_is_running(thread_id) != 0) {
            return 1;
        }
        g_battle_menu_thread_menu_data = desc;
        battle_thread_start(thread_id, desc->thread_entry);
        battle_thread_set_parameters(thread_id, (s32)g_battle_menu_thread_menu_data, 0, 0);
        g_equip_thread_state = 1;
        return 1;
    }
    g_equip_thread_state = battle_thread_is_running(thread_id);
    return g_equip_thread_state;
}
