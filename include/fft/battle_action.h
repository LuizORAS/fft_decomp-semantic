#ifndef BATTLE_ACTION_H
#define BATTLE_ACTION_H

/* Battle action preparation, targeting and result processing. */

#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/main.h"

/* turn */
/* action_run_between_turn_control modes. */
typedef enum battle_between_turn_mode {
    BATTLE_BETWEEN_TURN_MODE_RUN = 0,
    BATTLE_BETWEEN_TURN_MODE_RESET = 1,
    BATTLE_BETWEEN_TURN_MODE_FIND_ACTIVE_UNIT = 2,
} battle_between_turn_mode_e;

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

/* g_battle_between_turn_state and g_battle_between_turn_resume_state: the phases of
 * action_run_between_turn_control. */
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
extern menu_active_unit_t g_battle_active_turn_unit;
extern s32 g_battle_between_turn_resume_state;
extern s16 g_battle_acting_unit_used_ability_id;
extern s32 g_battle_between_turn_state;
extern s32 g_battle_turn_unit_id;
s32 action_run_between_turn_control(battle_between_turn_mode_e mode);

/* action */
/* Bitset in action_data_t.attack_type; one result can affect HP and MP. */
typedef enum action_type_flags {
    ACTION_TYPE_PSEUDO_STATUS = 0x01,
    ACTION_TYPE_STATUS_CHANGE = 0x08,
    ACTION_TYPE_MP_HEALING = 0x10,
    ACTION_TYPE_MP_DAMAGE = 0x20,
    ACTION_TYPE_HP_HEALING = 0x40,
    ACTION_TYPE_HP_DAMAGE = 0x80,
} action_type_flags_e;

typedef enum action_miss_type {
    ACTION_MISS_TYPE_HIT = 0,
    ACTION_MISS_TYPE_ACCESSORY_EVADE = 1,
    ACTION_MISS_TYPE_RIGHT_HAND_EVADE = 2,
    ACTION_MISS_TYPE_LEFT_HAND_EVADE = 3,
    ACTION_MISS_TYPE_CLASS_EVADE_OR_ARROW_GUARD = 4,
    ACTION_MISS_TYPE_NULLIFIED = 5,
    ACTION_MISS_TYPE_ACCURACY_MISS = 6,
    ACTION_MISS_TYPE_FORCED_FAILURE = 7,
    ACTION_MISS_TYPE_CANCELLED = 8,
    ACTION_MISS_TYPE_REFLECTED = 9,
    ACTION_MISS_TYPE_GOLEM_GUARD = 10,
    ACTION_MISS_TYPE_BLADE_GRASP_OR_FINGER_GUARD = 11,
    ACTION_MISS_TYPE_BUGGED_REFLECTION = 12,
    ACTION_MISS_TYPE_CATCH = 13,
} action_miss_type_e;

typedef enum action_special_effect {
    ACTION_SPECIAL_EFFECT_SET_GOLEM = 0x0001,
    ACTION_SPECIAL_EFFECT_MORBOL = 0x0002,
    ACTION_SPECIAL_EFFECT_BREAK_EQUIPMENT = 0x0004,
    ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_NOT_BROKEN = 0x0008,
    ACTION_SPECIAL_EFFECT_STEAL_ITEM = 0x0010,
    ACTION_SPECIAL_EFFECT_POACH = 0x0020,
    ACTION_SPECIAL_EFFECT_TEAM_CHANGE = 0x0040,
    ACTION_SPECIAL_EFFECT_LEVEL_UP = 0x0080,
    ACTION_SPECIAL_EFFECT_LEVEL_DOWN = 0x0100,
    ACTION_SPECIAL_EFFECT_NULLIFIED = 0x0200,
    ACTION_SPECIAL_EFFECT_ELEMENTAL_ABSORPTION = 0x0400,
    ACTION_SPECIAL_EFFECT_ELEMENTAL_WEAKNESS = 0x0800,
    ACTION_SPECIAL_EFFECT_DRAW_OUT_KATANA_BROKEN = 0x1000,
    ACTION_SPECIAL_EFFECT_KNOCKBACK = 0x4000,
    ACTION_SPECIAL_EFFECT_GOLEM_GUARD = 0x8000,
} action_special_effect_e;

typedef enum action_context {
    ACTION_CONTEXT_PRIMARY = 0,
    ACTION_CONTEXT_REACTION_OR_SIMULATION = 1,
} action_context_e;

typedef enum action_state {
    ACTION_STATE_EXECUTE = 0,
    ACTION_STATE_AI_SIMULATION = 1,
    ACTION_STATE_PREVIEW = 2,
} action_state_e;

/* g_action_phase: which unit's action the attack phase is announcing. */
typedef enum action_phase {
    ACTION_PHASE_PRE_ACTION = 0,    /* a reaction that pre-empts the pending action (Hamedo) */
    ACTION_PHASE_ACTING_UNIT = 1,   /* the acting unit's own action */
    ACTION_PHASE_REACTING_UNIT = 2, /* the reactions it provoked */
} action_phase_e;

enum { ACTION_PHASE_COUNT = 3 };

/* Pending post-action displays are queued as one little-endian 32-bit value in Misc
 * Unit Data 0x1b8..0x1bb.  The two dispatchers at 0x8007f5f8 and 0x800808b8
 * consume one bit at a time in ascending order. */
typedef enum action_display_flag {
    ACTION_DISPLAY_FLAG_HP_DAMAGE = 0x00000001,
    ACTION_DISPLAY_FLAG_HP_HEALING = 0x00000002,
    ACTION_DISPLAY_FLAG_MP_DAMAGE = 0x00000004,
    ACTION_DISPLAY_FLAG_MP_HEALING = 0x00000008,
    ACTION_DISPLAY_FLAG_SPEED_DAMAGE = 0x00000010,
    ACTION_DISPLAY_FLAG_SPEED_GAIN = 0x00000020,
    ACTION_DISPLAY_FLAG_CT_DAMAGE = 0x00000040,
    ACTION_DISPLAY_FLAG_CT_GAIN = 0x00000080,
    ACTION_DISPLAY_FLAG_BRAVE_DAMAGE = 0x00000100,
    ACTION_DISPLAY_FLAG_BRAVE_GAIN = 0x00000200,
    ACTION_DISPLAY_FLAG_FAITH_DAMAGE = 0x00000400,
    ACTION_DISPLAY_FLAG_FAITH_GAIN = 0x00000800,
    ACTION_DISPLAY_FLAG_PA_DAMAGE = 0x00001000,
    ACTION_DISPLAY_FLAG_PA_GAIN = 0x00002000,
    ACTION_DISPLAY_FLAG_MA_DAMAGE = 0x00004000,
    ACTION_DISPLAY_FLAG_MA_GAIN = 0x00008000,
    ACTION_DISPLAY_FLAG_LEVEL_DOWN = 0x00010000,
    ACTION_DISPLAY_FLAG_LEVEL_UP = 0x00020000,
    ACTION_DISPLAY_FLAG_STOLEN = 0x00040000,
    ACTION_DISPLAY_FLAG_BROKEN = 0x00080000,
    ACTION_DISPLAY_FLAG_QUICK = 0x00100000,
    ACTION_DISPLAY_FLAG_CT_ZERO = 0x00200000,
    ACTION_DISPLAY_FLAG_MISSED = 0x00400000,
    ACTION_DISPLAY_FLAG_GUARDED = 0x00800000,
    ACTION_DISPLAY_FLAG_CAUGHT = 0x01000000,
    ACTION_DISPLAY_FLAG_GAINED_EXP = 0x02000000,
    ACTION_DISPLAY_FLAG_GAINED_JP = 0x04000000,
    ACTION_DISPLAY_FLAG_NO_TARGET = 0x08000000,
    ACTION_DISPLAY_FLAG_SILENCED = 0x10000000,
    ACTION_DISPLAY_FLAG_NO_MP = 0x20000000,
} action_display_flag_e;

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

/* Encoding in action_result_display_t.selector. */
enum {
    BATTLE_NUMERIC_DISPLAY_SELECTOR_MASK = 0x1ff,
    BATTLE_NUMERIC_DISPLAY_MODIFIER_CRITICAL = 0x200,
};

/* Numeric values correspond to the first sixteen action display flags. */
enum {
    ACTION_DISPLAY_VALUE_COUNT = 16,
    ACTION_DISPLAY_NUMERIC_FLAGS_MASK = 0x0000ffff,
};

/* Image IDs used by the projected-action display. These index the 49-entry
 * texture-source table at 0x8014cf68; they are not battle status IDs. */
typedef enum action_display_image_id {
    ACTION_DISPLAY_IMAGE_DEAD = 0,
    ACTION_DISPLAY_IMAGE_UNDEAD = 1,
    ACTION_DISPLAY_IMAGE_PETRIFY = 2,
    ACTION_DISPLAY_IMAGE_INVITE = 3,
    ACTION_DISPLAY_IMAGE_DARKNESS = 4,
    ACTION_DISPLAY_IMAGE_CONFUSION = 5,
    ACTION_DISPLAY_IMAGE_SILENCE = 6,
    ACTION_DISPLAY_IMAGE_BLOOD_SUCK = 7,
    ACTION_DISPLAY_IMAGE_OIL = 8,
    ACTION_DISPLAY_IMAGE_FLOAT = 9,
    ACTION_DISPLAY_IMAGE_RERAISE = 10,
    ACTION_DISPLAY_IMAGE_TRANSPARENT = 11,
    ACTION_DISPLAY_IMAGE_BERSERK = 12,
    ACTION_DISPLAY_IMAGE_POISON = 13,
    ACTION_DISPLAY_IMAGE_REGEN = 14,
    ACTION_DISPLAY_IMAGE_PROTECT = 15,
    ACTION_DISPLAY_IMAGE_SHELL = 16,
    ACTION_DISPLAY_IMAGE_HASTE = 17,
    ACTION_DISPLAY_IMAGE_SLOW = 18,
    ACTION_DISPLAY_IMAGE_STOP = 19,
    ACTION_DISPLAY_IMAGE_FAITH = 20,
    ACTION_DISPLAY_IMAGE_INNOCENT = 21,
    ACTION_DISPLAY_IMAGE_CHARM = 22,
    ACTION_DISPLAY_IMAGE_SLEEP = 23,
    ACTION_DISPLAY_IMAGE_DONT_MOVE = 24,
    ACTION_DISPLAY_IMAGE_DONT_ACT = 25,
    ACTION_DISPLAY_IMAGE_REFLECT = 26,
    ACTION_DISPLAY_IMAGE_DEATH_SENTENCE = 27,
    ACTION_DISPLAY_IMAGE_STOLEN = 28,
    ACTION_DISPLAY_IMAGE_BROKEN = 29,
    ACTION_DISPLAY_IMAGE_QUICK = 36,
    ACTION_DISPLAY_IMAGE_HP = 37,
    ACTION_DISPLAY_IMAGE_MP = 38,
    ACTION_DISPLAY_IMAGE_CT = 39,
    ACTION_DISPLAY_IMAGE_SPEED = 40,
    ACTION_DISPLAY_IMAGE_BRAVE = 41,
    ACTION_DISPLAY_IMAGE_FAITH_STAT = 42,
    ACTION_DISPLAY_IMAGE_PHYSICAL_ATTACK = 43,
    ACTION_DISPLAY_IMAGE_MAGICAL_ATTACK = 44,
    ACTION_DISPLAY_IMAGE_LEVEL = 45,
    ACTION_DISPLAY_IMAGE_GIL = 46,
    ACTION_DISPLAY_IMAGE_EXP = 47,
    ACTION_DISPLAY_IMAGE_FROG = 48,
    ACTION_DISPLAY_IMAGE_NONE = 255,
} action_display_image_id_e;

enum { ACTION_DISPLAY_IMAGE_COUNT = ACTION_DISPLAY_IMAGE_FROG + 1 };

typedef struct action_display_image_rect {
    u8 u;
    u8 v;
    u8 width;
    u8 height;
} action_display_image_rect_t;
typedef char action_display_image_rect_size_must_be_4[(sizeof(action_display_image_rect_t) == 4) ? 1 : -1];

/* Ordinary SP/PA/MA/Brave/Faith action-result encoding. CT and EXP reuse
 * these bits with distinct sentinel or sign meanings and are excluded. */
enum {
    ACTION_STAT_CHANGE_VALUE_MASK = 0x7f,
    ACTION_STAT_CHANGE_INCREASE = 0x80,
};

/* Bit order mirrors the seven serialized battle_stats_t equipment slots. */
typedef enum action_equipment_slot_mask {
    ACTION_EQUIPMENT_SLOT_HEAD = 0x80,
    ACTION_EQUIPMENT_SLOT_BODY = 0x40,
    ACTION_EQUIPMENT_SLOT_ACCESSORY = 0x20,
    ACTION_EQUIPMENT_SLOT_RIGHT_WEAPON = 0x10,
    ACTION_EQUIPMENT_SLOT_RIGHT_SHIELD = 0x08,
    ACTION_EQUIPMENT_SLOT_LEFT_WEAPON = 0x04,
    ACTION_EQUIPMENT_SLOT_LEFT_SHIELD = 0x02,
    /* Every slot bit: stored by the Steal/Break slot picker before it chooses one. */
    ACTION_EQUIPMENT_SLOT_ALL = 0xfe,
} action_equipment_slot_mask_e;

/* Five-byte range/targeting work entry at 0x80192dd8, 256 entries per level.
 * Movement-cost propagation uses remaining_range as the residual budget;
 * the AI reads it at 0x801995f4 and 0x801996ac. */
typedef struct target_panel {
    u8 remaining_range;
    /* Targeting frontier/targeted mark; pathing stores the step number here. */
    u8 mark;
    /* Index 0..15 of the compact movement record on the tile (0x80174e84),
     * not a battle id. */
    u8 unit_record_index;
    /* Remaining range after landing on a rideable unit (0x80175fa4); cleared on
     * the acting tile by 0x80178dac; pathing reuses it on 512+ panels as the step number. */
    u8 ride_remaining_range;
    /* Largest single height step along the path, the equal-budget tie-breaker
     * (0x80175958, 0x80177794); 0xff initial value in mode 1. AI propagation
     * sets the target to 0 and old actor tile to 0xff. */
    u8 max_height_delta;
} target_panel_t;
typedef char battle_targeting_panel_size_must_be_5[(sizeof(target_panel_t) == 5) ? 1 : -1];

/* Current ability data at 0x801938c0: the attacker/target pair, weapons, formula
 * inputs and results of the strike being resolved, with copies of the ability's
 * secondary data, the weapon's data and the status-infliction record that
 * action_run_pre_formula_setup loads. */
typedef struct battle_current_ability {
    u8 attacker_id;                             /* 0x00 */
    u8 target_id;                               /* 0x01 */
    u8 strike_count;                            /* 0x02 */
    u8 strike_counter;                          /* 0x03: weapon hand, 0 = right, 1 = left */
    u8 primary_weapon_id;                       /* 0x04 */
    u8 secondary_weapon_id;                     /* 0x05 */
    u16 reaction_id;                            /* 0x06 */
    u16 weapon_proc_pending;                    /* 0x08; a weapon proc spell is still to strike */
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
    u8 _unused_1d;                              /* 0x1d */
    u8 accessory_evade;                         /* 0x1e */
    u8 right_hand_evade;                        /* 0x1f; shield, or weapon with Weapon Guard */
    u8 left_hand_evade;                         /* 0x20 */
    u8 class_evade;                             /* 0x21 */
    u8 facing_modifier;                         /* 0x22: 0 front, 1 side, 2 back */
    u8 _unused_23;                              /* 0x23 */
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

/* The compatibility table is indexed by absolute zodiac-ID difference, not
 * by a particular sign. Value 3 is resolved from sex and monster flags. */
typedef enum zodiac_compatibility {
    ZODIAC_COMPATIBILITY_NEUTRAL = 0,
    ZODIAC_COMPATIBILITY_BAD = 1,
    ZODIAC_COMPATIBILITY_GOOD = 2,
    ZODIAC_COMPATIBILITY_GENDER_DEPENDENT = 3,
    ZODIAC_COMPATIBILITY_WORST = 4,
    ZODIAC_COMPATIBILITY_BEST = 5,
} zodiac_compatibility_e;

/* battle_current_ability_t.knockback_flags: the kind of knockback found by
 * formula_calculate_knockback, with PENDING set until it is applied. */
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
 * the misc unit record: the message the post-action state opens. */
typedef enum battle_post_action_message {
    BATTLE_POST_ACTION_MESSAGE_NONE = 0,
    BATTLE_POST_ACTION_MESSAGE_POACHED = 6,
    BATTLE_POST_ACTION_MESSAGE_TRAINED = 9,
} battle_post_action_message_e;

/* Current attacker record at 0x8019389c, filled by
 * action_set_current_attacker_data and read by
 * action_set_mimic_ability. The AI saves and restores it whole as 30
 * bytes (ai_store_considered_action_data). */
typedef struct battle_current_attacker_data {
    u8 facing;               /* 0x00 */
    u8 mimic_pending;        /* 0x01: a non-Mime action was recorded; state 13 queues the Mimes */
    u8 right_hand_weapon_id; /* 0x02 */
    u8 left_hand_weapon_id;  /* 0x03 */
    u8 team;                 /* 0x04 */
    u8 _padding_05;          /* 0x05; aligns target_delta_x */
    s16 target_delta_x;      /* 0x06 */
    s16 target_delta_y;      /* 0x08 */
    action_command_t action; /* 0x0a: copy of battle_stats_t +0x16e */
} battle_current_attacker_data_t;
typedef char battle_current_attacker_data_size_must_be_30[(sizeof(battle_current_attacker_data_t) == 30) ? 1 : -1];

/* Active reaction abilities occupy four MSB-first bytes at
 * battle_stats_t + 0x08b. These values are verified against the target's
 * reaction dispatchers. */
/* Reaction behavior flags (g_main_reaction_behavior_flags). */
typedef enum battle_reaction_behavior_flag {
    BATTLE_REACTION_BEHAVIOR_FLAG_TRIGGER_ABILITY = 0x01,
    BATTLE_REACTION_BEHAVIOR_FLAG_ABILITY = 0x02,
    BATTLE_REACTION_BEHAVIOR_FLAG_TARGET_SELF = 0x04,
    BATTLE_REACTION_BEHAVIOR_FLAG_TARGET_ATTACKER = 0x08,
    BATTLE_REACTION_BEHAVIOR_FLAG_PASSIVE = 0x10,
    BATTLE_REACTION_BEHAVIOR_FLAG_ACTIVE = 0x20,
} battle_reaction_behavior_flag_e;

/* action_calculate_at_list modes. */
typedef enum battle_at_list_mode {
    BATTLE_AT_LIST_MODE_ALL = 0,
    BATTLE_AT_LIST_MODE_TURNS_ONLY = 1,   /* leave out the charged and performing actions */
    BATTLE_AT_LIST_MODE_ACTIONS_ONLY = 2, /* leave out the active unit and the simulated turns */
    BATTLE_AT_LIST_MODE_PREVIEW = 3,      /* as ALL, with the acting unit's new charge counted from this tick */
} battle_at_list_mode_e;

/* Result of battle_unit_get_chocobo_mount_state. */
typedef enum battle_chocobo_mount_state {
    BATTLE_CHOCOBO_MOUNT_NONE = 0, /* not a chocobo, or one in an unmountable status */
    BATTLE_CHOCOBO_MOUNT_FREE = 1,
    BATTLE_CHOCOBO_MOUNT_PAIRED = 2, /* already carries a rider */
} battle_chocobo_mount_state_e;

/* battle_unit_learn_from_crystal flags. */
enum {
    BATTLE_CRYSTAL_LEARN_ABILITIES = 0x1,
    BATTLE_CRYSTAL_RESTORE_HP_MP = 0x2,
    BATTLE_CRYSTAL_TAKE_TREASURE = 0x4, /* add the treasure's item instead */
};

/* Results of battle_unit_find_slot_by_character_identity that are not a battle_stats slot. */
typedef enum battle_character_slot {
    BATTLE_CHARACTER_SLOT_SELECTOR = -1, /* the identity is a selector value, not a character */
    BATTLE_CHARACTER_SLOT_OFF_FIELD = -2,
    BATTLE_CHARACTER_SLOT_UNAVAILABLE = -3, /* a crystal or a treasure, or a disabled unit */
    BATTLE_CHARACTER_SLOT_IN_PARTY = -4,    /* only in the party roster */
    BATTLE_CHARACTER_SLOT_ABSENT = -5,
} battle_character_slot_e;

/* Directions of target_calculate_linear_attack_tiles. */
typedef enum target_line_direction {
    TARGET_LINE_EAST = 0,  /* +x */
    TARGET_LINE_WEST = 2,  /* -x */
    TARGET_LINE_SOUTH = 4, /* -y */
    TARGET_LINE_NORTH = 6, /* +y */
} target_line_direction_e;

/* Result of action_prepare_attack, kept in battle_unit_misc_data_t.attack_phase_state. */
typedef enum battle_attack_preparation {
    BATTLE_ATTACK_PREPARATION_INVALID = -1,
    BATTLE_ATTACK_PREPARATION_CHARGE = 0, /* the action has a charge time; also Defend */
    BATTLE_ATTACK_PREPARATION_IMMEDIATE = 1,
    BATTLE_ATTACK_PREPARATION_CHANGE_EQUIPMENT = 2,
    BATTLE_ATTACK_PREPARATION_JUMP = 3,
} battle_attack_preparation_e;

/* Result of target_set_panels_for_action, kept in
 * battle_unit_misc_data_t.ability_preview_phase: how the action's target is chosen. */
typedef enum target_panels {
    TARGET_PANELS_INVALID = -1,
    TARGET_PANELS_UNIT_OR_TILE = 0, /* the ability can follow a unit */
    TARGET_PANELS_TILE_ONLY = 1,    /* ABILITY_SECONDARY_FLAG_2_NO_FOLLOW_TARGET */
    TARGET_PANELS_NO_TARGETING = 2, /* nothing to choose: the action is centred on the acting unit */
    TARGET_PANELS_NONE = 3,         /* the panel routine marked no tile */
} target_panels_e;

/* target_set_tile_background_color modes and the g_target_tile_mark_modes states:
 * one layer per map tile mark (move destination, ability range, targeted). A SHOW or HIDE
 * request recolours the current render buffer; map_draw_mesh_and_weather then does the
 * other buffer and leaves the layer SHOWN or NONE. */
typedef enum battle_tile_mark_mode {
    BATTLE_TILE_MARK_NONE = 0, /* as a mode: restore every polygon and request the three HIDEs */
    BATTLE_TILE_MARK_SHOW_MOVE = 1,
    BATTLE_TILE_MARK_SHOW_RANGE = 2,
    BATTLE_TILE_MARK_HIDE_RANGE = 3,
    BATTLE_TILE_MARK_HIDE_MOVE = 5,
    BATTLE_TILE_MARK_SHOW_TARGETS = 7,
    BATTLE_TILE_MARK_HIDE_TARGETS = 8,
    BATTLE_TILE_MARK_MOVE_SHOWN = 10,
    BATTLE_TILE_MARK_RANGE_SHOWN = 11,
    BATTLE_TILE_MARK_TARGETS_SHOWN = 12,
} battle_tile_mark_mode_e;

/* g_action_post_action_display_phase: the reports shown one after another once an action
 * has resolved. The post-movement reports use the first two. */
typedef enum battle_post_action_display_phase {
    BATTLE_POST_ACTION_DISPLAY_LEVEL_UP = 0,
    BATTLE_POST_ACTION_DISPLAY_JOB_LEVEL_UP = 1,
    BATTLE_POST_ACTION_DISPLAY_LEARN_ON_HIT = 2,
} battle_post_action_display_phase_e;

/* Provisional: the 12-entry zodiac compatibility table indexed by the absolute
 * difference of the two signs.  1/4 weaken the formula, 2/5 strengthen it, and
 * 3 marks the opposition pair whose result depends on sex. */
extern u8 g_battle_zodiac_compatibility_modifiers[];

/* Golem barrier HP per team, shared by action resolution and AI snapshots. */
extern u16 g_battle_team_golem[BATTLE_TEAM_COUNT];
extern s32 g_action_turn_event_type;
extern s32 g_action_at_list_active;
extern s32 g_action_at_list_id;
extern action_display_image_rect_t g_action_display_image_rects[ACTION_DISPLAY_IMAGE_COUNT];
extern const u8 g_action_display_numeric_selectors[ACTION_DISPLAY_VALUE_COUNT];
extern u16 g_action_display_values[ACTION_DISPLAY_VALUE_COUNT];

/* Eight per status_changes mask byte, indexed from the byte's high bit. */
extern u8 g_action_status_display_image_ids[STATUS_BYTE_COUNT][8];
extern const palette_t g_action_result_palette;
extern s32 g_action_phase;
extern s32 g_action_post_action;
extern s32 g_action_post_action_display_phase; /* battle_post_action_display_phase_e */
extern s32 g_action_post_effect_msg_counter;
extern action_data_t* g_action_attacker_data;
extern action_state_e g_action_state;
extern action_data_t* g_action_target_data;
extern action_context_e g_action_context;
extern s16 g_battle_current_reaction_ability_id;
extern s32 g_battle_distribute_target_count;
extern s32 g_battle_unit_source_misc_id;
extern s32 g_battle_casting_unit_id;
extern u8 g_target_ability_targets_list[BATTLE_UNIT_MISC_SLOT_COUNT];
extern target_panel_t g_target_panel_data[MAP_TILE_COUNT];
extern battle_current_ability_t g_action_current_ability;
extern s32 g_action_hamedo_flag;
extern s32 g_battle_state_wait_direction_facing;
extern action_command_t g_action_saved_command;
extern battle_stats_t* g_action_target;
extern battle_post_effect_msg_t g_action_post_effect_msgs[];
extern u8 g_action_menu_skillsets[];

/* Separate link name for g_action_saved_command.ability_id. */
extern s16 g_battle_reaction_unit_last_ability_id;
extern battle_current_attacker_data_t g_action_current_attacker;
extern battle_current_attacker_data_t g_ai_current_attacker_backup;
extern action_data_t g_action_attacker_action_data;
extern battle_stats_t* g_action_attacker;
extern u8 g_action_menu_row_types[]; /* per-row skillset kind; 13 keeps the row active */
extern s32 g_action_learn_on_hit_unit_id;
extern void (*g_formula_handlers[])(void); /* by formula id less 1: formula 0 has no handler */
extern u16 g_target_color_tile_x;
extern s16 g_target_color_tile_y;
extern s16 g_target_tile_mark_modes[3]; /* battle_tile_mark_mode_e, one per tile mark */
extern u8 g_action_canceled_statuses[];

/* Second name of g_target_panel_data, unsized: 512 map panels followed
 * by 16 auxiliary panels, all initialized at 0x80174e84. */
extern target_panel_t g_target_panels[];
extern SVECTOR g_battle_current_misc_screen_coords;
extern VECTOR g_battle_current_vector;
extern u32* g_battle_gfx_current_otag_entry;
extern battle_render_buffer_t* g_target_tile_color_buffer;
extern u16 g_battle_current_music_track_index; /* 0x80165fd8 */
s32 action_add_poached_item_to_fur_shop_inventory(void);
s32 action_calculate_at_list(battle_at_entry_t* list, s32 mode);
u32 action_get_number_of_turns_to_resolve(s32 unit_index, s32 ct, battle_at_entry_t* entries);
s32 action_calculate_at_list_and_get_specific_unit_id(s32 unit_idx);
s32 action_calculate_chance_to_react(const battle_stats_t* unit);
void action_call_attack_preparation(action_command_t* action);
void action_store_used_weapon(action_command_t* action);
s32 action_can_unit_react(const battle_stats_t* unit);

/* Provisional: the definition falls off the end after the tail call to
 * battle_unit_get_action_restriction, so v0 carries that result; every caller reads
 * it as an s32 status. */
s32 action_is_reaction_blocked(battle_stats_t* unit);
s32 action_check_and_consume_mp(battle_stats_t* unit);
void action_check_arrow_guard_usability(void);
void action_check_between_turn_events(void);
void action_check_blade_grasp_usability(void);
void action_check_counter_reaction_usability(u16 reaction_id, u32 mask);
s32 action_check_change_of_turn(s32 unit_id);
void action_check_face_up_and_absorb_used_mp_usability(s16 reaction_id);
void action_check_critical_quick_hp_restore_mp_restore_meatbone_slash_usability(s16 reaction_id);
void action_check_damage_split_usability(void);
void action_check_distribute_usability(void);
void action_check_mp_switch_usability(void);
void action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(s16 reaction_id);
s32 action_check_reaction(battle_stats_t* unit);
void action_check_reflect_reaction(void);
void action_choose_facing_for_wait(void);
void action_choose_wait(void);
void action_clear_at_list_active(void);
void action_clear_at_list_id(void);
void action_open_status_menu_at_list_start(void);
void action_clear_current_data(action_data_t* action);
void action_clear_data(void);
void action_clear_knockback_flag(void);
void action_clear_status_changes(action_data_t* action);
void action_confirm(void);
void battle_menu_copy_billboard_data_in(const menu_unit_status_record_t* banner, const menu_active_unit_t* unit,
    const menu_status_panel_slot_storage_t* billboard);
s32 action_end_turn(s32 unit_id);
void action_execute_ability(void);
void action_finalize_target_current_action(void);
void action_get_next_acting_unit(s32 arg);
void action_handle_move_command(void);
void action_handle_post_action_xp_jp_ability(void);
void action_increment_at_list_id(void);
s32 action_increment_item_quantity_for_steal_break(battle_stats_t* unit, u8 item);
void action_init_learn_ability_on_hit(void);
s32 action_init_movement_ability_benefit(battle_stats_t* unit);
void action_is_at_list_unit_charging_ability(void);
s32 action_report_job_level_up(battle_unit_misc_data_t* misc);
s32 action_report_level_up(battle_unit_misc_data_t* misc);
s32 action_resume_attack_phase_control(void);
void action_run_between_turn_events(void);
void action_set_at_list_active(void);
void action_set_casting_unit_id_ff(void);
void action_set_current_attacker_data(battle_stats_t* unit);
void action_set_damage_display_type_based_on_ability(void);
void action_set_item_throw_stone_ability_display(void);
s32 action_set_move_act_flags(s32 unit_id, s32 move_flag, s32 act_flag);
void action_set_only_action_taken(s32 unit_id);
void action_set_only_movement_taken(s32 unit_id);
void action_set_target_variables(battle_stats_t* unit);
s32 action_should_end_unit_turn(battle_stats_t* unit);
void battle_state_enter_move_range_exception(void);
void action_show_caster_post_effect_messages(void);
void action_switch_ability_to_default_attack(void);
void formula_3c_damage_caster_max_hp_one_fifth_heal_target_two_fifths(void);
void formula_43_damage_caster_missing_hp(void);
void formula_add_brave_x_stats_y(void);
void formula_apply_attack_up_and_martial_arts(void);
void formula_apply_attacker_berserk_frog(void);
void formula_apply_catch(void);
void formula_apply_charge(void);
void formula_apply_damage_and_knockback(void);
void formula_apply_dance_abilities(void);
void formula_apply_defense_up(void);
void formula_apply_elemental(void);
void formula_apply_elemental_absorption(void);
s32 formula_apply_elemental_absorption_and_status(void);

/* Provisional: the definition falls off the end after
 * formula_roll_conditional_status_proc, so v0 carries that result and
 * every caller tests it. */
s32 formula_apply_elemental_absorption_and_status_proc(void);
void formula_apply_elemental_strengthen(void);
void formula_apply_evasion_changes_due_to_statuses(void);
void formula_apply_finger_guard(void);
void formula_apply_full_hp_mp_heal(void);
void formula_apply_hp_absorption(void);
void formula_apply_magic_attack_up(void);
void formula_apply_magic_defense_up(void);
void formula_apply_magical_support_status_compatibility(void);
void formula_apply_maintenance(void);
void formula_apply_mp_recovery(void);
void formula_apply_physical_attack_supports(void);
void formula_apply_physical_status_support_compatibility(void);
void formula_apply_physical_xa_modifying_statuses_support(void);
void formula_apply_quick_effect(void);
void formula_apply_song_abilities(void);
void formula_apply_status(void);
void formula_apply_status_to_action(void);
s32 formula_apply_status_and_check_undead(void);
s32 formula_apply_steal_break_might_sword_hardcoding(void);
void formula_apply_target_magical_status_xa_modifiers(void);
void formula_apply_target_xa_affecting_statuses(void);
void formula_apply_talk_skill(void);
void formula_apply_undead_absorb_attack(void);
void formula_apply_undead_reversal(void);
void formula_apply_weapon_element_strengthen(void);
void formula_apply_weather_effects_on_bows(void);
void formula_apply_weather_elemental_effects(void);
void formula_set_brave_loss_y(void);
void formula_calculate_abandon(void);
s32 formula_calculate_attack_evaded(u16 numerator, u16 denominator, u8 miss_type, u8 item_lost);
void formula_calculate_base_xa(void);
void formula_calculate_concentrate(void);
void formula_calculate_critical_hit(void);
void formula_calculate_dark_confuse(void);
void formula_calculate_facing_evade(void);
void formula_calculate_faith(void);
void formula_calculate_final_hit_percent(void);
void formula_calculate_gravi2_damage(void);
void formula_calculate_hp_percent_damage(void);
void formula_calculate_katana_break_chance(void);
s32 formula_calculate_magic_accuracy_without_faith(void);
s32 formula_calculate_magical_evade(void);
void formula_calculate_magical_xa_times_ya(void);
void formula_calculate_mp_percent_damage(void);
void formula_calculate_physical_damage(void);
s32 formula_calculate_physical_evade(void);
void formula_calculate_stolen_gil(void);
void formula_calculate_transparent(void);
void formula_calculate_truth_damage(void);
void formula_calculate_truth_magical_damage(void);
s32 battle_unit_get_action_restriction(battle_stats_t* unit);
s32 formula_calculate_hit(void);

/* Provisional: falls off the end after formula_calculate_hit, whose
 * result stays in v0 for the caller's test. */
s32 formula_calculate_physical_evade_charge(void);
void formula_modify_damage_for_element(s32 element);
void formula_nullify_action(void);
void formula_cause_action_miss(void);
void formula_check_dragon(void);
void formula_clear_nullify_flags(void);
void formula_convert_hp_damage_to_mp_recovery(void);
void formula_determine_reduced_stat(void);
void formula_force_attack_miss(void);
void formula_force_sleeping_target_miss(void);
s32 formula_get_random_0_7fff(void);
void formula_heal_mp_z_times_ten(void);
void formula_init_weapon_xa_ya(void);
void formula_modify_elemental_damage(void);
void formula_roll_conditional_status_proc_inner(void);
s32 formula_roll_conditional_status_proc(void);
s32 formula_roll_random_at_least(s32 limit, s32 threshold);
void formula_select_magic_gun_ability(void);
void formula_set_exp_stolen(void);
void formula_store_ma_and_ma_plus_y_divided_by_two(void);
void formula_store_ma_and_pa_plus_y_divided_by_two(void);
void formula_store_ma_and_x(void);
void formula_store_ma_and_y(void);
void formula_store_magical_evade_values(void);
void formula_store_pa_and_pa_plus_y_divided_by_two(void);
void formula_store_pa_and_weapon_power(void);
void formula_store_pa_and_weapon_power_plus_y(void);
void formula_store_pa_and_x(void);
void formula_store_pa_and_y(void);
void formula_store_reaction_proc_id_and_target(void);
void formula_store_speed_and_x(void);
void formula_store_xa_plus_ya_status_damage(void);
void formula_store_xa_times_ya_damage(void);
void formula_use_hp_damage_as_action_hit_percent(void);
s32 target_set_panels_for_action(const action_command_t* action);
s32 action_build_reaction_targets(s32 actor_id, battle_strike_work_t* work, s32* target_count, u8* target_ids);
void target_apply_unit_team_eligibility(s32 raw_unit_id, u8 allow_allies, u8 allow_enemies, u8 aoe_is_0xff);
void target_calculate_aoe_vertical_tolerance(s32 x, s32 y, s32 lo, s32 hi);
void target_calculate_arc_range(battle_stats_t* unit, u8 range, u8 flags);
void target_calculate_linear_attack_tiles(s32 dir, s32 x, s32 y);
s32 target_calculate_map_for_action(action_command_t* action, u8* flags_3);
void target_calculate_strike_lunge_range(battle_stats_t* unit, u8 flags);
void target_calculate_tile_coords_and_glow_from_at_list(void);
void target_calculate_tile_coords_with_cursor_glow(void);
void target_calculate_weapon_range(battle_stats_t* unit);
void target_can_select_tile(void);
void target_clear_panels_on_untargetable_tiles(void);
s32 target_clear_selection_state_of_all_panels(void);
void target_clear_panel_data(void);
void target_clear_panel_spread_flags(void);
void target_disable_green_panel_flags(void);
s32 target_disable_green_panel_on_all_but_target_tile(const action_command_t* source);
void target_gather_x_y_data_for_attacks(battle_unit_misc_data_t* unit);
s32 target_get_unit_id_if_tile_targetable(s32 a0, s32 a1, s32 a2);
void target_move_cursor_to_unit(battle_unit_misc_data_t* unit);
void target_remove_close_range(s32 x, s32 y, s32 range);
void target_select_random_tile_for_random_fire_abilities(void);
void target_select_tile(void);
void target_show_move_range_in_free_cursor(void);
void target_sort_list(s32 target_battle_id);
void target_spread_panels(u8 passes, s32 unused);
void target_store_cursor_casting_unit_name_and_data(void);
void target_store_cursor_unit_name_and_data(void);
s32 target_set_boxes_red(void);
void target_store_cursor_unit_as_preview_target(void);
void target_update_free_cursor_selection(battle_unit_misc_data_t* unit, battle_unit_misc_data_t* selected_unit);

s32 target_validate_height_overlap(s32 attacker_id, s32 target_x, s32 target_y, s32 target_elevation, s32 target_id);

void action_queue_post_effect_messages_for_unit(battle_unit_misc_data_t* unit);
s32 target_calculate_cursor_tile_polygon(SVECTOR* coords, u8 layer, SVECTOR* quad);
s32 target_set_green_panels_for_action(action_command_t* action);
s32 target_is_unit_untargetable_and_store_tile(s32 unit_id, s32* tile_index);
s32 action_prepare_attack(action_command_t* action, action_command_t* target, s32 flag);
s32 action_resolve_ability_strike(s32 misc_unit_id, battle_strike_work_t* work);
void action_init_current_ability_strike_data(battle_stats_t* unit);
s32 battle_reaction_prepare_next(s16* reaction_id);
void formula_calculate_knockback(void);
void formula_apply_zodiac_compatibility(void);
void formula_store_physical_evade_values(void);
s32 action_run_pre_formula_setup(const action_command_t* source, u8 target_id);
s32 action_finalize_attack_and_flag_reactions(s32 misc_id);
s32 target_move_cursor_by_input(void);
void target_set_tile_background_color(s32 mode, s32 unused);
void action_set_target_coords_and_attacker_anim(void);
void battle_menu_copy_billboard_data_out(menu_unit_status_record_t* banner, menu_active_unit_t* unit,
    menu_status_panel_slot_storage_t* billboard, map_selected_tile_t* cursor_tile);
s32 action_add_war_funds(battle_stats_t* unit, s32 total, u8 item_id);
s32 action_apply_tile_trap(battle_stats_t* unit);
void action_build_targets_post_action_message(void);

/* Preview action calculation entry run on the main stack. */
s32 action_calculate_projected_effect(battle_stats_t* attacker, battle_stats_t* target, action_command_t* command);

s32 action_call_attack_preparation_at_preview(action_command_t* command);
s32 action_check_at_list_for_unit_battle_id(battle_stats_t* unit);
s32 action_check_battle_outcome(void);
void action_check_mp_switch_distribute_and_damage_split_usability(void);
void action_check_reflect_blade_grasp_and_arrow_guard(void);
void action_check_stat_save_and_restore_reaction_usability(void);
s32 action_decrement_player_item_quantity(battle_stats_t* unit, s32 item_id, s32 always_one);
void action_dispatch_target_reaction_ability(void);
s32 action_get_elemental_ability_id(battle_stats_t* unit);
void action_finalize_draw_out_katana_result(battle_stats_t* attacker, battle_strike_work_t* work, u8 hit_count);
void action_handle_steal_exp(battle_stats_t* unit, u8 amount);
s32 action_perform_reaction_ability(void);
s32 action_preview_at_list(battle_stats_t* unit, action_command_t* action, battle_at_entry_t* at_list);
s32 action_remove_broken_or_stolen_equipment(void);
void action_apply_jp_change(battle_stats_t* unit);
s32 action_select_auto_potion_item(battle_stats_t* unit);
void action_set_casting_unit_id_ff_and_init(void);
s32 action_set_mimic_ability(battle_stats_t* unit);
s32 action_sort_at_list(s32 unit_id, s32 mode, s32 sort_key, battle_at_entry_t* list);
void action_store_target_ids(u8* src);
s32 action_store_counter_ability(battle_stats_t* unit, s8 skillset_id, s16 ability_id, s32 validate_target);
void formula_apply_poach_and_train(void);
s32 formula_calculate_dance_song_hit(void);
s32 formula_calculate_elemental_xa_times_ya(void);
s32 formula_calculate_friendly_magic_accuracy(void);
s32 formula_calculate_golem_accuracy(void);
s32 formula_calculate_magic_accuracy(void);
s32 formula_calculate_magic_accuracy_no_elemental(void);
s32 formula_calculate_physical_accuracy(void);
s32 formula_calculate_physical_status_accuracy(void);
s32 formula_calculate_weapon_damage(void);
s32 battle_reaction_prepare_hamedo_for_pending_action(s32 id, s16* out_ability);
s32 target_apply_reflect(battle_stats_t* unit);
void target_apply_vertical_fixed(s32 x, s32 y);
void target_apply_vertical_tolerance(u8 ref_height, u8 tolerance, s32 single_layer);
s32 target_calculate_ability_range_with_map_parameters(battle_stats_t* unit, u8 range);
s32 target_calculate_for_menu_types(const action_command_t* source);
s32 target_count_hit_by_ability(u8* out);
s32 target_list_units_on_panels(u8* list, battle_stats_t* origin);
s32 target_move_cursor_to_battle_id(u32 battle_id);
void target_project_cursor_tile_to_screen(VECTOR* projected, SVECTOR* position, SVECTOR* raised);
s32 target_run_calculator(const action_command_t* source);
s32 target_set_ability_panels(const action_command_t* source);
void target_set_state_for_all_unit_panels(battle_stats_t* unit, s32 flags);
s32 target_set_all_panels_targeted_if_targetable(void);
void target_set_coordinates_for_ability(battle_stats_t* acting, battle_strike_work_t* out);
s32 target_set_weapon_attack_panels(action_command_t* source);
s32 target_set_jump_ability_panels(const action_command_t* source);
s32 target_validate_lunging_target(s32 unit_id, s32 x, s32 y, s32 elevation, s32 target_id);
s32 target_validate_weapon_target(const action_command_t* source);
void target_build_directional_attack_panels(const action_command_t* source, u8 mode);
void target_check_moldball_virus_depth(s16 ability_id);
s32 action_init_current_data(s32 unit_id);
void action_store_acting_unit_data(battle_stats_t* unit);
s32 action_store_target_stats_pointer_data(s32 unit_id);

#endif
