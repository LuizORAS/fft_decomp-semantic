#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/*
 * Evaluate reachable origins for reflecting an ability onto another unit.
 *
 * Return -1 to suspend and 0 after restoring the temporary unit's position
 * and submitting the action to the ranked list. Stored counters survive
 * suspension, but the target does not reload the local reflector pointer
 * on resume; later target iterations can therefore use the caller's s2.
 */
s32 ai_evaluate_reflected_target_origins(void) {
    ai_data_t* ai = &g_ai_data_base;
    battle_stats_t* reflector;
    ai_unit_decision_t* decision;
    ai_tile_mask_mode_e mode;
    s32 ct;

    if (g_ai_data_base.resume_pending) {
        /* Resume: the target jumps into the middle of the origin loop. */
        goto evaluate_targets;
    }
    g_ai_data_base.current_action.reflected_action = 1;
    g_ai_data_base.targetability.live.target_setting_flags &= ~AI_TARGET_SETTING_CONSUME_EVALUATED_ORIGINS;
    for (ai->reflector_unit_index = 0; ai->reflector_unit_index < BATTLE_UNIT_SLOT_COUNT; ai->reflector_unit_index++) {
        reflector = &g_battle_unit_stats[ai->reflector_unit_index];
        decision = &ai->unit_decisions[ai->reflector_unit_index];
        if (decision->targeting_flags_2 & AI_TARGET_UNTARGETABLE)
            continue;
        if (ai->reflector_unit_index == ai->acting_unit_id)
            continue;
        if (!ai_is_status_active_through_delay(ai->considered_ability.ct, reflector, STATUS_ID_REFLECT))
            continue;
        if (ai_can_unit_act_within_charge_time(ai->reflector_unit_index))
            continue;
        if (decision->targeting_flags_2 & AI_TARGET_DEAD_WITHOUT_RERAISE) {
            ct = ai->considered_ability.ct;
            if (ai_calculate_clockticks_until_death_counter_expires(reflector) < ct)
                continue;
        }
        ai->current_action.command.targeting_type = BATTLE_TARGETING_TYPE_TILE;
        ai_copy_unit_coordinates(ai->reflector_unit_index, &ai->considered_unit_coords);
        mode = AI_TILE_MASK_AOE;
        if (ai->current_action.command.targeting_type == BATTLE_TARGETING_TYPE_UNIT)
            mode = AI_TILE_MASK_EXPLICIT;
        ai_build_targetable_tile_mask(mode, 0);
        ai_copy_u16_values(ai->attack_origin_tiles, ai->targetable_tiles, sizeof(ai->attack_origin_tiles));
        for (ai->reflected_target_unit_index = 0; ai->reflected_target_unit_index < BATTLE_UNIT_SLOT_COUNT;
            ai->reflected_target_unit_index++) {
            if (!ai->targetability.live.unit_targetable[ai->reflected_target_unit_index])
                continue;
            if (ai->reflected_target_unit_index == ai->reflector_unit_index)
                continue;
            if (ai->reflected_target_unit_index == ai->acting_unit_id)
                continue;
            if (ai_can_unit_act_within_charge_time(ai->reflected_target_unit_index))
                continue;
            if (ai_classify_ability_effect(ai->reflected_target_unit_index, 0) != AI_ABILITY_EFFECT_USEFUL)
                continue;
            ai->current_action.coords.fields.x
                = reflector->x * 2 - g_battle_unit_stats[ai->reflected_target_unit_index].x;
            ai->current_action.coords.fields.y
                = reflector->position.y * 2 - g_battle_unit_stats[ai->reflected_target_unit_index].position.y;
            if (ai->current_action.coords.fields.x >= ai->map_max_x)
                continue;
            if (ai->current_action.coords.fields.y >= ai->map_max_y)
                continue;
            for (ai->reflected_candidate_level = 0; ai->reflected_candidate_level < MAP_TERRAIN_LAYER_COUNT;
                ai->reflected_candidate_level++) {
                ai->current_action.coords.fields.elevation = ai->reflected_candidate_level;
                if ((ai->reachable_tiles[ai->movement_scenario][ai->reflected_candidate_level]
                                        [ai->current_action.coords.fields.y]
                        << ai->current_action.coords.fields.x)
                    & 0x8000) {
                    ai_update_tile_pickup();
                    ai->current_action.act_before_move = 1;
                    ai->reflected_origin_phase = 0;
                    for (;;) {
                        if (ai_is_ability_origin_allowed(&ai->acting_unit_coords)) {
                        evaluate_targets:
                            if (ai_evaluate_target_tiles() == -1)
                                return -1;
                        }
                        if (ai->reflected_origin_phase != 0)
                            break;
                        ai->reflected_origin_phase = 1;
                        ai->current_action.act_before_move = 0;
                    }
                }
            }
        }
    }
    ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    ai->current_action.reflected_action = 0;
    ai_insert_ranked_action();
    return 0;
}
