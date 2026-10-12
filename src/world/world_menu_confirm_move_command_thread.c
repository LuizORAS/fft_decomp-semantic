#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_confirm_move_command_thread. */
void world_menu_confirm_move_command_thread(void) {
    world_menu_entry_t* entry = world_thread_get_current_parameter_1();
    s32 menu_id;
    s16* result = &g_world_menu_confirm_stored_unit_row_actions;

    *result = -2;
    if (world_unit_get_active_turn_unit_stats()
            ->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DONT_MOVE)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_MOVE)) {
        *result = -1;
        world_sound_set_effect_to_invalid();
        world_menu_icon_linked_entry_thread();
        world_thread_exit_current();
    }
    menu_id = g_world_menu_current_id;
    if (menu_id == BATTLE_MENU_ID_TURN_MOVED || menu_id == BATTLE_MENU_ID_TURN_MOVED_ACTED
        || menu_id == BATTLE_MENU_ID_TURN_NO_ACT_MOVED || menu_id == BATTLE_MENU_ID_RIDER_TURN_MOVED
        || menu_id == BATTLE_MENU_ID_RIDER_TURN_MOVED_ACTED || menu_id == BATTLE_MENU_ID_RIDER_TURN_NO_ACT_MOVED) {
        world_sound_set_effect_to_invalid();
        world_thread_exit_current();
    }
    world_sound_set_effect_to_confirm_checked();
    world_menu_confirm_action_silently(entry);
    world_thread_exit_current();
}
