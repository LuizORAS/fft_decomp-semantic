#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Run the ability formula for a preview of attacker's command against target.
 *
 * The attacker's 20 command bytes are saved, replaced by command, and
 * restored afterwards. Without the Auto flag in the loaded ability data, the
 * target's tile must be targetable; otherwise its action data is cleared. */
s32 battle_action_calculate_projected_effect(
    battle_stats_t* attacker, battle_stats_t* target, battle_action_command_t* command) {
    u8 flags_3;
    s32 result;
    s32 saved_action_state;
    s32 saved_action_context;

    main_util_copy_action_data(&attacker->action_command, &g_battle_action_saved_command);
    main_util_copy_action_data(command, &attacker->action_command);
    battle_action_init_current_ability_strike_data(attacker);
    saved_action_state = g_battle_action_state;
    g_battle_action_state = BATTLE_ACTION_STATE_PREVIEW;
    if (!(g_battle_loaded_ability_flags_1 & ABILITY_SECONDARY_FLAG_1_AUTO)) {
        result = battle_target_calculate_map_for_action(command, &flags_3);
        if (result != -1
            && !g_battle_map_tile_data[battle_map_calculate_location(target)].ceiling_depth_and_marks.targeted) {
            result = -1;
        }
        if (result == -1) {
            battle_action_clear_current_data(&target->action);
            target->action.hit = 0;
            target->action.attack_accuracy = 0;
            g_battle_action_state = saved_action_state;
            main_util_copy_action_data(&g_battle_action_saved_command, &attacker->action_command);
            return 0;
        }
    }
    saved_action_context = g_battle_action_context;
    g_battle_action_current_ability.defaulted_to_attack = 0;
    g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
    target->action.reaction_id = 0;
    battle_action_run_pre_formula_setup(&attacker->action_command, target->misc_unit_id);
    g_battle_action_context = saved_action_context;
    g_battle_action_state = saved_action_state;
    main_util_copy_action_data(&g_battle_action_saved_command, &attacker->action_command);
    return 0;
}
