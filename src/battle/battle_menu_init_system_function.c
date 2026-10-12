#include "fft/battle_menu.h"
#include "fft/battle_text.h"
#include "fft/battle_unit.h"

/* Run a system function (battle_menu_system_command_e) for unit_id, looking up
 * the unit's command action first except for SCENARIO_CHECK and CLOSE.
 *
 * SPELL_QUOTE goes to battle_text_determine_spell_quote, a void routine whose
 * leftover v0 is returned. ABILITY_ANNOUNCEMENT drops option, parameter and
 * enabled. Everything else returns battle_menu_dispatch_system_function's result;
 * for SCENARIO_CHECK and CLOSE the action pointer passed on is unset. */
s32 battle_menu_init_system_function(s32 type, s32 option, s32 unit_id, s32 parameter, s32 enabled) {
    battle_action_command_t* action;

    if (type != BATTLE_MENU_SYSTEM_COMMAND_SCENARIO_CHECK && type != BATTLE_MENU_SYSTEM_COMMAND_CLOSE) {
        action = battle_unit_get_misc_action_command_by_battle_id(unit_id);
        if (type == BATTLE_MENU_SYSTEM_COMMAND_SPELL_QUOTE) {
            /* The target returns the void callee's leftover $v0. */
            return ((s32 (*)(battle_action_command_t*, s32, s32))battle_text_determine_spell_quote)(
                action, unit_id, enabled);
        }
        if (type == BATTLE_MENU_SYSTEM_COMMAND_ABILITY_ANNOUNCEMENT) {
            return battle_menu_dispatch_system_function(
                BATTLE_MENU_SYSTEM_COMMAND_ABILITY_ANNOUNCEMENT, 0, unit_id, 0, 0, action);
        }
    }
    return battle_menu_dispatch_system_function(type, option, unit_id, parameter, enabled, action);
}
