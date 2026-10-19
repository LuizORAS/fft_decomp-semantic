#include "fft/battle_action.h"
#include "fft/battle_menu.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Check one list of the ability selection (battle_menu_select_unit_action_slots_thread) and show why
 * it cannot open. WORLD compiles this text too.
 *
 * For a skillset row marked disabled, the warning is 0x1003 for a submerged unit, 0x1007 for a Frog and
 * the menu type's default (g_battle_menu_restriction_message_ids) otherwise; it plays the invalid sound
 * and ends the calling thread. Math Skill shows the prompt 0x8009 - value for each of its three lists.
 * With no message, a menu type whose mode is negative confirms that row action at once. The message
 * shows in the restriction entry (battle_menu_icon_linked_entry_thread) unless navigation messages are
 * off and it is one of the 0x80xx prompts.
 *
 * `message = 0x1007` sits inside the status-bit arm rather than ahead of the inner `if`. Both spellings
 * are equivalent (the other arm overwrites it and GCC drops the dead store either way), but hoisting it
 * makes $17 look live across the evade branch, and reorg then refuses to lift `message = 0x1003` into
 * the bne delay slot, swapping the two delay-slot instructions at +0x64/+0x6c. Written this way, reorg fills
 * the bne slot with `message` and the jump slot with `mode`, which is the target's order. */
void menu_check_action_slot_restrictions(s32 menu, s32 value, s32 check_unit) {
    s32 mode;
    s32 message;
    world_menu_entry_t* parameter;
    battle_stats_t* unit;

    g_battle_active_menu = menu;
    parameter = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    mode = 0;
    message = 0;
    unit = battle_unit_get_active_turn_unit_stats();
    if (check_unit != 0) {
        if (battle_unit_get_action_block(unit) == BATTLE_UNIT_ACTION_BLOCK_SUBMERGED) {
            message = 0x1003;
            mode = 2;
        } else {
            if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)) {
                message = 0x1007;
                mode = 2;
            } else {
                mode = 2;
                message = g_battle_menu_restriction_message_ids[menu] + 0x1000;
            }
        }
    }
    if (message == 0 && menu == ACTION_MENU_TYPE_ARITHMETICKS) {
        message = 0x8009 - value;
        mode = 1;
    }
    if (mode != 0) {
        if (g_main_game_options.fields.navigation_messages != GAME_OPTION_OFF || (message & 0xff00) != 0x8000) {
            g_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE].text_id = message;
            thread_start(g_thread_current_id - 1, battle_menu_icon_linked_entry_thread);
            thread_set_parameters(g_thread_current_id - 1,
                (s32)&g_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE], 0, 0);
        }
        if (mode == 2) {
            sound_queue_sfx_invalid();
            if (g_main_game_options.fields.navigation_messages == GAME_OPTION_ON) {
                thread_wait_until_inactive(g_thread_current_id - 1);
            }
            thread_exit_current();
        }
    } else {
        mode = g_battle_menu_action_type_modes
            [g_action_menu_row_types[g_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index]][0];
        if (mode < 0) {
            g_battle_menu_action_slot_row_actions[0] = mode;
            battle_menu_confirm_action_silently(parameter);
            thread_exit_current();
        }
    }
    g_battle_menu_restore_depth = 2;
}
