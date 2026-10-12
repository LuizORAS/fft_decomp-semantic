#ifndef BATTLE_STATUS_H
#define BATTLE_STATUS_H

/* BATTLE status infliction, upkeep and display state. Shared status IDs and
 * stored status sets are defined in unit.h. */

#include "fft/battle_unit.h"

/* status */
extern u8 g_battle_player_unit_fallen;
/* Event infliction helpers number bits LSB-first, unlike stored status IDs. */
#define STATUS_LSB_INDEX(id) (((id) & ~7) | (7 - ((id) & 7)))

#define STATUS_LSB_MASK(id) ((u8)(1U << ((id) & 7)))

/* The special-status callback table reserves index 0; timed slots begin at Poison. */
#define STATUS_HANDLER_INDEX(id) ((id) + 1)

#define STATUS_CT_INDEX(id) ((id) - STATUS_ID_POISON)

#define STATUS_PACKED_MASK(id) ((u32)STATUS_BYTE_MASK(id) << (STATUS_BYTE_INDEX(id) * 8))

extern const u8 g_status_display_image_ids[STATUS_COUNT];
extern s32 g_status_menu_open;
extern SVECTOR g_status_bubble_zoom;

/* Texture coordinates for the status bubble graphics, indexed by status bubble
 * id (0x800949dc/0x800949f4) and by a counter digit (0x80094a0c/0x80094a24).
 * Each table holds 24 bytes; the pairs are read together as one u/v. */
extern u8 g_status_bubble_u[24];
extern u8 g_status_bubble_v[24];
extern u8 g_status_bubble_digit_u[24];
extern u8 g_status_bubble_digit_v[24];
s32 status_check_unit(battle_stats_t* unit);
void status_disable_acting(battle_stats_t* unit);
void status_enable_special_flags(s32 status_id, s32 enabled, s32 misc_unit_id);
void status_queue_current_status_graphics(s32 battle_id, s32 enabled);
s32 status_inflict_defending_to_battle_id(s32 unit_id);
s32 status_is_unit_absent_dead_crystal_treasure_petrified_or_ridden(const battle_stats_t* unit);
s32 status_modify_inflictions(s32 removal_only);
s32 status_prepare_terrain_poison(battle_stats_t* unit);
s32 status_remove_charging_ability_ct(battle_stats_t* unit, s32 do_disable);
void status_remove_control(battle_stats_t* unit);
s32 status_resolve_unit_changes(s32 unit_id, s32 removal_only);
void status_set_inflicted_ct_and_transfer_last_used_ct(s32 unit_idx);
void status_store_for_current_attack(s32 unit_id, s32 removal_only);
s32 status_apply_poison_and_regen(battle_stats_t* unit);
void status_apply_unit_action_removal(battle_stats_t* unit);
s32 status_build_upkeep_action(s32 unit_id, battle_stats_t* unit);
s32 status_check_crystal_dead_jump_petrify_treasure(battle_stats_t* unit);
void status_enable_disable_acting(battle_stats_t* unit);
void status_inflict_by_entd_unit_id(s32 entd_id, s32 status, s32 which, s32 arg);
s32 status_init_special_flag_enabling(s32 unit_id);
s32 status_queue_misc_graphics_flag_change(s32 status, s32 enabled, s32 battle_id);
void status_reapply_active_flags(battle_stats_t* unit);
void status_remove(void);
s32 status_remove_transparent(battle_stats_t* unit);
void status_remove_transparent_if_jump_used(battle_stats_t* unit);
void status_update_expiring(s32 unit_id);

#endif
