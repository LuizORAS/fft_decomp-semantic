#ifndef BATTLE_STATUS_H
#define BATTLE_STATUS_H

/* BATTLE status infliction, upkeep and display state. Shared status IDs and
 * stored status sets are defined in unit.h. */

#include "fft/unit.h"

/* status */
/* Status rules in battle: how a status set lands (infliction types, an action record's pending
 * inflictions and removals), the turn clock's status upkeep, the event's Inflict Status and the
 * status graphics queue (docs/mechanics/Status.md). */

s32 battle_turn_get_status_flags(const battle_stats_t* unit);
extern const u8 g_battle_status_display_image_ids[BATTLE_STATUS_COUNT];
extern s32 g_battle_status_menu_open;
extern SVECTOR g_battle_status_bubble_zoom;

/* Texture coordinates for the status bubble graphics, indexed by status bubble
 * id (0x800949dc/0x800949f4) and by a counter digit (0x80094a0c/0x80094a24).
 * Each table holds 24 bytes; the pairs are read together as one u/v. */
extern u8 g_battle_status_bubble_u[24];

extern u8 g_battle_status_bubble_v[24];
extern u8 g_battle_status_bubble_digit_u[24];
extern u8 g_battle_status_bubble_digit_v[24];
void battle_status_apply_pending_inflictions(s32 unit_id, s32 removal_only);
void battle_status_apply_pending_removals(s32 unit_idx);
s32 battle_status_apply_poison_and_regen(battle_stats_t* unit);
void battle_status_apply_unit_action_removal(battle_stats_t* unit);
s32 battle_status_build_upkeep_action(s32 unit_id, battle_stats_t* unit);
s32 battle_status_check_charging_charge(battle_stats_t* unit, s32 do_disable);
s32 battle_status_check_crystal_dead_jump_petrify_treasure(const battle_stats_t* unit);
void battle_status_clear_action_state(battle_stats_t* unit);
void battle_status_clear_action_state_2(battle_stats_t* unit);
void battle_status_clear_all(battle_stats_t* unit);
void status_inflict_by_entd_unit_id(s32 event_unit_id, s32 status, s32 which, s32 arg);
s32 battle_status_inflict_defending_to_battle_id(s32 unit_id);
s32 battle_status_init_special_flag_enabling(s32 unit_id);
s32 battle_status_is_unit_absent_dead_crystal_treasure_petrified_or_ridden(const battle_stats_t* unit);
s32 battle_status_modify_inflictions(s32 removal_only);
void battle_status_queue_current_status_graphics(s32 battle_id, s32 enabled);
void battle_status_queue_graphics_change_if_executing(s32 status_id, s32 enabled, s32 battle_id);
s32 battle_status_queue_runtime_graphics_flag_change(s32 status, s32 enabled, s32 battle_id);
void battle_status_remove_control(battle_stats_t* unit);
s32 battle_status_remove_transparent(battle_stats_t* unit);
void battle_status_remove_transparent_if_jump_used(battle_stats_t* unit);
s32 battle_status_resolve_unit_changes(s32 unit_id, s32 removal_only);
void battle_status_store_ability_cancellations(void);
void battle_status_update_expiring(s32 unit_id);
s32 battle_status_prepare_terrain_poison(battle_stats_t* unit);

/* Event infliction helpers number bits LSB-first, unlike stored status IDs. */
#define BATTLE_STATUS_LSB_INDEX(id) (((id) & ~7) | (7 - ((id) & 7)))
#define BATTLE_STATUS_LSB_MASK(id)  ((u8)(1U << ((id) & 7)))
/* The special-status callback table reserves index 0; timed slots begin at Poison. */
#define BATTLE_STATUS_HANDLER_INDEX(id) ((id) + 1)
#define BATTLE_STATUS_CT_INDEX(id)      ((id) - BATTLE_STATUS_ID_POISON)
#define BATTLE_STATUS_PACKED_MASK(id)   ((u32)BATTLE_STATUS_BYTE_MASK(id) << (BATTLE_STATUS_BYTE_INDEX(id) * 8))

/* turn flags */
/* Battle-wide flags the turn sets: whether a player unit has fallen. */

extern u8 g_battle_player_unit_fallen;

#endif
