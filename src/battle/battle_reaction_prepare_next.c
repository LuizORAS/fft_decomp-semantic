#include "fft/battle.h"
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
 * Counter Tackle with Dash at the actor's tile, each within range; Counter Magic casts the spell it
 * was hit by and Counter Flood the Geomancy of its own tile, both without a range test; Reflect sends
 * the spell on (battle_target_apply_reflect); Auto Potion drinks the first potion the party has
 * (Potion, then Hi-Potion, then X-Potion); the other reactions run their own effect, unless their
 * behaviour flags hold PASSIVE or ACTIVE. A counter through an ability pays its MP. The unit's command is saved in the
 * reaction snapshot first and restored when the reaction falls through. Returns the reacting unit's slot and writes the
 * reaction id to out_ability, or -1 when no unit reacts.
 *
 * The action block is addressed through a `u8*` at battle_stats_t +0x16e for
 * the same reason as battle_reaction_set_counter_command: the target keeps that
 * pointer in its own register and stores through it rather than through the
 * unit. */
s32 battle_reaction_prepare_next(u16* out_ability) {
    battle_stats_t* unit;
    u8* action;
    s32 i;
    s32 outcome;
    s32 result;
    s32 can_react;
    u16 ability;
    u16 last_attack;
    s32 reaction_id;

    if (g_battle_acting_unit_id == -1) {
        return -1;
    }
    i = 0;
    do {
        g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
        if (i != g_battle_acting_unit_id) {
            unit = &g_battle_unit_stats[i];
            action = &unit->action_actor_id;
            if (battle_formula_can_unit_evade(unit) == 0 && unit->entd_slot != BATTLE_ENTD_SLOT_NONE) {
                can_react = battle_reaction_is_prevented(unit);
                ability = unit->action.reaction_id;
                if (((can_react == 0 && g_battle_action_state != BATTLE_ACTION_STATE_AI_SIMULATION)
                        || ability == ABILITY_ID_REACTION_REFLECT)
                    && ability != 0) {
                    g_battle_action_context = BATTLE_ACTION_CONTEXT_REACTION_OR_SIMULATION;
                    main_util_copy_action_data(action, g_reaction_unit_action_data_16e);
                    g_battle_current_reaction_ability_id = ability;
                    action[0] = i;
                    action[1] = 0;
                    reaction_id = *(u16*)&g_battle_current_reaction_ability_id;
                    outcome = BATTLE_REACTION_OUTCOME_ABILITY;
                    *(u16*)(action + 2) = reaction_id;
                    last_attack = unit->action.last_received_attack;
                    unit->action.reaction_id = 0;
                    switch ((s16)(reaction_id - ABILITY_ID_REACTION_MEATBONE_SLASH)) {
                    case REACTION_CASE(REFLECT):
                        outcome = -(battle_target_apply_reflect(unit) == 0) & 3;
                        break;
                    case REACTION_CASE(MEATBONE_SLASH):
                        if (battle_reaction_set_counter_command(unit, SKILLSET_ID_ATTACK, 0, 1) != 0) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        } else {
                            action[1] = SKILLSET_ID_ATTACK;
                            action[0xA] = BATTLE_ACTION_TARGET_TILE;
                        }
                        break;
                    case REACTION_CASE(COUNTER_MAGIC):
                        outcome = (battle_reaction_set_counter_command(unit, 0xB, (s16)last_attack, 0) == 0) * 2;
                        break;
                    case REACTION_CASE(COUNTER_TACKLE):
                        if (battle_reaction_set_counter_command(unit, 0xB, ABILITY_ID_BASIC_SKILL_DASH, 1) != 0) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        }
                        break;
                    case REACTION_CASE(COUNTER_FLOOD):
                        outcome = (battle_reaction_set_counter_command(unit, SKILLSET_ID_ELEMENTAL,
                                       g_geomancy_terrain_ability_table
                                           [g_battle_map_tile_data[battle_map_calculate_location(unit)].surface.value
                                               & MAP_SURFACE_MASK],
                                       0)
                                      == 0)
                            * 2;
                        break;
                    case REACTION_CASE(AUTO_POTION):
                        result = battle_reaction_select_auto_potion_item(unit);
                        if (result == -1) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                            break;
                        }
                        action[1] = SKILLSET_ID_ITEM;
                        action[8] = result;
                        action[0xA] = BATTLE_ACTION_TARGET_TILE;
                        action[0xB] = i;
                        *(s16*)(action + 0xC) = unit->x;
                        *(s16*)(action + 0x10) = unit->position.bits.y;
                        *(u16*)(action + 0xE) = unit->position.raw >> 15;
                        result = battle_action_commit_command(action);
                        outcome = BATTLE_REACTION_OUTCOME_COUNTER;
                        unit->last_ability_id = ABILITY_ID_REACTION_AUTO_POTION;
                        if (result == -1 || (u32)(result - 2) < 2) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        }
                        break;
                    case REACTION_CASE(COUNTER):
                        if (battle_reaction_set_counter_command(unit, SKILLSET_ID_ATTACK, 0, 1) != 0) {
                            outcome = BATTLE_REACTION_OUTCOME_NONE;
                        }
                        break;
                    default:
                        outcome
                            = -((g_main_reaction_behavior_flags_by_ability_id[g_battle_current_reaction_ability_id]
                                    & (BATTLE_REACTION_BEHAVIOR_FLAG_PASSIVE | BATTLE_REACTION_BEHAVIOR_FLAG_ACTIVE))
                                == 0);
                        break;
                    }
                    if ((u32)(outcome - 1) < 2 && battle_action_check_and_consume_mp(unit) != 0) {
                        outcome = BATTLE_REACTION_OUTCOME_NONE;
                    }
                    if (outcome == BATTLE_REACTION_OUTCOME_NONE) {
                        main_util_copy_action_data(g_reaction_unit_action_data_16e, action);
                        i++;
                        continue;
                    }
                    if ((u32)(outcome - 1) < 3) {
                        battle_action_init_current_ability_strike_data(unit);
                    } else {
                        g_current_ability.strike_count = 1;
                        g_current_ability.strike_counter = 0;
                        g_current_ability.weapon_spell_pending = 0;
                        g_current_ability.knockback_flags = 0;
                        g_current_ability.primary_weapon_id = unit->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON];
                        g_current_ability.secondary_weapon_id = unit->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON];
                        action[1] = 0;
                        action[0xA] = BATTLE_ACTION_TARGET_TILE;
                        action[0xB] = i;
                        if (g_battle_current_reaction_ability_id == ABILITY_ID_REACTION_DAMAGE_SPLIT) {
                            action[0xB] = g_battle_acting_unit_id_byte;
                            unit = &g_battle_unit_stats[g_battle_acting_unit_id];
                        }
                        *(s16*)(action + 0xC) = unit->x;
                        *(s16*)(action + 0x10) = unit->position.bits.y;
                        *(u16*)(action + 0xE) = unit->position.raw >> 15;
                    }
                    *out_ability = *(u16*)&g_battle_current_reaction_ability_id;
                    action[0] = i;
                    return i;
                }
            }
        }
        i++;
    } while (i < BATTLE_UNIT_SLOT_COUNT);
    g_battle_action_context = BATTLE_ACTION_CONTEXT_PRIMARY;
    *out_ability = 0;
    return -1;
}
