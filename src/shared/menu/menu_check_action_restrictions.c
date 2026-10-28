#include "fft/battle_action.h"
#include "fft/battle_menu.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Check that the active unit may act from the current menu and show why not.
 *
 * In the menus after acting (TURN_ACTED, TURN_MOVED_ACTED and the rider ones) battle_unit_get_action_block
 * decides: no block plays the invalid sound and ends the thread, a disabled unit gets message 0x1002 and
 * a submerged one 0x1003. Elsewhere a Charging unit gets 0x8001 (the skillset or ability it charges) and
 * a Performing unit 0x8002. The message shows in the restriction entry under the current thread, unless
 * a menu restore is pending; with navigation messages off, 0x8001 and 0x8002 are only the confirm sound
 * (the test compares the message's high byte with the option bits, both 0x8000). 0x1002 and 0x1003 then
 * end the thread. WORLD compiles this text too. */
void menu_check_action_restrictions(void) {
    s32 message = 0;
    battle_stats_t* unit;
    s32 menu;
    s32 result;
    s32 mode;

    unit = battle_unit_get_active_turn_unit_stats();
    menu = g_menu_current_id;
    if (menu == BATTLE_MENU_ID_TURN_ACTED || menu == BATTLE_MENU_ID_TURN_MOVED_ACTED
        || menu == BATTLE_MENU_ID_RIDER_TURN_ACTED || menu == BATTLE_MENU_ID_RIDER_TURN_MOVED_ACTED) {
        result = battle_unit_get_action_block(unit);
        if (result == BATTLE_UNIT_ACTION_BLOCK_NONE) {
            sound_queue_sfx_invalid();
            thread_exit_current();
        }
        if (result == BATTLE_UNIT_ACTION_BLOCK_DISABLED) {
            message = 0x1002;
        }
        if (result == BATTLE_UNIT_ACTION_BLOCK_SUBMERGED) {
            message = 0x1003;
        }
    } else {
        if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING)) {
            if (g_main_action_menu_types_by_skillset[unit->action_command.skillset_id] == ACTION_MENU_TYPE_CHARGE) {
                g_text_substitution_values[0] = unit->action_command.skillset_id | TEXT_ID_SKILLSET_NAME_SOURCE_BASE;
            } else {
                g_text_substitution_values[0] = unit->action_command.ability_id + TEXT_ID_ABILITY_NAME_BASE;
            }
            message = 0x8001;
        } else if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PERFORMING)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING)) {
            message = 0x8002;
        }
    }
    if (message != 0) {
        mode = g_main_game_options.value & GAME_OPTIONS_NAVIGATION_MESSAGES_MASK;
        if (mode == GAME_OPTIONS_NAVIGATION_MESSAGES_OFF_BITS && (message & 0xff00) == mode) {
            sound_queue_sfx_confirm_unless_restoring();
        } else if (g_menu_restore_pending == 0) {
            sound_queue_sfx_invalid();
            g_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE].text_id = message;
            thread_start(g_thread_current_id - 1, battle_menu_icon_linked_entry_thread);
            thread_set_parameters(g_thread_current_id - 1,
                (s32)&g_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE], 0, 0);
            thread_wait_until_inactive(g_thread_current_id - 1);
        }
        if (message == 0x1002 || message == 0x1003) {
            thread_exit_current();
        }
    } else {
        sound_queue_sfx_confirm_unless_restoring();
    }
}
