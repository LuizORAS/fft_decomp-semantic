#ifndef BATTLE_ACTION_H
#define BATTLE_ACTION_H

/* Battle action preparation, execution and result processing. */

#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/main.h"

/* state */
/* The battle state machine: battle_state_run_game_loop runs the handler of g_battle_game_state each frame. */

void battle_state_enter_event(void);
void battle_state_enter_event_at_battle_start(void);
void battle_state_enter_unit_move(void);
void battle_state_enter_wait(void);
void battle_state_enter_wait_direction(void);

/* action */
/* The action pipeline: the ability and charge time a command resolves to, the action record, each
 * strike's targets and formula setup, the result applied to each unit, the rewards and the effect
 * messages. */

u32 battle_turn_count_entries_before_action(s32 battle_id, s32 ct, const battle_at_entry_t* entries);
void battle_state_enter_confirm_action(void);

/* Bitset in battle_action_data_t.attack_type; one result can affect HP and MP. */
typedef enum battle_action_type_flags {
    BATTLE_ACTION_TYPE_PSEUDO_STATUS = 0x01,
    BATTLE_ACTION_TYPE_STATUS_CHANGE = 0x08,
    BATTLE_ACTION_TYPE_MP_HEALING = 0x10,
    BATTLE_ACTION_TYPE_MP_DAMAGE = 0x20,
    BATTLE_ACTION_TYPE_HP_HEALING = 0x40,
    BATTLE_ACTION_TYPE_HP_DAMAGE = 0x80,
} battle_action_type_flags_e;

typedef enum battle_action_miss_type {
    BATTLE_ACTION_MISS_TYPE_HIT = 0,
    BATTLE_ACTION_MISS_TYPE_ACCESSORY_EVADE = 1,
    BATTLE_ACTION_MISS_TYPE_RIGHT_HAND_EVADE = 2,
    BATTLE_ACTION_MISS_TYPE_LEFT_HAND_EVADE = 3,
    BATTLE_ACTION_MISS_TYPE_CLASS_EVADE_OR_ARROW_GUARD = 4,
    BATTLE_ACTION_MISS_TYPE_NULLIFIED = 5,
    BATTLE_ACTION_MISS_TYPE_ACCURACY_MISS = 6,
    BATTLE_ACTION_MISS_TYPE_FORCED_FAILURE = 7,
    BATTLE_ACTION_MISS_TYPE_CANCELLED = 8,
    BATTLE_ACTION_MISS_TYPE_REFLECTED = 9,
    BATTLE_ACTION_MISS_TYPE_GOLEM_GUARD = 10,
    BATTLE_ACTION_MISS_TYPE_BLADE_GRASP_OR_FINGER_GUARD = 11,
    BATTLE_ACTION_MISS_TYPE_BUGGED_REFLECTION = 12,
    BATTLE_ACTION_MISS_TYPE_CATCH = 13,
} battle_action_miss_type_e;

typedef enum battle_action_special_effect {
    BATTLE_ACTION_SPECIAL_EFFECT_SET_GOLEM = 0x0001,
    BATTLE_ACTION_SPECIAL_EFFECT_MORBOL = 0x0002,
    BATTLE_ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT = 0x0004,
    BATTLE_ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_NOT_BROKEN = 0x0008,
    BATTLE_ACTION_SPECIAL_EFFECT_STEAL_ITEM = 0x0010,
    BATTLE_ACTION_SPECIAL_EFFECT_POACH = 0x0020,
    BATTLE_ACTION_SPECIAL_EFFECT_TEAM_CHANGE = 0x0040,
    BATTLE_ACTION_SPECIAL_EFFECT_LEVEL_UP = 0x0080,
    BATTLE_ACTION_SPECIAL_EFFECT_LEVEL_DOWN = 0x0100,
    BATTLE_ACTION_SPECIAL_EFFECT_PROC_TRIGGERED = 0x0200,
    BATTLE_ACTION_SPECIAL_EFFECT_ELEMENTAL_ABSORPTION = 0x0400,
    BATTLE_ACTION_SPECIAL_EFFECT_ELEMENTAL_WEAKNESS = 0x0800,
    BATTLE_ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_BROKEN = 0x1000,
    BATTLE_ACTION_SPECIAL_EFFECT_KNOCKBACK = 0x4000,
    BATTLE_ACTION_SPECIAL_EFFECT_GOLEM_GUARD = 0x8000,
} battle_action_special_effect_e;

typedef enum battle_action_context {
    BATTLE_ACTION_CONTEXT_PRIMARY = 0,
    BATTLE_ACTION_CONTEXT_REACTION_OR_SIMULATION = 1,
} battle_action_context_e;

typedef enum battle_action_state {
    BATTLE_ACTION_STATE_EXECUTE = 0,
    BATTLE_ACTION_STATE_AI_SIMULATION = 1,
    BATTLE_ACTION_STATE_PREVIEW = 2,
} battle_action_state_e;

/* g_battle_action_phase: which unit's action the attack phase is announcing. */
typedef enum battle_action_phase {
    BATTLE_ACTION_PHASE_PRE_ACTION = 0,    /* a reaction that pre-empts the pending action (Hamedo) */
    BATTLE_ACTION_PHASE_ACTING_UNIT = 1,   /* the acting unit's own action */
    BATTLE_ACTION_PHASE_REACTING_UNIT = 2, /* the reactions it provoked */
} battle_action_phase_e;

enum { BATTLE_ACTION_PHASE_COUNT = 3 };

/* Pending post-action displays are queued as one little-endian 32-bit value in runtime
 * Unit Data 0x1b8..0x1bb.  The two dispatchers at 0x8007f5f8 and 0x800808b8
 * consume one bit at a time in ascending order. */
typedef enum battle_action_display_flag {
    BATTLE_ACTION_DISPLAY_FLAG_HP_DAMAGE = 0x00000001,
    BATTLE_ACTION_DISPLAY_FLAG_HP_HEALING = 0x00000002,
    BATTLE_ACTION_DISPLAY_FLAG_MP_DAMAGE = 0x00000004,
    BATTLE_ACTION_DISPLAY_FLAG_MP_HEALING = 0x00000008,
    BATTLE_ACTION_DISPLAY_FLAG_SPEED_DAMAGE = 0x00000010,
    BATTLE_ACTION_DISPLAY_FLAG_SPEED_GAIN = 0x00000020,
    BATTLE_ACTION_DISPLAY_FLAG_CT_DAMAGE = 0x00000040,
    BATTLE_ACTION_DISPLAY_FLAG_CT_GAIN = 0x00000080,
    BATTLE_ACTION_DISPLAY_FLAG_BRAVE_DAMAGE = 0x00000100,
    BATTLE_ACTION_DISPLAY_FLAG_BRAVE_GAIN = 0x00000200,
    BATTLE_ACTION_DISPLAY_FLAG_FAITH_DAMAGE = 0x00000400,
    BATTLE_ACTION_DISPLAY_FLAG_FAITH_GAIN = 0x00000800,
    BATTLE_ACTION_DISPLAY_FLAG_PA_DAMAGE = 0x00001000,
    BATTLE_ACTION_DISPLAY_FLAG_PA_GAIN = 0x00002000,
    BATTLE_ACTION_DISPLAY_FLAG_MA_DAMAGE = 0x00004000,
    BATTLE_ACTION_DISPLAY_FLAG_MA_GAIN = 0x00008000,
    BATTLE_ACTION_DISPLAY_FLAG_LEVEL_DOWN = 0x00010000,
    BATTLE_ACTION_DISPLAY_FLAG_LEVEL_UP = 0x00020000,
    BATTLE_ACTION_DISPLAY_FLAG_STOLEN = 0x00040000,
    BATTLE_ACTION_DISPLAY_FLAG_BROKEN = 0x00080000,
    BATTLE_ACTION_DISPLAY_FLAG_QUICK = 0x00100000,
    BATTLE_ACTION_DISPLAY_FLAG_CT_ZERO = 0x00200000,
    BATTLE_ACTION_DISPLAY_FLAG_MISSED = 0x00400000,
    BATTLE_ACTION_DISPLAY_FLAG_GUARDED = 0x00800000,
    BATTLE_ACTION_DISPLAY_FLAG_CAUGHT = 0x01000000,
    BATTLE_ACTION_DISPLAY_FLAG_GAINED_EXP = 0x02000000,
    BATTLE_ACTION_DISPLAY_FLAG_GAINED_JP = 0x04000000,
    BATTLE_ACTION_DISPLAY_FLAG_NO_TARGET = 0x08000000,
    BATTLE_ACTION_DISPLAY_FLAG_SILENCED = 0x10000000,
    BATTLE_ACTION_DISPLAY_FLAG_NO_MP = 0x20000000,
} battle_action_display_flag_e;

/* Image IDs used by the projected-action display. These index the 49-entry
 * texture-source table at 0x8014cf68; they are not battle status IDs. */
typedef enum battle_action_display_image_id {
    BATTLE_ACTION_DISPLAY_IMAGE_DEAD = 0,
    BATTLE_ACTION_DISPLAY_IMAGE_UNDEAD = 1,
    BATTLE_ACTION_DISPLAY_IMAGE_PETRIFY = 2,
    BATTLE_ACTION_DISPLAY_IMAGE_INVITE = 3,
    BATTLE_ACTION_DISPLAY_IMAGE_DARKNESS = 4,
    BATTLE_ACTION_DISPLAY_IMAGE_CONFUSION = 5,
    BATTLE_ACTION_DISPLAY_IMAGE_SILENCE = 6,
    BATTLE_ACTION_DISPLAY_IMAGE_BLOOD_SUCK = 7,
    BATTLE_ACTION_DISPLAY_IMAGE_OIL = 8,
    BATTLE_ACTION_DISPLAY_IMAGE_FLOAT = 9,
    BATTLE_ACTION_DISPLAY_IMAGE_RERAISE = 10,
    BATTLE_ACTION_DISPLAY_IMAGE_TRANSPARENT = 11,
    BATTLE_ACTION_DISPLAY_IMAGE_BERSERK = 12,
    BATTLE_ACTION_DISPLAY_IMAGE_POISON = 13,
    BATTLE_ACTION_DISPLAY_IMAGE_REGEN = 14,
    BATTLE_ACTION_DISPLAY_IMAGE_PROTECT = 15,
    BATTLE_ACTION_DISPLAY_IMAGE_SHELL = 16,
    BATTLE_ACTION_DISPLAY_IMAGE_HASTE = 17,
    BATTLE_ACTION_DISPLAY_IMAGE_SLOW = 18,
    BATTLE_ACTION_DISPLAY_IMAGE_STOP = 19,
    BATTLE_ACTION_DISPLAY_IMAGE_FAITH = 20,
    BATTLE_ACTION_DISPLAY_IMAGE_INNOCENT = 21,
    BATTLE_ACTION_DISPLAY_IMAGE_CHARM = 22,
    BATTLE_ACTION_DISPLAY_IMAGE_SLEEP = 23,
    BATTLE_ACTION_DISPLAY_IMAGE_DONT_MOVE = 24,
    BATTLE_ACTION_DISPLAY_IMAGE_DONT_ACT = 25,
    BATTLE_ACTION_DISPLAY_IMAGE_REFLECT = 26,
    BATTLE_ACTION_DISPLAY_IMAGE_DEATH_SENTENCE = 27,
    BATTLE_ACTION_DISPLAY_IMAGE_STOLEN = 28,
    BATTLE_ACTION_DISPLAY_IMAGE_BROKEN = 29,
    BATTLE_ACTION_DISPLAY_IMAGE_QUICK = 36,
    BATTLE_ACTION_DISPLAY_IMAGE_HP = 37,
    BATTLE_ACTION_DISPLAY_IMAGE_MP = 38,
    BATTLE_ACTION_DISPLAY_IMAGE_CT = 39,
    BATTLE_ACTION_DISPLAY_IMAGE_SPEED = 40,
    BATTLE_ACTION_DISPLAY_IMAGE_BRAVE = 41,
    BATTLE_ACTION_DISPLAY_IMAGE_FAITH_STAT = 42,
    BATTLE_ACTION_DISPLAY_IMAGE_PHYSICAL_ATTACK = 43,
    BATTLE_ACTION_DISPLAY_IMAGE_MAGICAL_ATTACK = 44,
    BATTLE_ACTION_DISPLAY_IMAGE_LEVEL = 45,
    BATTLE_ACTION_DISPLAY_IMAGE_GIL = 46,
    BATTLE_ACTION_DISPLAY_IMAGE_EXP = 47,
    BATTLE_ACTION_DISPLAY_IMAGE_FROG = 48,
    BATTLE_ACTION_DISPLAY_IMAGE_NONE = 255,
} battle_action_display_image_id_e;

enum { BATTLE_ACTION_DISPLAY_IMAGE_COUNT = BATTLE_ACTION_DISPLAY_IMAGE_FROG + 1 };

typedef struct battle_action_display_image_rect {
    u8 u;
    u8 v;
    u8 width;
    u8 height;
} battle_action_display_image_rect_t;
typedef char
    battle_action_display_image_rect_size_must_be_4[(sizeof(battle_action_display_image_rect_t) == 4) ? 1 : -1];

/* Ordinary SP/PA/MA/Brave/Faith action-result encoding. CT and EXP reuse
 * these bits with distinct sentinel or sign meanings and are excluded. */
enum {
    BATTLE_ACTION_STAT_CHANGE_VALUE_MASK = 0x7f,
    BATTLE_ACTION_STAT_CHANGE_INCREASE = 0x80,
};

/* Bit order mirrors the seven serialized battle_stats_t equipment slots. */
typedef enum battle_action_equipment_slot_mask {
    BATTLE_ACTION_EQUIPMENT_SLOT_HEAD = 0x80,
    BATTLE_ACTION_EQUIPMENT_SLOT_BODY = 0x40,
    BATTLE_ACTION_EQUIPMENT_SLOT_ACCESSORY = 0x20,
    BATTLE_ACTION_EQUIPMENT_SLOT_RIGHT_WEAPON = 0x10,
    BATTLE_ACTION_EQUIPMENT_SLOT_RIGHT_SHIELD = 0x08,
    BATTLE_ACTION_EQUIPMENT_SLOT_LEFT_WEAPON = 0x04,
    BATTLE_ACTION_EQUIPMENT_SLOT_LEFT_SHIELD = 0x02,
    /* Every slot bit: stored by the Steal/Break slot picker before it chooses one. */
    BATTLE_ACTION_EQUIPMENT_SLOT_ALL = 0xfe,
} battle_action_equipment_slot_mask_e;

/* battle_current_ability_t.knockback_flags: the kind of knockback found by
 * battle_formula_calculate_knockback, with PENDING set until it is applied. */
typedef enum battle_knockback_kind {
    BATTLE_KNOCKBACK_FALL_DAMAGE = 1,    /* a grounded target: the fall height can hurt */
    BATTLE_KNOCKBACK_NO_FALL_DAMAGE = 2, /* a flying target */
} battle_knockback_kind_e;

/* Encoding of the kind and pending marker in the same byte. */
enum {
    BATTLE_KNOCKBACK_KIND_MASK = 0x7f,
    BATTLE_KNOCKBACK_PENDING = 0x80,
};

/* battle_current_ability_t.post_action_message, copied to the strike work and
 * the runtime unit record: the message the post-action state opens. */
typedef enum battle_post_action_message {
    BATTLE_POST_ACTION_MESSAGE_NONE = 0,
    BATTLE_POST_ACTION_MESSAGE_POACHED = 6,
    BATTLE_POST_ACTION_MESSAGE_TRAINED = 9,
} battle_post_action_message_e;

/* Current ability data at 0x801938c0: the attacker/target pair, weapons, formula
 * inputs and results of the strike being resolved, with copies of the ability's
 * secondary data, the weapon's data and the status-infliction record that
 * battle_action_run_pre_formula_setup loads. */
typedef struct battle_current_ability {
    u8 attacker_id;         /* 0x00 */
    u8 target_id;           /* 0x01 */
    u8 strike_count;        /* 0x02 */
    u8 strike_counter;      /* 0x03: weapon hand, 0 = right, 1 = left */
    u8 primary_weapon_id;   /* 0x04 */
    u8 secondary_weapon_id; /* 0x05 */
    u16 reaction_id;        /* 0x06 */
    /* 0x08: 1 while a weapon's spell waits for its strike: it follows the weapon's hit
     * (battle_formula_queue_weapon_spell), and the next strike casts reaction_id at post_action_target_id. */
    u16 weapon_spell_pending;                   /* 0x08; a weapon proc spell is still to strike */
    u8 target_new_x;                            /* 0x0a; knockback or random-fire tile */
    u8 target_new_y;                            /* 0x0b */
    u8 target_new_elevation;                    /* 0x0c */
    u8 terrain;                                 /* 0x0d: surface type under the target */
    s16 xa;                                     /* 0x0e */
    s16 ya;                                     /* 0x10 */
    u8 target_faith;                            /* 0x12 */
    u8 attacker_faith;                          /* 0x13 */
    u8 skillset;                                /* 0x14 */
    u8 _padding_15;                             /* 0x15; aligns ability_id */
    u16 ability_id;                             /* 0x16 */
    u8 weapon_id;                               /* 0x18 */
    u8 two_hands_active;                        /* 0x19 */
    u8 proc_id;                                 /* 0x1a */
    u8 used_item_id;                            /* 0x1b */
    u8 base_hit;                                /* 0x1c */
    u8 _unused_1d;                              /* 0x1d; never read or written; FFHacktics skips 0x801938dd */
    u8 accessory_evade;                         /* 0x1e */
    u8 right_hand_evade;                        /* 0x1f; shield, or weapon with Weapon Guard */
    u8 left_hand_evade;                         /* 0x20 */
    u8 class_evade;                             /* 0x21 */
    u8 facing_modifier;                         /* 0x22: 0 front, 1 side, 2 back */
    u8 _unused_23;                              /* 0x23; never read or written; FFHacktics skips 0x801938e3 */
    u8 charge_power;                            /* 0x24 */
    u8 formula;                                 /* 0x25 */
    u8 target_is_undead;                        /* 0x26 */
    u8 earned_experience;                       /* 0x27 */
    u8 can_earn_exp_jp;                         /* 0x28 */
    u8 defaulted_to_attack;                     /* 0x29 */
    u8 random_damage_factor;                    /* 0x2a */
    u8 mp_cost;                                 /* 0x2b: effective cost recorded before the MP checks at
                                                 * 0x8017db74; reaction eligibility reads it even when no
                                                 * MP was subtracted */
    u8 target_count;                            /* 0x2c */
    u8 knockback_flags;                         /* 0x2d */
    u8 knockback_fall_height;                   /* 0x2e */
    u8 post_action_target_id;                   /* 0x2f */
    ability_secondary_data_t range_data;        /* 0x30 */
    weapon_data_t weapon_data;                  /* 0x3e */
    status_infliction_data_t status_infliction; /* 0x46 */
    u8 level_gained_flag;                       /* 0x4c */
    u8 job_level_gained_flag;                   /* 0x4d */
    u8 post_action_message;                     /* 0x4e; 6 poached, 9 trained */
    u8 random_fire_flag;                        /* 0x4f */
} battle_current_ability_t;
typedef char battle_current_ability_size_must_be_80[(sizeof(battle_current_ability_t) == 80) ? 1 : -1];

/* Current attacker record at 0x8019389c, filled by
 * battle_action_set_current_attacker_data and read by
 * battle_action_prepare_mimic. The AI saves and restores it whole as 30
 * bytes (battle_ai_begin_simulation / battle_ai_end_simulation). */
typedef struct battle_current_attacker_data {
    u8 facing;               /* 0x00 */
    u8 mimic_pending;        /* 0x01: an action was recorded for the Mimes (battle_turn_run_clock) */
    u8 right_hand_weapon_id; /* 0x02 */
    u8 left_hand_weapon_id;  /* 0x03 */
    u8 team;                 /* 0x04 */
    u8 _padding_05;          /* 0x05; aligns target_delta_x */
    s16 target_delta_x;      /* 0x06 */
    s16 target_delta_y;      /* 0x08 */
    action_command_t action; /* 0x0a: copy of battle_stats_t +0x16e */
} battle_current_attacker_data_t;
typedef char battle_current_attacker_data_size_must_be_30[(sizeof(battle_current_attacker_data_t) == 30) ? 1 : -1];

extern battle_current_attacker_data_t g_battle_ai_current_attacker_backup;

/* battle_turn_build_at_list modes. */
typedef enum battle_at_list_mode {
    BATTLE_AT_LIST_MODE_ALL = 0,
    BATTLE_AT_LIST_MODE_TURNS_ONLY = 1,   /* leave out the charged and performing actions */
    BATTLE_AT_LIST_MODE_ACTIONS_ONLY = 2, /* leave out the active unit and the simulated turns */
    BATTLE_AT_LIST_MODE_PREVIEW = 3,      /* as ALL, with the acting unit's new charge counted from this tick */
} battle_at_list_mode_e;

/* Result of battle_action_prepare_attack, kept in battle_unit_t.attack_phase_state. */
typedef enum battle_attack_preparation {
    BATTLE_ATTACK_PREPARATION_INVALID = -1,
    BATTLE_ATTACK_PREPARATION_CHARGE = 0, /* the action has a charge time; also Defend */
    BATTLE_ATTACK_PREPARATION_IMMEDIATE = 1,
    BATTLE_ATTACK_PREPARATION_CHANGE_EQUIPMENT = 2,
    BATTLE_ATTACK_PREPARATION_JUMP = 3,
} battle_attack_preparation_e;

extern battle_action_display_image_rect_t g_battle_action_display_image_rects[BATTLE_ACTION_DISPLAY_IMAGE_COUNT];

/* Eight per status_changes mask byte, indexed from the byte's high bit. */
extern u8 g_battle_action_status_display_image_ids[BATTLE_STATUS_BYTE_COUNT][8];

extern const palette_t g_battle_action_result_palette;
extern s32 g_battle_action_phase;
extern s32 g_battle_action_post_action;
extern s32 g_battle_action_post_action_display_phase; /* battle_post_action_display_phase_e */
extern s32 g_battle_action_post_effect_msg_counter;
extern battle_action_data_t* g_battle_action_attacker_data;
extern battle_action_state_e g_battle_action_state;
extern battle_action_data_t* g_battle_action_target_data;
extern battle_action_context_e g_battle_action_context;
extern s32 g_battle_unit_source_runtime_id;
extern s32 g_battle_casting_runtime_id;
extern battle_current_ability_t g_battle_action_current_ability;
extern s32 g_battle_state_wait_direction_facing;
extern action_command_t g_battle_action_saved_command;
extern battle_stats_t* g_battle_action_target;
extern battle_post_effect_msg_t g_battle_action_post_effect_msgs[];

/* Separate link name for g_battle_action_saved_command.ability_id. */
extern s16 g_battle_action_saved_ability_id;

extern battle_current_attacker_data_t g_battle_action_current_attacker;
extern battle_action_data_t g_battle_action_attacker_action_data;
extern battle_stats_t* g_battle_action_attacker;
extern u8 g_action_menu_row_types[]; /* per-row skillset kind; 13 keeps the row active */
extern s32 g_battle_action_learn_on_hit_unit_id;
extern u8 g_battle_action_canceled_statuses[];

/* Golem barrier HP per team, shared by action resolution and AI snapshots. */
extern u16 g_battle_team_golem[BATTLE_TEAM_COUNT];

s32 battle_action_add_party_item(battle_stats_t* unit, u8 item);
s32 battle_action_add_poached_item_to_fur_shop_inventory(void);
s32 battle_action_add_war_funds(battle_stats_t* unit, s32 total, u8 item_id);
s32 battle_action_apply_actor_result(s32 unit_id);
void battle_action_apply_exp_change(battle_stats_t* unit, u8 amount);
void battle_action_apply_jp_change(battle_stats_t* unit);
void battle_action_apply_strike_results(void);
s32 battle_action_apply_target_result(s32 runtime_id);
s32 battle_action_apply_tile_trap(battle_stats_t* unit);
s32 battle_action_begin(s32 unit_id);

/* Preview action calculation entry run on the main stack. */
s32 battle_action_calculate_projected_effect(
    battle_stats_t* attacker, battle_stats_t* target, action_command_t* command);

void battle_action_cancel_knockback(void);
s32 battle_action_check_and_consume_mp(battle_stats_t* unit);
s32 battle_action_check_battle_outcome(void);
void battle_action_clear_current_data(battle_action_data_t* action);
void battle_action_clear_status_changes(battle_action_data_t* action);
void battle_action_clear_target_and_actor_data(void);
s32 battle_action_commit_command(action_command_t* command);
s32 battle_action_decrement_player_item_quantity(battle_stats_t* unit, s32 item_id, s32 always_one);
void battle_action_execute_ability(void);
void battle_action_finalize_draw_out_katana_result(battle_stats_t* attacker, battle_strike_work_t* work, u8 hit_count);
void battle_action_finalize_target_current_action(void);
s32 battle_action_get_elemental_ability_id(const battle_stats_t* unit);
void battle_action_grant_rewards(void);
void battle_action_init_current_ability_strike_data(battle_stats_t* unit);
void battle_action_init_learn_ability_on_hit(void);
s32 battle_action_init_movement_ability_benefit(battle_stats_t* unit);
void battle_action_play_ability_effect(void);
s32 battle_action_prepare_attack(action_command_t* action, action_command_t* target, s32 flag);
s32 battle_action_prepare_mimic(battle_stats_t* unit);
void battle_action_queue_target_effect_messages(void);
void battle_action_queue_unit_effect_messages(battle_unit_t* unit);
s32 battle_action_remove_broken_or_stolen_equipment(void);
s32 battle_action_report_job_level_up(battle_unit_t* runtime);
s32 battle_action_report_level_up(battle_unit_t* runtime);
s32 battle_action_resolve_ability_strike(s32 battle_id, battle_strike_work_t* work);
void battle_action_resolve_command_ability(action_command_t* action);
s32 battle_action_run_pre_formula_setup(const action_command_t* source, u8 target_id);
void battle_action_set_current_attacker_data(battle_stats_t* unit);
s32 battle_action_set_move_act_flags(s32 unit_id, s32 move_flag, s32 act_flag);
void battle_action_set_only_action_taken(s32 unit_id);
void battle_action_set_only_movement_taken(s32 unit_id);
void battle_action_set_target_unit(battle_stats_t* unit);
s32 battle_action_show_next_effect_message(void);
void battle_action_start_effect_messages(void);
void battle_action_start_strike(void);
void battle_action_store_acting_unit_data(battle_stats_t* unit);
void battle_action_store_used_weapon(const action_command_t* action);
void battle_action_switch_ability_to_default_attack(void);
void battle_action_post_action_facing_stub(battle_stats_t* unused_unit, s32 unused_facing);
s32 battle_reaction_prepare_hamedo_for_pending_action(s32 id, s16* out_ability);

/* The low 9 bits select a result graphic; bit 9 alters its motion for a
 * critical hit.  Values below 0x10 are also indices into the numeric-value
 * table at 0x800962d4. */
typedef enum battle_numeric_display_selector {
    BATTLE_NUMERIC_DISPLAY_HP_DAMAGE = 0x000,
    BATTLE_NUMERIC_DISPLAY_HP_HEALING = 0x001,
    BATTLE_NUMERIC_DISPLAY_MP_DAMAGE = 0x002,
    BATTLE_NUMERIC_DISPLAY_MP_HEALING = 0x003,
    BATTLE_NUMERIC_DISPLAY_SPEED_DAMAGE = 0x004,
    BATTLE_NUMERIC_DISPLAY_SPEED_GAIN = 0x005,
    BATTLE_NUMERIC_DISPLAY_CT_DAMAGE = 0x006,
    BATTLE_NUMERIC_DISPLAY_CT_GAIN = 0x007,
    BATTLE_NUMERIC_DISPLAY_BRAVE_DAMAGE = 0x008,
    BATTLE_NUMERIC_DISPLAY_BRAVE_GAIN = 0x009,
    BATTLE_NUMERIC_DISPLAY_FAITH_DAMAGE = 0x00a,
    BATTLE_NUMERIC_DISPLAY_FAITH_GAIN = 0x00b,
    BATTLE_NUMERIC_DISPLAY_PA_DAMAGE = 0x00c,
    BATTLE_NUMERIC_DISPLAY_PA_GAIN = 0x00d,
    BATTLE_NUMERIC_DISPLAY_MA_DAMAGE = 0x00e,
    BATTLE_NUMERIC_DISPLAY_MA_GAIN = 0x00f,
    BATTLE_NUMERIC_DISPLAY_MISSED = 0x010,
    BATTLE_NUMERIC_DISPLAY_GUARDED = 0x020,
    BATTLE_NUMERIC_DISPLAY_CAUGHT = 0x030,
    BATTLE_NUMERIC_DISPLAY_GAINED_EXP = 0x040,
    BATTLE_NUMERIC_DISPLAY_GAINED_JP = 0x050,
    BATTLE_NUMERIC_DISPLAY_STATUS_INFLICTED = 0x060,
    BATTLE_NUMERIC_DISPLAY_STATUS_REMOVED = 0x070,
    BATTLE_NUMERIC_DISPLAY_LEVEL_DOWN = 0x080,
    BATTLE_NUMERIC_DISPLAY_LEVEL_UP = 0x090,
    BATTLE_NUMERIC_DISPLAY_STOLEN = 0x0a0,
    BATTLE_NUMERIC_DISPLAY_BROKEN = 0x0b0,
    BATTLE_NUMERIC_DISPLAY_QUICK = 0x0c0,
    BATTLE_NUMERIC_DISPLAY_CT_ZERO = 0x0d0,
    BATTLE_NUMERIC_DISPLAY_NO_TARGET = 0x0e0,
    BATTLE_NUMERIC_DISPLAY_SILENCED = 0x0f0,
    BATTLE_NUMERIC_DISPLAY_NO_MP = 0x100,
} battle_numeric_display_selector_e;

/* Encoding in battle_action_result_display_t.selector. */
enum {
    BATTLE_NUMERIC_DISPLAY_SELECTOR_MASK = 0x1ff,
    BATTLE_NUMERIC_DISPLAY_MODIFIER_CRITICAL = 0x200,
};

/* Numeric values correspond to the first sixteen action display flags. */
enum {
    ACTION_DISPLAY_VALUE_COUNT = 16,
    ACTION_DISPLAY_NUMERIC_FLAGS_MASK = 0x0000ffff,
};

extern const u8 g_battle_action_display_numeric_selectors[ACTION_DISPLAY_VALUE_COUNT];
extern u16 g_battle_action_display_values[ACTION_DISPLAY_VALUE_COUNT];

/* g_battle_action_post_action_display_phase: the reports shown one after another once an action
 * has resolved. The post-movement reports use the first two. */
typedef enum battle_post_action_display_phase {
    BATTLE_POST_ACTION_DISPLAY_LEVEL_UP = 0,
    BATTLE_POST_ACTION_DISPLAY_JOB_LEVEL_UP = 1,
    BATTLE_POST_ACTION_DISPLAY_LEARN_ON_HIT = 2,
} battle_post_action_display_phase_e;

/* turn */
/* Turn order: the turn clock that turns CT into turn events, the end of a turn and the AT
 * list. */

typedef enum battle_turn_status_flags {
    BATTLE_TURN_STATUS_CT_FROZEN = 0x01,
    BATTLE_TURN_STATUS_INCAPACITATED = 0x02,
    BATTLE_TURN_STATUS_DEAD = 0x04,
    BATTLE_TURN_STATUS_SLEEP = 0x08,
    BATTLE_TURN_STATUS_HASTE = 0x10,
    BATTLE_TURN_STATUS_SLOW = 0x20,
    BATTLE_TURN_STATUS_BLOCKS_WAIT_MENU_MASK = 0x0f,
    BATTLE_TURN_STATUS_BLOCKS_TURN_MASK = 0x0d,
} battle_turn_status_flags_e;

typedef enum battle_turn_event {
    BATTLE_TURN_EVENT_UNIT_READY = 0x0100,
    BATTLE_TURN_EVENT_ABILITY_READY = 0x0200,
    BATTLE_TURN_EVENT_ACTION_RESULT = 0x0300,
    BATTLE_TURN_EVENT_UNKNOWN_0400 = 0x0400,
    BATTLE_TURN_EVENT_MIME = 0x0500,
    BATTLE_TURN_EVENT_NONE = 0xff00,
} battle_turn_event_e;

/* battle_turn_run_clock modes. */
typedef enum battle_between_turn_mode {
    BATTLE_BETWEEN_TURN_MODE_RUN = 0,
    BATTLE_BETWEEN_TURN_MODE_RESET = 1,
    BATTLE_BETWEEN_TURN_MODE_FIND_ACTIVE_UNIT = 2,
} battle_between_turn_mode_e;

/* g_battle_turn_clock_state and g_battle_turn_clock_resume_state: the phases of
 * battle_turn_run_clock. */
typedef enum battle_between_turn_state {
    BATTLE_BETWEEN_TURN_STATE_ADD_CT = 0,         /* every unit gains CT by its speed */
    BATTLE_BETWEEN_TURN_STATE_PICK_UNIT = 1,      /* the highest CT of 100 or more takes a turn */
    BATTLE_BETWEEN_TURN_STATE_TICK_CHARGES = 2,   /* count down every charged_ability_ct */
    BATTLE_BETWEEN_TURN_STATE_RESOLVE_CHARGE = 3, /* a charge that reached 0 resolves */
    BATTLE_BETWEEN_TURN_STATE_EXPIRE_STATUSES = 4,
    BATTLE_BETWEEN_TURN_STATE_COLLECT_PENDING_ACTIONS = 5, /* note the units whose action has an attack type */
    BATTLE_BETWEEN_TURN_STATE_RUN_PENDING_ACTIONS = 6,
    BATTLE_BETWEEN_TURN_STATE_PICK_UNIT_AFTER_CHARGE = 9, /* a unit whose CT is 0xff takes a turn at once */
    BATTLE_BETWEEN_TURN_STATE_UNIT_TURN = 10,             /* end the turn of each unit that has one */
    BATTLE_BETWEEN_TURN_STATE_MARK_MIMES = 13,
    BATTLE_BETWEEN_TURN_STATE_RUN_MIMES = 14,
    BATTLE_BETWEEN_TURN_STATE_TERRAIN_POISON = 15,
    BATTLE_BETWEEN_TURN_STATE_REMOVE_TRANSPARENT = 17,
    BATTLE_BETWEEN_TURN_STATE_POISON_AND_REGEN = 19,
} battle_between_turn_state_e;

extern u8 g_battle_acting_unit_elevation;
extern u8 g_battle_acting_unit_x;
extern u8 g_battle_acting_unit_y;
extern s32 g_battle_acting_unit_id;
extern s16 g_battle_acting_unit_used_ability_id;
extern s32 g_battle_turn_clock_state;
extern s32 g_battle_turn_clock_resume_state;
extern s32 g_battle_turn_unit_id;
extern s32 g_battle_turn_event;
extern u8 g_battle_turn_pending_flags[21];
extern s32 g_battle_turn_at_list_index;
void battle_turn_advance(void);
s32 battle_turn_build_at_list(battle_at_entry_t* list, battle_at_list_mode_e mode);
void battle_turn_clear_at_list_index(void);
s32 battle_turn_end(s32 unit_id);
s32 battle_turn_find_unit_in_at_list(const battle_stats_t* unit);
s32 battle_turn_get_at_entry_unit(s32 at_index);
s32 battle_turn_insert_at_entry(s32 battle_id, s32 mode, s32 sort_key, battle_at_entry_t* list);
s32 battle_turn_is_over(s32 unit_id);
void battle_turn_next_at_list_index(void);
s32 battle_turn_preview_at_list(battle_stats_t* unit, action_command_t* action, battle_at_entry_t* at_list);
s32 battle_turn_run_clock(battle_between_turn_mode_e mode);
s32 battle_turn_should_end(const battle_stats_t* unit);
void battle_turn_show_at_entry(s32 arg);
void battle_turn_show_next_at_entry(void);
void battle_turn_start_at_list_browse(void);
void battle_turn_take_next_event(void);

/* reaction */
/* Reaction abilities: the Brave roll, who may react, the checks before the formula, when targeted,
 * before HP and MP move and after the result, and the reaction phase (Hamedo, counters and
 * each reaction's effect). */

/* Reaction behavior flags (g_main_reaction_behavior_flags). */
typedef enum battle_reaction_behavior_flag {
    BATTLE_REACTION_BEHAVIOR_FLAG_TRIGGER_ABILITY = 0x01,
    BATTLE_REACTION_BEHAVIOR_FLAG_ABILITY = 0x02,
    BATTLE_REACTION_BEHAVIOR_FLAG_TARGET_SELF = 0x04,
    BATTLE_REACTION_BEHAVIOR_FLAG_TARGET_ATTACKER = 0x08,
    BATTLE_REACTION_BEHAVIOR_FLAG_PASSIVE = 0x10,
    BATTLE_REACTION_BEHAVIOR_FLAG_ACTIVE = 0x20,
} battle_reaction_behavior_flag_e;

extern s16 g_battle_reaction_ability_id;
extern s32 g_battle_reaction_distribute_count;
extern s32 g_battle_reaction_hamedo_cancels;
s32 battle_reaction_apply_effect(void);
s32 battle_reaction_build_targets(s32 actor_id, battle_strike_work_t* work, s32* target_count, u8* target_ids);
void battle_reaction_check_after_result(void);
void battle_reaction_check_before_formula(void);
void battle_reaction_check_before_hp_change(void);
void battle_reaction_check_when_targeted(void);
s32 battle_reaction_fails_brave_roll(const battle_stats_t* unit);
s32 battle_reaction_get_pending(const battle_stats_t* unit);
s32 battle_reaction_is_blocked(const battle_stats_t* unit);
s32 battle_reaction_is_prevented(const battle_stats_t* unit);
void battle_reaction_mark_reflected(void);
s32 battle_reaction_prepare_next(s16* reaction_id);
s32 battle_reaction_select_auto_potion_item(battle_stats_t* unit);
s32 battle_reaction_set_counter_command(battle_stats_t* unit, s8 skillset_id, s16 ability_id, s32 validate_target);
void battle_reaction_try_arrow_guard(void);
void battle_reaction_try_blade_grasp(void);
void battle_reaction_try_counter(u16 reaction_id, u32 mask);
void battle_reaction_try_damage_split(void);
void battle_reaction_try_distribute(void);
void battle_reaction_try_mp_switch(void);
void battle_reaction_try_on_hp_damage(s16 reaction_id);
void battle_reaction_try_on_mp_cost(s16 reaction_id);
void battle_reaction_try_while_critical(s16 reaction_id);

/* target */
/* Targeting (docs/mechanics/Targeting.md): the range and area an ability or weapon marks as panels,
 * with their height and arc limits, the targets a strike takes from them, the map cursor and the tints
 * of the marked tiles. */

extern s32 g_battle_target_cursor_visible;
void battle_target_hide_cursor(void);
void battle_target_set_ability_targets(u8* src);
void battle_target_show_cursor(void);

#endif
