#include "fft/battle.h"
#include "psx/types.h"

/* Thread entry of menu entry 0x22, which confirms the Move command.
 *
 * A unit with Don't Move gets the invalid sound and the linked-entry icon thread
 * (stored row action -1). In a menu whose move is already used (ids 0x0e, 0x14,
 * 0x21, 0x2f, 0x31 and 0x33) only the invalid sound plays. Otherwise the command
 * is confirmed at once, as a silent Circle press. */
void battle_menu_confirm_move_command_thread(void) {
    s16* stored_actions;
    world_menu_entry_t* entry;
    s32 menu;

    entry = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    stored_actions = &g_battle_menu_confirm_stored_unit_row_actions;
    *stored_actions = -2;
    if (battle_unit_get_attacker_data_pointer()
            ->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DONT_MOVE)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_MOVE)) {
        *stored_actions = -1;
        battle_sound_set_effect_to_invalid();
        battle_menu_icon_linked_entry_thread();
        battle_thread_exit_current();
    }
    menu = g_battle_menu_current_id;
    if (menu == BATTLE_MENU_ID_TURN_MOVED || menu == BATTLE_MENU_ID_TURN_MOVED_ACTED
        || menu == BATTLE_MENU_ID_TURN_NO_ACT_MOVED || menu == BATTLE_MENU_ID_RIDER_TURN_MOVED
        || menu == BATTLE_MENU_ID_RIDER_TURN_MOVED_ACTED || menu == BATTLE_MENU_ID_RIDER_TURN_NO_ACT_MOVED) {
        battle_sound_set_effect_to_invalid();
        battle_thread_exit_current();
    }
    battle_sound_set_effect_to_confirm_checked();
    battle_menu_confirm_action_silently(entry);
    battle_thread_exit_current();
}
