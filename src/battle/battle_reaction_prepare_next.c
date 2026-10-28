#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Reaction-ability outcome carried in `outcome` between the per-ability arms and the common tail.
 * NONE skips the unit; ABILITY (range checked) and COUNTER act through an ability and pay its MP;
 * REFLECT sends a spell on; EFFECT resolves the reaction's own effect at the unit's tile, or at the
 * actor's for Damage Split. */
enum {
    BATTLE_REACTION_OUTCOME_EFFECT = -1,
    BATTLE_REACTION_OUTCOME_NONE = 0,
    BATTLE_REACTION_OUTCOME_ABILITY = 1,
    BATTLE_REACTION_OUTCOME_COUNTER = 2,
    BATTLE_REACTION_OUTCOME_REFLECT = 3,
};

/* The switch runs on the offset from Meatbone Slash, the first reaction with an arm. */
#define REACTION_CASE(name) (ABILITY_ID_REACTION_##name - ABILITY_ID_REACTION_MEATBONE_SLASH)

/* Find the first unit other than the actor with a queued reaction, and set that reaction up as the
 * next action phase. The unit must be present and able to act; it also needs to be free to react, and
 * outside an AI simulation, except for Reflect. Counter and Meatbone Slash answer with Attack and
 * Counter Tackle with Dash at the actor's tile, each within range; Counter Magic casts the spell it was
 * hit by and Counter Flood the Elemental ability of its own tile, both without a range test; Reflect
 * sends the spell on (battle_target_apply_reflect); Auto Potion drinks the first potion the party has
 * (Potion, then Hi-Potion, then X-Potion); the other reactions run their own effect on the unit's own
 * tile (Damage Split on the actor's), unless their behavior flags hold PASSIVE or ACTIVE. A counter
 * through an ability pays its MP. The unit's command is saved in the reaction snapshot first and
 * restored when the reaction falls through. Returns the reacting unit's slot and writes the reaction id
 * to out_ability, or -1 when no unit reacts.
 *
 * The action pointer keeps its own register: the stores use that base while the unit pointer changes
 * for Damage Split. */
s32 battle_reaction_prepare_next(s16* out_ability) {
    battle_stats_t* unit;
    action_command_t* action;
    s32 i;
    s32 outcome;
    s32 result;
    s32 reaction_prevented;
    u16 ability;
    u16 last_attack;
    s16 reaction_id;

    if (g_battle_acting_unit_id == -1) {
        return -1;
    }
    i = 0;
    do {
        g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
        if (i != g_battle_acting_unit_id) {
            unit = &g_battle_unit_stats[i];
            action = &unit->action_command;
            if (battle_unit_get_action_block(unit) == BATTLE_UNIT_ACTION_BLOCK_NONE
                && unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
                reaction_prevented = battle_reaction_is_prevented(unit);
                ability = unit->action.reaction_id;
                if (((reaction_prevented == 0 && g_battle_action_state != BATTLE_ACTION_STATE_AI_SIMULATION)
                        || ability == ABILITY_ID_REACTION_REFLECT)
                    && ability != 0) {
                    g_battle_action_context = BATTLE_ACTION_CONTEXT_REACTION_OR_SIMULATION;
                    main_util_copy_action_data(action, &g_battle_action_saved_command);
                    g_battle_reaction_ability_id = ability;
                    action->actor_id = i;
                    action->skillset_id = 0;
                    reaction_id = g_battle_reaction_ability_id;
                    outcome = BATTLE_REACTION_OUTCOME_ABILITY;
                    action->ability_id = reaction_id;
                    last_attack = unit->action.last_received_attack;
                    unit->action.reaction_id = 0;
                    switch (reaction_id) {
                    case ABILITY_ID_REACTION_REFLECT:
                        outcome = -(battle_target_apply_reflect(unit) == 0) & 3;
                        break;
                    case ABILITY_ID_REACTION_MEATBONE_SLASH:
                        if (battle_reaction_set_counter_command(unit, SKILLSET_ID_ATTACK, 0, 1) != 0) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        } else {
                            action->skillset_id = 1;
                            action->targeting_type = BATTLE_TARGETING_TYPE_TILE;
                        }
                        break;
                    case ABILITY_ID_REACTION_COUNTER_MAGIC:
                        outcome
                            = (battle_reaction_set_counter_command(unit, SKILLSET_ID_BLACK_MAGIC, last_attack, 0) == 0)
                            * 2;
                        break;
                    case ABILITY_ID_REACTION_COUNTER_TACKLE:
                        if (battle_reaction_set_counter_command(
                                unit, SKILLSET_ID_BLACK_MAGIC, ABILITY_ID_BASIC_SKILL_DASH, 1)
                            != 0) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        }
                        break;
                    case ABILITY_ID_REACTION_COUNTER_FLOOD:
                        outcome = (battle_reaction_set_counter_command(unit, SKILLSET_ID_ELEMENTAL,
                                       g_main_terrain_geomancy_abilities
                                           [g_battle_map_tile_data[battle_map_calculate_location(unit)].surface.type],
                                       0)
                                      == 0)
                            * 2;
                        break;
                    case ABILITY_ID_REACTION_AUTO_POTION:
                        result = battle_reaction_select_auto_potion_item(unit);
                        if (result == -1) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                            break;
                        }
                        action->skillset_id = SKILLSET_ID_ITEM;
                        /* Auto-Potion updates only the low byte of the 16-bit item/equipment field. */
                        *(u8*)&action->item_or_equipment = result;
                        action->targeting_type = BATTLE_TARGETING_TYPE_TILE;
                        action->target_id = i;
                        action->target_position.vx = unit->x;
                        action->target_position.vz = unit->position.y;
                        action->target_position.vy = unit->position.higher_elevation;
                        result = battle_action_commit_command(action);
                        outcome = BATTLE_REACTION_OUTCOME_COUNTER;
                        unit->action_command.ability_id = ABILITY_ID_REACTION_AUTO_POTION;
                        if (result == -1 || result == 2 || result == 3) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        }
                        break;
                    case ABILITY_ID_REACTION_COUNTER:
                        if (battle_reaction_set_counter_command(unit, SKILLSET_ID_ATTACK, 0, 1) != 0) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        }
                        break;
                    default:
                        outcome = -(
                            (g_main_reaction_behavior_flags[g_battle_reaction_ability_id - ABILITY_ID_REACTION_FIRST]
                                & (BATTLE_REACTION_BEHAVIOR_FLAG_PASSIVE | BATTLE_REACTION_BEHAVIOR_FLAG_ACTIVE))
                            == 0);
                        break;
                    }
                    if ((outcome == 1 || outcome == 2) && battle_action_check_and_consume_mp(unit) != 0) {
                        outcome = BATTLE_REACTION_OUTCOME_NONE;
                    }
                    if (outcome == BATTLE_REACTION_OUTCOME_NONE) {
                        main_util_copy_action_data(&g_battle_action_saved_command, action);
                        i++;
                        continue;
                    }
                    if (outcome >= 1 && outcome <= 3) {
                        battle_action_init_current_ability_strike_data(unit);
                    } else {
                        g_battle_action_current_ability.strike_count = 1;
                        g_battle_action_current_ability.strike_counter = 0;
                        g_battle_action_current_ability.weapon_spell_pending = 0;
                        g_battle_action_current_ability.knockback_flags = 0;
                        g_battle_action_current_ability.primary_weapon_id
                            = unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON];
                        g_battle_action_current_ability.secondary_weapon_id
                            = unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON];
                        action->skillset_id = 0;
                        action->targeting_type = BATTLE_TARGETING_TYPE_TILE;
                        action->target_id = i;
                        if (g_battle_reaction_ability_id == ABILITY_ID_REACTION_DAMAGE_SPLIT) {
                            action->target_id = g_battle_acting_unit_id;
                            unit = &g_battle_unit_stats[g_battle_acting_unit_id];
                        }
                        action->target_position.vx = unit->x;
                        action->target_position.vz = unit->position.y;
                        action->target_position.vy = unit->position.higher_elevation;
                    }
                    *out_ability = g_battle_reaction_ability_id;
                    action->actor_id = i;
                    return i;
                }
            }
        }
        i++;
    } while (i < 21);
    g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
    *out_ability = 0;
    return -1;
}
