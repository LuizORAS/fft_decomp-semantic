#include "fft/battle.h"
#include "psx/types.h"

extern void battle_text_determine_spell_quote(world_unit_command_action_t* action, s32 unit_id, s32 enabled);

enum { BATTLE_ACTION_PHASE_COUNT = 3 };

/* Announce the next part of the action (ANNOUNCE_ABILITY, with its name or spell quote): phase
 * 0 a First Strike from the single target, phase 1 the action itself, phase 2 each reaction in
 * turn. When the action cannot go ahead (after a First Strike, or when
 * battle_action_check_and_consume_mp refuses it) its unit shows a number sprite for results 0-3
 * and finishes its animation. Returns 1 when an ability was announced, 0 when the three phases
 * are done. */
s32 battle_state_announce_next_ability(void) {
    battle_ai_command_action_t command;
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* actor;
    s32 result;
    s32 ability_id;
    battle_stats_t* actor_stats;

    unit = battle_unit_get_source_misc_data();
    do {
        switch (g_battle_action_phase) {
        case 0:
            result = battle_reaction_prepare_hamedo_for_pending_action(
                unit->battle_data->misc_unit_id, (u16*)&command.ability_id);
            if (result != -1) {
                g_battle_game_state = BATTLE_GAME_STATE_ANNOUNCE_ABILITY;
                actor = battle_unit_get_misc_data_by_battle_id(result & 0xffff);
                g_battle_casting_unit_id = actor->unit_id;
                command.skillset = 0;
                actor_stats = actor->battle_data;
                battle_text_determine_spell_quote((world_unit_command_action_t*)&command, actor_stats->misc_unit_id, 1);
                ability_id = command.ability_id;
                result = 0;
                actor->continue_attack_count = 0;
                g_battle_action_post_action = 0;
                actor->sp2_ability_id = ability_id;
                actor->used_ability_id = ability_id;
            }
            break;
        case 1:
            result = battle_action_begin(unit->battle_data->misc_unit_id);
            if (result == 0) {
                g_battle_game_state = BATTLE_GAME_STATE_ANNOUNCE_ABILITY;
                g_battle_casting_unit_id = unit->unit_id;
                battle_text_determine_spell_quote((world_unit_command_action_t*)&unit->battle_data->action_actor_id,
                    unit->battle_data->misc_unit_id, 1);
                ability_id = unit->command_state.ai.data.action.ability_id;
                unit->continue_attack_count = 0;
                g_battle_action_post_action = 0;
                unit->sp2_ability_id = ability_id;
                unit->used_ability_id = ability_id;
            } else {
                if (result >= 0) {
                    if (result < 4) {
                        battle_gfx_activate_numerical_sprite_data(unit, result);
                    }
                }
                battle_unit_set_animation_based_on_status(unit);
                battle_unit_update_post_command_animation_display(unit);
                result = -1;
            }
            break;
        case 2:
            result = battle_reaction_prepare_next(&command.ability_id);
            if (result != -1) {
                g_battle_game_state = BATTLE_GAME_STATE_ANNOUNCE_ABILITY;
                actor = battle_unit_get_misc_data_by_battle_id(result & 0xffff);
                g_battle_casting_unit_id = actor->unit_id;
                command.skillset = 0;
                actor_stats = actor->battle_data;
                battle_text_determine_spell_quote((world_unit_command_action_t*)&command, actor_stats->misc_unit_id, 1);
                ability_id = command.ability_id;
                result = 0;
                actor->continue_attack_count = 0;
                g_battle_action_post_action = 0;
                actor->sp2_ability_id = ability_id;
                actor->used_ability_id = ability_id;
            }
            break;
        }
        if (result != -1) {
            return 1;
        }
        g_battle_action_phase = g_battle_action_phase + 1;
    } while (g_battle_action_phase < BATTLE_ACTION_PHASE_COUNT);
    return 0;
}
