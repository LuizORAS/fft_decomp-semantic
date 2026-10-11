#include "fft/battle.h"

/* Chooses whether an ability announcement shows a spell quote.
 *
 * Battle twin of world_text_determine_spell_quote. Unknown
 * skillsets (not in the list and not the unit's flagged primary/secondary)
 * get quote command 5 unless the ability repeats the last quote
 * (g_battle_spell_quote_last_ability_id), the unit is a Calculator or Mime, the ability is >= 0x170 or
 * lacks the spell-quote flag, status blocks it, options disable it, or
 * EVENT_SCRIPT_VAR_RANDOM_VALUE is 4 or more. Everything else uses command 7. */
void battle_text_determine_spell_quote(battle_action_command_t* action, s32 unit_id, s32 enabled) {
    ability_data_t* ability;
    main_ability_secondary_pointer_t secondary;
    battle_stats_t* unit;
    s32 known;

    main_ability_calculate_pointers_and_type(action->ability_id & ABILITY_ID_MASK, &ability, &secondary);
    unit = battle_unit_get_stats_from_battle_id(unit_id);
    if (action->skillset_id != 0) {
        known = battle_is_skillset_in_spell_quote_exception_list(action->skillset_id);
    } else {
        known = 1;
    }
    if (action->skillset_id == unit->primary_skillset
        && (unit->position.spell_quote_skillsets & BATTLE_UNIT_SPELL_QUOTE_PRIMARY_SKILLSET)) {
        known = 1;
    }
    if (action->skillset_id == unit->secondary_skillset
        && (unit->position.spell_quote_skillsets & BATTLE_UNIT_SPELL_QUOTE_SECONDARY_SKILLSET)) {
        known = 1;
    }
    if (known == 0 && g_battle_spell_quote_last_ability_id != (action->ability_id & ABILITY_ID_MASK)
        && unit->job_id != JOB_ID_CALCULATOR && unit->job_id != JOB_ID_MIME
        && action->ability_id < ABILITY_ID_ITEM_FIRST
        && (secondary.range->flags_3 & ABILITY_SECONDARY_FLAG_3_SPELL_QUOTE)
        && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)))
        && g_main_game_options.fields.ability_names == GAME_OPTION_ON
        && (battle_script_get_variable(EVENT_SCRIPT_VAR_RANDOM_VALUE) & 0xF) < 4) {
        g_battle_spell_quote_last_ability_id = action->ability_id & ABILITY_ID_MASK;
        battle_menu_dispatch_system_function(BATTLE_MENU_SYSTEM_COMMAND_SPELL_QUOTE, 0, unit_id, 0, enabled, action);
        return;
    }
    if (action->skillset_id != 0) {
        known = battle_is_skillset_in_spell_quote_exception_list(action->skillset_id);
    } else {
        known = 0;
    }
    if ((known == 0 && (ability->type_flags & ABILITY_TYPE_FLAG_SHOW_NAME)
            && g_main_game_options.fields.ability_names == GAME_OPTION_ON)
        || action->skillset_id == SKILLSET_ID_ITEM || action->skillset_id == SKILLSET_ID_THROW) {
        battle_menu_dispatch_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_ABILITY_ANNOUNCEMENT, 0, unit_id, 0, enabled, action);
    } else {
        battle_menu_dispatch_system_function(BATTLE_MENU_SYSTEM_COMMAND_ABILITY_ANNOUNCEMENT, 0, unit_id, 0, 0, action);
    }
}
