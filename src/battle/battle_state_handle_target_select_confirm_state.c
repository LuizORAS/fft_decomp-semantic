#include "fft/battle.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* TARGET_SELECT_CONFIRM: 7 stores the unit under the cursor as the casting unit's Auto-Battle
 * target (the command as its setting) and reopens the action menus; 8 or cancel return to
 * TARGET_SELECT. */
void battle_state_handle_target_select_confirm_state(void) {
    battle_unit_t* caster;
    battle_unit_t* target;

    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    switch ((s32)battle_menu_get_selected_command_address()->kind) {
    case 7:
        caster = battle_unit_get_casting();
        target = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
        battle_menu_store_auto_battle_main_target_id(target->stats->battle_id, caster->stats->battle_id);
        battle_target_move_cursor_to_unit(caster);
        battle_state_enter_open_action_menus();
        break;
    case 8:
    case 0xFF:
        battle_state_enter_target_select();
        break;
    }
}
