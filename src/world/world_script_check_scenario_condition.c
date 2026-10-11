/* Evaluates one five-field 16-bit scenario condition record for world_script_run_scenario_conditions
 * (the WORLD sibling of BATTLE process_scenario_conditionals): returns 1 when
 * the condition holds, 2 for type 0x19 (stores the id in script variable
 * 0x27), otherwise 0. Twin: battle_script_check_scenario_condition (0x80142694),
 * identical apart from addresses and the divide checks.
 * Types: 1-3 script variable ==/>=/<=; 4-10 unit exists / HP / HP% / MP
 * bounds; 11 acting unit (g_world_script_acting_unit_id) is the unit or a special-unit
 * (0x78..0x7c) copy of it; 13 party member has item equipped; 14-15 variable
 * war-funds bounds; 16-17 compare the month/day pair before/after; 18-19 compare casualties
 * bounds; 22 battle_action_check_battle_outcome() == 0; 24 unit stands on x/y/level; 37 any
 * misc-unit of that sprite palette stands on x/y/level.
 * The range test on unit_id is written as two comparisons: that spelling
 * makes GCC reload the byte (as the target does) where the (u32)(x - 0x78) < 5
 * form lets cse reuse the earlier load. */
#include "fft/battle.h"
#include "fft/world.h"

s32 world_script_check_scenario_condition(s32 type, s32 id, s32 value, s32 tile_y, s32 elevation) {
    battle_stats_t* unit;
    s32 i;
    s32 j; /* equipment slot, then battle index: one variable (a0 in both loops) */

    if (type == EVENT_SCENARIO_CONDITION_VARIABLE_EQUAL && world_script_get_variable(id) == value)
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type == EVENT_SCENARIO_CONDITION_VARIABLE_AT_LEAST && world_script_get_variable(id) >= value)
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type == EVENT_SCENARIO_CONDITION_VARIABLE_AT_MOST && value >= world_script_get_variable(id))
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type >= 4 && type <= 10) {
        unit = world_unit_get_by_unit_id(id);
        if (unit != (battle_stats_t*)-1) {
            if (type == EVENT_SCENARIO_CONDITION_UNIT_PRESENT)
                return EVENT_SCENARIO_CONDITION_RESULT_PASS;
            if (type == EVENT_SCENARIO_CONDITION_UNIT_HP_AT_LEAST && unit->hp >= value)
                return EVENT_SCENARIO_CONDITION_RESULT_PASS;
            if (type == EVENT_SCENARIO_CONDITION_UNIT_HP_AT_MOST && value >= unit->hp)
                return EVENT_SCENARIO_CONDITION_RESULT_PASS;
            if (type == EVENT_SCENARIO_CONDITION_UNIT_HP_PERCENT_AT_LEAST
                && unit->hp * 100 / (unit->max_hp + 1) >= value)
                return EVENT_SCENARIO_CONDITION_RESULT_PASS;
            if (type == EVENT_SCENARIO_CONDITION_UNIT_HP_PERCENT_AT_MOST
                && value >= unit->hp * 100 / (unit->max_hp + 1))
                return EVENT_SCENARIO_CONDITION_RESULT_PASS;
            if (type == EVENT_SCENARIO_CONDITION_UNIT_MP_AT_LEAST && unit->mp >= value)
                return EVENT_SCENARIO_CONDITION_RESULT_PASS;
            if (type == EVENT_SCENARIO_CONDITION_UNIT_MP_AT_MOST && value >= unit->mp)
                return EVENT_SCENARIO_CONDITION_RESULT_PASS;
        }
    }
    if (type == EVENT_SCENARIO_CONDITION_ACTING_UNIT && g_world_script_acting_unit_id != 0xff) {
        unit = battle_unit_get_stats_from_battle_id(g_world_script_acting_unit_id);
        if (unit->unit_id == id
            || (unit->unit_id >= EVENT_UNIT_ID_DEPLOYED_FIRST && unit->unit_id < EVENT_UNIT_ID_DEPLOYED_END
                && unit->character_identity == id && id >= 0 && id < CHARACTER_IDENTITY_GENERIC_FIRST
                && id != CHARACTER_IDENTITY_ENTD_NONE))
            return EVENT_SCENARIO_CONDITION_RESULT_PASS;
        return EVENT_SCENARIO_CONDITION_RESULT_FAIL;
    }
    if (type == EVENT_SCENARIO_CONDITION_ITEM_EQUIPPED) {
        for (i = 1; i < 4; i++) {
            unit = world_unit_get_by_unit_id(i);
            if (unit != (battle_stats_t*)-1) {
                for (j = 0; j < 7; j++) {
                    if (unit->equipment[j] == id)
                        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
                }
            }
        }
    }
    if (type == EVENT_SCENARIO_CONDITION_WAR_FUNDS_AT_LEAST
        && world_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS) >= value)
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type == EVENT_SCENARIO_CONDITION_WAR_FUNDS_AT_MOST
        && value >= world_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS))
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type == EVENT_SCENARIO_CONDITION_DATE_ON_OR_BEFORE) {
        if (world_script_get_variable(EVENT_SCRIPT_VAR_MONTH) < id)
            return EVENT_SCENARIO_CONDITION_RESULT_PASS;
        if (world_script_get_variable(EVENT_SCRIPT_VAR_MONTH) == id
            && value >= world_script_get_variable(EVENT_SCRIPT_VAR_DAY))
            return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    }
    if (type == EVENT_SCENARIO_CONDITION_DATE_ON_OR_AFTER) {
        if (id < world_script_get_variable(EVENT_SCRIPT_VAR_MONTH))
            return EVENT_SCENARIO_CONDITION_RESULT_PASS;
        if (world_script_get_variable(EVENT_SCRIPT_VAR_MONTH) == id
            && world_script_get_variable(EVENT_SCRIPT_VAR_DAY) >= value)
            return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    }
    if (type == EVENT_SCENARIO_CONDITION_CASUALTIES_AT_LEAST
        && world_script_get_variable(EVENT_SCRIPT_VAR_CASUALTIES) >= id)
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type == EVENT_SCENARIO_CONDITION_CASUALTIES_AT_MOST
        && id >= world_script_get_variable(EVENT_SCRIPT_VAR_CASUALTIES))
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type == EVENT_SCENARIO_CONDITION_ENEMIES_DEFEATED && battle_action_check_battle_outcome() == 0)
        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    if (type == EVENT_SCENARIO_CONDITION_UNIT_AT_TILE) {
        unit = world_unit_get_by_unit_id(id);
        if (unit != (battle_stats_t*)-1 && unit->x == value && unit->position.y == tile_y
            && unit->position.higher_elevation == elevation)
            return EVENT_SCENARIO_CONDITION_RESULT_PASS;
    }
    if (type == EVENT_SCENARIO_CONDITION_PALETTE_UNIT_AT_TILE) {
        for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
            if (battle_unit_has_misc_id(i)) {
                j = battle_unit_get_battle_index_by_misc_id(i);
                if (j != -1) {
                    unit = battle_unit_get_stats_from_battle_id(j);
                    if (unit->x == value && unit->position.y == tile_y && unit->position.higher_elevation == elevation
                        && unit->sprite_palette == id)
                        return EVENT_SCENARIO_CONDITION_RESULT_PASS;
                }
            }
        }
    }
    if (type == EVENT_SCENARIO_CONDITION_RUN_EVENT) {
        world_script_set_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT, id);
        return EVENT_SCENARIO_CONDITION_RESULT_RUN_EVENT;
    }
    return EVENT_SCENARIO_CONDITION_RESULT_FAIL;
}
