#include "fft/sound.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_confirm_move_command_thread. */
void world_menu_confirm_move_command_thread(void) {
    world_menu_entry_t* entry = (world_menu_entry_t*)world_thread_get_current_parameter_1();
    s32 menu_id;
    s16* result = &g_world_menu_confirm_stored_unit_row_actions;

    *result = -2;
    if (world_unit_get_selected_unit_stats()->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DONT_MOVE)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_MOVE)) {
        *result = -1;
        sound_queue_sfx_invalid();
        world_menu_icon_linked_entry_thread();
        thread_exit_current();
    }
    menu_id = g_menu_current_id;
    if (menu_id == BATTLE_MENU_ID_TURN_MOVED || menu_id == BATTLE_MENU_ID_TURN_MOVED_ACTED
        || menu_id == BATTLE_MENU_ID_TURN_NO_ACT_MOVED || menu_id == BATTLE_MENU_ID_RIDER_TURN_MOVED
        || menu_id == BATTLE_MENU_ID_RIDER_TURN_MOVED_ACTED || menu_id == BATTLE_MENU_ID_RIDER_TURN_NO_ACT_MOVED) {
        sound_queue_sfx_invalid();
        thread_exit_current();
    }
    sound_queue_sfx_confirm_unless_restoring();
    world_menu_confirm_action_silently(entry);
    thread_exit_current();
}
