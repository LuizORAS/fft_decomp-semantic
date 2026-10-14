#include "fft/battle.h"

/* Announce the ability a unit is about to use (battle_state_announce_next_ability). A spell quote
 * (system command 5: the unit speaks a line) comes for a default ability flagged
 * ABILITY_SECONDARY_FLAG_3_SPELL_QUOTE from a skillset that is neither in
 * g_battle_spell_quote_exception_skillsets nor a primary or secondary skillset the unit marks
 * (spell_quote_skillsets), unless it repeats the last quote (g_battle_spell_quote_last_ability_id),
 * the unit is a Calculator or Mime, Transparent or a Frog, the Ability Names option is off, or the
 * random script variable's low nibble is 4 or more. Otherwise command 7 shows the ability's name (the
 * item's for Item and Throw) when the record has ABILITY_TYPE_FLAG_SHOW_NAME, its skillset is not an
 * exception and the option is on, and always for Item and Throw; anything else passes 0, which opens
 * no window (system function 8). Battle twin of world_text_determine_spell_quote. */
void battle_text_determine_spell_quote(world_unit_command_action_t* action, s32 unit_id, s32 enabled) {
    ability_data_t* ability;
    ability_secondary_data_t* secondary;
    battle_stats_t* unit;
    s32 known;

    main_ability_calculate_pointers_and_type(action->ability_id & ABILITY_ID_MASK, (u8**)&ability, (u8**)&secondary);
    unit = battle_unit_get_stats_from_battle_id(unit_id);
    if (action->skillset != 0) {
        known = battle_text_is_skillset_in_spell_quote_exception_list(action->skillset);
    } else {
        known = 1;
    }
    if (action->skillset == unit->primary_skillset
        && (unit->position.bits.spell_quote_skillsets & BATTLE_UNIT_SPELL_QUOTE_PRIMARY_SKILLSET)) {
        known = 1;
    }
    if (action->skillset == unit->secondary_skillset
        && (unit->position.bits.spell_quote_skillsets & BATTLE_UNIT_SPELL_QUOTE_SECONDARY_SKILLSET)) {
        known = 1;
    }
    if (known == 0 && g_battle_spell_quote_last_ability_id != (action->ability_id & ABILITY_ID_MASK)
        && unit->job_id != JOB_ID_CALCULATOR && unit->job_id != JOB_ID_MIME
        && action->ability_id < ABILITY_ID_ITEM_FIRST && (secondary->flags_3 & ABILITY_SECONDARY_FLAG_3_SPELL_QUOTE)
        && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
            & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG)))
        && g_main_game_options.fields.ability_names == GAME_OPTION_ON
        && (battle_script_get_variable(EVENT_SCRIPT_VAR_RANDOM_VALUE) & 0xF) < 4) {
        g_battle_spell_quote_last_ability_id = action->ability_id & ABILITY_ID_MASK;
        battle_menu_dispatch_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_SPELL_QUOTE, 0, unit_id, 0, enabled, (battle_ai_command_action_t*)action);
        return;
    }
    if (action->skillset != 0) {
        known = battle_text_is_skillset_in_spell_quote_exception_list(action->skillset);
    } else {
        known = 0;
    }
    if ((known == 0 && (ability->type_flags & ABILITY_TYPE_FLAG_SHOW_NAME)
            && g_main_game_options.fields.ability_names == GAME_OPTION_ON)
        || action->skillset == SKILLSET_ID_ITEM || action->skillset == SKILLSET_ID_THROW) {
        battle_menu_dispatch_system_function(BATTLE_MENU_SYSTEM_COMMAND_ABILITY_ANNOUNCEMENT, 0, unit_id, 0, enabled,
            (battle_ai_command_action_t*)action);
    } else {
        battle_menu_dispatch_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_ABILITY_ANNOUNCEMENT, 0, unit_id, 0, 0, (battle_ai_command_action_t*)action);
    }
}
