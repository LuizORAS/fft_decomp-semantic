#include "fft/battle_ai.h"

/*
 * Evaluate self-target alternatives from the actor and candidate coordinates.
 *
 * Record the resume stage when returning -1 to suspend. Completed searches
 * restore the actor coordinates and submit the action to the ranked list.
 */
s32 ai_evaluate_self_target_origins(void) {
    ai_data_t* ai = &g_ai_data_base;
    ai_tile_mask_mode_e mode;

    if (g_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        if (g_ai_self_target_origin_stage == 0)
            goto evaluate_map;
        goto evaluate_targets;
    }
    g_ai_data_base.targetability.live.target_setting_flags &= ~AI_TARGET_SETTING_CONSUME_EVALUATED_ORIGINS;
    g_ai_data_base.current_action.coords = ai->candidate_coords[g_ai_data_base.movement_scenario];
    ai_update_tile_pickup();
    if ((g_ai_data_base.considered_ability.ai_flags.word & AI_ABILITY_FOLLOW_TARGET)
        && !(g_main_ability_range_data[g_ai_data_base.considered_ability.ability_id].flags_1
            & ABILITY_SECONDARY_FLAG_1_CANNOT_TARGET_SELF)) {
        g_ai_data_base.current_action.act_before_move = 1;
        g_ai_data_base.current_action.command.targeting_type = BATTLE_TARGETING_TYPE_UNIT;
        g_ai_data_base.current_action.command.target_id = g_ai_data_base.acting_unit_id;
    evaluate_map:
        if (ai_evaluate_cast_origins() == -1) {
            g_ai_self_target_origin_stage = 0;
            return -1;
        }
    }
    ai->current_action.command.targeting_type = BATTLE_TARGETING_TYPE_TILE;
    g_ai_self_target_origin_stage = 0;
    ai->current_action.act_before_move = 1;
    /* The 32-bit view of the first member keeps this load after the phase store.
     * Direct .word or whole-union assignment changes target scheduling. */
    ai->considered_unit_coords.word = *(u32*)&ai->acting_unit_coords;
prepare_origin:
    mode = AI_TILE_MASK_AOE;
    if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT)
        mode = AI_TILE_MASK_EXPLICIT;
    ai_build_targetable_tile_mask(mode, 0);
    ai_transfer_halfword_values(ai->attack_origin_tiles, ai->targetable_tiles, sizeof(ai->attack_origin_tiles));
    if (ai_is_ability_origin_allowed(&ai->acting_unit_coords)) {
    evaluate_targets:
        if (ai_evaluate_target_tiles() == -1) {
            /* A first-origin suspension skips the moved-origin pass on resume. */
            g_ai_self_target_origin_stage = 1;
            return -1;
        }
    }
    if (g_ai_self_target_origin_stage == 0) {
        ai->current_action.act_before_move = 0;
        ai->considered_unit_coords = ai->current_action.coords;
        g_ai_self_target_origin_stage++;
        /* A goto retry, not a loop: GCC's loop pass would move the suspend
         * exit out of line. */
        goto prepare_origin;
    }
    ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    ai_insert_ranked_action();
    return 0;
}
