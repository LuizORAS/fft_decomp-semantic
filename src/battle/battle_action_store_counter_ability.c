#include "fft/battle.h"

/* Writes the acting unit's tile into unit's action fields (0x16e..) as the
 * target of skillset/ability and runs the attack preview.  Returns -1 when the
 * action itself fails, -2 when the acting unit's tile is not targetable and
 * -3 when the preview fails; otherwise the preview result. */
s32 battle_action_store_counter_ability(battle_stats_t* unit, s8 skillset_id, s16 ability_id, s32 validate_target) {
    battle_unit_action_command_t* action;
    battle_stats_t* acting_unit;
    s32 result;
    s32 acting_unit_id;
    s32 preview_result;
    u8 saved_charged_ability_ct;

    acting_unit_id = g_battle_acting_unit_id;
    action = &unit->action_command;
    unit->action_command.skillset_id = skillset_id;
    unit->action_command.ability_id = ability_id;
    acting_unit = &g_battle_unit_stats[acting_unit_id];
    if (validate_target != 0) {
        result = battle_target_set_panels_for_action((u8*)action);
        if (result == -1 || result == 2 || result == 3) {
            return -1;
        }
        if ((g_battle_map_tile_data[battle_map_calculate_location(acting_unit)].ceiling_depth_and_marks.value
                & MAP_TILE_FLAG_ABILITY_RANGE)
            == 0) {
            return -2;
        }
    }
    action->targeting_type = BATTLE_ACTION_TARGET_TILE;
    action->target_id = g_battle_acting_unit_id;
    action->target_x = acting_unit->x;
    action->target_y = acting_unit->position.bits.y;
    action->target_elevation = acting_unit->position.raw >> 15;
    saved_charged_ability_ct = unit->charged_ability_ct;
    result = battle_action_call_attack_preparation_at_preview((u8*)action);
    unit->charged_ability_ct = saved_charged_ability_ct;
    if (result == -1 || (preview_result = (result == 2 || result == 3), preview_result != 0)) {
        preview_result = -3;
    }
    return preview_result;
}
