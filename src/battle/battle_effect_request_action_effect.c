#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_effect.h"
#include "fft/battle_unit.h"
#include "fft/data.h"
#include "fft/main.h"
#include "fft/unit.h"
#include "psx/types.h"

/* Request the effect of the unit's action: build its effect init record and pass it to
 * battle_effect_request_ability_effect. battle_action_start_strike calls it.
 *
 * Each of the acting unit's targets contributes a target block carrying its unit id and a result animation;
 * with no targets the first block instead holds the post-action destination tile. Every other living unit
 * that is not already a target is appended to spectator_ids. The packed argument combines the ability's
 * charge animation set with the element used as the effect's sprite palette, and stays 0 while the action has
 * no attack to animate. The failed-condition path is written as its own `packed = 0` block: the target keeps
 * that block, and a pre-initialized `packed` lets GCC thread the failing tests straight to the call. Items
 * 0x4a..0x4c (the elemental guns) and Throw Ball (0x189) are hard-coded exceptions. */
void battle_effect_request_action_effect(s32 ability, battle_unit_t* unit) {
    battle_effect_secondary_init_t init;
    battle_unit_t* target;
    battle_unit_t* spectator;
    battle_stats_t* stats;
    s32 i;
    s32 j;
    s32 spectators;
    u32 packed;
    u8 item;
    battle_strike_work_t* block;

    init.target_count = unit->strike.target_count;
    init.targets[BATTLE_EFFECT_CASTER_ENTRY].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    init.targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.runtime_id = unit->runtime_id;
    main_util_set_svector(&init.target_tile.data.tile, unit->strike.target_new_x, unit->strike.target_new_map_level,
        unit->strike.target_new_y);
    block = &unit->strike;
    if (init.target_count != 0) {
        for (i = 0; i < init.target_count; i++) {
            target = battle_unit_get_by_battle_id(block->target_list[i]);
            init.targets[i].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
            init.targets[i].data.unit.runtime_id = target->runtime_id;
            {
                battle_stats_t* target_stats;

                target_stats = target->stats;

                if (target_stats == 0) {
                    init.targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_NORMAL;
                } else if (target_stats->action.critical != 0) {
                    init.targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_CRITICAL;
                } else if (target_stats->action.miss_type == BATTLE_ACTION_MISS_TYPE_REFLECTED) {
                    init.targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_REFLECTED;
                } else if (target_stats->action.miss_type != BATTLE_ACTION_MISS_TYPE_HIT) {
                    init.targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_MISSED;
                } else {
                    init.targets[i].data.unit.result_animation = BATTLE_EFFECT_HIT_NORMAL;
                }
            }
        }
    } else if (unit->strike.skip_empty_tile_effect == 0) {
        init.target_count = 1;
        init.targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_TILE;
        init.targets[0].data.tile.vx = unit->strike.target_new_x;
        init.targets[0].data.tile.vz = unit->strike.target_new_y;
        init.targets[0].data.tile.vy = unit->strike.target_new_map_level;
    }
    spectators = 0;
    for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
        if (i != unit->runtime_id) {
            spectator = battle_unit_get_by_runtime_id(i);
            if (spectator != 0) {
                stats = spectator->stats;
                if (stats != 0) {
                    for (j = 0; j < block->target_count; j++) {
                        if (stats->battle_id == block->target_list[j]) {
                            break;
                        }
                    }
                    if (j < block->target_count) {
                        continue;
                    }
                    init.spectator_ids[spectators++] = spectator->runtime_id;
                }
            }
        }
    }
    init.spectator_count = spectators;
    init.math_skill_flag = block->math_skill_flag;
    init.used_weapon_id = block->used_weapon_id;
    if (block->last_attack_id == 0 || block->reaction_occurred != 0
        || g_battle_action_phase != BATTLE_ACTION_PHASE_ACTING_UNIT
        || (unit->used_item_or_weapon_id - ITEM_ID_BLAZE_GUN) < 2U
        || (item = unit->used_item_or_weapon_id) == ITEM_ID_BLAST_GUN) {
        packed = 0;
    } else {
        if (ability == ABILITY_ID_THROW_BALL) {
            packed = g_battle_ability_animation_data[ABILITY_ID_THROW_BALL].charge_animation_set_id;
            packed |= g_main_item_weapon_data[item].element << 16;
        } else {
            packed = g_battle_ability_animation_data[ability].charge_animation_set_id;
            packed |= g_main_ability_range_data[ability].element << 16;
        }
    }
    battle_effect_request_ability_effect(packed, ability, &init);
}
