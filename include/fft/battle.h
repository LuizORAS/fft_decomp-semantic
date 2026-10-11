#ifndef BATTLE_H
#define BATTLE_H

/* BATTLE.BIN: the battle engine. Event and effect overlays run alongside it. */

#include "fft/main.h"
#include "psx/libc.h"

/* ability */
typedef enum battle_ability_facing_modifier {
    BATTLE_ABILITY_FACING_FRONT = 0,
    BATTLE_ABILITY_FACING_SIDE = 1,
    BATTLE_ABILITY_FACING_BACK = 2,
} battle_ability_facing_modifier_e;

typedef struct battle_ability_animation_data {
    u8 charge_animation_set_id; /* row of g_battle_ability_charge_animation_sets */
    u8 attack_animation;        /* battle_unit_animation_e, or BATTLE_ATTACK_ANIMATION_USE_WEAPON or _USE_ITEM */
    u8 text_display; /* low byte of the message text id (0x1800 | this) shown when the ability hits; 0 none */
} battle_ability_animation_data_t;
typedef char battle_ability_animation_data_size_must_be_3[(sizeof(battle_ability_animation_data_t) == 3) ? 1 : -1];

enum {
    BATTLE_ABILITY_ANIMATION_COUNT = 0x1c6,
    BATTLE_CHARGE_ANIMATION_SET_COUNT = 20,
    BATTLE_CHARGE_ANIMATION_VARIANT_COUNT = 2,
};

/* Selectors in attack_animation; other values are battle_unit_animation_e IDs. */
enum {
    BATTLE_ATTACK_ANIMATION_USE_WEAPON = 0,
    BATTLE_ATTACK_ANIMATION_USE_ITEM = 1,
};

extern battle_ability_animation_data_t g_battle_ability_animation_data[BATTLE_ABILITY_ANIMATION_COUNT];

/* Two battle_unit_animation_e for each set: the one that starts a charge and
 * the one held while the unit charges or performs; 0 for none. */
extern u8 g_battle_ability_charge_animation_sets[BATTLE_CHARGE_ANIMATION_SET_COUNT]
                                                [BATTLE_CHARGE_ANIMATION_VARIANT_COUNT];

extern s16 g_battle_ability_effect_id;
extern s32 g_battle_spell_quote_last_ability_id;
extern u8 g_battle_loaded_ability_flags_1;

/* Ability-list window layout at 0x80166994. Column modes: 0 text, 1 number,
 * 2 hidden, 3 alternate number. The columns read the work buffer's u16 text
 * ids and values through the layout's s16 pointers. */
extern world_menu_text_layout_t g_battle_ability_menu_layout;
extern u8 g_battle_spell_quote_exception_skillsets[];
s32 battle_ability_find_learn_on_hit_unit(s32 attacker_id, u16* ability_id);

/* ai */
typedef enum battle_ai_command_kind {
    BATTLE_AI_COMMAND_MOVE = 0,
    BATTLE_AI_COMMAND_ACT = 1,
    BATTLE_AI_COMMAND_END_TURN = 2,
    /* The rest reach the command from battle_menu_resolve_selection, for a player's menu choice.
     * The auto-battle choices are 0x0c plus twice the option, plus one once a unit has been
     * picked for the two that name one (SELECT picks it, DISPLAY shows it); SELECT and the two
     * others equal their battle_ai_autobattle_mode_e values. */
    BATTLE_AI_COMMAND_NO_MENU_RECORD = 0x07, /* the current menu id has no record: back to the turn events */
    BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_SELECT = 0x0c,
    BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_DISPLAY = 0x0d,
    BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_SELECT = 0x0e,
    BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_DISPLAY = 0x0f,
    BATTLE_AI_COMMAND_AUTOBATTLE_FADING_LIFE = 0x10,
    BATTLE_AI_COMMAND_AUTOBATTLE_RETREAT = 0x11,
    BATTLE_AI_COMMAND_REFRESH_CONTROL = 0x12, /* copy the control flag back from the stats */
    BATTLE_AI_COMMAND_MOVE_SELF = 0x19,       /* MOVE that never redirects to the unit's mount */
    BATTLE_AI_COMMAND_UNRESOLVED_FE = 0xfe,
    BATTLE_AI_COMMAND_UNRESOLVED_FF = 0xff,
} battle_ai_command_kind_e;

typedef enum battle_ai_simulation_phase {
    BATTLE_AI_SIMULATION_PREPARE_ACTION = 0,
    BATTLE_AI_SIMULATION_ADVANCE_TURNS = 1,
    BATTLE_AI_SIMULATION_SCORE_STATE = 2,
} battle_ai_simulation_phase_e;

/* The four AI flag bytes of an ability, one enum per byte. */
enum {
    BATTLE_AI_ABILITY_FLAG_1_TARGET_ALLIES = 0x01,
    BATTLE_AI_ABILITY_FLAG_1_TARGET_ENEMIES = 0x02,
    BATTLE_AI_ABILITY_FLAG_1_AFFECTS_HP = 0x80,
};

enum {
    BATTLE_AI_ABILITY_FLAG_2_EVADEABLE = 0x02,
    BATTLE_AI_ABILITY_FLAG_2_UNDEAD_REVERSE = 0x20,
    BATTLE_AI_ABILITY_FLAG_2_TARGET_MAP_TILES = 0x80,
};

enum {
    BATTLE_AI_ABILITY_FLAG_3_PHYSICAL_ATTACK = 0x01,
    BATTLE_AI_ABILITY_FLAG_3_MAGICAL_ATTACK = 0x02,
    /* Four area-shape bits, tested only together: 0x20 a line from the user
     * (BATTLE_AI_ABILITY_LINEAR_TRAJECTORY in the word view), 0x10 the eight
     * breath and bracelet cones, 0x08 Triple Attack alone, 0x04 Triple Bracelet
     * alone. With any of them set the AI tries every origin tile. */
    BATTLE_AI_ABILITY_FLAG_3_EXHAUSTIVE_ORIGIN_MASK = 0x3c,
    BATTLE_AI_ABILITY_FLAG_3_DIRECT_TRAJECTORY = 0x40,
    BATTLE_AI_ABILITY_FLAG_3_ARC_TRAJECTORY = 0x80,
};

enum {
    BATTLE_AI_ABILITY_FLAG_4_EVADE_WITH_MOTION = 0x01, /* the static table's bit; the AI working copy reuses byte 4 */
    BATTLE_AI_ABILITY_FLAG_4_USABLE_BY_AI = 0x80,
};

/* Word flags of the considered ability. Bytes 1..3 retain the static table
 * meanings, but byte 4 is replaced by battle_ai_ability_entry_flag_e: its
 * low bit means CONSUME_EVALUATED_ORIGINS, not the static EVADE_WITH_MOTION.
 * Team preferences remain distinct from the absolute ONLY restrictions. */
enum {
    BATTLE_AI_ABILITY_TARGET_ALLIES = 0x00000001,
    BATTLE_AI_ABILITY_TARGET_ENEMIES = 0x00000002,
    BATTLE_AI_ABILITY_UNEQUIP = 0x00000004,
    BATTLE_AI_ABILITY_AFFECTS_STATS = 0x00000008,
    BATTLE_AI_ABILITY_ADD_STATUS = 0x00000010,
    BATTLE_AI_ABILITY_CANCEL_STATUS = 0x00000020,
    BATTLE_AI_ABILITY_AFFECTS_MP = 0x00000040,
    BATTLE_AI_ABILITY_AFFECTS_HP = 0x00000080,
    BATTLE_AI_ABILITY_AFFECTED_BY_SILENCE = 0x00000100,
    BATTLE_AI_ABILITY_EVADEABLE = 0x00000200,
    BATTLE_AI_ABILITY_AFFECTED_BY_FAITH = 0x00000400,
    BATTLE_AI_ABILITY_RANDOM_USE = 0x00000800,
    BATTLE_AI_ABILITY_FOLLOW_TARGET = 0x00001000,
    BATTLE_AI_ABILITY_UNDEAD_REVERSE = 0x00002000,
    BATTLE_AI_ABILITY_REFLECTABLE = 0x00004000,
    BATTLE_AI_ABILITY_TARGET_MAP_TILES = 0x00008000,
    BATTLE_AI_ABILITY_PHYSICAL_ATTACK = 0x00010000,
    BATTLE_AI_ABILITY_MAGICAL_ATTACK = 0x00020000,
    BATTLE_AI_ABILITY_LINEAR_TRAJECTORY = 0x00200000,
    BATTLE_AI_ABILITY_DIRECT_TRAJECTORY = 0x00400000,
    BATTLE_AI_ABILITY_ARC_TRAJECTORY = 0x00800000,
    BATTLE_AI_ABILITY_CONSUME_EVALUATED_ORIGINS = 0x01000000,
    BATTLE_AI_ABILITY_EXHAUSTIVE_ORIGIN_SCAN = 0x04000000,
    BATTLE_AI_ABILITY_REQUIRES_MONSTER_SKILL = 0x08000000,
    BATTLE_AI_ABILITY_ENEMIES_ONLY = 0x20000000,
    BATTLE_AI_ABILITY_ALLIES_ONLY = 0x40000000,
    BATTLE_AI_ABILITY_USABLE_BY_AI = 0x80000000,
};

/* The setup dispatcher writes this at Misc +0x158 and copies 24 bytes into
 * each history slot. ACT copies the 20-byte payload from AI action +0x08;
 * MOVE writes only x/elevation/y, and END_TURN leaves the payload unchanged. */
typedef struct battle_ai_command {
    battle_ai_command_kind_e kind;
    union {
        unit_tile_coordinates_t move;
        battle_action_command_t action;
    } data;
} battle_ai_command_t;
typedef char battle_ai_command_size_must_be_24[sizeof(battle_ai_command_t) == 24 ? 1 : -1];

/*
 * AI data block, 0x19c8 bytes at 0x8019f3c4 (g_battle_ai_data_base).
 * Unidentified ranges stay explicit padding.  Sites spell fields as members,
 * directly or through a local `ai = &g_battle_ai_data_base`: a direct member
 * address is related to the other member addresses of its block (one shared
 * base register), a pointer assigned first is not.
 */
/* X / map elevation / Y / zero, written in that order by transfer_unit_coordinates_to_ai
 * and read as one word for equality tests. */
typedef union battle_ai_coords {
    u32 word;
    struct {
        u8 x;
        u8 elevation;
        u8 y;
        u8 zero;
    } bytes;
} battle_ai_coords_t;

/* Ability AI behaviour flags 1..4. Consumers read both the full word and its
 * constituent bytes; use the corresponding masks above for each view. */
typedef union battle_ai_ability_flags {
    u32 word;
    struct {
        u8 flags_1;
        u8 flags_2;
        u8 flags_3;
        u8 flags_4;
    } bytes;
} battle_ai_ability_flags_t;

/* Shared prefix of the baseline and suspended AI unit snapshots.
 * Byte 2 holds Death Sentence CT only in the compact suspended record;
 * the baseline uses status_ct[15] and leaves this byte untouched. */
typedef struct battle_ai_status_snapshot {
    u8 entd_slot;
    u8 death_counter;
    u8 death_sentence_ct; /* see 0x8019e214 */
    u8 current_status[5];
} battle_ai_status_snapshot_t;
typedef char battle_ai_status_snapshot_must_be_8[sizeof(battle_ai_status_snapshot_t) == 8 ? 1 : -1];

/* Selected unit state saved at 0x8019e160 and restored at 0x8019e378.
 * HP occupies unaligned bytes 0x1b..0x1c; max HP/MP are not saved. */
typedef struct battle_ai_extended_snapshot {
    battle_ai_status_snapshot_t status; /* 0x00 */
    u8 status_ct[16];                   /* 0x08 */
    u8 team_flags;                      /* 0x18 */
    u8 faith;                           /* 0x19 */
    u8 transparent_removal_flag;        /* 0x1a */
    u8 hp_bytes[2];                     /* 0x1b */
    u8 _padding_1d;                     /* aligns mp */
    u16 mp;                             /* 0x1e */
    u8 charged_ability_ct;              /* 0x20 */
    u8 base_attributes[3];              /* 0x21; unit_attribute_index_e */
    u8 attributes[3];                   /* 0x24; unit_attribute_index_e */
    u8 ct;                              /* 0x27 */
    u8 x;                               /* 0x28 */
    u8 position_bytes[2];               /* 0x29; packed unit position */
    u8 has_turn;                        /* 0x2b */
    u8 movement_taken;
    u8 action_taken;
    u8 auto_battle_setting; /* 0x2e */
    u8 auto_battle_target;
    u8 initial_team_flags;
    u8 inflicted_status[5]; /* 0x31 */
    u8 equipment[7];        /* 0x36 */
    u8 brave;               /* 0x3d */
    u8 mount_info;
    u8 _padding_3f; /* tail padding to 2-byte alignment */
} battle_ai_extended_snapshot_t;
typedef char battle_ai_extended_snapshot_must_be_64[sizeof(battle_ai_extended_snapshot_t) == 64 ? 1 : -1];
typedef char battle_ai_snapshot_hp_must_be_0x1b[((unsigned long)&((battle_ai_extended_snapshot_t*)0)->hp_bytes == 0x1b)
        ? 1
        : -1];
typedef char battle_ai_snapshot_equipment_must_be_0x36
    [((unsigned long)&((battle_ai_extended_snapshot_t*)0)->equipment == 0x36) ? 1 : -1];

/* AI ability +0x04..0x0b: byte parameters and an aligned status-tail word.
 * 0x8019e9f4 loads status bytes 1..4 together; reading only one byte would
 * silently lose the high status bits tested by the decision evaluator. */
typedef union battle_ai_ability_parameters {
    struct {
        u8 range;
        u8 aoe;
        u8 item_id;
        u8 status_infliction[5];
    } bytes;
    struct {
        u32 _first;
        u32 status_tail;
    } words;
} battle_ai_ability_parameters_t;

/* Pack canonical status IDs 8..39 into the aligned word covering status
 * bytes 1..4 of battle_ai_ability_parameters_t. */
#define BATTLE_AI_STATUS_TAIL_PACKED_MASK(id)                                                                          \
    ((u32)BATTLE_STATUS_BYTE_MASK(id) << ((BATTLE_STATUS_BYTE_INDEX(id) - 1) * 8))

/* Considered ability, base+0x00. */
typedef struct battle_ai_considered_ability {
    u8 skillset;                               /* 0x00 */
    u8 ct;                                     /* 0x01 */
    s16 ability_id;                            /* 0x02; read with both lh and lhu */
    battle_ai_ability_parameters_t parameters; /* 0x04 */
    battle_ai_ability_flags_t ai_flags;        /* 0x0c */
    u8 element;                                /* 0x10 */
    u8 mp_cost;                                /* 0x11 */
    u8 _padding_12[2];                         /* tail padding to 4-byte alignment */
} battle_ai_considered_ability_t;
typedef char battle_ai_considered_ability_size_must_be_0x14[(sizeof(battle_ai_considered_ability_t) == 0x14) ? 1 : -1];
typedef char battle_ai_status_tail_must_be_at_offset_8
    [((unsigned long)&((battle_ai_considered_ability_t*)0)->parameters.words.status_tail == 8) ? 1 : -1];

/* battle_ai_action_rank_t.decided of the selected action once it is chosen. */
enum {
    BATTLE_AI_ACTION_DECIDED = 0xff,
};

/* How an action follows its target, in the AI/menu command and stored unit command. */
typedef enum battle_targeting_type {
    BATTLE_TARGETING_TYPE_TILE = 5, /* aims at target_x, target_elevation, target_y */
    BATTLE_TARGETING_TYPE_UNIT = 6, /* follows the unit target_id */
} battle_targeting_type_e;

/* The ranking tail of an action, compared by
 * battle_ai_compare_target_priority_and_hit_rate. */
typedef struct battle_ai_action_rank {
    /* BATTLE_AI_ACTION_DECIDED on the selected action once the turn's action is
     * chosen, 0 on every candidate; the first comparison key at 0x80196db0. */
    u8 decided;
    u8 base_hit_percent;
    s16 priority;
} battle_ai_action_rank_t;
typedef char battle_ai_action_rank_size_must_be_4[(sizeof(battle_ai_action_rank_t) == 4) ? 1 : -1];

/*
 * One considered/recorded action, 0x20 bytes.  The current action lives at
 * base+0x14, the best so far at +0x34, eight ranked outcomes at +0x54, the
 * inverted-priority scratch at +0x154 and the selected action at +0x17fc.
 */
typedef struct battle_ai_action_data {
    u8 act_before_move;  /* 0x00; 1: act from the current tile, then move; 0: move to coords first */
    u8 reflected_action; /* 0x01; set while evaluating reflected-origin candidates */
    u8 _unused_02;
    u8 wait_facing_hint;             /* 0x03; direction 0..3, 4 keeps current, 5 chooses during Wait */
    battle_ai_coords_t coords;       /* 0x04 */
    battle_action_command_t command; /* 0x08; copied as the ACT payload */
    battle_ai_action_rank_t rank;    /* 0x1c */
} battle_ai_action_data_t;
typedef char battle_ai_action_data_size_must_be_0x20[(sizeof(battle_ai_action_data_t) == 0x20) ? 1 : -1];
typedef char battle_ai_action_command_must_be_at_offset_8[((unsigned long)&((battle_ai_action_data_t*)0)->command == 8)
        ? 1
        : -1];

/* unit_t AI ability list entry, 4 bytes, 0x22 per unit. */
typedef struct battle_ai_ability_entry {
    s16 packed_id; /* bits 0..9: ability ID; bits 10..15: physical unit slot */
    /* The AI updates the flag bits with lhu/sh over both bytes, so the
     * halfword view covers skillset and usage_flags together. */
    union {
        u16 packed; /* skillset | usage_flags << 8 */
        struct {
            u8 skillset;    /* 0x02; 0xff terminates the list (chose_move_from_move_list) */
            u8 usage_flags; /* 0x03; battle_ai_ability_entry_flag_e */
        } bytes;
    } skillset_flags;
} battle_ai_ability_entry_t;
typedef char battle_ai_ability_entry_must_be_4[sizeof(battle_ai_ability_entry_t) == 4 ? 1 : -1];

/* OR-combined categories gathered from both equipped weapon formulas. They
 * are flags, not ability_formula_e IDs; mixed categories bypass the builder
 * switch's single-category cases. */
enum {
    BATTLE_AI_WEAPON_FORMULA_NONE = 0, /* no weapon in either hand */
    BATTLE_AI_WEAPON_FORMULA_DAMAGE = 1,
    BATTLE_AI_WEAPON_FORMULA_ABSORB_HP = 2,
    BATTLE_AI_WEAPON_FORMULA_HEAL = 4,
};

/* Per-compact-unit weapon summary. The builder at 0x8019a2f0 clears two
 * words, then merges weapon properties through the byte view. */
typedef union battle_ai_weapon_data {
    u32 words[2];
    struct {
        u8 range;
        u8 flags_1;
        u8 flags_2;
        u8 flags_3;
        u8 weapon_id;
        u8 element;
        u8 _unused_06[2];
    } bytes;
} battle_ai_weapon_data_t;
typedef char battle_ai_weapon_data_must_be_8[sizeof(battle_ai_weapon_data_t) == 8 ? 1 : -1];

/* Per-unit AI behavior flags copied from ENTD data to decision +0x04. The
 * three transient status bits are rebuilt during action simulation. */
typedef enum battle_ai_decision_flags {
    BATTLE_AI_DECISION_TRANSIENT_DEAD = 0x01,
    BATTLE_AI_DECISION_TRANSIENT_PETRIFIED = 0x02,
    BATTLE_AI_DECISION_TRANSIENT_JUMP = 0x04,
    BATTLE_AI_DECISION_SPECIAL_BEHAVIOR = 0x08,
    BATTLE_AI_DECISION_AGGRESSIVE = 0x10,
    BATTLE_AI_DECISION_STAY_NEAR_COORDINATES = 0x20,
    BATTLE_AI_DECISION_FOCUS_TARGET = 0x40,
    /* Worth acting on: every non-enemy and every enemy with an action charging;
     * the refresh simulation adds each enemy that ends it alive (or dead with
     * Reraise), unpetrified and without Death Sentence. Target searches skip a
     * unit without it unless the unit is confused. The copy from the ENTD
     * record leaves the stay-near tile's level here until the first refresh. */
    BATTLE_AI_DECISION_CONSIDERED = 0x80,
} battle_ai_decision_flags_e;

/* Targeting policy copied from ENTD AI Flags 2 to decision +0x07. The low
 * policy bits survive battle_ai_refresh_unit_decision_flags; the remaining
 * target-state bits are rebuilt there before action simulation. */
typedef enum battle_ai_targeting_flags_2 {
    BATTLE_AI_TARGET_HP_BELOW_HALF = 0x01,
    BATTLE_AI_TARGET_MP_CONSTRAINED = 0x02,
    BATTLE_AI_TARGET_CONSERVE_CT = 0x04,
    BATTLE_AI_TARGET_TEAM_SUPPORT_UNAVAILABLE = 0x08,
    BATTLE_AI_TARGET_UNTARGETABLE = 0x10,
    BATTLE_AI_TARGET_DEAD_WITHOUT_RERAISE = 0x20,
    BATTLE_AI_TARGET_DEAD_WITH_RERAISE = 0x40,
    BATTLE_AI_TARGET_HP_BELOW_THREE_QUARTERS = 0x80,
} battle_ai_targeting_flags_2_e;

typedef enum battle_ai_target_setting_flags {
    BATTLE_AI_TARGET_SETTING_REFRESH_UNIT_STATUS_FLAGS = 0x01000000,
    BATTLE_AI_TARGET_SETTING_CONSUME_EVALUATED_ORIGINS = 0x02000000,
    BATTLE_AI_TARGET_SETTING_HAS_HOLY_WATER = 0x20000000, /* the acting unit can use the Holy Water item ability */
    BATTLE_AI_TARGET_SETTING_RETREAT_SUPPORT_AVAILABLE = 0x40000000,
} battle_ai_target_setting_flags_e;

/*
 * Per-unit AI decision record, 0x10 bytes at base+0x182c (unit*16 + 0x182c).
 * acting_unit_decision points at the acting unit's record.
 */
typedef struct battle_ai_unit_decision {
    battle_ai_coords_t target; /* 0x00..0x03; x, level, y, 0 */
    u8 flags;                  /* 0x04; battle_ai_decision_flags_e */
    u8 main_target_id;         /* 0x05 */
    u8 targeting_flags_1; /* 0x06; 0x08 selected reflected action; 0x04 helpful-ability ratio >= 77/128, cleared for
                             the lowest-ratio nonenemy (0x8019537c..0x80195408) */
    u8 targeting_flags_2; /* 0x07; battle_ai_targeting_flags_2_e */
    u8 enemy_flag;        /* 0x08 */
    u8 highest_mp_cost;   /* 0x09 */
    u8 lowest_mp_cost;    /* 0x0a */
    u8 mp_ability_mod;    /* 0x0b */
    u8 silence_mod;       /* 0x0c */
    u8 evade_mod;         /* 0x0d */
    u8 enemy_ability_min_reach; /* 0x0e; least range + AoE of the enemy-targeting abilities, plus Move */
    u8 ally_ability_min_reach;  /* 0x0f; least range + AoE of the ally-targeting abilities, plus Move */
} battle_ai_unit_decision_t;
typedef char battle_ai_unit_decision_size_must_be_0x10[(sizeof(battle_ai_unit_decision_t) == 0x10) ? 1 : -1];

/* AI +0x0c78..0x0cb7. The target's 21-byte save at +0xca2 overlaps
 * the low three bytes of target_setting_flags at +0xcb4. Loops at
 * 0x8019b184..0x8019b1b8 and 0x8019b260..0x8019b274 establish the span.
 * The CONSUME_EVALUATED_ORIGINS flag lies in the fourth byte, outside the saved array.
 * Preserve both views and word alignment; this is not a 16-byte backup. */
typedef union battle_ai_targetability {
    struct {
        u8 unit_active[BATTLE_UNIT_SLOT_COUNT];
        u8 unit_targetable[BATTLE_UNIT_SLOT_COUNT];
        u8 saved_prefix[16];
        u8 _padding_3a[2]; /* aligns target_setting_flags */
        u32 target_setting_flags;
    } live;
    struct {
        u8 _unused_00[42];
        u8 unit_targetable_saved[BATTLE_UNIT_SLOT_COUNT];
        u8 _unused_3f;
    } snapshot;
} battle_ai_targetability_t;
typedef char battle_ai_targetability_size_must_be_64[(sizeof(battle_ai_targetability_t) == 64) ? 1 : -1];

typedef struct battle_ai_data {
    battle_ai_considered_ability_t considered_ability; /* 0x0000 */
    battle_ai_action_data_t current_action;            /* 0x0014 */
    battle_ai_action_data_t best_action;               /* 0x0034 */
    battle_ai_action_data_t ranked_actions[8];         /* 0x0054 */
    battle_ai_action_data_t inverted_priority_action;  /* 0x0154 */
    u8 tile_foe_proximity[2][18][16];                  /* 0x0174; level/y/x, strides
                                                        * 288/16/1 at 0x80196e68..0x80196ebc */
    u8 tile_target_distance[2][18][16];                /* 0x03b4; level/y/x, strides 288/16/1 */
    s16 tile_priority[2][18][16];                      /* 0x05f4; level/y/x, byte strides 576/32/2 */
    /* Scenario/level/y; row bits 15..0 represent x=0..15. */
    u16 reachable_tiles[3][2][18];  /* 0x0a74; byte strides 72/36/2 */
    u16 targetable_tiles[2][18];    /* 0x0b4c */
    u16 movable_tiles[2][18];       /* 0x0b94 */
    u16 attack_origin_tiles[2][18]; /* 0x0bdc */
    u16 walkable_tiles[2][18];      /* 0x0c24 */
    u8 _unused_0c6c;
    u8 initial_targeting_state; /* 0x0c6d */
    u8 action_selection_phase;  /* 0x0c6e; 0x801971b8 selects scorer/distance resume; 0x80197ff4 uses 0..4 */
    u8 _unused_0c6f;
    u8 outcome_evaluation_state;               /* 0x0c70 */
    u8 movement_decision_state;                /* 0x0c71 */
    u8 search_resume_flag;                     /* 0x0c72; distance-search initialization/resume flag
                                                * at 0x801994f8; reused as a phase flag at 0x8019cd9c. */
    u8 _padding_0c73;                          /* aligns considered_unit_coords */
    battle_ai_coords_t considered_unit_coords; /* 0x0c74 */
    battle_ai_targetability_t targetability;   /* 0x0c78 */
    /* Included target/strike pairs, not unique units; 0xff disables counting. */
    u16 total_hit_percent;       /* 0x0cb8 */
    u8 hit_counter;              /* 0x0cba */
    u8 attack_origin_tile_count; /* 0x0cbb */
    u8 movable_tile_count;       /* 0x0cbc */
    u8 ability_counter;          /* 0x0cbd */
    u8 has_mp_recovery_ability;  /* 0x0cbe; a usable ability affects MP and does not target enemies */
    u8 crystal_treasure_status;  /* 0x0cbf; 1 crystal, 2 treasure */
    u8 _unused_0cc0[4];
    battle_ai_coords_t candidate_coords[3]; /* 0x0cc4; indexed by movement_scenario */
    u8 _unused_0cd0[4];
    battle_action_command_t unit_action_records[16]; /* 0x0cd4 */
    u16 throw_ability_id;                            /* 0x0e14 */
    u8 throw_weapon_id;                              /* 0x0e16 */
    /* All slots with entd_slot != 0xff, capped at 255 by 0x80199d20. */
    u8 present_unit_average_max_hp;                  /* 0x0e17 */
    u8 unit_acts_before_me[BATTLE_UNIT_SLOT_COUNT];  /* 0x0e18..0x0e2c; all slots cleared at 0x8019d408–0x8019d41c */
    u8 movement_scenario;                            /* 0x0e2d; 0..2 */
    u8 acting_unit_id;                               /* 0x0e2e */
    u8 acting_unit_compact_index;                    /* 0x0e2f; unit_compact_indices[acting_unit_id] */
    battle_ai_coords_t acting_unit_coords;           /* 0x0e30 */
    battle_ai_unit_decision_t* acting_unit_decision; /* 0x0e34 */
    u8 acting_unit_move;                             /* 0x0e38 */
    u8 acting_unit_team;                             /* 0x0e39 */
    u8 map_max_x;                                    /* 0x0e3a */
    u8 map_max_y;                                    /* 0x0e3b */
    u16 team_golem[4];                               /* 0x0e3c; halfword scoring at 0x8019d37c */
    u8 status_to_cancel[5]; /* 0x0e44; usable actor abilities' aggregate cancellation mask, built at 0x80195410 */
    /* Resumable reflected-origin search, 0x8019b7b8–0x8019bb20.
     * Candidate x/y = 2 * reflector position - intended target position. */
    u8 reflector_unit_index;                             /* 0x0e49 */
    u8 reflected_target_unit_index;                      /* 0x0e4a */
    u8 reflected_candidate_level;                        /* 0x0e4b */
    u8 ability_effect_on_self;                           /* 0x0e4c; 0 none, 1 usable, 2 adverse */
    u8 ranked_origin_scan;                               /* 0x0e4d; consume the ranked origin tiles */
    u8 useful_on_caster_flag;                            /* 0x0e4e */
    u8 acting_unit_remaining_clockticks;                 /* 0x0e4f */
    battle_ai_status_snapshot_t unit_status_records[16]; /* 0x0e50 */
    u8 saved_ability_ct;                                 /* 0x0ed0 */
    u8 saved_current_status;                             /* 0x0ed1 */
    u8 saved_inflicted_status;                           /* 0x0ed2 */
    u8 _unused_0ed3;
    u8 main_ai_state;             /* 0x0ed4 */
    u8 autobattle_state;          /* 0x0ed5 */
    u8 charging_state;            /* 0x0ed6 */
    u8 find_highest_target_state; /* 0x0ed7; check_if_map_allows_use_and_find_highest_target's progress variable */
    u8 reflected_origin_phase;    /* 0x0ed8; cleared at 0x8019ba40, set at 0x8019ba78 */
    u8 highest_priority_state;    /* 0x0ed9 */
    u8 max_possibilities;         /* 0x0eda; byte-truncated combination count, then scan x at 0x8019bf2c */
    u8 y_counter;                 /* 0x0edb */
    u8 map_level_counter;         /* 0x0edc */
    /* Calculator search counters survive suspension at 0x8019bbbc–0x8019bf2c. */
    u8 math_ability_id;       /* 0x0edd; low byte of the selected ability entry */
    u8 math_type_index;       /* 0x0ede; 0..3 */
    u8 math_multiplier_index; /* 0x0edf; 4..7 */
    /* Turn-controller globals saved/restored by 0x8019d37c–0x8019db80. */
    s32 saved_between_turn_state;                      /* 0x0ee0; g_battle_between_turn_state during the simulation */
    s32 saved_between_turn_resume_state;               /* 0x0ee4; g_battle_between_turn_resume_state */
    s32 saved_turn_unit_id;                            /* 0x0ee8; g_battle_turn_unit_id */
    u16 simulated_turn_events;                         /* 0x0eec; interturn loop stops at 256 */
    u8 action_simulation_phase;                        /* 0x0eee; battle_ai_simulation_phase_e */
    u8 _padding_0eef;                                  /* aligns ability_lists */
    battle_ai_ability_entry_t ability_lists[16][0x22]; /* 0x0ef0; 0x88 bytes per unit */
    /* Blood Suck/Frog IDs start without unit bits; 0x8019729c/0x80197510
     * OR the acting physical slot into bits 10..15 before loading the entry. */
    battle_ai_ability_entry_t hardcoded_status_abilities[2]; /* 0x1770 */
    battle_ai_weapon_data_t unit_weapon_data[16];            /* 0x1778 */
    battle_stats_t* acting_unit;                             /* 0x17f8 */
    battle_ai_action_data_t selected_action;                 /* 0x17fc */
    /* 0x8019aea0 compares/copies coordinate words; 0x80194da4 consumes
     * x/elevation/y to clear reachable tiles. The list's broader policy is unknown. */
    battle_ai_coords_t known_trap_coords[4];                          /* 0x181c; x == 0xff terminates the list */
    battle_ai_unit_decision_t unit_decisions[BATTLE_UNIT_SLOT_COUNT]; /* 0x182c */
    /* 0x80195ed4 saves the complete decision before retreat; 0x80195efc
     * restores it only after the child finishes, not when it suspends. */
    battle_ai_unit_decision_t saved_acting_unit_decision; /* 0x197c */
    u8 unit_compact_indices[BATTLE_UNIT_SLOT_COUNT];      /* 0x198c; slot -> index among present units (0..15) */
    u8 unit_autobattle_modes[BATTLE_UNIT_SLOT_COUNT];     /* 0x19a1; battle_ai_autobattle_mode_e from ENTD AI flags */
    u8 autobattle_setting;                                /* 0x19b6 */
    u8 resume_pending;                                    /* 0x19b7; an AI routine suspended on its frame budget */
    u8 random_use_threshold;                              /* 0x19b8; 0x80195778..0x80195788 stores
                                                           * floor(128 * random-use entries / eligible entries).
                                                           * A zero denominator leaves the prior byte unchanged. */
    u8 water_penalty;                                     /* 0x19b9 */
    u8 valuable_target_hit;                               /* 0x19ba; a counted hit on unit_targetable sets this
                                                           * at 0x8019e010..0x8019e020. */
    u8 single_enemy_remaining;                            /* 0x19bb; exactly one enemy counted */
    u8 ability_processing_done;                           /* 0x19bc */
    u8 action_taken;                                      /* 0x19bd */
    u8 movement_taken;                                    /* 0x19be */
    u8 _unused_19bf[8];
    u8 _padding_19c7; /* 0x19c7; last documented byte; tail padding to 4-byte alignment */
} battle_ai_data_t;
typedef char battle_ai_data_size_must_be_0x19c8[(sizeof(battle_ai_data_t) == 0x19c8) ? 1 : -1];
typedef char battle_ai_saved_targetability_must_be_0xca2
    [((unsigned long)&((battle_ai_data_t*)0)->targetability.snapshot.unit_targetable_saved == 0xca2) ? 1 : -1];
typedef char battle_ai_target_flags_must_be_0xcb4
    [((unsigned long)&((battle_ai_data_t*)0)->targetability.live.target_setting_flags == 0xcb4) ? 1 : -1];

/* Runtime policy byte in battle_ai_ability_entry_t. The low three bits control
 * how the AI searches and consumes candidate action origins; initialization
 * recomputes them while preserving the remaining bits. */
typedef enum battle_ai_ability_entry_flag {
    BATTLE_AI_ABILITY_ENTRY_CONSUME_EVALUATED_ORIGINS = 0x01,
    BATTLE_AI_ABILITY_ENTRY_FORCE_CONSUME_EVALUATED_ORIGINS = 0x02,
    BATTLE_AI_ABILITY_ENTRY_EXHAUSTIVE_ORIGIN_SCAN = 0x04,
    BATTLE_AI_ABILITY_ENTRY_REQUIRES_MONSTER_SKILL = 0x08,
    BATTLE_AI_ABILITY_ENTRY_ENEMIES_ONLY = 0x20,
    BATTLE_AI_ABILITY_ENTRY_ALLIES_ONLY = 0x40,
    BATTLE_AI_ABILITY_ENTRY_USABLE = 0x80,
    BATTLE_AI_ABILITY_ENTRY_INIT_PRESERVE_MASK = 0xf8,
} battle_ai_ability_entry_flag_e;

typedef enum battle_ai_rank_order {
    BATTLE_AI_RANK_BELOW = 0,
    BATTLE_AI_RANK_EQUAL = 1,
    BATTLE_AI_RANK_ABOVE = 2,
} battle_ai_rank_order_e;

typedef enum battle_ai_unit_filter {
    BATTLE_AI_UNIT_FILTER_ANY = 0,
    BATTLE_AI_UNIT_FILTER_ENEMIES = 1,
    BATTLE_AI_UNIT_FILTER_ALLIES = 2,
    BATTLE_AI_UNIT_FILTER_SPECIFIC = 3,
    BATTLE_AI_UNIT_FILTER_STATUS = 4,
} battle_ai_unit_filter_e;

typedef enum battle_ai_nearest_target_mode {
    BATTLE_AI_NEAREST_WITHOUT_BLOOD_SUCK = 0,
    BATTLE_AI_NEAREST_IMMINENT_ENEMY = 1,
    BATTLE_AI_NEAREST_ENEMY = 2,
    BATTLE_AI_NEAREST_UNIT = 3,
    BATTLE_AI_NEAREST_ELIGIBLE_ENEMY = 4,
    BATTLE_AI_NEAREST_HEALER_OR_CRYSTAL = 5,
} battle_ai_nearest_target_mode_e;

typedef enum battle_ai_destination_order {
    BATTLE_AI_DESTINATION_PRIORITY_FIRST = 0,
    BATTLE_AI_DESTINATION_DISTANCE_FIRST = 1,
} battle_ai_destination_order_e;

typedef enum battle_ai_tile_mask_mode {
    BATTLE_AI_TILE_MASK_RANGE = 0,
    BATTLE_AI_TILE_MASK_AOE = 1,
    BATTLE_AI_TILE_MASK_RANGE_PLUS_AOE = 2,
    BATTLE_AI_TILE_MASK_EXPLICIT = 3,
} battle_ai_tile_mask_mode_e;

typedef enum battle_ai_command_build_stage {
    BATTLE_AI_COMMAND_BUILD_REFRESH = 0,
    BATTLE_AI_COMMAND_BUILD_MOVEMENT = 1,
    BATTLE_AI_COMMAND_BUILD_CHOOSE_ACTION = 2,
} battle_ai_command_build_stage_e;

typedef enum battle_ai_direction {
    BATTLE_AI_DIRECTION_SOUTH = 0,
    BATTLE_AI_DIRECTION_WEST = 1,
    BATTLE_AI_DIRECTION_NORTH = 2,
    BATTLE_AI_DIRECTION_EAST = 3,
    BATTLE_AI_DIRECTION_OVERLAP = 4,
    BATTLE_AI_DIRECTION_CHOOSE_AT_WAIT = 5,
} battle_ai_direction_e;

typedef enum battle_ai_targeting_flags_1 {
    BATTLE_AI_TARGETING_FLAG_1_HAS_RANGED_ABILITY = 0x01, /* a usable ability has a direct or arc trajectory */
    BATTLE_AI_TARGETING_FLAG_1_CAN_HEAL_ALLIES = 0x02,    /* a usable ability affects HP and targets allies */
    BATTLE_AI_TARGETING_FLAG_1_MOSTLY_SUPPORT = 0x04,     /* ally-targeting share of the usable abilities >= 77/128 */
    BATTLE_AI_TARGETING_FLAG_1_REFLECTED_ACTION = 0x08,
} battle_ai_targeting_flags_1_e;

/* Classification from the acting unit's side, not the sign of HP damage.
 * A useful effect may include deliberately hitting an ally to affect status. */
typedef enum battle_ai_ability_effect {
    BATTLE_AI_ABILITY_EFFECT_NONE = 0,
    BATTLE_AI_ABILITY_EFFECT_USEFUL = 1,
    BATTLE_AI_ABILITY_EFFECT_ADVERSE = 2,
} battle_ai_ability_effect_e;

/* Cardinal map-tile deltas stored as wrapping unsigned bytes. */
typedef struct battle_ai_facing_tile_offsets {
    u8 by_direction[4][2];
} battle_ai_facing_tile_offsets_t;

enum {
    BATTLE_AI_RECOVERY_STATUS_COUNT = 3,
};

extern s32 g_battle_ai_propagation_resume_pass;
extern u16 g_battle_ai_range_row_masks[];
extern s32 g_battle_ai_self_target_origin_stage;
extern battle_ai_command_t g_battle_ai_command_history[2]; /* 0x801a0d94, 0x801a0dac */
extern s32 g_battle_ai_cowardly_movement_step;

/* One shared AI workspace at 0x8019f3c4, not a separate record per candidate.
 * Preserve resume-state writes before returning -1. */
extern battle_ai_data_t g_battle_ai_data_base;
extern battle_ai_facing_tile_offsets_t g_battle_ai_facing_tile_offsets; /* 0x8019f358 */
extern s32 g_battle_ai_math_ability_index;
extern u8 g_battle_math_extra_known_bit_ids[10];
extern u16* g_battle_ai_math_ability_list; /* borrowed from main's temporary skillset list */
extern s32 g_battle_ai_math_skillset;
extern s32 g_battle_ai_math_skillset_index;
extern u8 g_battle_ai_recovery_status_ids[BATTLE_AI_RECOVERY_STATUS_COUNT];
extern s32 g_battle_ai_recovery_status_index; /* 0x8019f380; status-list cursor preserved for resume */
extern s32 g_battle_ai_recovery_status_retry; /* 0x8019f37c; skip status pass after one failed attempt */
extern s32 g_battle_ai_retreat_phase;         /* 0x8019f390; retreat child resume stage */

/* 0x8019f384–0x8019f38c; cancellation-search accounting survives suspension. */
extern s32 g_battle_ai_status_candidate_count;
extern s32 g_battle_ai_status_insufficient_mp;

/* Signed weights for status IDs 0..39, read by 0x8019d864–0x8019d988. */
extern s16 g_battle_ai_status_priority_weights[40]; /* 0x8019f308..0x8019f357 */
extern s32 g_battle_ai_status_sufficient_mp;

/* Shares the 0x80193924 scratchpad-save arena used during movement propagation. */
extern battle_ai_extended_snapshot_t g_battle_ai_unit_snapshot_storage[];
extern battle_ai_extended_snapshot_t* g_battle_ai_unit_snapshots; /* 0x8019f3c0 */
extern battle_ai_data_t* g_battle_ai_workspace;                   /* 0x8019f3ac */

/* BATTLE-resident state that the EVENT, EFFECT and WORLD overlays also read. */
extern world_event_work_t* g_battle_ai_workspace_ptr; /* pointer cell reloaded per subsystem */
s32 battle_ai_decide_status_ct_based(s32 limit, s32 unit_id);
s32 battle_ai_set_movement_panel_data(s32 movement_taken);
void battle_ai_set_ability_considerations(s32 action_taken);
void battle_ai_evaluate_linear_ability_behaviors(void);
s32 battle_ai_build_command(s32 unit_id, battle_ai_command_t* command);
void battle_ai_build_monster_skill_tile_mask(void);
s32 battle_ai_build_movement_scenarios(void);
void battle_ai_build_targetable_tile_mask(battle_ai_tile_mask_mode_e mode, s32 radius);
s32 battle_ai_build_targeted_movement(void);
void battle_ai_build_unit_ability_list(s32 unit_id);
s32 battle_ai_calculate_ability_range(void);
s32 battle_ai_calculate_clockticks_until_death_counter_expires();
s32 battle_ai_calculate_clockticks_until_unit_acts(battle_stats_t* unit);
s32 battle_ai_calculate_distance_between_units(battle_stats_t* a, battle_stats_t* b);
s32 battle_ai_calculate_ratio_times_4(s32 divisor, s32 dividend);
s32 battle_ai_calculate_height_difference_between_units(battle_ai_coords_t* coords, battle_stats_t* unit);
s32 battle_ai_call_ability_processing(battle_action_command_t* action);
s32 battle_ai_can_elemental_ability_be_used(battle_ai_coords_t* coords);
s32 battle_ai_should_exclude_from_imminent_enemies(s32 unit_id);
s32 battle_ai_is_unit_untargetable(s32 unit_id);
s32 battle_ai_check_ability_use_at_coords(battle_ai_coords_t* coordinates);
s32 battle_ai_check_ability_use_based_on_ct(s32 unit_index);
s32 battle_ai_check_facing_tile(battle_ai_coords_t* source, battle_ai_coords_t* target);
s32 battle_ai_can_cancel_current_status(battle_stats_t* unit, s32 status_bit);
s32 battle_ai_is_status_active_through_delay(s32 delay, battle_stats_t* unit, s32 status_id);
s32 battle_ai_check_map_allows_use_and_find_highest_target(void);
s32 battle_ai_check_set_highest_unit_priority(void);
s32 battle_ai_check_target_type(s32 unit_id);
void battle_ai_check_unit_for_crystal_or_treasure_status(void);
s32 battle_ai_check_unit_gets_turn_without_status(battle_stats_t* unit, s32 status);
s32 battle_ai_choose_move_from_move_list(void);
s32 battle_ai_choose_random_action(void);
s32 battle_ai_choose_status_action(void);
s32 battle_ai_choose_wait_facing(void);
battle_ai_ability_effect_e battle_ai_classify_ability_effect(s32 unit_id, s32 check_reflect);
void battle_ai_clear_current_action_rank_byte(void);

battle_ai_rank_order_e battle_ai_compare_target_priority_and_hit_rate(
    battle_ai_action_rank_t* a, battle_ai_action_rank_t* b);

s32 battle_ai_count_flagged_tiles_within_map_bounds(u16 rows[2][18]);
void battle_ai_disable_ability_if_only_inflicts_status(battle_ai_ability_entry_t* entry, s32 status_id);
s32 battle_ai_evaluate_ability_outcome(void);
s32 battle_ai_evaluate_charging_movement(void);
s32 battle_ai_evaluate_math_targets(void);
s32 battle_ai_evaluate_movement_origins(void);
s32 battle_ai_evaluate_reflected_target_origins(void);
s32 battle_ai_evaluate_self_target_origins(void);
s32 battle_ai_evaluate_status_cancellation(s32 unit_id, s32 status_id);
s32 battle_ai_evaluate_target_tiles(void);
void battle_ai_fill_foe_proximity_grid(void);
s32 battle_ai_fill_target_distance_grid(battle_ai_coords_t* target);
s32 battle_ai_filter_reachable_tiles_by_target_distance(s32 distance_limit);
battle_ai_direction_e battle_ai_find_direction_of_target(
    const battle_ai_coords_t* target, const battle_ai_coords_t* origin);
s32 battle_ai_find_nearest_target(battle_ai_nearest_target_mode_e mode);
s32 battle_ai_find_unit_at_coordinates(battle_ai_coords_t* coords);
s32 battle_ai_get_movement_range(void);
s32 battle_ai_handle_autobattle(void);
s32 battle_ai_has_any_unit_decided_to_use_ability(void);
s32 battle_ai_has_any_unit_on_target_panel(void);
void battle_ai_init_acting_unit_data(void);
void battle_ai_init_selected_action(void);
void battle_ai_init_target_consideration(void);
void battle_ai_init_unit_abilities(s32 unit_id);
void battle_ai_init_workspace(void);
void battle_ai_insert_ranked_action(void);
void battle_ai_invert_target_priority(void);
s32 battle_ai_is_action_higher_ranked(battle_ai_action_data_t* a, battle_ai_action_data_t* b);
s32 battle_ai_is_vsync_hblank_past_threshold(void);
void battle_ai_load_ability_entry(battle_ai_ability_entry_t* entry);
s32 battle_ai_load_known_ability_flag(s32 unit_id, s32 skillset_id, s32 bit);

s32 battle_ai_propagate_target_movement(
    s32 unit_id, s32 target_x, s32 target_y, s32 target_elevation, s32 initialize, s32* suspended, s32 check_budget);

void battle_ai_record_considered_coords(s32 x, s32 y, s32 level);
s32 battle_ai_refresh_unit_decision_flags(void);
void battle_ai_reset_action_ranking_fields(battle_ai_action_data_t* action);
void battle_ai_restore_considered_action_data(void);
void battle_ai_restore_unit_state(s32 mode);
s32 battle_ai_run_cowardly_movement(void);
s32 battle_ai_run_non_specific_autobattle(void);
void battle_ai_move_temp_unit_to_coords(battle_ai_coords_t* coordinates);
s32 battle_ai_select_fading_life_action(void);
void battle_ai_save_unit_state(s32 mode);
s32 battle_ai_score_reachable_tiles(void);
s32 battle_ai_select_active_units(battle_ai_unit_filter_e mode, s32 target_id);
void battle_ai_select_candidate_coords_and_check_crystal(void);
s32 battle_ai_select_destination(s32 max_distance, battle_ai_destination_order_e order);
void battle_ai_select_destination_with_nearest_fallback(s32 range);
void battle_ai_select_destination_with_range_floor(s32 range);
s32 battle_ai_select_initial_action(void);
s32 battle_ai_select_peril_target(void);
s32 battle_ai_select_ranked_action(void);
s32 battle_ai_select_recovery_action(void);
s32 battle_ai_select_retreat_action(void);
s32 battle_ai_select_targeted_action(void);
s32 battle_ai_simulate_action_and_score(void);
void battle_ai_set_candidate_and_actor_reachable_tiles(void);
s32 battle_ai_exclude_enemies_acting_before_ct(void);
void battle_ai_simulate_movement_and_pickup(void);
void battle_ai_simulate_strikes(battle_action_command_t* action, s32 reaction_mode);
void battle_ai_store_considered_action_data(void);
void battle_ai_store_main_target_id_and_focus_on_target_flag(s32 unit_id);
void battle_ai_store_weapon_attack_data(battle_ai_weapon_data_t* out, s32 item_id, s32* flags_out);
void battle_ai_take_next_movement_origin(void);
s32 battle_ai_take_random_tile(u16 tiles[2][18], battle_ai_coords_t* coordinates, s32 count);
void battle_ai_transfer_ability_data_and_set_defend_flag(void);
void battle_ai_transfer_byte_values(void* destination, const void* source, s32 byte_count);
void battle_ai_transfer_halfword_values(void* destination, void* source, s32 byte_count);
void battle_ai_transfer_unit_coordinates(s32 unit_index, battle_ai_coords_t* destination);
void battle_ai_update_team_targeting_flags(void);
s32 battle_unit_add_unique_entd_spritesheet(s32 mode, s16 value, s32 limit);
s32 battle_ai_add_usable_skillset_abilities(s32 unit_id, s32 skillset, s32 count);
void battle_ai_clear_words(void* destination, s32 byte_count);

/* animation */
/* SEQ sequence bytes, as battle_gfx_run_unit_seq_script (unit sheets) and
 * battle_gfx_run_wep_eff_seq_script (weapon and effect sheets) read them.
 * A sequence is a run of two-byte frame instructions (an SHP frame, then the
 * frames to wait; a wait of 0 holds the frame for good) and of commands:
 * BATTLE_SEQ_OP_PREFIX, an opcode and its operand bytes. A jump offset is a
 * signed byte counted from the command's own prefix byte, so -2 repeats the
 * frame instruction before it. UNKNOWN_XX commands only skip their operands. */
enum {
    BATTLE_SEQ_OP_PREFIX = 0xff,
};

typedef enum battle_seq_opcode {
    /* 0xbe-0xd2 and 0xd4 are unit commands: a weapon or effect sequence that uses one is an error. */
    BATTLE_SEQ_OP_STOP_POSITION_COPIES = 0xbe,
    BATTLE_SEQ_OP_START_POSITION_COPIES = 0xbf, /* the trail of battle_gfx_draw_unit_sprite_position_copies */
    BATTLE_SEQ_OP_WAIT_FOR_DISTORTION = 0xc0,   /* offset: jump while a distortion animation runs */
    BATTLE_SEQ_OP_START_DISTORTION = 0xc1,      /* distortion animation id - 2, timer */
    BATTLE_SEQ_OP_NOP = 0xc2,
    BATTLE_SEQ_OP_HIDE_ITEM = 0xc3,
    BATTLE_SEQ_OP_SET_ITEM_POSITION = 0xc4, /* x, y: signed offsets of the item sprite from the unit */
    BATTLE_SEQ_OP_SHOW_ITEM = 0xc5,         /* draw the unit's item display (battle_gfx_init_item_get_rendering) */
    BATTLE_SEQ_OP_WAIT_FOR_INPUT = 0xc6,    /* offset: jump while g_battle_action_post_action is 0 */
    BATTLE_SEQ_OP_UNKNOWN_C7 = 0xc7,        /* two operands */
    BATTLE_SEQ_OP_UNKNOWN_C8 = 0xc8,        /* two operands */
    BATTLE_SEQ_OP_UNKNOWN_C9 = 0xc9,        /* two operands */
    BATTLE_SEQ_OP_UNKNOWN_CA = 0xca,        /* one operand */
    BATTLE_SEQ_OP_MOVE_UP_2 = 0xcb,         /* 0xcb-0xd2 shift the sprite by one or two units of seq_offset */
    BATTLE_SEQ_OP_MOVE_UP_1 = 0xcc,
    BATTLE_SEQ_OP_MOVE_BACKWARD_2 = 0xcd,
    BATTLE_SEQ_OP_MOVE_BACKWARD_1 = 0xce,
    BATTLE_SEQ_OP_MOVE_DOWN_2 = 0xcf,
    BATTLE_SEQ_OP_MOVE_DOWN_1 = 0xd0,
    BATTLE_SEQ_OP_MOVE_FORWARD_2 = 0xd1,
    BATTLE_SEQ_OP_MOVE_FORWARD_1 = 0xd2,
    BATTLE_SEQ_OP_WAIT_FOR_EFFECT_LOAD = 0xd3, /* offset: jump while g_battle_state_animation_continue_check is 1 */
    BATTLE_SEQ_OP_PLAY_WEAPON_SOUND = 0xd4,    /* 0 swing, 1 hit: battle_sound_select_weapon_hit */
    BATTLE_SEQ_OP_RESTART = 0xd5,              /* back to the sequence's first byte, counting the restart */
    BATTLE_SEQ_OP_WAIT_FOR_EFFECT = 0xd6,      /* offset: jump while g_battle_state_animation_continue_check is not 0 */
    BATTLE_SEQ_OP_UNKNOWN_D7 = 0xd7,           /* one operand; the sequences of death and revival start with it */
    BATTLE_SEQ_OP_SET_FRAME_BIAS = 0xd8,       /* signed; added to every later frame id */
    BATTLE_SEQ_OP_LAUNCH_PROJECTILE = 0xd9,    /* two unread operands: the thrown item, stone or arrow of the action */
    BATTLE_SEQ_OP_UNDEFINED_DA = 0xda,         /* no handler: MAIN_SYSTEM_ERROR_SEQ_OPCODE like any other value */
    BATTLE_SEQ_OP_SET_WAIT_BIAS = 0xdb,        /* signed; added to every later wait */
    BATTLE_SEQ_OP_RETURN = 0xdc,               /* resume the sequence BATTLE_SEQ_OP_CALL_SEQUENCE left */
    BATTLE_SEQ_OP_CALL_SEQUENCE = 0xdd,        /* sequence id: run it from its start; one level deep */
    BATTLE_SEQ_OP_STRIKE_TARGETS = 0xde,       /* the blow of a weapon-like ability lands: its targets react */
    BATTLE_SEQ_OP_CLEAR_ROTATION = 0xdf,
    BATTLE_SEQ_OP_HIDE_SHADOW = 0xe0,
    BATTLE_SEQ_OP_SHOW_SHADOW = 0xe1,
    BATTLE_SEQ_OP_SET_LAYER_PRIORITY = 0xe2, /* row of g_battle_gfx_animation_layer_priorities */
    BATTLE_SEQ_OP_UNKNOWN_E3 = 0xe3,         /* one operand */
    BATTLE_SEQ_OP_UNKNOWN_E4 = 0xe4,         /* one operand */
    BATTLE_SEQ_OP_SET_ROTATION = 0xe5,       /* low byte, high byte: the display's rotation, ONE to the turn */
    BATTLE_SEQ_OP_UNKNOWN_E6 = 0xe6,         /* two operands */
    BATTLE_SEQ_OP_UNKNOWN_E7 = 0xe7,         /* two operands */
    BATTLE_SEQ_OP_UNKNOWN_E8 = 0xe8,         /* two operands */
    BATTLE_SEQ_OP_UNKNOWN_E9 = 0xe9,         /* one operand */
    BATTLE_SEQ_OP_UNKNOWN_EA = 0xea,         /* two operands */
    BATTLE_SEQ_OP_FLIP_VERTICAL = 0xeb,
    BATTLE_SEQ_OP_FLIP_HORIZONTAL = 0xec,
    BATTLE_SEQ_OP_UNKNOWN_ED = 0xed,           /* one operand */
    BATTLE_SEQ_OP_MOVE_UNIT_FB = 0xee,         /* signed: forward (+) or backward; 0xee-0xf0 and 0xfa move units only */
    BATTLE_SEQ_OP_MOVE_UNIT_DU = 0xef,         /* signed: down (+) or up */
    BATTLE_SEQ_OP_MOVE_UNIT_RL = 0xf0,         /* signed: right (+) or left */
    BATTLE_SEQ_OP_UNKNOWN_F1 = 0xf1,           /* two operands */
    BATTLE_SEQ_OP_START_BLOCK_SEQUENCE = 0xf2, /* sprite block 1-3, sequence id: show the weapon or effect sprite */
    BATTLE_SEQ_OP_UNKNOWN_F3 = 0xf3,           /* one operand */
    BATTLE_SEQ_OP_UNKNOWN_F4 = 0xf4,           /* one operand */
    BATTLE_SEQ_OP_UNKNOWN_F5 = 0xf5,           /* one operand */
    BATTLE_SEQ_OP_PLAY_SOUND = 0xf6,           /* main_sfx_e */
    BATTLE_SEQ_OP_UNKNOWN_F7 = 0xf7,           /* three operands */
    BATTLE_SEQ_OP_UNKNOWN_F8 = 0xf8,           /* two operands */
    BATTLE_SEQ_OP_MOVE_SCREEN_OFFSET = 0xf9,   /* signed x, y: added to the unit's screen_offset */
    BATTLE_SEQ_OP_MOVE_UNIT = 0xfa,            /* right, down, forward: the operands of 0xf0, 0xef and 0xee */
    BATTLE_SEQ_OP_UNKNOWN_FB = 0xfb,           /* two operands */
    BATTLE_SEQ_OP_REPEAT = 0xfc,               /* offset, count: jump count times, then go on */
    BATTLE_SEQ_OP_JUMP = 0xfd,                 /* offset */
    BATTLE_SEQ_OP_END_ANIMATION = 0xfe,        /* stop; a weapon or effect sprite is hidden too */
    BATTLE_SEQ_OP_PAUSE_ANIMATION = 0xff, /* stop; either one puts a unit in its g_battle_gfx_targeted_frame_* pose */
} battle_seq_opcode_e;

/* A facing is an angle with 0x1000 to the turn. Units rest on the four
 * cardinals; the diagonals are the boundaries a free angle is snapped at. */
typedef enum battle_unit_facing {
    BATTLE_FACING_SOUTH = 0x000,
    BATTLE_FACING_SOUTH_WEST = 0x200,
    BATTLE_FACING_WEST = 0x400,
    BATTLE_FACING_NORTH_WEST = 0x600,
    BATTLE_FACING_NORTH = 0x800,
    BATTLE_FACING_NORTH_EAST = 0xa00,
    BATTLE_FACING_EAST = 0xc00,
    BATTLE_FACING_SOUTH_EAST = 0xe00,
} battle_unit_facing_e;

enum {
    BATTLE_FACING_QUARTER_TURN = 0x400,
    BATTLE_FACING_MASK = 0x0fff,
};

/* Unit animation ids: the ones the code and the two animation tables of
 * BATTLE.BIN select by number. Animation n is sequences 2n (seen from the
 * front) and 2n + 1 (from behind) of the sheet's SEQ file; the comments
 * describe the sequences of the human sheets. */
typedef enum battle_unit_animation {
    /* Plays sequences 1 to 5 instead: the standing pose for each direction the unit is seen from. */
    BATTLE_UNIT_ANIMATION_STAND = 0x02, /* stop, petrify, cursed; the default hit reaction */
    BATTLE_UNIT_ANIMATION_IDLE = 0x03,
    BATTLE_UNIT_ANIMATION_IDLE_HASTE = 0x04,
    BATTLE_UNIT_ANIMATION_IDLE_SLOW = 0x05,
    BATTLE_UNIT_ANIMATION_IDLE_FLYING = 0x06, /* sheets with the flying flag */
    BATTLE_UNIT_ANIMATION_IDLE_FLYING_HASTE = 0x07,
    BATTLE_UNIT_ANIMATION_IDLE_FLYING_SLOW = 0x08,
    BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE
    = 0x09, /* in water two or more deep (battle_move_get_water_depth); also the crystal sprite */
    BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE_HASTE = 0x0a,
    BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE_SLOW = 0x0b,
    BATTLE_UNIT_ANIMATION_WALK = 0x0c, /* a movement step on foot (battle_unit_set_idle_animation_for_movement) */
    BATTLE_UNIT_ANIMATION_WALK_FAST = 0x0d,
    BATTLE_UNIT_ANIMATION_WALK_SLOW = 0x0e,
    BATTLE_UNIT_ANIMATION_FLY = 0x12,      /* a flying unit's movement step */
    BATTLE_UNIT_ANIMATION_TREASURE = 0x15, /* the treasure chest sprite */
    BATTLE_UNIT_ANIMATION_KNEEL = 0x16,    /* one held frame, the picture of SLEEP: event poison, a failed mimic */
    BATTLE_UNIT_ANIMATION_DEFEND = 0x17,
    BATTLE_UNIT_ANIMATION_DODGE = 0x18,
    BATTLE_UNIT_ANIMATION_HURT = 0x19,
    BATTLE_UNIT_ANIMATION_DEAD = 0x1a,
    BATTLE_UNIT_ANIMATION_RAISE_HANDS = 0x1b, /* healed, blade grasp or catch, crystal pickup */
    /* Jumps for joy; in water one deep and for a rider, small hops instead. */
    BATTLE_UNIT_ANIMATION_LEVEL_UP = 0x1c,
    BATTLE_UNIT_ANIMATION_LEVEL_UP_HOP = 0x1d,
    BATTLE_UNIT_ANIMATION_JUMP = 0x1e,
    BATTLE_UNIT_ANIMATION_FALL = 0x1f,
    BATTLE_UNIT_ANIMATION_LAND = 0x20, /* after a jump or a fall */
    /* Held while charging: the second animation of several charge animation sets. */
    BATTLE_UNIT_ANIMATION_CHARGE = 0x22,
    BATTLE_UNIT_ANIMATION_CLIMB = 0x23,
    BATTLE_UNIT_ANIMATION_SLEEP = 0x24, /* kneeling: also the critical status */
    BATTLE_UNIT_ANIMATION_CONFUSION = 0x25,
    BATTLE_UNIT_ANIMATION_SING = 0x28,
    BATTLE_UNIT_ANIMATION_DANCE = 0x29,
    BATTLE_UNIT_ANIMATION_CAST_START = 0x2a,  /* the first animation of most charge animation sets */
    BATTLE_UNIT_ANIMATION_CAST_CHARGE = 0x2b, /* its loop, held while charging as CHARGE is */
    /* Arms raised until the effect ends: the usual attack animation of the
     * ability table, and every ability of the RUKA, ARUTE and KANZEN sequences. */
    BATTLE_UNIT_ANIMATION_CAST = 0x2c,
    BATTLE_UNIT_ANIMATION_HOP = 0x30, /* short hops alternate between the two */
    BATTLE_UNIT_ANIMATION_HOP_ALTERNATE = 0x31,
    BATTLE_UNIT_ANIMATION_RIDING = 0x32, /* a mounted unit; the rider while its mount moves */
    BATTLE_UNIT_ANIMATION_FOUND_ITEM = 0x33,
    BATTLE_UNIT_ANIMATION_DEATH = 0x34,
    BATTLE_UNIT_ANIMATION_REVIVE = 0x35,
    BATTLE_UNIT_ANIMATION_HOLD_ITEM = 0x39,
    BATTLE_UNIT_ANIMATION_TELEPORT = 0x3c,
    /* attacks: HIGH at a target at least 12 higher, LOW at one at least 12 lower */
    BATTLE_UNIT_ANIMATION_UNARMED_ATTACK_HIGH = 0x3d,
    BATTLE_UNIT_ANIMATION_UNARMED_ATTACK_LEVEL = 0x3e,
    BATTLE_UNIT_ANIMATION_UNARMED_ATTACK_LOW = 0x3f,
    BATTLE_UNIT_ANIMATION_SWING_HIGH = 0x40,
    BATTLE_UNIT_ANIMATION_SWING_LEVEL = 0x41,
    BATTLE_UNIT_ANIMATION_SWING_LOW = 0x42,
    BATTLE_UNIT_ANIMATION_THROW_ITEM = 0x4c, /* an item used on a target that is not adjacent */
    BATTLE_UNIT_ANIMATION_THRUST_HIGH = 0x4d,
    BATTLE_UNIT_ANIMATION_THRUST_LEVEL = 0x4e,
    BATTLE_UNIT_ANIMATION_THRUST_LOW = 0x4f,
    BATTLE_UNIT_ANIMATION_GUN_HIGH = 0x50,
    BATTLE_UNIT_ANIMATION_GUN_LEVEL = 0x51,
    BATTLE_UNIT_ANIMATION_GUN_LOW = 0x52,
    BATTLE_UNIT_ANIMATION_BOW_HIGH = 0x53, /* a bow shot takes its row from battle_effect_get_bow_shot_angle */
    BATTLE_UNIT_ANIMATION_BOW_LEVEL = 0x54,
    BATTLE_UNIT_ANIMATION_BOW_LOW = 0x55,
    BATTLE_UNIT_ANIMATION_BOOK = 0x56,
    BATTLE_UNIT_ANIMATION_INSTRUMENT = 0x57,
    BATTLE_UNIT_ANIMATION_WEAPON_GUARD_HIGH = 0x58, /* the attacker stands at least 12 higher */
    BATTLE_UNIT_ANIMATION_WEAPON_GUARD_LEVEL = 0x59,
    BATTLE_UNIT_ANIMATION_WEAPON_GUARD_LOW = 0x5a,
    BATTLE_UNIT_ANIMATION_SWING_BLOCKED_HIGH = 0x5b, /* the attacker's recoil from a blocked attack */
    BATTLE_UNIT_ANIMATION_SWING_BLOCKED_LEVEL = 0x5c,
    BATTLE_UNIT_ANIMATION_SWING_BLOCKED_LOW = 0x5d,
    BATTLE_UNIT_ANIMATION_UNARMED_ATTACK_BLOCKED_HIGH = 0x5e,
    BATTLE_UNIT_ANIMATION_UNARMED_ATTACK_BLOCKED_LEVEL = 0x5f,
    BATTLE_UNIT_ANIMATION_UNARMED_ATTACK_BLOCKED_LOW = 0x60,
    BATTLE_UNIT_ANIMATION_THRUST_BLOCKED_HIGH = 0x61,
    BATTLE_UNIT_ANIMATION_THRUST_BLOCKED_LEVEL = 0x62,
    BATTLE_UNIT_ANIMATION_THRUST_BLOCKED_LOW = 0x63,
    BATTLE_UNIT_ANIMATION_JUMP_DEPART = 0x75, /* the jump status is added in event mode; charging a Jump */
    BATTLE_UNIT_ANIMATION_JUMP_LAND = 0x76,
    BATTLE_UNIT_ANIMATION_CLOTH_HIGH = 0x77, /* bags and cloths */
    BATTLE_UNIT_ANIMATION_CLOTH_LEVEL = 0x78,
    BATTLE_UNIT_ANIMATION_CLOTH_LOW = 0x79,
} battle_unit_animation_e;

/* Animation ids from 500 up are not doubled into a front and a back
 * sequence: 500 + n and 600 + n play sequence n of EVTCHR slot 0 and 1. */
enum {
    BATTLE_SEQUENCE_EVTCHR_SLOT_0 = 500,
    BATTLE_SEQUENCE_EVTCHR_SLOT_1 = 600,
};

/* Word-view masks for the high motion byte at misc-unit +0x83. */
typedef enum battle_motion_flags {
    BATTLE_MOTION_FLAG_SUPPRESS_FACING_UPDATE = 0x01000000,
    BATTLE_MOTION_FLAG_SUPPRESS_SFX_AND_LANDING_EFFECTS = 0x02000000,
    BATTLE_MOTION_FLAG_SUPPRESS_PALETTE_UPDATE = 0x04000000,
    BATTLE_MOTION_FLAG_ALTERNATE_SHORT_HOP = 0x08000000,
    BATTLE_MOTION_FLAG_FLOAT = 0x10000000,
    BATTLE_MOTION_FLAG_FORCE_ANIMATION_UPDATE = 0x20000000, /* set with each stored animation request */
} battle_motion_flags_e;

/* Native effect handlers receive the record, callback slot, emitter and frame.
 * Individual handlers narrow the record index or ignore unused arguments. */
typedef s32 (*battle_effect_callback_t)(s32 record_index, s32 callback_slot, s32 emitter_index, s32 frame);

extern u8 g_battle_rotation_speed_frames[];
extern u16 g_battle_animation_speed_forced; /* 1 while the event speed is forced */
extern s32 g_main_system_animation_speed;
void animation_exception_handler(s32 exception_id);
void battle_script_rotate_unit(const event_rotate_unit_parameters_t* parameters);

/* dead unit */

extern s32 g_battle_dead_unit_action;
extern crystal_pickup_result_t* g_battle_dead_unit_context;
extern s32 g_battle_dead_unit_result;
crystal_pickup_result_t* battle_unit_generate_crystal_or_treasure(battle_stats_t* mover);

/* deploy */
extern const RECT g_battle_deployment_clear_rect;
extern palette_t g_battle_deployed_unit_palettes[2];
extern s32 g_battle_deployment_skipped;

/* input */
extern s32 g_battle_cursor_x;
extern s32 g_battle_cursor_y;
extern s32 g_battle_cursor_z;
extern s32 g_battle_cursor_input_mode_saved;
extern u16 g_battle_cursor_screen_x;
extern s32 g_battle_cursor_previous_x;     /* cursor X before this step */
extern s32 g_battle_cursor_previous_y;     /* cursor Y before this step */
extern s32 g_battle_cursor_previous_z;     /* cursor level before this step */
extern u32 g_battle_cursor_repeat_counter; /* free-cursor repeat counter */
extern s16 g_battle_cursor_screen_y;
extern s32 g_battle_cursor_input_mode;
extern s32 g_main_input_pressed_buttons;
extern s32 g_main_input_previous_buttons;
extern u32 g_main_input_current_buttons;
extern u32 g_main_input_released_buttons;
extern u32 g_controller_pressed_copy_0;
extern u32 g_controller_pressed_copy_1;
extern u32 g_controller_pressed_copy_2;
extern u32 g_controller_pressed_copy_3;
extern u32 g_controller_previous_copy_0;
extern u32 g_controller_previous_copy_1;
extern u32 g_controller_previous_copy_2;
extern u32 g_controller_previous_copy_3;
extern u32 g_controller_stable_frames;

/* item */
/* battle_unit_animation_e of each item type's attack at a higher, a level and a lower target. */
extern u8 g_battle_weapon_attack_animations[][3];
extern s16 g_battle_item_icon_clut_base_x;
extern s16 g_battle_item_icon_clut_base_y;
extern point16_t g_battle_item_icon_image_location;
void battle_gfx_set_item_icon_sprite(SPRT* sprite, s32 item_id);

/* script */
typedef struct battle_script_weather_environment {
    u8 environment_id;
    u8 _unused_01;
} battle_script_weather_environment_t;
typedef char
    battle_script_weather_environment_size_must_be_2[(sizeof(battle_script_weather_environment_t) == 2) ? 1 : -1];

/* Per-byte status removal masks applied while an event stages a unit: three
 * eight-byte sets, of which the first BATTLE_STATUS_BYTE_COUNT bytes are used
 * (0x80169758). */
typedef enum battle_event_status_mask_set {
    BATTLE_EVENT_STATUS_MASK_BATTLE_STARTED = 0,
    BATTLE_EVENT_STATUS_MASK_PRE_BATTLE = 1,
    /* Monster skillsets, both Altima forms and the six undead jobs. */
    BATTLE_EVENT_STATUS_MASK_PRE_BATTLE_MONSTER_OR_UNDEAD = 2,
} battle_event_status_mask_set_e;

enum { BATTLE_EVENT_STATUS_MASK_COUNT = 3 };

extern unit_tile_coordinates_t g_battle_script_warp_coords;
extern u16 g_battle_script_attack_entry_mode; /* script_attack_entry_mode_e */
extern s32 g_battle_script_condition_unit_battle_id;
extern s32 g_battle_script_rand16_state;
extern u32 g_battle_script_unfiltered_controller_input;
extern const char g_battle_script_pause_message[];
extern s16 g_battle_script_debug_paused;
extern s16 g_battle_event_effect_target_x;
extern s16 g_battle_event_effect_target_y;
extern u8 g_battle_script_condition_param_lengths[];
extern u16 g_battle_script_map_darkness_durations[];
extern u16 g_battle_script_unit_filter_modes[];
extern u32 g_battle_script_event_input;
extern u32 g_battle_script_saved_event_input;
extern s16 g_battle_event_map_jump_in_request;
extern s16 g_battle_script_threads_idle;
extern s32 g_battle_next_event_id;
extern u16 g_battle_script_frame_result_override;
extern VECTOR g_battle_script_light_scale;
extern s32 g_battle_event_pending_loaded_evtchr_slot;
extern MATRIX g_battle_script_light_base_direction_matrix;
struct battle_gfx_packed_evtchr_record;
extern struct battle_gfx_packed_evtchr_record* g_battle_event_loaded_evtchr_buffer;
extern SVECTOR g_battle_script_light_angles;
extern MATRIX g_battle_script_light_base_color_matrix;
extern u8 g_battle_event_status_masks[BATTLE_EVENT_STATUS_MASK_COUNT][8];

extern u8
    g_battle_event_thread_status_snapshot[]; /* per-thread running flags sampled before the yield, indexed 1..15 */

extern s16 g_battle_scenario_event_active;
extern u32 g_battle_script_event_current_input;
extern u32 g_battle_script_event_initial_repeat_counter;
extern u32 g_battle_script_event_previous_input;
extern u32 g_battle_script_event_repeat_counter;
extern u32 g_battle_script_event_secondary_repeat_counter;
/* g_main_script_variables: a variable id below EVENT_SCRIPT_VAR_PACKED_BIT_FIRST
 * (event_script_variable_id_e) is its word index. */
extern s32* g_battle_script_variables;
extern u16 g_battle_scenario_music_1_id;
extern u16 g_battle_scenario_music_2_id;
extern u16 g_battle_event_input_suppression_frames;
extern s32 g_battle_menu_input_lock_mode;

/* BATTLE pointer slot 0x80173ca4; WORLD pointer slot 0x801cd75c. */
extern event_file_block_t* g_battle_event_block;
extern u16 g_battle_event_effect_target_misc_id; /* 0x80165ff8 */

/* 0x80166000: consumer passes mode 2 when equal to 2, otherwise mode 0.
 * The last Effect operand is not a second Y coordinate. */
extern s16 g_battle_event_effect_target_mode;
extern u16 g_battle_event_3d_object_id;                     /* 0x80173c94 */
extern u16 g_battle_event_3d_object_state;                  /* 0x80173c96 */
extern u16 g_battle_event_map_command_83_arg1;              /* 0x80174058 */
extern u16 g_battle_event_map_command_83_arg2;              /* 0x8017405a */
extern u16 g_battle_event_music_switch_fade;                /* 0x80173f56; consumer multiplies by 4 */
extern u16 g_battle_event_music_switch_volume;              /* 0x80173f54; scaled 0..96 to 0..127 */
extern s16 g_battle_event_pending_effect_id;                /* 0x80173cb4 */
extern s32 g_battle_event_pending_evtchr_save_slot;         /* 0x80173f4c */
extern s32 g_battle_event_pending_loaded_evtchr_clear_slot; /* 0x80173ca0 */
extern s16 g_battle_event_pending_map_jump_out_2_id;        /* 0x8017405c */

/* Deferred BATTLE commands: producer 0x80143bd8, consumer
 * 0x80143418..0x801439c0. The outer update at 0x80142d58 clears one-shot
 * requests, schedules script threads, then applies commands on its normal
 * path. These are separate globals, not a contiguous struct. Signed pending
 * slots use -1; request bits and polled states do not. */
extern s16 g_battle_event_pending_map_jump_out_id;            /* 0x80174054 */
extern s16 g_battle_event_pending_map_state;                  /* 0x80173f64; script variable 0x24 */
extern s32 g_battle_event_pending_reserved_vram_release_slot; /* 0x80173f48 */
extern s32 g_battle_event_pending_saved_evtchr_clear_slot;    /* 0x80173f50 */

/* 0x80173cac: packed misc ID << 8 | reserved VRAM slot; -1 absent. */
extern s32 g_battle_event_pending_unit_vram_copy;
extern s32 g_battle_event_speed;
extern s16 g_battle_event_unit_slots[4];

/* 0x80173f68: packed weather flags; consumer replaces it with a table index. */
extern s16 g_battle_event_weather_request;
extern battle_script_weather_environment_t g_battle_event_weather_commands[];

/* Operand-byte counts, excluding the opcode; not per-operand width schemas.
 * BATTLE 0x8014d170, WORLD 0x8013a454. The tables end after opcode 0xf3. */
extern u8 g_battle_script_event_instruction_sizes[0xf4];

/* 0x801660a3: suppress recursive write filtering while getters use operand 0. */
extern u8 g_battle_script_variable_write_guard;
extern s32 g_battle_event_frame_counter;

extern map_background_gradient_colors_t g_battle_event_background_colors;

void battle_script_advance_date_by_one_year(void);
void battle_script_apply_input_overrides(u32* input);
void battle_script_assign_bitset_flag(u32* bitset, s32 bit_index, s32 value);
void battle_script_block_start_thread(void);
void battle_script_blue_remove_unit(s32 unit_id);
void battle_script_blue_remove_all_enemy_units(void);
void battle_script_change_stats(const event_change_stats_parameters_t* parameters);
void battle_script_clamp_s32_to_range(s32* value, s32 minimum, s32 maximum);
void battle_script_earthquake_start(void);
void battle_script_face_tile(const event_face_tile_parameters_t* parameters);
void battle_script_face_unit(const event_face_unit_parameters_t* parameters, s32 second_only);
void battle_script_filter_controller_input(u32* input);
void battle_script_focus(event_focus_parameters_t* parameters);
/* A typed operand plus a local Camera cursor reorders the original argument saves. */
void battle_script_focus_speed(u8* parameters, s32* position, s32* rotation);
u32* battle_script_get_controller_input_pointer(s32 use_current);
s32 battle_script_get_current_scenario_finish_operation(void);
s32 battle_script_get_variable(s32 variable_id);
s32 battle_script_interpolate_range_fixed12(s32 start, s32 end, s32 fraction, s32 value);
void battle_script_join_units_silently(void);
void battle_script_jump_event_instruction(const event_jump_parameters_t* parameters);
void battle_script_load_event(s32 id);

/* The byte-stream reader at 0x80146078 preserves signed halfwords. Some
 * callers forward their incoming a0 without an additional argument move.
 * unit_t lookup at 0x80133158 accepts and returns full words; narrowing is
 * caller-specific, not part of its interface. */
s16 battle_script_load_halfword(const u8* data);
s32 battle_script_load_next_event(void);
void battle_script_map_darkness(void);
void battle_script_march_units(const event_march_parameters_t* parameters);
void battle_script_print_debug_message(void);
void battle_script_reset_event_state(void);
void battle_script_run_next_event(void);
void battle_script_seed_random_from_vsync(void);
void battle_script_set_variable(s32 variable_id, s32 value);
void battle_script_show_graphic(void);
void battle_script_sprite_move(void);
void battle_script_sprite_move_beta(void);
void battle_script_start_current_event_thread(void);
void battle_gfx_ramp_screen_overlay_intensity_to_max(void);
void battle_script_unit_anim(const event_unit_anim_parameters_t* parameters);
u32 battle_script_get_event_finish_operation(void);
void battle_script_store_halfword(u8* destination, s16 value);
void battle_script_update_event_input_state(u32 state);
void battle_script_set_current_otag_entry_and_update_input(u32* entry, s32 input);
void battle_script_wait_for_unit_ready(s32 misc_id);
void battle_script_warp_unit(const event_warp_unit_parameters_t* parameters);
void battle_script_set_units_movement_effect_suppression(const event_movement_effect_parameters_t* parameters);
s32 battle_script_run_scenario_conditions(void);
s32 battle_script_check_scenario_condition(s32 type, s32 id, s32 value, s32 tile_y, s32 elevation);
void battle_script_run_sprite_move(const event_sprite_move_parameters_t* parameters, s32 use_speed);

/* Returns the event result that the battle state handlers switch on;
 * proven by 0x80078fb4 and 0x800785ac, which both consume it. */
s32 battle_script_run_event_frame(u32* ot, u32 buttons);
void battle_script_load_event_input_state(const menu_input_state_t* saved);
void battle_camera_clear_zoom_variable_low_bit(void);
void battle_script_copy_matrix(MATRIX* destination, const MATRIX* source);
s32 battle_script_filter_unit_id_by_mode(u16* out_id, u16* in_id, s32* mode);
u32 battle_script_get_event_initial_repeat_counter(void);
s32 battle_script_is_deployment_running(u32* ot, u32 buttons);
void battle_script_process_pending_requests(void);
s32 battle_script_scale_music_volume(s32 x);
void battle_script_pulse_tutorial_wait_value(s32 value);
void battle_script_set_specialized_map_destroyed(void);
void battle_script_dismiss_unit_event_instruction(s32 unit_id);

s32 battle_script_add_ghost_unit_event_instruction(
    s32 map_x, s32 map_y, s32 map_level, s16 facing, s16 spritesheet_id, s32 misc_id, s32 flags);

void battle_script_add_unit_start_thread(void);
s32 battle_unit_is_moving(s32 misc_id);
void battle_script_color_screen_thread(void);
void battle_script_color_unit_event_instruction(const event_color_unit_parameters_t* parameters);
void battle_script_toggle_message_portrait_flip(u8* ptr);
void battle_script_set_units_palette_update_suppression(s32 unit_id, s32 enable);
void battle_script_apply_relative_camera(event_camera_values_t* parameters, s32* src);
void battle_script_load_attack_graphics_event_instruction(void);
void battle_script_execute_display_conditions_instruction(void);

/* Whole matching interpreter ranges:
 * BATTLE 0x80143bd8..0x80145f78, WORLD 0x800f6f20..0x800f92a0 (exclusive). */
void battle_script_execute_event(void);
s32 battle_script_find_instruction_byte_offset(s32 offset, s32 opcode);

/* 0x80149d6c..0x80149ebc: returns offset after the matching target marker;
 * missing targets stop the current thread. alternate_opcode accepts -1. */
s32 battle_script_find_jump_target(s32 limit_offset, event_opcode_e target_opcode, s32 alternate_opcode, s32 target_id);
s32 battle_script_get_rand16(void);
s32 battle_script_get_variable_bit_position(s32 variable_id);

/* Encoded variable IDs select words, bits, or nibbles. The command runner's
 * fourth argument is supplied by existing callers but is not read. */
s32* battle_script_get_variable_word_pointer_from_id(s32 variable_id);
void battle_script_inflict_status_thread(void);
s32 battle_script_is_tutorial_event_slot(void);
void battle_script_load_portrait_colors_event_instruction(s32 portrait_id);
void battle_script_mirrorsprite_event_instruction(const event_mirror_sprite_parameters_t* parameters);
void battle_script_pause_event_instruction(void);
void battle_script_play_effect_thread(void);
void battle_script_resetpalette_event_instruction(const event_unit_parameters_t* parameters);
void battle_script_run_condition(event_opcode_e opcode);
void battle_script_run_variable_command(s32 opcode, s32 destination_id, s32 source, s32 unused);
void battle_script_set_event_speed(s32 speed);
void battle_gfx_ramp_screen_overlay_intensity_to_zero(void);
void battle_script_teleportin_event_instruction(s32 unit_id, s32 unused);
void battle_script_teleportout_event_instruction(s32 arg, s32 remove);
void battle_script_unit_animation_rotate_event_instruction(const event_unit_anim_rotate_parameters_t* parameters);
void battle_script_unlockdate_event_instruction(u32* bitset, s32 date_index, s32 month, s32 day);
void battle_script_wait_value_event_instruction(const event_wait_value_parameters_t* parameters);
void battle_script_waitrotateunit_and_waitrotateall_event_instruction(s32 unit_id);
void battle_script_waitspritemove_event_instruction(s32 unit_id);
void battle_script_waitwalk_event_instruction(s32 unit_id);
void battle_script_walkto_event_instruction(const event_walk_to_parameters_t* parameters);
void battle_script_warp_unit_display_to_paired_unit(s32 misc_id);

/* status */
/* Event infliction helpers number bits LSB-first, unlike stored status IDs. */
#define BATTLE_STATUS_LSB_INDEX(id) (((id) & ~7) | (7 - ((id) & 7)))

#define BATTLE_STATUS_LSB_MASK(id) ((u8)(1U << ((id) & 7)))

/* The special-status callback table reserves index 0; timed slots begin at Poison. */
#define BATTLE_STATUS_HANDLER_INDEX(id) ((id) + 1)

#define BATTLE_STATUS_CT_INDEX(id) ((id) - BATTLE_STATUS_ID_POISON)

#define BATTLE_STATUS_PACKED_MASK(id) ((u32)BATTLE_STATUS_BYTE_MASK(id) << (BATTLE_STATUS_BYTE_INDEX(id) * 8))

extern const u8 g_battle_status_display_image_ids[BATTLE_STATUS_COUNT];
extern s32 g_battle_status_menu_open;
extern SVECTOR g_battle_status_bubble_zoom;
extern s16 g_battle_numeric_digit_0_zoom_curve[21];
extern s16 g_battle_numeric_digit_1_zoom_curve[21];
extern s16 g_battle_numeric_digit_2_zoom_curve[21];
extern s16 g_battle_numeric_digit_0_critical_zoom_curve[21];
extern s16 g_battle_numeric_digit_1_critical_zoom_curve[21];
extern s16 g_battle_numeric_digit_2_critical_zoom_curve[21];

/* Texture coordinates for the status bubble graphics, indexed by status bubble
 * id (0x800949dc/0x800949f4) and by a counter digit (0x80094a0c/0x80094a24).
 * Each table holds 24 bytes; the pairs are read together as one u/v. */
extern u8 g_battle_status_bubble_u[24];
extern u8 g_battle_status_bubble_v[24];
extern u8 g_battle_status_bubble_digit_u[24];
extern u8 g_battle_status_bubble_digit_v[24];
s32 battle_status_check_unit(battle_stats_t* unit);
void battle_status_disable_acting(battle_stats_t* unit);
void battle_status_enable_special_flags(s32 status_id, s32 enabled, s32 misc_unit_id);
void battle_status_queue_current_status_graphics(s32 battle_id, s32 enabled);
s32 battle_status_inflict_defending_to_battle_id(s32 unit_id);
s32 battle_status_is_unit_absent_dead_crystal_treasure_petrified_or_ridden(const battle_stats_t* unit);
s32 battle_status_modify_inflictions(s32 removal_only);
s32 battle_status_remove_charging_ability_ct(battle_stats_t* unit, s32 do_disable);
void battle_status_remove_control(battle_stats_t* unit);
s32 battle_status_resolve_unit_changes(s32 unit_id, s32 removal_only);
void battle_status_set_inflicted_ct_and_transfer_last_used_ct(s32 unit_idx);
void battle_status_store_for_current_attack(s32 unit_id, s32 removal_only);
s32 battle_status_apply_poison_and_regen(battle_stats_t* unit);
void battle_status_apply_unit_action_removal(battle_stats_t* unit);
s32 battle_status_build_upkeep_action(s32 unit_id, battle_stats_t* unit);
s32 battle_status_check_crystal_dead_jump_petrify_treasure(battle_stats_t* unit);
void battle_status_enable_disable_acting(battle_stats_t* unit);
void battle_status_inflict_by_entd_unit_id(s32 entd_id, s32 status, s32 which, s32 arg);
s32 battle_status_init_special_flag_enabling(s32 unit_id);
s32 battle_status_queue_misc_graphics_flag_change(s32 status, s32 enabled, s32 battle_id);
void battle_status_reapply_active_flags(battle_stats_t* unit);
void battle_status_remove(void);
s32 battle_status_remove_transparent(battle_stats_t* unit);
void battle_status_remove_transparent_if_jump_used(battle_stats_t* unit);
void battle_status_update_expiring(s32 unit_id);

/* system */
/*
 * Values dispatched by battle_state_run_game_loop, verified against the
 * target's switch table.  The target has no explicit handler for value 0x32.
 */
typedef enum battle_game_state {
    BATTLE_GAME_STATE_FREE_CURSOR = 0x00,
    BATTLE_GAME_STATE_FREE_CURSOR_HELP = 0x01,
    BATTLE_GAME_STATE_HIGHLIGHT_UNITS = 0x02,
    BATTLE_GAME_STATE_OPEN_ACTION_MENUS = 0x03,
    BATTLE_GAME_STATE_IDLING_ACTION_MENUS = 0x04,
    BATTLE_GAME_STATE_MENU_TO_TARGETING = 0x05,
    BATTLE_GAME_STATE_ACTION_HELP_MENU = 0x06,
    BATTLE_GAME_STATE_DISPLAY_MOVE_AREA = 0x07,
    BATTLE_GAME_STATE_MINI_MENU = 0x08,
    BATTLE_GAME_STATE_MINI_MENU_HELP = 0x09,
    BATTLE_GAME_STATE_ACTIVE_TURN = 0x0a,
    BATTLE_GAME_STATE_STATUS_EXECUTE = 0x0b,
    BATTLE_GAME_STATE_UNIT_MOVE = 0x0c,
    BATTLE_GAME_STATE_CLOSE_MOVE_HELP = 0x0d,
    BATTLE_GAME_STATE_MOVE_RANGE_EXCEPTION = 0x0e,
    BATTLE_GAME_STATE_ILLEGAL_MOVE_MENU = 0x0f,
    BATTLE_GAME_STATE_UNIT_MOVING_SETUP = 0x10,
    BATTLE_GAME_STATE_UNIT_MOVING = 0x11,
    BATTLE_GAME_STATE_MOVE_CONFIRM_MENU = 0x12,
    BATTLE_GAME_STATE_WAIT_DIRECTION = 0x13,
    BATTLE_GAME_STATE_WAIT_MENU = 0x14,
    BATTLE_GAME_STATE_CRYSTAL_LEARN = 0x15,
    BATTLE_GAME_STATE_ACTION_EXECUTE_SETUP = 0x16,
    BATTLE_GAME_STATE_TARGETING_RANGE = 0x17,
    BATTLE_GAME_STATE_ILLEGAL_RANGE = 0x18,
    BATTLE_GAME_STATE_ABILITY_PREVIEW_HANDLING = 0x19,
    BATTLE_GAME_STATE_ABILITY_PREVIEW_HELP = 0x1a,
    BATTLE_GAME_STATE_CONFIRM_ACTION = 0x1b,
    BATTLE_GAME_STATE_PRE_ATTACK_ANIMATION = 0x1c,
    BATTLE_GAME_STATE_ACTION_CAST = 0x1d,
    BATTLE_GAME_STATE_COMMENCE_ATTACK_PHASE = 0x1e,
    BATTLE_GAME_STATE_TARGET_SELECT_START = 0x1f,
    BATTLE_GAME_STATE_TARGET_SELECT = 0x20,
    BATTLE_GAME_STATE_TARGET_SELECT_DENIED = 0x21,
    BATTLE_GAME_STATE_TARGET_SELECT_CONFIRM = 0x22,
    BATTLE_GAME_STATE_TARGET_DISPLAY_START = 0x23,
    BATTLE_GAME_STATE_TARGET_DISPLAY = 0x24,
    BATTLE_GAME_STATE_AFTER_COMMAND = 0x25,
    BATTLE_GAME_STATE_JP_EXP_GAIN = 0x26,
    BATTLE_GAME_STATE_CHANGE_TURN = 0x27,
    BATTLE_GAME_STATE_LEARN_ABILITY_ON_HIT = 0x28,
    BATTLE_GAME_STATE_ACTION_ANNOUNCEMENT = 0x29,
    BATTLE_GAME_STATE_OPEN_SP2_FILES = 0x2a,
    BATTLE_GAME_STATE_START_EFFECT_FILE_OPEN = 0x2b,
    BATTLE_GAME_STATE_SECONDARY_EFFECT = 0x2c,
    BATTLE_GAME_STATE_ACTION_EXECUTE = 0x2d,
    BATTLE_GAME_STATE_BATTLE_MESSAGE_DISPLAY = 0x2e,
    BATTLE_GAME_STATE_RESUME_ATTACK_PHASE = 0x2f,
    BATTLE_GAME_STATE_DEEP_DUNGEON_MESH_LOAD = 0x30,
    BATTLE_GAME_STATE_DEEP_DUNGEON_MESH_FINISH = 0x31,
    BATTLE_GAME_STATE_EFFECT = 0x33,
    BATTLE_GAME_STATE_EVENT = 0x34,
    BATTLE_GAME_STATE_MAP_JUMPING_OUT = 0x35,
    BATTLE_GAME_STATE_MAP_INITIALIZE = 0x36,
    BATTLE_GAME_STATE_MAP_JUMPING_IN = 0x37,
    BATTLE_GAME_STATE_MAP_JUMPING_OUT_2 = 0x38,
    BATTLE_GAME_STATE_MAP_INITIALIZE_2 = 0x39,
    BATTLE_GAME_STATE_MAP_JUMPING_IN_2 = 0x3a,
    BATTLE_GAME_STATE_CLOSE_BATTLE = 0x3b,
} battle_game_state_e;

/* Battle heap block header: blocks are counted in 8-byte units and linked
 * either on the circular free list (g_battle_heap_rover) or on one of the 16
 * per-owner lists in g_battle_heap_owner_lists. */
typedef struct battle_heap_node {
    struct battle_heap_node* next;
    u16 size; /* in 8-byte blocks */
    s16 owner;
} battle_heap_node_t;

typedef struct battle_heap_owner_list {
    battle_heap_node_t* head;
    s32 _unused_04;
} battle_heap_owner_list_t;

/* A random value between `min` and `max`, in either order. Spelled with rand()
 * first, unlike RAND_BETWEEN; both spellings occur in the target. */
#define RANDOM_BETWEEN(min, max)                                                                                       \
    ((min) == (max) ? (min) : (max) < (min) ? rand() % ((min) - (max)) + (max) : rand() % ((max) - (min)) + (min))

/* Array view: as a scalar, GCC hoists the menu-index reload above its store
 * in battle_menu_run_system_function_thread. */
extern battle_system_function_t g_battle_system_function_table[];
extern s32 g_battle_state_map_init_step;
extern u8 g_battle_random_battle_music_tracks[];
extern s32 g_battle_system_function_thread_busy;
extern battle_heap_node_t* g_battle_heap_base;
extern s16 g_battle_heap_block_count;
extern battle_heap_owner_list_t g_battle_heap_owner_lists[];
extern battle_heap_node_t* g_battle_heap_rover;
extern u32 g_battle_heap_end_address;
extern s32 g_battle_heap_min_largest_free; /* Lowest largest-free_node-block size seen, in 8-byte units. */
extern s32 g_battle_game_state;
extern s32 g_battle_state_animation_continue_check;
extern s32 g_battle_state_game_flow_running;
extern s32 g_battle_state_screen_fade_step;
extern s32 g_battle_frame_counter;
extern s32 g_main_system_battle_frame_hsync_count;
extern s32 g_frame_pacing;
extern s32 g_frame_pacing_suppressed;
extern s32 g_frame_pacing_timer;
void battle_state_enter_action_cast(void);
void battle_state_enter_commence_attack_phase(void);
void battle_action_post_action_facing_stub(battle_stats_t* unused_unit, s32 unused_facing);
void battle_state_enter_open_sp2_files(void);
void battle_bin_entrypoint(void);
s32 battle_mul_div_s64(s32 a, s32 b, s32 c);
void battle_menu_init_stub(void);
void battle_script_add_unit_stub(void);
void battle_state_enter_pre_attack_animation(void);
void* game_realloc(void*, u32);
void battle_state_deployment_frame_stub(void);
void battle_menu_run_companion_overlay_stub(void);
void battle_script_frame_after_threads_stub(void);
void battle_script_frame_after_requests_stub(void);

/* BATTLE 0x8014bf54..0x8014bf7c: destination first. This do-while loop
 * requires a positive count; it is not a general-purpose memcpy. */
void battle_copy_bytes(void* destination, const void* source, s32 count);
void* battle_heap_alloc_block(u32 size, s32 kind);
void battle_heap_free_block(void* ptr);
void battle_heap_init(battle_heap_node_t* buffer, u32 buffer_size);
void battle_heap_print_stats(void);
void battle_state_run_deployment(void);
void battle_state_enter_action_execution_setup(void);
void battle_state_enter_after_command(void);
void battle_state_enter_highlight_units_by_team(void);
void battle_state_enter_jp_xp_gain(void);
void battle_state_enter_status_execute(void);
void battle_state_enter_target_display(void);
void battle_state_enter_target_out_of_range(void);
void battle_state_enter_target_select(void);
void battle_state_enter_target_select_confirm(void);
void battle_state_enter_target_select_denied(void);
void battle_state_enter_unit_moving(void);
void battle_state_stop_map_animations(void);
s32 battle_state_get_animation_speed(void);
void battle_state_handle_ability_preview_help_state(void);
void battle_state_handle_ability_preview_state(void);
void battle_state_handle_action_cast_state(void);
void battle_state_handle_action_execute_setup_state(void);
void battle_state_update_action_execute_mode(void);
void battle_state_handle_action_help_menu_state(void);
void battle_state_handle_active_turn_state(void);
void battle_state_handle_after_command_state(void);
void battle_state_handle_battle_message_display_state(void);
void battle_state_handle_change_turn_state(void);
void battle_state_handle_close_battle_state(void);
void battle_state_handle_close_move_help_state(void);
void battle_state_handle_commence_attack_phase_state(void);
void battle_state_handle_confirm_action_state(void);
void battle_state_handle_crystal_learn_state(void);
void battle_state_handle_deep_dungeon_mesh_finish_state(void);
void battle_state_handle_deep_dungeon_mesh_load_state(void);

/* Per-frame handlers dispatched by battle_state_run_game_loop. */
void battle_state_handle_default_state(void);
void battle_state_handle_display_move_area_state(void);
void battle_state_handle_action_announcement_state(void);
void battle_state_handle_effect_state(void);
void battle_state_update_event_mode(void);
void battle_state_handle_free_cursor_help_state(void);
void battle_state_handle_free_cursor_input(void);
void battle_state_handle_highlight_units_state(void);
void battle_state_handle_idling_action_menus_state(void);
void battle_state_handle_illegal_move_menu_state(void);
void battle_state_handle_illegal_range_state(void);
void battle_state_handle_jp_exp_gain_state(void);
void battle_state_handle_learn_ability_on_hit_state(void);
void battle_state_handle_change_map_init_state(void);
void battle_state_handle_map_init_state(void);
void battle_state_handle_change_map_jumping_in_state(void);
void battle_state_handle_map_jumping_in_state(void);
void battle_state_handle_change_map_jumping_out_state(void);
void battle_state_handle_map_jumping_out_state(void);
void battle_state_handle_menu_to_targeting_state(void);
void battle_state_handle_mini_menu_help_state(void);
void battle_state_handle_mini_menu_state(void);
void battle_state_handle_move_confirm_menu_state(void);
void battle_state_handle_move_range_exception_state(void);
void battle_state_handle_open_action_menus_state(void);
void battle_state_handle_open_sp2_files_state(void);
void battle_state_handle_pre_attack_animation_state(void);
void battle_state_handle_resume_attack_phase_state(void);
void battle_state_handle_secondary_effect_state(void);
void battle_state_handle_start_effect_file_open_state(void);
void battle_state_handle_status_execute_state(void);
void battle_state_handle_target_display_start_state(void);
void battle_state_handle_target_display_state(void);
void battle_state_handle_target_select_confirm_state(void);
void battle_state_handle_target_select_denied_state(void);
void battle_state_handle_target_select_start_state(void);
void battle_state_handle_target_select_state(void);
void battle_state_handle_targeting_range_state(void);
void battle_state_handle_unit_move_state(void);
void battle_state_handle_unit_moving_setup_state(void);
void battle_state_handle_unit_moving_state(void);
void battle_state_handle_wait_direction_state(void);
void battle_state_handle_wait_menu_state(void);
void battle_state_restart_menu_to_targeting(void);
void battle_state_run_game_loop(void);
void battle_state_start_close_battle(s32 duration);
void battle_state_start_game_flow(void);
void battle_state_stop_game_flow(void);
s32 battle_state_sync_frame(u32* ordering_table);
s32 battle_state_update_controller_input(void);
s32 battle_state_announce_next_charged_action(void);
void battle_state_enter_target_display_start(void);
void battle_state_enter_target_select_start(void);
s32 battle_state_get_animation_continue_check(void);
void battle_state_halve_animation_speed_and_queue_close(s32 transition_step, s32 close_flow_state);
void battle_state_init_deployment_display(s32 width, s32 height, s32 projection, u8 red, u8 green, u8 blue);
s32 battle_state_set_animation_speed(s32 speed);
void battle_state_set_free_cursor(void);
void battle_state_set_time_scale(s32 value);
void battle_state_start_battle_message_display(void);
void battle_state_start_change_map_jump_in(s32 duration);
void battle_state_start_map_jump_out(s32 map_id, s32 duration);
void battle_state_start_change_map_jump_out(s32 map_id, s32 duration);
s32 battle_state_sync_and_submit_deployment_frame(u32* ordering_table);
s32 battle_state_update_deployment_controller_input(void);
void battle_state_enter_unit_moving_setup(void);
void battle_script_report_missing_unit_stub(s32 unused_unit_id);
void battle_script_opcode_74_event_instruction(const u8* unused_parameters);

/* thread */
/* 0x8014ceb4..0x8014cf58 forwards a0-a3 to the callback in 0x80173ca8,
 * using thread 0's stack when necessary. It does not copy stacked arguments
 * across that stack switch; callers must not assume they are transferred.
 * The unspecified argument list is intentional, not a missing prototype. */
extern void (*g_battle_thread_call_target)(void);

/* Full BATTLE backing array at 0x8016986c. The pointer slot 0x80165f98 is
 * separate. */
extern native_thread_t g_battle_thread_contexts[];

/* View of g_battle_thread_contexts biased to task_id (0x801698b8): [i][0] is
 * thread i's task_id and [i][1] its task_words[0]. Target code addresses the
 * biased symbol directly, so it must not be rebound to the context base. The
 * WORLD twin is g_world_thread_task_ids. */
extern s32 g_battle_thread_task_ids[][256];

/* BATTLE's pointer slot at 0x80165f98, set to g_battle_thread_contexts by
 * battle_menu_init_subsystem_pointers. Every access indexes it with a 0x400
 * stride, and the scheduler primitives reach is_running (0x48) and task_id
 * (0x4c) through it, so it is a native_thread_t*, not the u8* placeholder it
 * was spelled as in 22 files. Files doing raw byte arithmetic cast (u8*) at
 * the use site. */
extern native_thread_t* g_battle_threads;
void battle_thread_wait_for_unit_view_panels(void);
s32 battle_thread_call_on_main_stack();
void battle_thread_exit_current(void);
s32 battle_thread_find_running_by_task(s32 task_id);
void* battle_thread_get_current_global_pointer(void);
s32 battle_thread_get_current_parameter_1(void);
s32 battle_thread_get_current_parameter_2(void);
s32 battle_thread_get_current_parameter_3(void);
s32 battle_thread_get_current_task_id(void);
s32 battle_thread_is_previous_running(void);
s32 battle_thread_is_running(s32 thread_id);
s32 battle_thread_is_running_asm(s32 thread_id);
void battle_thread_reset_scheduler(void);
s32 battle_thread_resolve_id(s32 requested_thread_id);
s32 battle_thread_resolve_id_after_current(s32 requested_thread_id);
void battle_thread_resume(s32 thread_id);
void battle_thread_set_current_task_id(s32 task_id);
void battle_thread_set_parameters(s32 thread_id, s32 first, s32 second, s32 third);
void battle_thread_set_four_parameters(s32 thread_id, s32 first, s32 second, s32 third, s32 fourth);
void battle_thread_start(s32 thread_id, void (*function)(void));
void battle_thread_suspend(s32 thread_id);
void battle_thread_wait_frames(s32 count);
void battle_thread_wait_until_inactive(s32 thread_id);

/* 0x8014ca80..0x8014cbb4: save this native context and resume the next
 * active one. Shared state can change before this caller resumes. */
void battle_thread_yield(void);

/* screen */
typedef struct battle_action_reward_display {
    u8 earned_experience;     /* +0x0 */
    u8 earned_jp;             /* +0x1 */
    u8 level_for_display;     /* +0x2; nonzero selects the "Level UP!" display */
    u8 job_level_for_display; /* +0x3 */
} battle_action_reward_display_t;
typedef char battle_action_reward_display_size_must_be_4[(sizeof(battle_action_reward_display_t) == 4) ? 1 : -1];

enum {
    BATTLE_ACTION_STATUS_DISPLAY_REMOVAL = 0x80,
    BATTLE_ACTION_STATUS_DISPLAY_IMAGE_MASK = 0x7f,
    BATTLE_ACTION_STATUS_DISPLAY_CAPACITY = 0x1b,
};

typedef union battle_move_destination {
    u32 word;
    struct {
        u8 destination_x;
        u8 destination_y;
        u8 destination_z;
        u8 motion_flags;
    } bytes;
} battle_move_destination_t;

/* One frame of an SHP file: a 2-byte header followed by 4-byte parts, each a
 * rectangle of the sheet and where it is drawn (battle_gfx_load_trap_and_unit_frame_parts). */
typedef struct battle_gfx_source_part {
    s8 x_shift; /* pixels from the sprite's origin to the part's left and top edges */
    s8 y_shift;
    u16 attributes; /* BATTLE_SHP_PART_* masks */
} battle_gfx_source_part_t;

/* Encoded fields of battle_gfx_source_part_t.attributes. */
enum {
    BATTLE_SHP_PART_TILE_MASK = 0x03ff, /* the picture's place in the sheet in 8-pixel tiles: bits 0-4 u, 5-9 v */
    BATTLE_SHP_PART_SIZE_MASK = 0x3c00, /* bits 10-13: index of g_battle_gfx_part_sizes */
    BATTLE_SHP_PART_FLIP_MASK = 0xc000, /* shifted down 13: BATTLE_MISC_DISPLAY_HORIZONTAL_FLIP and _VERTICAL_FLIP */
};

/* The part size that a unit sheet draws as two 48x24 halves, the upper one nearer the camera. */
enum {
    BATTLE_SHP_PART_SIZE_48X48 = 14,
};

typedef struct battle_gfx_source_frame {
    /* Bits 0-2 are the part count minus one, bits 3-7 index the rotation
     * table g_battle_gfx_sprite_y_rotations (0x80094508). */
    u8 part_count_and_rotation;
    u8 flags; /* the BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT and _BLEND_MASK bits of every part */
    battle_gfx_source_part_t parts[1];
} battle_gfx_source_frame_t;

/* WEP/EFF SHP data: the first frame of each weapon type, then the frame
 * pointers from 0x40. */
typedef struct battle_gfx_weapon_shp {
    u16 first_frame[0x20];                    /* 0x000; indexed by weapon type */
    battle_gfx_source_frame_t* frames[0x200]; /* 0x040 */
} battle_gfx_weapon_shp_t;
typedef char battle_gfx_weapon_shp_size_must_be_0x840[sizeof(battle_gfx_weapon_shp_t) == 0x840 ? 1 : -1];

/* Native WEP/EFF storage pairs the SHP frame table with its SEQ pointers. */
typedef struct battle_gfx_wep_eff_graphics {
    battle_gfx_weapon_shp_t shp; /* 0x000 */
    u8* seq[0x100];              /* 0x840; sequence command streams */
} battle_gfx_wep_eff_graphics_t;
typedef char battle_gfx_wep_eff_graphics_size_must_be_0xc40[sizeof(battle_gfx_wep_eff_graphics_t) == 0xc40 ? 1 : -1];

/* Texture rectangle and screen offset used by menu overlay graphics. */
typedef struct battle_gfx_sprite_region {
    s16 u;      /* 0x00 */
    s16 v;      /* 0x02 */
    s16 width;  /* 0x04 */
    s16 height; /* 0x06 */
    s16 x;      /* 0x08 */
    s16 y;      /* 0x0a */
} battle_gfx_sprite_region_t;
typedef char battle_gfx_sprite_region_size_must_be_12[sizeof(battle_gfx_sprite_region_t) == 12 ? 1 : -1];

/* One part of a sprite display: a rectangle of the display's texture page,
 * in pixels, and its place relative to the sprite's origin. */
typedef struct battle_gfx_sprite_part_display_data {
    s8 x_shift;
    s8 y_shift;
    u8 width;
    u8 height;
    u8 u;
    u8 v;
    u8 flags; /* battle_misc_display_flags_e and battle_misc_display_blend_mode_e */
} battle_gfx_sprite_part_display_data_t;
typedef char
    battle_sprite_part_display_data_size_must_be_7[(sizeof(battle_gfx_sprite_part_display_data_t) == 7) ? 1 : -1];

/* A sprite display: texture page, CLUT, colour and the parts cut from it.
 * Renderer APIs use this common header and variable part tail; the fixed
 * storage records below describe each slot inside a 0x440-byte misc record. */
typedef struct battle_gfx_sprite_display_data {
    u8 red;        /* 0x00 */
    u8 green;      /* 0x01 */
    u8 blue;       /* 0x02 */
    u8 part_count; /* 0x03; parts in use */
    s16 tpage;     /* 0x04; BATTLE_GFX_TPAGE_*, with battle_misc_display_blend_mode_e */
    u16 clut;      /* 0x06 */
    u16 scale_x;   /* 0x08; ONE is neutral scale */
    u16 scale_y;   /* 0x0a; ONE is neutral scale */
    /* 0x0c; turns the sprite about its origin, ONE to the turn. Each frame
     * sets it from its header; BATTLE_SEQ_OP_SET_ROTATION and _CLEAR_ROTATION
     * then replace it. */
    u16 rotation;
    battle_gfx_sprite_part_display_data_t parts[0]; /* 0x0e; variable tail */
} battle_gfx_sprite_display_data_t;
typedef char
    battle_gfx_sprite_display_data_size_must_be_0x0e[sizeof(battle_gfx_sprite_display_data_t) == 0x0e ? 1 : -1];

typedef struct battle_gfx_unit_sprite_display {
    battle_gfx_sprite_display_data_t header;        /* 0x00 */
    battle_gfx_sprite_part_display_data_t parts[8]; /* 0x0e */
} battle_gfx_unit_sprite_display_t;
typedef char
    battle_gfx_unit_sprite_display_size_must_be_0x46[sizeof(battle_gfx_unit_sprite_display_t) == 0x46 ? 1 : -1];

typedef struct battle_gfx_weapon_sprite_display {
    battle_gfx_sprite_display_data_t header;        /* 0x00 */
    battle_gfx_sprite_part_display_data_t parts[3]; /* 0x0e */
    u8 _padding_23;                                 /* aligns the following display */
} battle_gfx_weapon_sprite_display_t;
typedef char
    battle_gfx_weapon_sprite_display_size_must_be_0x24[sizeof(battle_gfx_weapon_sprite_display_t) == 0x24 ? 1 : -1];

typedef struct battle_gfx_single_part_sprite_display {
    battle_gfx_sprite_display_data_t header;        /* 0x00 */
    battle_gfx_sprite_part_display_data_t parts[1]; /* 0x0e */
    u8 _padding_15;                                 /* aligns the following display */
} battle_gfx_single_part_sprite_display_t;
typedef char battle_gfx_single_part_sprite_display_size_must_be_0x16
    [sizeof(battle_gfx_single_part_sprite_display_t) == 0x16 ? 1 : -1];

/* Weapon/effect sprite animation block, misc unit data 0x208, 0x238 and
 * 0x268: block 0 plays the weapon sheet's sequences, blocks 1 and 2 the
 * effect sheet's. It shares the first 0x1c bytes of battle_unit_anim_state_t,
 * which has their meanings, but keeps its display pointer at 0x24
 * (battle_gfx_run_wep_eff_seq_script, battle_gfx_draw_unit_sprite_layers,
 * battle_unit_init_misc_data). Its SEQ table has no header: seq[sequence_id]
 * is the sequence. */
typedef struct battle_unit_sprite_block {
    u16 trigger;                               /* 0x00; nonzero while the sprite is drawn */
    u16 graphic_type;                          /* 0x02; battle_sprite_graphic_type_e */
    u16 sequence_id;                           /* 0x04 */
    u16 script_pos;                            /* 0x06 */
    u16 frame;                                 /* 0x08; includes the weapon type's first frame */
    u16 wait;                                  /* 0x0a */
    u16 restart_count;                         /* 0x0c */
    u16 saved_sequence_id;                     /* 0x0e */
    u16 saved_script_pos;                      /* 0x10 */
    u16 wait_bias;                             /* 0x12 */
    u16 frame_bias;                            /* 0x14 */
    u16 repeat_count;                          /* 0x16 */
    u16 flags;                                 /* 0x18 */
    u8 _padding_1a[2];                         /* 0x1a; aligns shp */
    battle_gfx_weapon_shp_t* shp;              /* 0x1c */
    u8** seq;                                  /* 0x20 */
    battle_gfx_sprite_display_data_t* display; /* 0x24 */
    u8 _unused_28[8];                          /* 0x28 */
} battle_unit_sprite_block_t;
typedef char battle_unit_sprite_block_size_must_be_0x30[(sizeof(battle_unit_sprite_block_t) == 0x30) ? 1 : -1];

/* Manual targeting and WAIT_DIRECTION reuse the AI command storage.
 * The facing word overlaps the action's item/type/target-ID bytes. Cursor
 * coordinate readers use signed halfwords over the action's target fields. */
typedef union battle_unit_command_state {
    battle_ai_command_t ai;
    struct {
        u8 _unused_00[12];
        s32 facing_hint;
        SVECTOR target_panel; /* vx = panel x, vy = map level, vz = panel y */
    } cursor;
} battle_unit_command_state_t;
typedef char battle_misc_command_state_size_must_be_24[sizeof(battle_unit_command_state_t) == 24 ? 1 : -1];

/* g_battle_entd_selection_mode: which ENTD battle_unit_select_and_open_entd opens. */
typedef enum battle_entd_selection {
    BATTLE_ENTD_SELECTION_SCRIPT_VARIABLE = 0, /* EVENT_SCRIPT_VAR_CURRENT_ENTD */
    BATTLE_ENTD_SELECTION_ENTD_250 = 1,
    BATTLE_ENTD_SELECTION_RANDOM_75_TO_79 = 2,
    BATTLE_ENTD_SELECTION_RANDOM_1_TO_59 = 3,
    BATTLE_ENTD_SELECTION_MAP_ID = 4, /* the map's own id when it is not 0 */
} battle_entd_selection_e;

/* graphic_type of a unit's sprite blocks: which sheet the block animates. */
typedef enum battle_sprite_graphic_type {
    BATTLE_SPRITE_GRAPHIC_UNIT = 0,
    BATTLE_SPRITE_GRAPHIC_WEAPON = 1, /* WEP sheets */
    BATTLE_SPRITE_GRAPHIC_EFFECT = 2, /* EFF sheets */
} battle_sprite_graphic_type_e;

/* battle_walk_path_t.pending_mount: the mount standing on the path's destination tile. */
enum {
    BATTLE_PENDING_MOUNT_ID_MASK = 0x7f,
    BATTLE_PENDING_MOUNT_SET = 0x80,
};

/* Unit SHP frame tables filled by battle_gfx_unpack_unit_shp_data (0x800873bc):
 * SHP header bytes 4..5 and 6..7, then two 0xd0-entry frame pointer tables,
 * each entry a battle_gfx_source_frame_t. The second table draws the same
 * frames without the legs, for a unit standing in water or riding
 * (battle_gfx_select_unit_shp_frame); it repeats the first when the file
 * carries a single frame set (header 8). */
typedef struct battle_gfx_unit_shp_frame_tables {
    s32 first_attack_frame;                  /* 0x000; frames from here up are drawn from the sheet's attack image */
    s32 first_sp2_frame;                     /* 0x004; set from the file and never read */
    battle_gfx_source_frame_t* frames[0xd0]; /* 0x008 */
    battle_gfx_source_frame_t* submerged_frames[0xd0]; /* 0x348 */
} battle_gfx_unit_shp_frame_tables_t;
typedef char
    battle_gfx_unit_shp_frame_tables_size_must_be_0x688[sizeof(battle_gfx_unit_shp_frame_tables_t) == 0x688 ? 1 : -1];

/* Block view of the numeric display fields at misc unit data 0x2bc..0x2cf,
 * taken as one pointer by the action result display builders
 * (battle_gfx_build_next_action_result_display, 0x800808b8, battle_gfx_animate_post_action_text). */
typedef struct battle_action_result_display {
    u8 active;                                     /* 0x00 */
    u8 _padding_01;                                /* 0x01; aligns selector */
    u16 selector;                                  /* 0x02; battle_numeric_display_selector_e */
    u16 value;                                     /* 0x04 */
    u16 progress;                                  /* 0x06 */
    battle_gfx_sprite_display_data_t* displays[3]; /* 0x08 */
} battle_action_result_display_t;
typedef char battle_action_result_display_size_must_be_0x14[(sizeof(battle_action_result_display_t) == 0x14) ? 1 : -1];

/* Common header of an unpacked unit SEQ table. The pointer tail has 0x100
 * entries in a native table and 0x40 in an EVTCHR slot. */
typedef struct battle_gfx_unit_seq_table {
    s32 first_attack_sequence; /* 0x00; sequences from here use the attack image */
    s32 first_sp2_sequence;    /* 0x04; sequences from here use the SP2 file */
    u8* sequences[0];          /* 0x08; variable pointer tail */
} battle_gfx_unit_seq_table_t;
typedef char battle_gfx_unit_seq_table_size_must_be_8[sizeof(battle_gfx_unit_seq_table_t) == 8 ? 1 : -1];

/* Native table storage: five resident tables and the separate boss table.
 * Their sequences live in the shared sequence-data buffer. */
typedef struct battle_gfx_native_unit_seq_table {
    battle_gfx_unit_seq_table_t header; /* 0x000 */
    u8* sequences[0x100];               /* 0x008 */
} battle_gfx_native_unit_seq_table_t;
typedef char
    battle_gfx_native_unit_seq_table_size_must_be_0x408[sizeof(battle_gfx_native_unit_seq_table_t) == 0x408 ? 1 : -1];

/* Unit SPR file: 16 palettes, two uncompressed 4bpp image blocks, then
 * a variable-length compressed attack-image stream. */
typedef struct battle_gfx_unit_sprite_file {
    u8 palettes[0x200];                   /* 0x0000; little-endian color bytes */
    u8 pixels[256][128];                  /* 0x0200; 256 x 256 pixels, two per byte */
    u8 portrait_and_body_pixels[32][128]; /* 0x8200; 256 x 32 pixels, two per byte */
    u8 compressed_attack_image[0];        /* 0x9200; variable byte tail */
} battle_gfx_unit_sprite_file_t;
typedef char battle_gfx_unit_sprite_file_size_must_be_0x9200[sizeof(battle_gfx_unit_sprite_file_t) == 0x9200 ? 1 : -1];

/* Packed unit SEQ file. Values and offsets are little-endian byte
 * sequences, so this view needs no native halfword or word alignment. */
typedef struct battle_gfx_packed_seq_record {
    u8 first_attack_sequence[2];   /* 0x000; unsigned little-endian halfword */
    u8 first_sp2_sequence[2];      /* 0x002; unsigned little-endian halfword */
    u8 sequence_offsets[0x100][4]; /* 0x004; signed little-endian words, -1 means offset zero */
    u8 sequence_data_size[2];      /* 0x404; unsigned little-endian halfword */
    u8 sequence_data[0];           /* 0x406; variable byte tail */
} battle_gfx_packed_seq_record_t;
typedef char battle_gfx_packed_seq_record_size_must_be_0x406[sizeof(battle_gfx_packed_seq_record_t) == 0x406 ? 1 : -1];

/* Unit sprite animation state passed to the sequence and frame renderers.
 * Weapon/effect sprite blocks share its first 0x1c bytes, with distinct tails. */
typedef struct battle_unit_anim_state {
    u16 trigger;      /* 0x00; nonzero while the sprite is drawn */
    u16 graphic_type; /* 0x02; battle_sprite_graphic_type_e */
    /* 0x04; index of the sequence in the SEQ table. A unit's is its
     * battle_unit_animation_e * 2, + 1 when it faces away from the camera;
     * from BATTLE_SEQUENCE_EVTCHR_SLOT_0 up it is a sequence of an EVTCHR slot. */
    u16 sequence_id;
    u16 script_pos;        /* 0x06; offset of the next byte to read in the sequence */
    u16 frame;             /* 0x08; SHP frame on display */
    u16 wait;              /* 0x0a; frames until the sequence is read again; 0 when it has stopped */
    u16 restart_count;     /* 0x0c; BATTLE_SEQ_OP_RESTART commands run since the sequence started */
    u16 saved_sequence_id; /* 0x0e; where BATTLE_SEQ_OP_RETURN resumes */
    u16 saved_script_pos;  /* 0x10 */
    u16 wait_bias;         /* 0x12; BATTLE_SEQ_OP_SET_WAIT_BIAS: added to each wait read */
    u16 frame_bias;        /* 0x14; BATTLE_SEQ_OP_SET_FRAME_BIAS: added to each frame id read */
    u16 repeat_count;      /* 0x16; jumps the BATTLE_SEQ_OP_REPEAT in progress has left; 0 when none is */
    u16 flags;             /* 0x18; the flip bits of battle_misc_display_flags_e that the sequence toggled */
    u8 _padding_1a[2];     /* 0x1a; aligns shp */
    battle_gfx_unit_shp_frame_tables_t* shp;       /* 0x1c */
    battle_gfx_unit_seq_table_t* seq;              /* 0x20 */
    battle_gfx_unit_shp_frame_tables_t* saved_shp; /* 0x24; the sheet's own tables while a status sheet replaces them */
    battle_gfx_unit_seq_table_t* saved_seq;        /* 0x28 */
    battle_gfx_sprite_display_data_t* display;     /* 0x2c */
} battle_unit_anim_state_t;
typedef char battle_unit_anim_state_size_must_be_0x30[sizeof(battle_unit_anim_state_t) == 0x30 ? 1 : -1];
typedef char
    battle_unit_anim_state_frame_must_be_0x08[((unsigned long)&((battle_unit_anim_state_t*)0)->frame == 0x08) ? 1 : -1];
typedef char
    battle_unit_anim_state_flags_must_be_0x18[((unsigned long)&((battle_unit_anim_state_t*)0)->flags == 0x18) ? 1 : -1];
typedef char
    battle_unit_anim_state_shp_must_be_0x1c[((unsigned long)&((battle_unit_anim_state_t*)0)->shp == 0x1c) ? 1 : -1];
typedef char
    battle_unit_anim_state_display_must_be_0x2c[((unsigned long)&((battle_unit_anim_state_t*)0)->display == 0x2c) ? 1
                                                                                                                  : -1];

/* One movement step: a path element, the unit's current step and the step
 * the movement code is working on. */
typedef union battle_move_step {
    u8 value;
    struct {
        u8 length : 2;    /* extra tiles covered: the jump length */
        u8 slow : 1;      /* start the step at climb speed (battle_move_update_path_step) */
        u8 climb : 1;     /* continue with a climb-speed step */
        u8 mount : 1;     /* 0x10: the step ends on another unit */
        u8 higher : 1;    /* 0x20: destination on the higher elevation */
        u8 direction : 2; /* battle_move_step_direction_e */
    } bits;
} battle_move_step_t;

/* Packed half-height offsets at 0x8018f86e. A missing presence flag preserves
 * that side's previous value when applying a saved candidate at 0x80177b64. */
typedef union battle_move_height_offsets {
    u8 byte;
    struct {
        u8 destination_present : 1;
        u8 destination_height_offset : 3;
        u8 source_present : 1;
        u8 source_height_offset : 3;
    } bits;
} battle_move_height_offsets_t;
typedef char battle_movement_height_offsets_size_must_be_1[(sizeof(battle_move_height_offsets_t) == 1) ? 1 : -1];

/* Tile coordinates stored in teleport paths and relocation reservations. */
typedef struct battle_tile_coordinates {
    u8 x;
    u8 y;
    u8 elevation;
} battle_tile_coordinates_t;
typedef char battle_tile_coordinates_size_must_be_3[(sizeof(battle_tile_coordinates_t) == 3) ? 1 : -1];

/* Complete movement record copied from 0x8018f7f0 into misc +0x9c.
 * Count 0xfe selects the explicit teleport destination; 0xff is a failed
 * teleport. Ordinary counts select packed steps, each spanning one or more
 * tiles. Byte alignment preserves the original whole-record copies. */
typedef struct battle_walk_path {
    u8 count; /* 0x00; step count or BATTLE_MOVE_PATH_TELEPORT/TELEPORT_FAILED */
    union {
        battle_move_step_t steps[0x7c]; /* 0x01; packed direction, elevation and length */
        battle_tile_coordinates_t teleport;
    } data;
    u8 flags;                                    /* 0x7d; battle_move_effective_flags_e */
    battle_move_height_offsets_t height_offsets; /* 0x7e; source/destination offsets */
    u8 pending_mount;                            /* 0x7f; BATTLE_PENDING_MOUNT_SET | battle id */
} battle_walk_path_t;
typedef char battle_walk_path_size_must_be_0x80[sizeof(battle_walk_path_t) == 0x80 ? 1 : -1];
typedef char battle_walk_path_flags_must_be_0x7d[((unsigned long)&((battle_walk_path_t*)0)->flags == 0x7d) ? 1 : -1];

/* battle_walk_path_t.count values that are not a step count
 * (battle_move_build_path_to_tile). */
enum {
    BATTLE_MOVE_PATH_TELEPORT = 0xfe, /* the path bytes hold the destination x, y and level */
    BATTLE_MOVE_PATH_TELEPORT_FAILED = 0xff,
};

/* Travel direction of the step in progress, as battle_move_get_direction derives it from the unit's
 * tile and its destination tile. */
typedef enum battle_move_direction {
    BATTLE_MOVE_DIRECTION_SOUTH = 0, /* -y */
    BATTLE_MOVE_DIRECTION_EAST = 1,  /* +x */
    BATTLE_MOVE_DIRECTION_NORTH = 2, /* +y */
    BATTLE_MOVE_DIRECTION_WEST = 3,  /* -x */
} battle_move_direction_e;

/* battle_move_step_t.bits.direction (value >> 6). */
typedef enum battle_move_step_direction {
    BATTLE_MOVE_STEP_DIRECTION_EAST = 0,  /* +x */
    BATTLE_MOVE_STEP_DIRECTION_WEST = 1,  /* -x */
    BATTLE_MOVE_STEP_DIRECTION_SOUTH = 2, /* -y */
    BATTLE_MOVE_STEP_DIRECTION_NORTH = 3, /* +y */
} battle_move_step_direction_e;

/* battle_unit_misc_data_t.step_phase: the stage of the movement step in progress. Every stage has one
 * value per battle_move_direction_e direction, in that order. The handlers dispatch on the stage and
 * take the direction from the unit's coordinates, and the stores that change stage in the middle of
 * a step write the NORTH value whatever the direction. */
typedef enum battle_move_step_phase {
    BATTLE_MOVE_STEP_PHASE_NONE = 0,
    /* walking: from the tile centre to its exit edge, then on to the destination centre */
    BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_SOUTH = 0x01,
    BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_SOUTH = 0x02,
    BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_EAST = 0x03,
    BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_EAST = 0x04,
    BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_NORTH = 0x05,
    BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_NORTH = 0x06,
    BATTLE_MOVE_STEP_PHASE_WALK_TO_EDGE_WEST = 0x07,
    BATTLE_MOVE_STEP_PHASE_WALK_TO_CENTRE_WEST = 0x08,
    /* the same two stages at climb speed */
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_SOUTH = 0x09,
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_SOUTH = 0x0a,
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_EAST = 0x0b,
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_EAST = 0x0c,
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_NORTH = 0x0d,
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_NORTH = 0x0e,
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_EDGE_WEST = 0x0f,
    BATTLE_MOVE_STEP_PHASE_CLIMB_TO_CENTRE_WEST = 0x10,
    /* jumping: the take-off animation, rising, falling to the destination edge, the landing animation */
    BATTLE_MOVE_STEP_PHASE_JUMP_START_SOUTH = 0x11,
    BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_SOUTH = 0x12,
    BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_SOUTH = 0x13,
    BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_SOUTH = 0x14,
    BATTLE_MOVE_STEP_PHASE_JUMP_START_EAST = 0x15,
    BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_EAST = 0x16,
    BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_EAST = 0x17,
    BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_EAST = 0x18,
    BATTLE_MOVE_STEP_PHASE_JUMP_START_NORTH = 0x19,
    BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_NORTH = 0x1a,
    BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_NORTH = 0x1b,
    BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_NORTH = 0x1c,
    BATTLE_MOVE_STEP_PHASE_JUMP_START_WEST = 0x1d,
    BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_WEST = 0x1e,
    BATTLE_MOVE_STEP_PHASE_JUMP_DESCENT_WEST = 0x1f,
    BATTLE_MOVE_STEP_PHASE_JUMP_LANDING_WEST = 0x20,
    /* flying */
    BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_SOUTH = 0x22,
    BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_SOUTH = 0x23,
    BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_EAST = 0x24,
    BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_EAST = 0x25,
    BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_NORTH = 0x26,
    BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_NORTH = 0x27,
    BATTLE_MOVE_STEP_PHASE_FLY_TO_EDGE_WEST = 0x28,
    BATTLE_MOVE_STEP_PHASE_FLY_TO_CENTRE_WEST = 0x29,
    /* floating: the two walking stages, and the jump up to a higher tile */
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_SOUTH = 0x2b,
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_SOUTH = 0x2c,
    BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_SOUTH = 0x2d,
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_EAST = 0x2f,
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_EAST = 0x30,
    BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_EAST = 0x31,
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_NORTH = 0x33,
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_NORTH = 0x34,
    BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_NORTH = 0x35,
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_EDGE_WEST = 0x37,
    BATTLE_MOVE_STEP_PHASE_FLOAT_TO_CENTRE_WEST = 0x38,
    BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_WEST = 0x39,
    /* a teleport path (BATTLE_MOVE_PATH_TELEPORT and _FAILED): start the animation, then wait for it */
    BATTLE_MOVE_STEP_PHASE_TELEPORT_START = 0x3b,
    BATTLE_MOVE_STEP_PHASE_TELEPORT_ANIMATION = 0x3c,
} battle_move_step_phase_e;

/* battle_unit_misc_data_t.distortion_animation_id: the motion that
 * battle_unit_dispatch_distortion_animation steps once an animation frame
 * until its handler clears the id. BATTLE_SEQ_OP_START_DISTORTION gives the
 * id less 2 and, for most, the frames it takes (distortion_timer). */
typedef enum battle_distortion_animation {
    BATTLE_DISTORTION_ANIMATION_NONE = 0,
    BATTLE_DISTORTION_ANIMATION_SCALE_PULSE = 1,      /* squeezes the sprite thin and tall; nothing starts it */
    BATTLE_DISTORTION_ANIMATION_RISE = 2,             /* 0x24 up */
    BATTLE_DISTORTION_ANIMATION_SETTLE = 3,           /* back to the height of where the unit stands */
    BATTLE_DISTORTION_ANIMATION_MOVE_TO_TARGET = 4,   /* to the first target, or to the targeted panel */
    BATTLE_DISTORTION_ANIMATION_MOVE_TO_OWN_TILE = 5, /* to the centre of the unit's own tile */
    BATTLE_DISTORTION_ANIMATION_JUMP_TO_TARGET = 6,   /* the same two along a jump arc */
    BATTLE_DISTORTION_ANIMATION_JUMP_TO_OWN_TILE = 7,
    BATTLE_DISTORTION_ANIMATION_MOVE_TO_TARGET_LEVEL = 8, /* 4 and 5 without the change of height */
    BATTLE_DISTORTION_ANIMATION_MOVE_TO_OWN_TILE_LEVEL = 9,
    BATTLE_DISTORTION_ANIMATION_CLEAR = 10, /* 10 and 11 clear the id at once */
    BATTLE_DISTORTION_ANIMATION_CLEAR_2 = 11,
    BATTLE_DISTORTION_ANIMATION_JUMP_START = 12,     /* straight up to 0x120 above the ground, trailing copies */
    BATTLE_DISTORTION_ANIMATION_FALL_TO_TARGET = 13, /* down onto the target's tile */
    BATTLE_DISTORTION_ANIMATION_TELEPORT = 14,       /* fade out, move to the step's destination, fade in */
    BATTLE_DISTORTION_ANIMATION_BACK_AWAY = 15,      /* part of a tile away from the target */
    BATTLE_DISTORTION_ANIMATION_JUMP_TO_OWN_TILE_FADED = 16, /* 7, blended, and dark until four frames remain */
    BATTLE_DISTORTION_ANIMATION_JUMP_RISE = 17, /* 12 with the jump sound, unless the unit is that high already */
} battle_distortion_animation_e;

enum {
    BATTLE_DISTORTION_ANIMATION_COUNT = 18, /* the handlers of g_battle_unit_distortion_animation_handlers */
};

/* The strike work record is embedded in Miscellaneous Unit Data at +0x18c
 * and used as a standalone AI work record. Reaction targeting writes its
 * redirected tile at +0x1c..+0x1e before the strike is processed. */
typedef struct battle_strike_work {
    u8 actor_id;
    u8 target_count;
    u8 target_list[16];
    u8 skip_empty_tile_effect;
    u8 math_skill_flag;
    s16 last_attack_id; /* 0x14 */
    u8 post_action_message;
    u8 reaction_occurred;
    u8 continue_attack;
    u8 current_hit_number;
    s16 reaction_id;
    u8 target_new_x;
    u8 target_new_y;
    u8 target_new_map_level;
    u8 used_weapon_id;
    s16 reaction_ability_id; /* 0x20; misc 0x1ac */
    /* 0x22; misc 0x1ae. g_battle_action_current_ability.knockback_flags & 0x7f, stored with
     * the knockback destination. */
    u8 knockback_flags;
    u8 can_earn_experience;
    battle_action_reward_display_t action_rewards; /* 0x24; misc 0x1b0..0x1b3 */
} battle_strike_work_t;
typedef char battle_strike_work_must_be_40[sizeof(battle_strike_work_t) == 40 ? 1 : -1];
typedef char battle_strike_targets_must_be_2[((unsigned long)&((battle_strike_work_t*)0)->target_list == 2) ? 1 : -1];
typedef char
    battle_strike_continue_must_be_0x18[((unsigned long)&((battle_strike_work_t*)0)->continue_attack == 0x18) ? 1 : -1];

/* Misc unit_t Data has 16 renderer-side slots, each 0x440 bytes.
 * The record links to its simulation-side battle_stats_t at 0x134. */
typedef struct battle_unit_misc_data {
    struct battle_unit_misc_data* previous; /* 0x000; previous Misc unit_t Data */
    u8 unit_id;                             /* 0x004; Misc unit_t Data ID */
    u8 spritesheet_vram_slot;               /* 0x005 */
    u8 spritesheet_id;                      /* 0x006 */
    u8 stored_palette;                      /* 0x007; the palette of its sheet that the unit wears, 0 to 5 */
    u16 state_frame_counter;                /* 0x008; incremented while renderer action states wait */
    u16 draw_enabled;                       /* 0x00a; set by event Draw, cleared by Erase; a halfword */
    u16 requested_animation;                /* 0x00c; battle_unit_animation_e + 1 to start at the next update; 0 none */
    u16 tpage;                              /* 0x00e; VRAM slot + 0x14: the sheet's texture page */
    u16 clut;                               /* 0x010 */
    /* battle_misc_display_flags_e and battle_misc_display_blend_mode_e.
     * Read-modify-written as a halfword over
     * 0x012..0x013 (battle_gfx_update_sprite_transparency_flag). */
    u16 sprite_display_flags; /* 0x012 */
    /* 0x014; BATTLE_SEQ_OP_SET_LAYER_PRIORITY: the row of g_battle_gfx_animation_layer_priorities. */
    u16 layer_priority;
    u8 _padding_016[2]; /* aligns real */
    VECTOR real;        /* 0x018; vx = x, vy = height, vz = map depth */
    /* Per-frame real-coordinate velocity; the jump-start distortion animation
     * (0x8008a35c) copies all four words as one block. */
    VECTOR velocity; /* 0x028 */
    /* Movement interpolation speed, stored and passed as a signed word. */
    s32 walk_speed; /* 0x038 */
    s32 step_speed; /* 0x03c; interpolation speed of the current step; 0x2000 when a move/climb starts, raised by
                       jump gravity */
    SVECTOR screen; /* 0x040; vx = x, vy = height, vz = map depth */
    u8 _unused_048[8];
    /* The sprite is drawn at screen + event_offset + seq_offset
     * (battle_unit_copy_misc_data_xyz_values); both offsets are in the units
     * of screen (vx = x, vy = height, vz = map depth). */
    SVECTOR seq_offset; /* 0x050; the shift of the running sequence's move commands; cleared when one starts */
    /* Added to the projected sprite position, in pixels, by
     * BATTLE_SEQ_OP_MOVE_SCREEN_OFFSET; cleared when a sequence starts and,
     * with the other two offsets, by battle_unit_place_in_new_location. vz
     * is only ever cleared. */
    SVECTOR screen_offset; /* 0x058 */
    SVECTOR event_offset;  /* 0x060; accumulated by the event SpriteMove instructions */
    u8 _unused_068[4];
    /* Camera-relative facing, ((camera yaw + facing) & 0xfff) / 1024 and / 256.
     * Initialized to -1 and copied from mount to rider. */
    s16 camera_facing_quadrant;   /* 0x06c */
    s16 camera_facing_sixteenth;  /* 0x06e */
    s16 facing;                   /* 0x070 */
    s16 attack_facing;            /* 0x072 */
    s16 depth_height_offset;      /* 0x074 */
    u16 float_bob_offset;         /* 0x076; from the float bob table */
    u16 float_bob_phase;          /* 0x078; advanced by g_main_system_animation_speed at 0x8007ea98 */
    u16 special_graphic_y_offset; /* 0x07a; added to the v of each part: a status sheet's rows in OTHER.SPR */
    u8 map_x;                     /* 0x07c */
    u8 map_y;                     /* 0x07d */
    u8 map_z;                     /* 0x07e */
    u8 step_phase;                /* 0x07f; battle_move_step_phase_e */
    battle_move_destination_t movement;
    u8 previous_map_x;          /* 0x084 */
    u8 previous_map_y;          /* 0x085 */
    u8 previous_map_z;          /* 0x086 */
    u8 distortion_animation_id; /* 0x087; battle_distortion_animation_e */
    s32 distortion_phase;       /* 0x088; the handler's own step, 0 when the distortion animation starts */
    s32 distortion_timer;       /* 0x08c; frames left: the second operand of BATTLE_SEQ_OP_START_DISTORTION */
    s32 distortion_target;      /* 0x090; target real.vy or frame threshold of the distortion step */
    u16 previous_facing;        /* 0x094 */
    u8 destination_edge_height; /* 0x096; destination slope corner, as g_battle_move_destination_edge_height */
    u8 current_edge_height;     /* 0x097; current slope corner, as g_battle_move_current_edge_height */
    u32 movement_path_offset;   /* 0x098; index of the next packed path step */
    battle_walk_path_t path;    /* 0x09c..0x11b */
    battle_move_step_t current_path_step;     /* 0x11c; path step byte being executed */
    u8 last_path_count;                       /* 0x11d; receives prior movement-path count at 0x8006d7b8 */
    u8 standing_on_unit_id_plus_one;          /* 0x11e; unit stood on, 0 = none */
    u8 previous_standing_on_unit_id_plus_one; /* 0x11f; restored when a move is undone */
    s16 camera_relative_x;                    /* 0x120; camera-space x from RotTrans */
    s16 camera_relative_y;                    /* 0x122 */
    u8 _unused_124[4];
    u32 otag_depth_index;              /* 0x128 */
    s32 gte_flag;                      /* 0x12c; RotTrans flag output of the unit and status bubble transforms */
    u8 mount_state;                    /* 0x130; battle_misc_mount_state_e */
    u8 mount_partner_misc_id;          /* 0x131; linked rider or mount misc ID */
    u8 previous_mount_state;           /* 0x132; battle_misc_mount_state_e */
    u8 previous_mount_partner_misc_id; /* 0x133 */
    battle_stats_t* battle_data;       /* 0x134; battle_stats_t backlink */
    u16 used_ability_id;               /* 0x138 */
    u8 used_item_or_weapon_id;         /* 0x13a; the item the weapon and effect blocks draw */
    u8 equipped_weapon_type;           /* 0x13b; its item type, or 0x15 thrown: the frame set of the weapon SHP */
    u8 turn_action_flags;              /* 0x13c; |= 1 moved, |= 2 acted this turn */
    u8 team_flags;                     /* 0x13d */
    u8 fade_out_phase;                 /* 0x13e; 0 none, 1 flash, 2 darken, 3 removed */
    u8 horizontal_flip_flag;           /* 0x13f; BATTLE_MISC_DISPLAY_HORIZONTAL_FLIP or 0: the event MirrorSprite */
    u32 status_flags_1_4;              /* 0x140; battle_misc_status_flags_1_4_e */
    u32 status_flags_5_6;              /* 0x144; battle_misc_status_flags_5_6_e */
    /* 0x148..0x157: statuses to add/remove;
     * battle_unit_update_float_and_jump_height tests the pending removal of Jump. */
    u32 statuses_to_add_1_4;                   /* 0x148 */
    u32 statuses_to_add_5_6;                   /* 0x14c */
    u32 statuses_to_remove_1_4;                /* 0x150 */
    u32 statuses_to_remove_5_6;                /* 0x154 */
    battle_unit_command_state_t command_state; /* 0x158..0x16f */
    s32 sp2_ability_id;                        /* 0x170: ability whose SP2 file battle_open_sp2 loads */
    s32 ability_preview_phase;                 /* 0x174: battle_target_panels_e */
    /* 0x178; battle_target_calculate_for_menu_types result stored by
     * battle_target_select_tile: 0/1 preview, 2 stop, -1 back to the menu. */
    s32 target_select_result;
    s32 attack_phase_state;    /* 0x17c: battle_attack_preparation_e */
    s32 pending_attack_result; /* 0x180: attack result code; -1 signals death */
    /* Death-by-dismount destination (Miscellaneous unit_t Data 0x184/0x186/
     * 0x188); battle_unit_set_map_coords_after_death_dismount copies them to
     * map_x/map_z/map_y.  attack_result_animation_update passes &dismount. */
    unit_tile_coordinates_t dismount; /* 0x184 */
    u8 _unused_18a[2];
    battle_strike_work_t strike; /* 0x18c..0x1b3; current action targets, result and rewards */
    u8 command_ready;            /* 0x1b4; 0x800753b8 sets 1 after AI setup completes; gates command dispatch */
    /* 0x1b5; strikes run after the first while continue_attack is set; zeroed
     * for each new action. */
    u8 continue_attack_count;
    /* 0x1b6; the auto-battle command kind stored before target select (the
     * TARGETED_SELECT kind requires an enemy, RECOVERY_SELECT an ally) or target
     * display (the two DISPLAY kinds). */
    u8 target_select_command;
    u8 status_display_image_count; /* 0x1b7; capped at 0x1b */
    u32 action_display_flags;      /* 0x1b8..0x1bb */
    /* Action-display image IDs, not canonical status IDs.  Bit 0x80 marks a
     * removal; the dispatcher consumes entries from the end. */
    u8 status_display_image_list[BATTLE_ACTION_STATUS_DISPLAY_CAPACITY]; /* 0x1bc..0x1d6 */
    u8 _padding_1d7;                                                     /* aligns animation */
    battle_unit_anim_state_t animation;                                  /* 0x1d8..0x207 */
    battle_unit_sprite_block_t sprite_blocks[3];                         /* 0x208..0x297 */
    u8 shadow_graphic_trigger;                                           /* 0x298; nonzero: draw the unit's shadow */
    u8 shadow_dirty;        /* 0x299; bit 0 set when a step lands, cleared on shadow redraw */
    u8 shadow_counter;      /* 0x29a; shadow animation counter (battle_gfx_draw_unit_shadow) */
    u8 _padding_29b;        /* aligns shadow_quad */
    SVECTOR shadow_quad[4]; /* 0x29c; shadow corners written by battle_gfx_calculate_sprite_shadow_from_tile_slope */
    battle_action_result_display_t numeric_display; /* 0x2bc..0x2cf */
    u8 item_display_active;                         /* 0x2d0; BATTLE_SEQ_OP_SHOW_ITEM: draw item_display */
    u8 _unused_2d1;                                 /* 0x2d1 */
    s8 item_get_x_offset;                           /* 0x2d2; BATTLE_SEQ_OP_SET_ITEM_POSITION, in pixels */
    s8 item_get_y_offset;                           /* 0x2d3 */
    u8 _unused_2d4[4];
    battle_gfx_sprite_display_data_t* item_display; /* 0x2d8; item_t/Equip display pointer */
    /* Status bubble (0x2dc..0x2e7);
     * battle_gfx_update_status_bubble_graphic_trigger sets the flag and clears
     * the timer with a halfword store, so the timer is declared u16. */
    u8 status_bubble_active;                                 /* 0x2dc */
    u8 status_bubble_id;                                     /* 0x2dd */
    s8 status_bubble_x;                                      /* 0x2de */
    s8 status_bubble_y;                                      /* 0x2df */
    u16 status_bubble_timer;                                 /* 0x2e0 */
    u16 status_bubble_alternate_row;                         /* 0x2e2; non-zero selects the second texture row */
    battle_gfx_sprite_display_data_t* status_bubble_display; /* 0x2e4 */
    /* 0x2e8; enables the six-copy position trail seeded by
     * battle_gfx_init_position_vector_copies. */
    u32 position_copies_active;
    /* Six vectors used during sprite rotation/scaling render (0x2ec);
     * battle_gfx_init_position_vector_copies seeds all six from screen_x/z/y. */
    SVECTOR position_copies[6]; /* 0x2ec..0x31b */
    /* Fixed sprite-display storage; API pointers address each header. */
    battle_gfx_unit_sprite_display_t unit_sprite_display;                 /* 0x31c; 8 parts */
    battle_gfx_weapon_sprite_display_t weapon_sprite_displays[3];         /* 0x362; 3 parts each */
    battle_gfx_single_part_sprite_display_t numeric_sprite_displays[3];   /* 0x3ce */
    battle_gfx_single_part_sprite_display_t status_bubble_sprite_display; /* 0x410 */
    battle_gfx_single_part_sprite_display_t item_sprite_display;          /* 0x426 */
    u8 _unused_43c[4]; /* 0x43c; the next slot's in-use word (g_battle_unit_misc_slot_flags) */
} battle_unit_misc_data_t;
typedef char battle_misc_data_size_must_be_0x440[(sizeof(battle_unit_misc_data_t) == 0x440) ? 1 : -1];
typedef char
    battle_misc_step_speed_must_be_0x03c[((unsigned long)&((battle_unit_misc_data_t*)0)->step_speed == 0x03c) ? 1 : -1];
typedef char battle_misc_distortion_target_must_be_0x090
    [((unsigned long)&((battle_unit_misc_data_t*)0)->distortion_target == 0x090) ? 1 : -1];
typedef char
    battle_misc_strike_must_be_0x18c[((unsigned long)&((battle_unit_misc_data_t*)0)->strike == 0x18c) ? 1 : -1];
typedef char battle_misc_animation_frame_must_be_0x1e0
    [((unsigned long)&((battle_unit_misc_data_t*)0)->animation.frame == 0x1e0) ? 1 : -1];
typedef char battle_misc_animation_flags_must_be_0x1f0
    [((unsigned long)&((battle_unit_misc_data_t*)0)->animation.flags == 0x1f0) ? 1 : -1];
typedef char battle_misc_sprite_blocks_must_be_0x208
    [((unsigned long)&((battle_unit_misc_data_t*)0)->sprite_blocks == 0x208) ? 1 : -1];
typedef char
    battle_misc_shadow_must_be_0x299[((unsigned long)&((battle_unit_misc_data_t*)0)->shadow_dirty == 0x299) ? 1 : -1];
typedef char
    battle_misc_command_must_be_0x158[((unsigned long)&((battle_unit_misc_data_t*)0)->command_state == 0x158) ? 1 : -1];
typedef char battle_misc_facing_hint_must_be_0x164
    [((unsigned long)&((battle_unit_misc_data_t*)0)->command_state.cursor.facing_hint == 0x164) ? 1 : -1];
typedef char battle_misc_target_x_must_be_0x168
    [((unsigned long)&((battle_unit_misc_data_t*)0)->command_state.cursor.target_panel == 0x168) ? 1 : -1];

/* battle_unit_misc_data_t.fade_out_phase: the fade-out removal's progress. */
typedef enum battle_misc_fade_out_phase {
    BATTLE_MISC_FADE_OUT_NONE = 0,
    BATTLE_MISC_FADE_OUT_FLASH = 1,
    BATTLE_MISC_FADE_OUT_DARKEN = 2,
    BATTLE_MISC_FADE_OUT_REMOVED = 3,
} battle_misc_fade_out_phase_e;

/* battle_unit_misc_data_t.turn_action_flags. */
typedef enum battle_misc_turn_action_flags {
    BATTLE_MISC_TURN_ACTION_MOVED = 0x01,
    BATTLE_MISC_TURN_ACTION_ACTED = 0x02,
} battle_misc_turn_action_flags_e;

extern s16 g_battle_screen_fade_intensity;
extern POLY_F4 g_battle_screen_fade_polygons[2];
extern DR_MODE g_battle_screen_fade_draw_modes[];
extern s32 g_battle_screen_fade;

s32 battle_gfx_calculate_screen_z_from_input_coords(struct battle_unit_misc_data* unit, const SVECTOR* coords);

/* camera */
/* g_battle_camera_rotation_action. */
typedef enum battle_camera_rotation_action {
    BATTLE_CAMERA_ROTATION_NONE = 0,
    BATTLE_CAMERA_ROTATION_LEFT = 1,  /* the yaw falls to its destination */
    BATTLE_CAMERA_ROTATION_RIGHT = 2, /* the yaw rises to its destination */
} battle_camera_rotation_action_e;

/* g_battle_camera_tilt_action and g_battle_camera_zoom_action: the move in progress towards
 * g_battle_map_tilt_target or g_battle_map_zoom_target. */
typedef enum battle_camera_step_action {
    BATTLE_CAMERA_STEP_NONE = 0,
    BATTLE_CAMERA_STEP_INCREASE = 2, /* tilt up; zoom in */
    BATTLE_CAMERA_STEP_DECREASE = 4, /* tilt down; zoom out */
} battle_camera_step_action_e;

/* g_battle_map_tilt_target values the tilt code tests; the target is a tilt band plus one, and
 * bands 3 and 4 have no pitch here. */
enum {
    BATTLE_MAP_TILT_LOW = 1,  /* pitch 0x12e */
    BATTLE_MAP_TILT_HIGH = 2, /* pitch 0x1c0 */
};

/* g_battle_map_zoom_target. */
enum {
    BATTLE_MAP_ZOOM_IN = 1,  /* scale ONE */
    BATTLE_MAP_ZOOM_OUT = 4, /* scale 0xc00 */
};

/* battle_camera_get_input_direction modes: which button state it reads. */
typedef enum battle_camera_input_source {
    BATTLE_CAMERA_INPUT_HELD = 0,
    BATTLE_CAMERA_INPUT_PRESSED = 1, /* newly pressed, repeating at the cursor repeat speed while held */
    BATTLE_CAMERA_INPUT_RELEASED = 2,
} battle_camera_input_source_e;

/* g_battle_cursor_input_mode: how the directional buttons map to map directions. The first two are
 * the Cursor Movement option's settings; FREE_CURSOR sends the four buttons along the screen's axes. */
typedef enum battle_cursor_input_mode {
    BATTLE_CURSOR_INPUT_OPTION_A = 0,
    BATTLE_CURSOR_INPUT_OPTION_B = 1,
    BATTLE_CURSOR_INPUT_FREE_CURSOR = 2,
} battle_cursor_input_mode_e;

extern u16 g_battle_camera_shake_z_offset;
extern s32 g_battle_previous_camera_yaw;
extern u16 g_battle_camera_script_variable_indices[7];
extern u16 g_battle_camera_position_locked;
extern s32 g_battle_camera_yaw_band;
extern s32 g_battle_camera_wrap_yaw_pending;
extern VECTOR g_battle_camera_script_vector;
extern VECTOR* g_battle_camera_published_coords_ptr;
extern SVECTOR g_battle_camera_script_rotation;
extern SVECTOR* g_battle_camera_published_rotation_ptr;
/* Maps a camera quadrant index to its map-tile camera-block direction. */
extern u8 g_battle_camera_quadrant_block_directions[4];
extern VECTOR g_battle_camera_current_real_coords;
extern SVECTOR g_battle_camera_integer_coords;
extern MATRIX g_battle_camera_matrix;
extern u16 g_battle_camera_offset_screen_coord_countdown;
extern VECTOR g_battle_camera_offset_screen_coords;
extern VECTOR g_battle_camera_offset_screen_coords_fixed;
extern u16 g_battle_camera_real_coord_countdown;
extern VECTOR g_battle_camera_real_coord_delta;
extern s32 g_battle_camera_rotation_action;
extern SVECTOR g_battle_camera_rotation_backup;
extern u16 g_battle_camera_rotation_countdown;
extern SVECTOR g_battle_camera_rotation_delta;
extern s32 g_battle_camera_rotation_increment;
extern VECTOR g_battle_camera_saved_real_coords;
extern s32 g_battle_camera_tilt_action;
extern s32 g_battle_camera_tilt_increment;
extern VECTOR g_battle_camera_zoom;
extern s32 g_battle_camera_zoom_action;
extern VECTOR g_battle_camera_zoom_backup;
extern u16 g_battle_camera_zoom_countdown;
extern VECTOR g_battle_camera_zoom_delta;
extern s32 g_battle_camera_zoom_increment;
extern s32 g_battle_camera_cross_product_overflow; /* set by battle_fixed_cross_product_q12 on Q12 overflow */
extern s16 g_battle_camera_diagonal_yaws[4];

/* Base of the large render-state records; low offsets overlap camera/GPU fields.
 * .vx is the camera pitch and .vy its yaw. */
extern SVECTOR g_battle_camera_render_state;

/* 0x80166054: camera consumer unpacks two 2-bit fields and a high nibble. */
extern s32 g_battle_camera_speed_curve;
void battle_camera_convert_screen_coords_by_rotation(const SVECTOR* input, SVECTOR* output, s32 offset);
void battle_camera_convert_screen_coords_modify_by_1(const SVECTOR* screen, SVECTOR* destination);
void battle_camera_convert_screen_coords_by_1_from_misc_id(u32 misc_id, SVECTOR* coords);
void battle_camera_toggle_tilt(void);
s16 battle_camera_pick_nearest_unblocked_yaw(s32 misc_id_a, s32 misc_id_b);
void battle_camera_step_focus_toward_cursor_tile(s16 height);
void battle_camera_execute_angle_command(s32 command, s32 target, s32 duration, SVECTOR* angles);
void battle_camera_execute_position_command(s32 command, s32 target, s32 duration, VECTOR* offset);
s32 battle_camera_get_input_direction(s32 mode);
void battle_camera_update_effect_rotation_and_zoom(void);

/* Some routines address these components as separate linker symbols. */
void battle_camera_step_real_coords_toward_target(battle_unit_misc_data_t* target);
void battle_camera_update_vector_from_game_state(void);
void battle_camera_calculate_acting_unit_vector_normal(void);
s32 battle_camera_calculate_cursor_tile_vector_normal(void);
void battle_camera_call_rotate_left(s32 target_angle);
void battle_camera_call_rotate_right(s32 target_angle);
void battle_camera_call_toggle_tilt(void);
void battle_camera_call_zoom_map(void);
void battle_camera_copy_current_real_coords(void);
void battle_camera_copy_rotation(void);
void battle_camera_copy_zoom(void);
void battle_camera_execute_zoom_command(s32 command, s32 unused, s32 duration, VECTOR* zoom);
VECTOR* battle_camera_get_current_real_coords(void);
SVECTOR* battle_camera_get_rotation(void);
VECTOR* battle_camera_get_zoom(void);
void battle_camera_handle_rotation_input(void);
void battle_camera_init_defaults(void);
void battle_camera_init_tilt_and_zoom(void);
void battle_camera_normalize_yaw_angle(void);
s32 battle_camera_is_active(void);
void battle_camera_project_world_coords_to_screen(SVECTOR* world_coords, SVECTOR* screen_coords);
void battle_camera_request_rotation(s32 angle);
void battle_map_reset_script_light_transform(void);
void battle_camera_restore_current_real_coords(void);
void battle_camera_restore_rotation(void);
void battle_camera_restore_zoom(void);
void battle_camera_rotate_left(s32 target_angle);
void battle_camera_rotate_right(s32 target_angle);
void battle_camera_rotate_when_unit_tile_not_visible(struct battle_unit_misc_data* unit);
s32 battle_camera_scale_cursor_tile_vector(void);
void battle_camera_set_current_real_coords(const VECTOR* coords);
void battle_camera_set_rotation(const SVECTOR* rotation);
void battle_camera_set_zoom(const VECTOR* zoom);
s32 battle_camera_step_track(battle_camera_track_t* track, s32 component);
void battle_camera_store_state_to_script_variables(void);
void battle_camera_store_yaw_band(s32 value);
void battle_camera_toggle_zoom(void);
void battle_camera_update_map_pan(void);

void battle_camera_update_matrices(
    MATRIX* camera_matrix, SVECTOR* rotation, VECTOR* screen_offset, VECTOR* camera_zoom);

void battle_camera_update_offset_screen_coord_animation(void);
void battle_camera_update_real_coord_animation(void);
void battle_camera_update_rotation(void);
void battle_camera_update_rotation_animation(void);
void battle_camera_update_tilt(void);
void battle_camera_update_zoom(void);
void battle_camera_update_zoom_animation(void);
s32 battle_camera_wrap_yaw_angle(void);
void battle_camera_advance_effect_tracks(s16 frame);
void battle_camera_calculate_relative_offset(const SVECTOR* input, SVECTOR* output, s32 offset);
void battle_camera_calculate_relative_offset_by_1(const SVECTOR* input, SVECTOR* output);
void battle_camera_calculate_relative_offset_by_5(const SVECTOR* input, SVECTOR* output);

void battle_camera_choose_unblocked_rotation_for_tile(const SVECTOR* tile_coords, SVECTOR* rotation_out);

void battle_camera_convert_screen_coords_modify_by_5(const SVECTOR* screen, SVECTOR* destination);
s32 battle_camera_find_active_keyframe(s16 frame, s16 mask, s32* out_index, s32* out_frame);
void battle_camera_load_state_from_script_variables(void);

void battle_camera_set_focus_and_cursor_tile(const SVECTOR* coords, const SVECTOR* rotation);

void battle_camera_start_effect_keyframe_moves(s16 frame);
void battle_camera_update_cursor_tile_vector(void);
void battle_camera_shift_acting_unit_vector(void);
void battle_camera_fusion_thread(void);
void battle_camera_wait_until_idle(void);
void battle_camera_thread(void);

/* sound */
extern s32 g_battle_sound_primary_weather_sfx_id;
extern s32 g_battle_sound_secondary_weather_sfx_id;
extern s32 g_battle_sound_suppressed;
extern s32 g_battle_sound_tertiary_weather_sfx_id;
extern u8 g_battle_sound_weather_sfx_enabled;
extern s32 g_battle_sound_pending_sfx_id; /* main_sfx_e; -1 while the event frame has no request */
/* main_sfx_e by the weapon's item_type_e (0x20 entries each): the swing, the
 * hit, and the hit a shield or Golem stops (battle_sound_select_weapon_hit). */
extern u8 g_battle_sound_weapon_miss_sfx_ids[];
extern u8 g_battle_sound_weapon_hit_sfx_ids[];
extern u8 g_battle_sound_shield_deflect_sfx_ids[];
extern s16 g_battle_music_set_track_request;

/* SwitchTrack's first operand is a trigger, not the selected track ID.
 * The consumer toggles the zero-based index, then uses main music slot 1/2. */
extern s16 g_battle_music_switch_request; /* 0x80165fc8 */

/* u16: the only loads of these three in the tree are in
 * attack_sound_resync_scenario_music_and_apply_map_darkness, which the target
 * reaches with lhu (that file's own comment records the andi a u16 merge would
 * add). Every other use is a store, which is sign-agnostic. */
extern u16 g_battle_music_track_1_id;          /* 0x80165fd4 */
extern u16 g_battle_music_track_2_id;          /* 0x80165fd6 */
extern s16 g_battle_music_unload_slot_request; /* 0x80165fca */

/* 0x80165fc0: -1 absent; volume = low 16 bits,
 * duration = (packed >> 14) & 0x3ffc. Not a linear duration field. */
extern s32 g_battle_music_volume_transition_request;
void battle_sound_play_movement_sfx(battle_unit_misc_data_t* unit, s32 sound_id);
void battle_sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id);
void battle_sound_select_weapon_hit(battle_unit_misc_data_t* unit, s32 hit);
void battle_sound_set_effect_to_cancel(void);
void battle_sound_set_effect_to_confirm(void);
void battle_sound_set_effect_to_confirm_checked(void);
void battle_sound_set_effect_to_cursor_move(void);
void battle_sound_set_effect_to_invalid(void);
void battle_sound_start_weather_sfx(void);
s32 battle_sound_stop_weather_sfx(void);
void battle_sound_wait_for_music_stub(void);
void battle_sound_bg_thread(void);
void battle_sound_edit_bg_thread(void);

/* effect */
typedef enum battle_effect_secondary_phase {
    BATTLE_SECONDARY_EFFECT_ENDED = 0, /* the slot is free */
    BATTLE_SECONDARY_EFFECT_INITIALIZING = 1,
    BATTLE_SECONDARY_EFFECT_EXECUTING = 2,
    BATTLE_SECONDARY_EFFECT_FINALIZING = 3
} battle_effect_secondary_phase_e;

/* Verified prefix only: BATTLE 0x801aa7c4 reads/writes a halfword at +6.
 * Neither the full record nor the individual bit meanings are known. */
typedef struct battle_effect_flag_prefix {
    u8 _unused_00[6];
    u16 flags;
} battle_effect_flag_prefix_t;
typedef char
    battle_effect_flag_prefix_flags_must_be_6[((unsigned long)&((battle_effect_flag_prefix_t*)0)->flags == 6) ? 1 : -1];

/* target_type of an effect target record. */
typedef enum battle_effect_target_type {
    BATTLE_EFFECT_TARGET_TYPE_UNIT = 0,
    BATTLE_EFFECT_TARGET_TYPE_TILE = 1,
} battle_effect_target_type_e;

enum {
    BATTLE_EFFECT_CASTER_ENTRY = 16, /* index of the caster's record in battle_effect_secondary_init_t.targets */
    /* the target index that makes battle_effect_resolve_target_render_coords resolve the caster */
    BATTLE_EFFECT_TARGET_INDEX_CASTER = -1,
};

/* Ten-byte target records at g_battle_effect_context.targets (0x801bad10).
 * Unit targets store a misc id at 0x02; tile targets reuse that halfword for X
 * and carry map Z/Y at 0x04/0x06. Entry BATTLE_EFFECT_CASTER_ENTRY is the
 * caster. */
typedef struct battle_effect_target {
    u16 target_type; /* 0x00; battle_effect_target_type_e */
    union {
        struct {
            u8 misc_id;
            u8 result_animation; /* battle_effect_hit_result_e */
        } unit;
        SVECTOR tile; /* x, layer, y */
    } data;           /* 0x02 */
} battle_effect_target_t;
typedef char battle_effect_target_size_must_be_0x0a[(sizeof(battle_effect_target_t) == 0x0a) ? 1 : -1];

/* One running secondary effect: a 0x54-byte record of
 * g_battle_effect_secondary_data (0x801b8b9c), ids 1-16 with record 0 the
 * sentinel. The secondary effects are the ones resident in BATTLE.BIN (weapon
 * hits, projectiles, the charges, the level-up and so on): each is an
 * animation id run by its handler of g_battle_effect_secondary_handlers
 * through the phases of battle_effect_secondary_phase_e. Records chain by id.
 * The particles a handler draws are effect slots (battle_effect_slot_t), a
 * separate pool, whose ids it keeps in slot_ids or in its own state. */
typedef struct battle_effect_secondary_data {
    u8 previous_slot_id; /* 0x00; the record before it on the active chain, or the next free record */
    u8 next_slot_id;     /* 0x01 */
    u8 own_slot_id;      /* 0x02; index of this record in g_battle_effect_secondary_data */
    u8 function_id;      /* 0x03; index of its handler in g_battle_effect_secondary_handlers */
    u16 animation_id;    /* 0x04; battle_effect_secondary_animation_id_e */
    /* 0x06; the animation's own: the charge's element mask, then its battle_effect_element_index_e; the
     * weapon hit's element index; the caster's sprite palette; the frames a reflect shield stays */
    u16 parameter;
    u32 phase; /* 0x08; battle_effect_secondary_phase_e */
    s16 timer; /* 0x0c; frames the handler has run */
    u8 _unused_0e[2];
    battle_effect_target_t caster; /* 0x10; copied from the init record's caster */
    union {
        battle_effect_target_t record; /* from the init record's target */
        struct {
            s16 _unused_00;
            s16 x; /* 0x1c; fall-dust position, 1/16 of the real coordinate */
            s16 z; /* 0x1e */
            s16 y; /* 0x20 */
        } position;
    } target;           /* 0x1a */
    s32 used_weapon_id; /* 0x24; init record used_weapon_id */
    s32 target_count;   /* 0x28; init record target_count (projectile setup) */
    /* 0x2c; live effect slots of its particle group: battle_effect_start_group counts them up and
     * battle_effect_update_active_slots down; the handlers finish at 0. */
    s16 active_count;
    s16 finish_timer;  /* 0x2e; counts frames after the last slot finished */
    u8 slot_ids[0x20]; /* 0x30; its effect slots, 0 for none; cleared when the record is allocated */
    void* allocation;  /* 0x50; the handler's work block; freed when the record is removed */
} battle_effect_secondary_data_t;
typedef char battle_secondary_effect_data_size_must_be_0x54[(sizeof(battle_effect_secondary_data_t) == 0x54) ? 1 : -1];

/* Complete 0xc8-byte effect context: built on the stack for secondary effects
 * and copied whole to g_battle_effect_context for the ability effect. */
typedef struct battle_effect_secondary_init {
    u16 target_count;                   /* 0x00; effect targets: one child record each, spawn_delay apart */
    u8 spectator_count;                 /* 0x02; entries in spectator_ids */
    u8 math_skill_flag;                 /* 0x03; enables the effect flags' spawn_delay_override */
    battle_effect_target_t targets[17]; /* 0x04; caster at BATTLE_EFFECT_CASTER_ENTRY */
    /* 0xae; living units that are neither targets nor the caster; also accessed
     * through the separately bound g_battle_effect_palette_target_misc_ids. */
    u8 spectator_ids[14];
    battle_effect_target_t target_tile; /* 0xbc; post-action destination tile */
    u16 used_weapon_id;                 /* 0xc6 */
} battle_effect_secondary_init_t;
typedef char battle_secondary_effect_init_size_must_be_0xc8[(sizeof(battle_effect_secondary_init_t) == 0xc8) ? 1 : -1];
typedef char battle_secondary_effect_init_caster_offset_must_be_0xa4
    [((unsigned long)&((battle_effect_secondary_init_t*)0)->targets[BATTLE_EFFECT_CASTER_ENTRY] == 0xa4) ? 1 : -1];

/* result_animation of a target in an effect init record. The records built by
 * battle_effect_store_targets_and_display_types use the first four; the weapon-hit
 * records (battle_effect_build_secondary_init_from_action) use all but REFLECTED. */
typedef enum battle_effect_hit_result {
    BATTLE_EFFECT_HIT_NORMAL = 0,
    BATTLE_EFFECT_HIT_CRITICAL = 1,
    BATTLE_EFFECT_HIT_REFLECTED = 2,
    BATTLE_EFFECT_HIT_MISSED = 3,    /* weapon hits: evaded by an accessory, a hand or the class */
    BATTLE_EFFECT_HIT_NO_EFFECT = 4, /* nullified, missed outright, cancelled, grasped or caught */
    BATTLE_EFFECT_HIT_GOLEM = 5,
    BATTLE_EFFECT_HIT_BREAK = 6,     /* equipment broken */
    BATTLE_EFFECT_HIT_BREAK_ALT = 7, /* handled like BREAK; nothing stores it */
} battle_effect_hit_result_e;

/* The secondary animation parameter that makes the projectile handler draw the thrown item's own
 * graphic instead of a model. */
enum { BATTLE_EFFECT_SECONDARY_PARAMETER_ITEM_GRAPHIC = -1 };

/* Four-byte records at 0x801b84dc indexed by animation id
 * (battle_effect_secondary_animation_id_e); byte 0 selects the
 * g_battle_effect_secondary_handlers handler. */
typedef struct battle_effect_secondary_animation {
    u8 function_id; /* 0x00 */
    u8 _padding_01; /* 0x01; aligns parameter */
    /* 0x02; the projectile handlers: battle_effect_model_e in the low byte, or
     * BATTLE_EFFECT_SECONDARY_PARAMETER_ITEM_GRAPHIC; the charge handler: its parameter row */
    s16 parameter;
} battle_effect_secondary_animation_t;

/* Queue of post-action "effect message" popups, drained newest-first by
 * battle_action_resume_attack_phase_control. The stride is 8: the producer at
 * 0x80072d8c indexes the array directly and the consumer walks it by
 * `idx * 8`, and only these three fields are ever read or written. */
typedef struct battle_post_effect_msg {
    u8 code;           /* 0x00 message code passed to battle_menu_init_system_function */
    u8 unit;           /* 0x01 misc ID of the acting unit */
    u8 _padding_02[2]; /* 0x02; aligns value */
    s32 value;         /* 0x04 reported amount */
} battle_post_effect_msg_t;

/* The projectile models of g_battle_effect_model_data_ptrs. */
typedef enum battle_effect_model {
    BATTLE_EFFECT_MODEL_ARROW = 0,
    BATTLE_EFFECT_MODEL_STONE = 1,
    BATTLE_EFFECT_MODEL_REFLECT = 2,
    BATTLE_EFFECT_MODEL_SHURIKEN = 3, /* drawn for the thrown items of ITEM_TYPE_SHURIKEN */
} battle_effect_model_e;

/* The inertia threshold every secondary effect handler runs its slots under, with a gravity of
 * (0, ONE, 0); an effect file's script sets its own pair (opcode 27). */
enum { BATTLE_EFFECT_SECONDARY_INERTIA_THRESHOLD = 0x230 };

/* One 14-byte parameter row for the spell-charge secondary effect. */
typedef struct battle_effect_charge_params {
    s16 speed;
    s16 spread_range;
    s16 spread_offset;
    s16 duration;
    s16 radius;
    u8 spawn_interval;
    u8 _unused_0b;
    u8 max_trails;
    u8 fade_row;
} battle_effect_charge_params_t;
typedef char battle_effect_charge_params_size_must_be_0x0e[sizeof(battle_effect_charge_params_t) == 0x0e ? 1 : -1];

/* Per-secondary-slot spawn state for the two-particle death poof. */
typedef struct battle_effect_death_poof_state {
    s32 count;        /* 0x00; slots started so far */
    u16 finish_timer; /* 0x04; reaches 1 when the effect is finished */
    u8 ids[2];        /* 0x06 */
} battle_effect_death_poof_state_t;
typedef char battle_effect_death_poof_state_size_must_be_8[sizeof(battle_effect_death_poof_state_t) == 8 ? 1 : -1];

typedef s32 (*battle_secondary_effect_function_t)(void);

/* One trail of the spell charge effect: the recent screen positions of the effect slot it follows. */
typedef struct battle_effect_charge_trail {
    DVECTOR points[7]; /* 0x00; screen positions, newest at age % 7 */
    s16 age;           /* 0x1c */
    u8 slot_id;        /* 0x1e */
    u8 _padding_1f;    /* tail padding to 2-byte alignment */
} battle_effect_charge_trail_t;

typedef struct battle_effect_charge_state {
    s16 x;                                   /* 0x0000 */
    s16 y;                                   /* 0x0002 */
    DR_MODE mode[2];                         /* 0x0004 */
    LINE_G2 lines[2][96];                    /* 0x001c */
    battle_effect_charge_trail_t trails[16]; /* 0x0f1c */
    u8 ids_a[2];                             /* 0x111c */
    u8 ids_b[14];                            /* 0x111e */
    struct {
        s16 trails;
        u8 auxiliary_particles;
        u8 particles;
    } count;            /* 0x112c */
    s32 angle;          /* 0x1130 */
    s32 speed;          /* 0x1134 */
    s32 spread_range;   /* 0x1138 */
    s32 spread_offset;  /* 0x113c */
    s32 duration;       /* 0x1140 */
    s32 radius;         /* 0x1144 */
    s32 spawn_interval; /* 0x1148 */
    s32 colour;         /* 0x114c */
    s32 max_trails;     /* 0x1150 */
    s32 fade_row;       /* 0x1154 */
    s32 done;           /* 0x1158 */
} battle_effect_charge_state_t;

/* Summon-charge state: three rings of ten trailing points. */
typedef struct battle_effect_ring_state {
    s32 index;             /* 0x00; newest trail column, 0..9 */
    s32 radius;            /* 0x04 */
    s32 angle;             /* 0x08 */
    s32 angle_step;        /* 0x0c */
    SVECTOR center;        /* 0x10 */
    s32 brightness;        /* 0x18 */
    SVECTOR points[3][10]; /* 0x1c */
} battle_effect_ring_state_t;

/* Zodiac-poof spawn state for the 24-slot particle group 3. */
typedef struct battle_effect_trap_state_24 {
    s32 done;   /* 0x00; reaches 1 when the effect is finished */
    s32 count;  /* 0x04; slots started so far */
    u8 ids[24]; /* 0x08 */
} battle_effect_trap_state_24_t;

/* Work area at 0x801bade0 shared by the secondary effect handlers; the
 * spell-charge, summon-charge and zodiac-poof handlers each lay it out as
 * their own state. */
typedef union battle_effect_secondary_state {
    battle_effect_charge_state_t charge;
    battle_effect_ring_state_t summon_ring;
    battle_effect_trap_state_24_t zodiac_poof;
} battle_effect_secondary_state_t;

/* One 0x80-byte particle channel of an effect timeline. Keyframe k lasts
 * from frame_start[k] to frame_start[k + 1]. Entering it runs its action word
 * (unit reactions, and binding a native handler to a callback slot); on each
 * of its frames it spawns its emitter, through the handler of the action's
 * callback slot when that is set. Keyframe 0 is passed over without spawning,
 * so emitter[0] is never read; retail channels have up to 24 keyframes, and
 * the end of keyframe 23, frame_start[24], is the halfword at 0x30. */
typedef struct battle_effect_keyframe_table {
    u16 frame_start[0x18]; /* 0x00 */
    u8 _unused_30;         /* 0x30 */
    u8 emitter[0x19];      /* 0x31; emitter index + 1, 0 for none */
    u16 action[0x18];      /* 0x4a; EFFECT_KEYFRAME_ACTION_* */
    u8 _unused_7a[4];      /* 0x7a */
    s16 last_index;        /* 0x7e; highest valid keyframe index */
} battle_effect_keyframe_table_t;
typedef char battle_effect_keyframe_table_size_must_be_0x80[(sizeof(battle_effect_keyframe_table_t) == 0x80) ? 1 : -1];

enum {
    BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES = 33,
};

/* Three-byte colours or signed colour offsets, selected by the keyframe preset. */
typedef struct battle_effect_track_color {
    s8 r;
    s8 g;
    s8 b;
} battle_effect_track_color_t;
typedef char battle_effect_track_color_size_must_be_3[sizeof(battle_effect_track_color_t) == 3 ? 1 : -1];

/* The screen colour track: the palette-track layout with a second colour per
 * keyframe. An APPLY keyframe shifts the map background gradient by twice
 * its colour; any other sets the gradient's two stops to its two colours. */
typedef struct battle_effect_background_track {
    s16 duration[BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES];
    battle_effect_track_color_t color[BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES];
    battle_effect_track_color_t second_color[BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES];
    u8 preset[BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES];
    s16 count;
} battle_effect_background_track_t;
typedef char
    battle_effect_background_track_size_must_be_0x12c[(sizeof(battle_effect_background_track_t) == 0x12c) ? 1 : -1];

/* preset of a colour track keyframe. */
enum {
    EFFECT_COLOR_KEYFRAME_MODE_MASK = 0x7f, /* the map colour mode (MAP_COLOR_MODE_*) the colour is applied in */
    EFFECT_COLOR_KEYFRAME_APPLY = 0x80,     /* clear on the unit tracks: the keyframe only waits */
};

/* A colour track of an effect timeline: timed palette changes of the
 * surroundings, the caster or the targets. A keyframe lasts duration * 8
 * frames (one frame for 0); a track of count keyframes plays count - 1. */
typedef struct battle_effect_palette_track {
    s16 duration[BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES];
    battle_effect_track_color_t color[BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES];
    u8 preset[BATTLE_EFFECT_PALETTE_TRACK_KEYFRAMES];
    s16 count;
} battle_effect_palette_track_t;
typedef char battle_effect_palette_track_size_must_be_0xc8[sizeof(battle_effect_palette_track_t) == 0xC8 ? 1 : -1];

/* One phase's colour tracks. The surroundings are the map's field colour and
 * the spectators: the living units that are neither a target nor the caster. */
typedef struct effect_timeline_color_tracks {
    battle_effect_palette_track_t surroundings; /* 0x000: the map and the units that only look on */
    battle_effect_palette_track_t caster;       /* 0x0c8 */
    battle_effect_palette_track_t target;       /* 0x190 */
    battle_effect_background_track_t screen;    /* 0x258 */
} effect_timeline_color_tracks_t;

/* A sound track of the child timeline: up to 17 entries, each naming an
 * effect sound channel to play when it is entered. The parent timeline's
 * tracks hold nine (effect_timeline_t.sound_tracks). */
typedef struct battle_effect_tick_sound_track {
    u16 duration[17]; /* 0x00; frames each entry lasts */
    u8 channel[17];   /* 0x22; 0 and 1 none, otherwise a sound channel of the flags section + 2 */
    u8 _padding_33;   /* aligns count */
    s16 count;        /* 0x34 */
} battle_effect_tick_sound_track_t;

/* The child timeline, eight bytes into the timeline section
 * (g_battle_effect_timing_channels): its duration and its channels.
 * effect_timeline_t names the section's other parts. */
typedef struct battle_effect_tick_channels {
    u16 _unused_000;
    u16 duration;                               /* 0x002; frames: effect_timeline_t.child.duration */
    battle_effect_keyframe_table_t particle[5]; /* 0x004 */
    battle_effect_tick_sound_track_t sound[3];  /* 0x284 */
    battle_effect_palette_track_t surroundings; /* 0x326 */
    battle_effect_palette_track_t caster;       /* 0x3ee */
    battle_effect_palette_track_t target;       /* 0x4b6 */
    battle_effect_background_track_t screen;    /* 0x57e */
} battle_effect_tick_channels_t;
typedef char battle_effect_tick_channels_size_must_be_0x6aa[sizeof(battle_effect_tick_channels_t) == 0x6aa ? 1 : -1];

/* target_mode of battle_effect_start_new. */
typedef enum battle_effect_start_target {
    BATTLE_EFFECT_START_ON_UNIT = 0,
    BATTLE_EFFECT_START_ON_TILE = 1,
    BATTLE_EFFECT_START_ON_CASTER = 2, /* a unit that is both caster and target */
} battle_effect_start_target_e;

/* target_mode selects the unit id or the tile coordinate pointer. */
typedef union battle_effect_start_target_data {
    s32 misc_id;
    const SVECTOR* tile;
} battle_effect_start_target_data_t;
typedef char battle_effect_start_target_data_size_must_be_4[sizeof(battle_effect_start_target_data_t) == 4 ? 1 : -1];

/* An entry of g_battle_effect_ability_ids: the effect file of an ability, negative for none. */
enum {
    BATTLE_EFFECT_ABILITY_FILE_MASK = 0x1ff, /* the number in the file's name, EFFECT/E###.BIN */
    BATTLE_EFFECT_ABILITY_FLAG_ITEM = 0x800, /* set on the Item abilities: battle_effect_is_item_ability */
};

/* The effect files the code names by number. */
enum {
    BATTLE_EFFECT_FILE_CURE = 0x01, /* loaded in place of a file of no size */
    BATTLE_EFFECT_FILE_FIRE = 0x10,
    BATTLE_EFFECT_FILE_BOLT = 0x14,
    BATTLE_EFFECT_FILE_ICE = 0x18,
};

/* What battle_effect_get_ninja_ball makes of an element mask: the position of its first element in
 * g_battle_effect_ninja_ball_masks, 0 for no element. It picks the CLUT of the weapon hit and
 * spell charge particles and the effect of a thrown ball. */
typedef enum battle_effect_element_index {
    BATTLE_EFFECT_ELEMENT_INDEX_NONE = 0,
    BATTLE_EFFECT_ELEMENT_INDEX_FIRE = 1,
    BATTLE_EFFECT_ELEMENT_INDEX_WATER = 2,
    BATTLE_EFFECT_ELEMENT_INDEX_EARTH = 3,
    BATTLE_EFFECT_ELEMENT_INDEX_WIND = 4,
    BATTLE_EFFECT_ELEMENT_INDEX_HOLY = 5,
    BATTLE_EFFECT_ELEMENT_INDEX_DARK = 6,
    BATTLE_EFFECT_ELEMENT_INDEX_ICE = 7,
    BATTLE_EFFECT_ELEMENT_INDEX_LIGHTNING = 8,
} battle_effect_element_index_e;

/* g_effect_load_state: the steps of battle_effect_init_data, which loads the ability's effect file
 * while its charge secondary effect plays. */
typedef enum battle_effect_load_state {
    BATTLE_EFFECT_LOAD_START = 0,            /* start reading the file */
    BATTLE_EFFECT_LOAD_READING = 1,          /* wait for the read */
    BATTLE_EFFECT_LOAD_COUNTDOWN = 2,        /* wait out the 60 frames given to a charge secondary effect */
    BATTLE_EFFECT_LOAD_ENDING_SECONDARY = 3, /* wait for the charge secondary effect to end */
} battle_effect_load_state_e;

/* g_battle_effect_phase: the stages of battle_effect_update_stage. */
typedef enum battle_effect_phase {
    BATTLE_EFFECT_PHASE_IDLE = 0,
    BATTLE_EFFECT_PHASE_REQUESTED = 1, /* waiting for battle_effect_play */
    BATTLE_EFFECT_PHASE_INIT_PALETTE = 2,
    BATTLE_EFFECT_PHASE_START_SCRIPT = 3, /* bind the effect file's sections and start its script */
    BATTLE_EFFECT_PHASE_RUNNING = 4,
    BATTLE_EFFECT_PHASE_RESTART_SCRIPT = 6, /* start the script again without rebinding the file */
} battle_effect_phase_e;

/* Opcodes (top header byte) of the effect model commands that battle_effect_decode_next_model_command
 * decodes: each draws the primitive it is named for. Nothing in the code tells the two GT3 opcodes
 * apart except that battle_effect_draw_projectile_model draws only the second. */
enum {
    BATTLE_EFFECT_MODEL_COMMAND_G3 = 0x31,
    BATTLE_EFFECT_MODEL_COMMAND_GT3_35 = 0x35,
    BATTLE_EFFECT_MODEL_COMMAND_GT3_37 = 0x37,
    BATTLE_EFFECT_MODEL_COMMAND_G4 = 0x39,
};

/* Index into g_battle_effect_trap_particle_configs: the particle groups of the secondary effect
 * handlers, named after the handler that spawns each. */
typedef enum battle_effect_particle_group {
    /* battle_effect_weapon_hit_secondary_handler: its first slot set */
    BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_A = 0,
    BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_B = 1, /* its second slot set */
    BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF = 3,
    BATTLE_EFFECT_PARTICLE_GROUP_DEATH_POOF = 4,
    BATTLE_EFFECT_PARTICLE_GROUP_LEVEL_UP = 5,
    BATTLE_EFFECT_PARTICLE_GROUP_FALL_DUST = 6,
    BATTLE_EFFECT_PARTICLE_GROUP_SPLASH = 7,
    BATTLE_EFFECT_PARTICLE_GROUP_VENOM_TRAP = 8,
    BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_B_MISSED = 9, /* the second slot set of a missed hit */
    BATTLE_EFFECT_PARTICLE_GROUP_RED_ORBS = 10,
    BATTLE_EFFECT_PARTICLE_GROUP_EQUIP_BREAK = 11,
    BATTLE_EFFECT_PARTICLE_GROUP_SPELL_CHARGE = 12,
    BATTLE_EFFECT_PARTICLE_GROUP_SPELL_CHARGE_13 = 13, /* given the height of group 12; nothing spawns it */
    BATTLE_EFFECT_PARTICLE_GROUP_SUMMON_CHARGE = 14,
    BATTLE_EFFECT_PARTICLE_GROUP_TELEPORT = 15,
    BATTLE_EFFECT_PARTICLE_GROUP_GOLEM = 16,
} battle_effect_particle_group_e;

/* Index into g_battle_effect_secondary_animations: the secondary effects resident in BATTLE.BIN. Those
 * the code does not start by number come from two tables: the weapon type's projectile
 * (g_battle_effect_weapon_type_extra_flags) and the ability's charge animation set
 * (g_battle_effect_secondary_by_charge_animation). */
typedef enum battle_effect_secondary_animation_id {
    BATTLE_EFFECT_SECONDARY_ANIMATION_NONE = 0x00,
    BATTLE_EFFECT_SECONDARY_ANIMATION_BOW_ARROW = 0x01,       /* bows: the arrow model on an arc */
    BATTLE_EFFECT_SECONDARY_ANIMATION_SPELL_CHARGE = 0x02,    /* charge animation set 1 */
    BATTLE_EFFECT_SECONDARY_ANIMATION_CROSSBOW_BOLT = 0x03,   /* crossbows: the arrow model on a straight line */
    BATTLE_EFFECT_SECONDARY_ANIMATION_SPELL_CHARGE_04 = 0x04, /* the charge with parameter row 1; no table names it */
    BATTLE_EFFECT_SECONDARY_ANIMATION_SUMMON_CHARGE = 0x05,   /* charge animation set 2 */
    BATTLE_EFFECT_SECONDARY_ANIMATION_THROWN_STONE = 0x06,
    BATTLE_EFFECT_SECONDARY_ANIMATION_WEAPON_HIT = 0x07,
    BATTLE_EFFECT_SECONDARY_ANIMATION_WEAPON_HIT_08 = 0x08, /* the weapon hit handler again; nothing starts it */
    BATTLE_EFFECT_SECONDARY_ANIMATION_FALL_DUST = 0x09,
    BATTLE_EFFECT_SECONDARY_ANIMATION_REFLECT = 0x0a,
    BATTLE_EFFECT_SECONDARY_ANIMATION_ZODIAC_POOF = 0x0b,
    BATTLE_EFFECT_SECONDARY_ANIMATION_DEATH_SMOKE = 0x0c,
    BATTLE_EFFECT_SECONDARY_ANIMATION_LEVEL_UP = 0x0d,
    BATTLE_EFFECT_SECONDARY_ANIMATION_VENOM_TRAP = 0x0e,
    BATTLE_EFFECT_SECONDARY_ANIMATION_SPLASH = 0x0f,
    BATTLE_EFFECT_SECONDARY_ANIMATION_THROWN_ITEM = 0x10, /* Item and Throw: the item's graphic or the shuriken model */
    BATTLE_EFFECT_SECONDARY_ANIMATION_RED_ORBS = 0x11,    /* the Charge abilities and Accumulate */
    BATTLE_EFFECT_SECONDARY_ANIMATION_EQUIP_BREAK = 0x12, /* abilities 0x8a-0x91 */
    BATTLE_EFFECT_SECONDARY_ANIMATION_TELEPORT = 0x13,
    BATTLE_EFFECT_SECONDARY_ANIMATION_GOLEM = 0x14,
    /* the null handler; g_battle_effect_ending_secondary_function_id is this record's function byte */
    BATTLE_EFFECT_SECONDARY_ANIMATION_ENDING = 0x15,
} battle_effect_secondary_animation_id_e;

/* One particle group of the secondary effects: a 0x2e-byte record of
 * g_battle_effect_trap_particle_configs. Its leading bytes schedule the
 * group's effect slots (battle_effect_start_group); the rest is a fixed
 * emitter, from which battle_effect_spawn_particle_motion draws each slot's
 * motion as the effect file emitter does a particle's. */
typedef struct battle_effect_emitter_values {
    u8 trap_animation;      /* 0x00; trap groups: index into g_battle_effect_trap_animation_data */
    u8 _unused_01;          /* 0x01 */
    u8 spawn_start_frame;   /* 0x02; trap groups */
    u8 spawn_end_frame;     /* 0x03; trap groups */
    u8 count;               /* 0x04; trap groups: slots the group owns */
    u8 _unused_05;          /* 0x05 */
    u16 anchor_flags;       /* 0x06; effect_emitter_origin_e: CASTER and TARGET are honoured */
    u16 motion_flags;       /* 0x08; effect_emitter_velocity_e */
    vec3s16_t position;     /* 0x0a */
    vec3s16_t spread;       /* 0x10 */
    vec3s16_t angle;        /* 0x16 */
    vec3s16_t angle_spread; /* 0x1c */
    s16 weight_min;         /* 0x22 */
    s16 weight_max;         /* 0x24 */
    u16 speed_min;          /* 0x26 */
    u16 speed_max;          /* 0x28 */
    u8 spawns_per_frame;    /* 0x2a; trap groups: slots one frame may start */
    u8 _unused_2b;          /* 0x2b */
    s8 lifetime_min;        /* 0x2c */
    s8 lifetime_max;        /* 0x2d */
} battle_effect_emitter_values_t;
typedef char battle_effect_emitter_values_size_must_be_0x2e[(sizeof(battle_effect_emitter_values_t) == 0x2e) ? 1 : -1];

/* flags of a sprite part (battle_effect_sprite_part_t, battle_effect_sprite_frameset_t). */
enum {
    EFFECT_SPRITE_PART_CLUT_MASK = 0x000f,
    EFFECT_SPRITE_PART_FLAG_SECOND_CLUT_ROW = 0x0010,  /* the CLUT is of MAP_CLUT_ROW_EFFECT_SECOND */
    EFFECT_SPRITE_PART_TPAGE_MODE_MASK = 0x00e0,       /* the tpage's blend rate and 8-bit colour bits */
    EFFECT_SPRITE_PART_FLAG_SEMI_TRANSPARENT = 0x0200, /* draw it semi-transparent */
};

/* One textured quad of a multi-part effect sprite: a frame of an effect file's
 * frame data holds a count of them. */
typedef struct battle_effect_sprite_part {
    u16 flags; /* 0x00; EFFECT_SPRITE_PART_*; the tpage mode bits repeat those of tpage and are not read here */
    u16 tpage; /* 0x02 */
    u8 u;
    u8 v;
    u8 width;
    u8 height;
    /* Four-byte alignment preserves the original x/y word copies. */
    DVECTOR corners[4] __attribute__((aligned(4))); /* 0x08 */
} battle_effect_sprite_part_t;
typedef char battle_effect_sprite_part_size_must_be_0x18[(sizeof(battle_effect_sprite_part_t) == 0x18) ? 1 : -1];

/* Arc endpoint handed to the per-arc obstacle test: the unit's raised tile
 * position followed by its id and height allowance. */
typedef struct battle_arc_endpoint {
    SVECTOR position;
    s32 unit_id;
    s32 height;
} battle_arc_endpoint_t;

/* Offset to the target plus its fixed-point horizontal length. */
typedef struct battle_arc_delta {
    s32 x;
    s32 y;
    s32 z;
    s32 distance;
} battle_arc_delta_t;

/* Query read by battle_effect_calculate_arcing_trajectory: the offset to the
 * target and both endpoints. */
typedef struct battle_arc_query {
    battle_arc_delta_t delta;
    battle_arc_endpoint_t source;
    battle_arc_endpoint_t target;
} battle_arc_query_t;

/* Unit list built in the scratchpad at 0x1f800000 by
 * battle_effect_list_valid_unit_targeting_data (0x801af770) and read by the
 * obstacle tests. */
typedef struct battle_effect_obstacle_unit_list {
    s32 count;                                  /* 0x00 */
    u8 battle_ids[22];                          /* 0x04 */
    SVECTOR positions[BATTLE_UNIT_SLOT_COUNT];  /* 0x1a; tile centre and height */
    s32 sprite_heights[BATTLE_UNIT_SLOT_COUNT]; /* 0xc4 */
} battle_effect_obstacle_unit_list_t;

/* The tile reference g_battle_effect_trajectory_tile (0x801b8b68)
 * that battle_effect_test_position_for_obstacle fills in. */
typedef struct battle_effect_tile_ref {
    s16 x;     /* 0x00 */
    s16 layer; /* 0x02 */
    s16 y;     /* 0x04 */
} battle_effect_tile_ref_t;

/* Header of one packed projectile model command. */
typedef union battle_effect_model_command_header {
    u32 word;
    struct {
        u8 _unused_00;
        u8 word_count;
        u8 _unused_02;
        u8 opcode;
    } fields;
} battle_effect_model_command_header_t;

/* Shared output of the model-command decoder and input of the projectile renderer. */
typedef struct battle_effect_decoded_model_command {
    battle_effect_model_command_header_t header; /* 0x00 */
    CVECTOR colors[4];                           /* 0x04 */
    u16 texture[5];                              /* 0x14; tpage, CLUT and three packed u/v pairs */
    u8 _unused_1e[0x4a];
    /* The decoder copies two packed pairs; the renderer reads four indices. */
    union {
        u32 pairs[2];
        u16 indices[4];
    } vertices; /* 0x68 */
} battle_effect_decoded_model_command_t;
typedef char battle_effect_decoded_model_command_size_must_be_0x70[sizeof(battle_effect_decoded_model_command_t) == 0x70
        ? 1
        : -1];

/* 0xc88-byte arrow work buffer of the bow-arrow (0x801b0ffc) and projectile
 * (0x801b3938) secondary handlers: two sets of four G3 and thirteen G4
 * polygons. */
typedef struct battle_effect_arrow_buffer {
    POLY_G3 g3[2][4];  /* 0x000 */
    POLY_G4 g4[2][13]; /* 0x0e0 */
    u8 _unused_488[0x800];
} battle_effect_arrow_buffer_t;
typedef char battle_effect_arrow_buffer_size_must_be_0xc88[(sizeof(battle_effect_arrow_buffer_t) == 0xc88) ? 1 : -1];

/* Double-buffered polygons initialized by the secondary handler and drawn by
 * battle_effect_draw_projectile_model. */
typedef struct battle_effect_stone_buffer {
    POLY_G3 g3[2][12];
} battle_effect_stone_buffer_t;

typedef struct battle_effect_shuriken_buffer {
    POLY_G3 g3[2][16];
} battle_effect_shuriken_buffer_t;

typedef struct battle_effect_reflect_buffer {
    u16 clut[2][6];
    POLY_GT3 prims[2][24];
} battle_effect_reflect_buffer_t;

/* 21-keyframe camera table of the parent timeline's two phases. A keyframe
 * moves the tracks its command word names (BATTLE_EFFECT_CAMERA_TRACK_*)
 * toward its values, arriving at end_frame; a track takes its next keyframe
 * only once its previous move has ended. */
typedef struct effect_camera_keyframes_21 {
    s16 end_frame[21];      /* frame the keyframe's moves arrive, counted from the start of the table's phase */
    vec3s16_t rotation[21]; /* pitch, yaw, roll */
    vec3s16_t position[21];
    vec3s16_t zoom[21];
    s16 command[21]; /* BATTLE_EFFECT_CAMERA_TRACK_* | battle_effect_camera_source_e | battle_effect_camera_mode_e */
    s16 last_keyframe_index;
} effect_camera_keyframes_21_t;

/* A parent-timeline sound track: channel 0/1 is silent; other values select
 * the effect's sound channel plus two. */
typedef struct battle_effect_sound_track {
    u16 durations[9]; /* 0x00 */
    u8 channels[9];   /* 0x12 */
    u8 _padding_1b;   /* aligns entry_count */
    s16 entry_count;  /* 0x1c */
} battle_effect_sound_track_t;

/* The timeline section of an effect file (effect_file_header_t.timeline).
 *
 * A script that spawns children (opcode 29) runs the parent timeline in three
 * stages: phase 1 for phase1_duration frames; then one child record per
 * effect target, spawn_delay frames apart, each running the child timeline
 * for child.duration frames; then phase 2, which starts when the last child's
 * timeline ends, for phase2_duration frames. A script without children
 * (opcode 28 on the root record) runs only the child timeline, once.
 *
 * The child timeline's channels are the battle_effect_tick_channels_t that
 * g_battle_effect_timing_channels addresses at 0x008; its camera table is the
 * 17-keyframe one at 0x6b2, laid out like effect_camera_keyframes_21_t. Each
 * phase has five particle channels, three sound tracks, four colour tracks
 * and a 21-keyframe camera table. */
typedef struct effect_timeline {
    u16 _unused_0000;
    u16 phase2_duration;                 /* 0x002 */
    u16 phase1_duration;                 /* 0x004 */
    u16 spawn_delay;                     /* 0x006; frames between child spawns (see effect_flags_section_t) */
    battle_effect_tick_channels_t child; /* 0x008; child duration, particle, sound and colour tracks */
    s16 end_frame[17];                   /* 0x6b2; child camera table: frame each keyframe's moves arrive */
    vec3s16_t rotation[17];              /* 0x6d4 */
    vec3s16_t position[17];              /* 0x73a */
    vec3s16_t zoom[17];                  /* 0x7a0 */
    s16 command[17];                     /* 0x806; as effect_camera_keyframes_21_t.command */
    s16 last_keyframe_index;             /* 0x828; highest valid keyframe index */
    battle_effect_keyframe_table_t particle_channels[2][5]; /* 0x82a; [phase][channel] */
    battle_effect_sound_track_t sound_tracks[2][3];         /* 0xd2a; [phase][track] */
    effect_timeline_color_tracks_t color_tracks[2];         /* 0xdde */
    effect_camera_keyframes_21_t phase1_camera;             /* 0x14e6 */
    effect_camera_keyframes_21_t phase2_camera;             /* 0x16b6 */
} effect_timeline_t;
typedef char effect_timeline_child_offset_must_be_8[((unsigned long)&((effect_timeline_t*)0)->child == 8) ? 1 : -1];
typedef char
    effect_timeline_command_offset_must_be_0x806[((unsigned long)&((effect_timeline_t*)0)->command == 0x806) ? 1 : -1];
typedef char effect_timeline_rotation_offset_must_be_0x6d4[((unsigned long)&((effect_timeline_t*)0)->rotation == 0x6d4)
        ? 1
        : -1];
typedef char effect_timeline_phase1_camera_offset_must_be_0x14e6
    [((unsigned long)&((effect_timeline_t*)0)->phase1_camera == 0x14e6) ? 1 : -1];
typedef char effect_timeline_size_must_be_0x1886[(sizeof(effect_timeline_t) == 0x1886) ? 1 : -1];

/* Byte offset to a group's frame-offset table, relative to the group-table base. */
typedef struct battle_effect_sprite_group {
    s16 frame_table_offset;
} battle_effect_sprite_group_t;

/* Frame sections start with a group count; group offsets are relative to this table. */
typedef struct battle_effect_sprite_section {
    u8 group_count;                         /* 0x00 */
    u8 _unused_01[3];                       /* 0x01; reserved header bytes */
    battle_effect_sprite_group_t groups[1]; /* 0x04; variable-length group table */
} battle_effect_sprite_section_t;

/* One frame selected by the animation stream; its parts render together. */
typedef struct battle_effect_sprite_frameset {
    u16 flags;                            /* 0x00; EFFECT_SPRITE_PART_*: SEMI_TRANSPARENT and the tpage mode bits */
    s16 count;                            /* 0x02; number of sprite parts */
    battle_effect_sprite_part_t parts[0]; /* 0x04; variable-length part table */
} battle_effect_sprite_frameset_t;
typedef char battle_effect_sprite_frameset_size_must_be_4[sizeof(battle_effect_sprite_frameset_t) == 4 ? 1 : -1];

/* Section table at the start of an effect file's header section: byte offsets
 * from the table itself, one per battle_effect_*_address/_pointer getter. */
typedef struct effect_file_header {
    s32 frame_data;          /* 0x00: the getter skips the section's leading word */
    s32 animation_sequences; /* 0x04: likewise */
    s32 script;              /* 0x08: effect script section */
    s32 particle_system;     /* 0x0c: emitter motion and control */
    s32 animation_curve;     /* 0x10 */
    s32 timing_curves;       /* 0x14 */
    s32 flags_section;       /* 0x18 */
    s32 timeline;            /* 0x1c: timing, palette and camera tracks */
    s32 sound_section;       /* 0x20: 0 when the effect has no sound */
    s32 palette;             /* 0x24 */
} effect_file_header_t;
typedef char effect_file_header_size_must_be_0x28[sizeof(effect_file_header_t) == 0x28 ? 1 : -1];

/* Two palette rows, then a 24-bit image size, width flag and pixel tail. */
typedef struct battle_effect_palette_section {
    clut_t primary_palettes;   /* 0x000 */
    clut_t secondary_palettes; /* 0x200 */
    u8 image_size[3];          /* 0x400; little-endian bytes */
    u8 wide_image;             /* 0x403; 256-byte rows when nonzero, otherwise 128 */
    u32 pixels[0];             /* 0x404; packed GPU image-transfer words */
} battle_effect_palette_section_t;
typedef char
    battle_effect_palette_section_size_must_be_0x404[sizeof(battle_effect_palette_section_t) == 0x404 ? 1 : -1];

/* An effect script instruction is a halfword, its opcode in the low nine bits
 * (g_battle_effect_instructions holds 120 handlers; retail scripts stay below
 * 2e) and small operands in the bits above, followed by up to three operand
 * halfwords. Script offsets are bytes from the start of the script. */
enum { EFFECT_SCRIPT_OPCODE_MASK = 0x1ff };

/* Halfword-aligned instructions have a nine-bit opcode and seven selector bits. */
typedef struct battle_effect_script_header {
    u8 opcode_low;        /* 0x00 */
    u8 packed_parameters; /* 0x01: opcode bit 8 and opcode-specific selectors */
} battle_effect_script_header_t;
typedef char battle_effect_script_header_size_must_be_2[(sizeof(battle_effect_script_header_t) == 2) ? 1 : -1];

/* A view at one instruction boundary in a variable-length byte stream.
 * Immediate, branch and camera instructions occupy 4, 6 and 8 bytes respectively;
 * two-byte instructions contain only the header. No array uses this union's stride. */
typedef union battle_effect_script_instruction {
    battle_effect_script_header_t header;
    struct {
        battle_effect_script_header_t header; /* 0x00 */
        s16 value;                            /* 0x02 */
    } immediate;
    struct {
        battle_effect_script_header_t header; /* 0x00 */
        s16 comparison;                       /* 0x02 */
        u16 target_offset;                    /* 0x04: script byte offset */
    } branch;
    struct {
        battle_effect_script_header_t header; /* 0x00 */
        s16 x;                                /* 0x02 */
        s16 y;                                /* 0x04 */
        s16 z;                                /* 0x06 */
    } vector;
} battle_effect_script_instruction_t;
typedef char battle_effect_script_instruction_size_must_be_8[sizeof(battle_effect_script_instruction_t) == 8 ? 1 : -1];

/* What an opcode handler returns to battle_effect_run_block. */
typedef enum effect_script_result {
    EFFECT_SCRIPT_RESULT_YIELD = 0,    /* stop for this frame */
    EFFECT_SCRIPT_RESULT_CONTINUE = 1, /* run the next instruction */
    EFFECT_SCRIPT_RESULT_ABORT = 2,    /* the script has ended: free the record */
} effect_script_result_e;

/* The particle updater reserves -1 for animation-driven lifetime. */
enum { EFFECT_PARTICLE_LIFETIME_ANIMATION_DRIVEN = -1 };

/* How an effect sound channel picks among its three sound ids on each use. */
typedef enum battle_effect_sound_mode {
    EFFECT_SOUND_MODE_FIRST = 0,                /* always the first */
    EFFECT_SOUND_MODE_ALTERNATE = 1,            /* the first and second in turn */
    EFFECT_SOUND_MODE_FIRST_THEN_SECOND = 2,    /* the first once, then the second */
    EFFECT_SOUND_MODE_FIRST_THEN_ALTERNATE = 3, /* the first once, then the third and second in turn */
    EFFECT_SOUND_MODE_CYCLE = 4,                /* the three in turn */
} battle_effect_sound_mode_e;

/* One of the four sound channels of an effect: the sound tracks of the
 * timelines name a channel, and each use plays the id its mode picks. */
typedef struct effect_sound_channel {
    u8 mode;         /* 0x00; battle_effect_sound_mode_e */
    u8 sound_ids[3]; /* 0x01; ids in the effect's sound section, 0 for none */
} effect_sound_channel_t;

/* Effect flags section at header[0x18] of an effect file. The engine loads
 * flags as a word and tests single bits; spawn_delay_override replaces the
 * timeline's spawn_delay only while g_battle_effect_context.math_skill_flag is
 * set. */
typedef struct effect_flags_section {
    u32 flags;                          /* 0x00; EFFECT_FLAG_* bits */
    u8 spawn_delay_override;            /* 0x04 */
    u8 _unused_05[3];                   /* 0x05 */
    effect_sound_channel_t channels[4]; /* 0x08 */
} effect_flags_section_t;
typedef char effect_flags_section_size_must_be_0x18[(sizeof(effect_flags_section_t) == 0x18) ? 1 : -1];

/* effect_flags_section_t::flags bits read by the engine. */
enum {
    /* caster and target anchors take the tile's surface height: battle_effect_resolve_target_render_coords */
    EFFECT_FLAG_TERRAIN_HEIGHT_ADJUST = 0x08,
    EFFECT_FLAG_QUIET_MUSIC = 0x10,         /* lower the music while the effect loads and plays */
    EFFECT_FLAG_TIMING_CURVE_PHASE1 = 0x20, /* phase 1 of the parent timeline sets the time scale from its curve */
    EFFECT_FLAG_TIMING_CURVE_CHILD = 0x40,  /* a child timeline sets the time scale from its curve */
};

/* Per-handler phase byte shared by every EFFECT overlay state machine. */
typedef enum effect_phase {
    EFFECT_PHASE_IDLE = 0,
    EFFECT_PHASE_INIT = 1,
    EFFECT_PHASE_UPDATE = 2,
    EFFECT_PHASE_DESTROY = 3,
} effect_phase_e;

/* Emitter spawn origin: bits 9-11 of an emitter's placement word (byte 0x03
 * mask 0x0e). 0xa00 is the map centre:
 * every handler adds battle_map_store_max_coordinates() * 14 there. */
typedef enum effect_emitter_origin {
    EFFECT_EMITTER_ORIGIN_WORLD = 0x000,
    EFFECT_EMITTER_ORIGIN_CURSOR_TILE = 0x200, /* centre of g_battle_effect_target_tile */
    EFFECT_EMITTER_ORIGIN_CASTER = 0x400,
    EFFECT_EMITTER_ORIGIN_TARGET = 0x600,
    EFFECT_EMITTER_ORIGIN_PARENT_PARTICLE = 0x800, /* position of the spawning work record */
    EFFECT_EMITTER_ORIGIN_MAP_CENTRE = 0xa00,
    EFFECT_EMITTER_ORIGIN_TRACKED_ENTITY = 0xc00, /* target plus its unit coordinates */
} effect_emitter_origin_e;

enum { EFFECT_EMITTER_ORIGIN_MASK = 0xe00 };

/* Point particles home toward: bits 5-7 of the placement word (byte 0x02 mask
 * 0xe0). 0x00 and 0x20 both leave the
 * interpolated offset unchanged; 0x40 is the map centre. */
typedef enum effect_emitter_homing {
    EFFECT_EMITTER_HOMING_OFFSET = 0x00,
    EFFECT_EMITTER_HOMING_OFFSET_ALT = 0x20,
    EFFECT_EMITTER_HOMING_MAP_CENTRE = 0x40,
    EFFECT_EMITTER_HOMING_CASTER = 0x60,
    EFFECT_EMITTER_HOMING_TARGET = 0x80,
    EFFECT_EMITTER_HOMING_CURSOR_TILE = 0xa0,
} effect_emitter_homing_e;

enum { EFFECT_EMITTER_HOMING_MASK = 0xe0 };

/* Initial particle velocity mode: the velocity_inward bit (byte 0x06 mask
 * 0x10) and align_to_unit_facing bit (byte 0x07 mask 0x04) of the emitter's
 * child/velocity word. OUTWARD scatters around the base angles, INWARD aims at
 * the spawn point, CASTER_FACING rotates OUTWARD by the caster's facing, and
 * TOWARD_TARGET (battle_effect_spawn_particle_motion) rotates it along the
 * caster-to-target direction. */
typedef enum effect_emitter_velocity {
    EFFECT_EMITTER_VELOCITY_OUTWARD = 0x000,
    EFFECT_EMITTER_VELOCITY_INWARD = 0x010,
    EFFECT_EMITTER_VELOCITY_TOWARD_TARGET = 0x400,
    EFFECT_EMITTER_VELOCITY_CASTER_FACING = 0x410,
} effect_emitter_velocity_e;

enum { EFFECT_EMITTER_VELOCITY_MASK = 0x410 };

/* Bits 0-2 of an effect timeline camera command: the tracks the keyframe moves. */
enum {
    BATTLE_EFFECT_CAMERA_TRACK_ROTATION = 0x1,
    BATTLE_EFFECT_CAMERA_TRACK_POSITION = 0x2,
    BATTLE_EFFECT_CAMERA_TRACK_ZOOM = 0x4,
};

/* The camera table battle_camera_find_active_keyframe found its keyframe in: the high half of the
 * index it reports. */
enum {
    BATTLE_EFFECT_CAMERA_TABLE_PHASE1 = 0,
    BATTLE_EFFECT_CAMERA_TABLE_CHILD = 1,
    BATTLE_EFFECT_CAMERA_TABLE_PHASE2 = 2,
};

/* Bits 0x1e0 of an effect timeline camera command: where the track's target value comes from.
 * The zoom command takes the command's vector for every source but CURRENT and SAVED. */
typedef enum battle_effect_camera_source {
    BATTLE_EFFECT_CAMERA_SOURCE_TARGET = 0x000,           /* the effect target the command names */
    BATTLE_EFFECT_CAMERA_SOURCE_CURRENT_YAW_ROLL = 0x020, /* angle: given pitch; current yaw and roll plus the given */
    BATTLE_EFFECT_CAMERA_SOURCE_GIVEN = 0x040,            /* the command's vector */
    BATTLE_EFFECT_CAMERA_SOURCE_SAVED_PITCH = 0x060,      /* angle: saved pitch, current yaw and roll */
    BATTLE_EFFECT_CAMERA_SOURCE_MAP_CENTRE = 0x080,
    BATTLE_EFFECT_CAMERA_SOURCE_CURRENT = 0x0c0,
    BATTLE_EFFECT_CAMERA_SOURCE_SAVED = 0x100,
    BATTLE_EFFECT_CAMERA_SOURCE_CASTER = 0x140,
    BATTLE_EFFECT_CAMERA_SOURCE_TARGET_AVERAGE = 0x180, /* position: the mean of all unit targets */
    BATTLE_EFFECT_CAMERA_SOURCE_CURSOR_TILE = 0x1c0,
} battle_effect_camera_source_e;

enum { BATTLE_EFFECT_CAMERA_SOURCE_MASK = 0x1e0 };

/* Bits 0x1e00 of an effect timeline camera command, kept in g_battle_effect_camera_rotation_mode,
 * _position_mode and _zoom_mode while the move runs: how the track goes from its start to its
 * target over the command's duration. */
typedef enum battle_effect_camera_mode {
    BATTLE_EFFECT_CAMERA_MODE_IDLE = 0x0000,
    BATTLE_EFFECT_CAMERA_MODE_SNAP = 0x0200,
    BATTLE_EFFECT_CAMERA_MODE_COSINE = 0x0400,
    BATTLE_EFFECT_CAMERA_MODE_COSINE_Q12 = 0x0600,
    BATTLE_EFFECT_CAMERA_MODE_LINEAR = 0x0800,
    BATTLE_EFFECT_CAMERA_MODE_ADD_ACCELERATING = 0x0a00, /* add a step that grows from 0 to the target */
    BATTLE_EFFECT_CAMERA_MODE_ADD = 0x0c00,              /* add the target every frame */
    BATTLE_EFFECT_CAMERA_MODE_ADD_DECELERATING = 0x0e00, /* add a step that shrinks from the target to 0 */
    BATTLE_EFFECT_CAMERA_MODE_SHAKE_GROWING = 0x1000,    /* random offsets around the start */
    BATTLE_EFFECT_CAMERA_MODE_SHAKE = 0x1200,
    BATTLE_EFFECT_CAMERA_MODE_SHAKE_DECAYING = 0x1400,
} battle_effect_camera_mode_e;

enum { BATTLE_EFFECT_CAMERA_MODE_MASK = 0x1e00 };

/* Ordering-table depth clamp shared by the EFFECT renderers: a depth below MIN
 * becomes MIN and one at or beyond LIMIT (the OT slot count) becomes MAX. */
enum {
    EFFECT_OT_DEPTH_MIN = 4,
    EFFECT_OT_DEPTH_MAX = 0x17f,
    EFFECT_OT_DEPTH_LIMIT = 0x180,
    EFFECT_SPRITE_OT_DEPTH_MAX = 0x17e, /* the farthest depth of a particle sprite */
};

/* primitive_type of battle_effect_alloc_primitive: an index into g_battle_effect_primitive_sizes. */
typedef enum battle_effect_primitive_type {
    BATTLE_EFFECT_PRIMITIVE_FT4 = 0,
    BATTLE_EFFECT_PRIMITIVE_DR_MODE = 1,
} battle_effect_primitive_type_e;

/* Size of the per-effect arena used by battle_effect_alloc_primitive. */
enum { BATTLE_EFFECT_PRIMITIVE_BUFFER_SIZE = 0x8000 };

/* Initial Z coordinate of each particle sprite's four corner vectors. */
enum { BATTLE_EFFECT_CORNER_Z_DEFAULT = 0x200 };

/* Word 0x06 of an emitter record (effect_geometry_entry_t.flags), copied to each particle's flags
 * (effect_work_record_t.flags). Bits 0x410 are effect_emitter_velocity_e. A child mode of 1 or 2
 * enables the child emitter; the engine does not tell the two apart. */
enum {
    EFFECT_EMITTER_FLAG_CHILD_ON_DEATH_MASK = 0x0003, /* spawn child_emitter_on_death when the particle dies */
    EFFECT_EMITTER_FLAG_CHILD_ON_DEATH_MODE_1 = 0x0001,
    EFFECT_EMITTER_FLAG_CHILD_ON_DEATH_MODE_2 = 0x0002,
    EFFECT_EMITTER_FLAG_CHILD_MID_LIFE_MASK = 0x000c, /* spawn child_emitter_mid_life every frame it lives */
    EFFECT_EMITTER_FLAG_CHILD_MID_LIFE_MODE_1 = 0x0004,
    EFFECT_EMITTER_FLAG_CHILD_MID_LIFE_MODE_2 = 0x0008,
    EFFECT_EMITTER_FLAG_COLOUR_CURVES = 0x0040, /* tint by the three colour curves; clear, a neutral 0x80 */
    /* A particle that comes within this distance of its homing target on all three axes ends its
     * life: the field shifted right by SHIFT is 16, 32 or 48 world units, 0 for never. */
    EFFECT_EMITTER_FLAG_ARRIVAL_DISTANCE_MASK = 0x0300,
    EFFECT_EMITTER_FLAG_ARRIVAL_DISTANCE_SHIFT = 4,
};

/* texture_page_flags of the effect geometry records. */
enum {
    EFFECT_TEXTURE_PAGE_ABR_MASK = 0x03, /* the semi-transparency rate, shifted into bits 5-6 of the tpage */
    EFFECT_TEXTURE_PAGE_FLAG_CLEAR_ABE = 0x04,
};

/* Word 0x02 of an emitter record (effect_geometry_entry_t.placement_flags), copied to each
 * particle's kind_flags. Bits 0x0e0 are effect_emitter_homing_e and bits 0xe00
 * effect_emitter_origin_e. The work kind says what a particle's data is: a sprite node placed one
 * of the four EFFECT_LIST_NODE_* ways, or a heap block. Retail emitters use SPRITE_WORLD and
 * SPRITE_WORLD_UNSCALED only. */
enum {
    EFFECT_EMITTER_ORIENT_TO_VELOCITY = 0x0002, /* turn the sprite along its heading on screen */
    EFFECT_EMITTER_BOX_SPREAD = 0x0100,         /* spawn inside the spread box; clear, on the spread ellipsoid */
    EFFECT_WORK_KIND_MASK = 0xf000,
};

/* The work-record payload selected by the high nibble of placement_flags. */
typedef enum effect_work_kind {
    EFFECT_WORK_KIND_SPRITE_WORLD = 0x0000,
    EFFECT_WORK_KIND_SPRITE_SCREEN_OFFSET = 0x1000,
    EFFECT_WORK_KIND_SPRITE_WORLD_UNSCALED = 0x2000,
    EFFECT_WORK_KIND_SPRITE_SCREEN = 0x3000,
    EFFECT_WORK_KIND_HEAP_BLOCK = 0x4000,
} effect_work_kind_e;

/* A particle channel keyframe's action word (battle_effect_keyframe_table_t.action), run by
 * battle_effect_run_keyframe_actions when the keyframe is entered. Bit 7 is set in a few retail
 * files and read by nothing. */
enum {
    EFFECT_KEYFRAME_ACTION_CALLBACK_MASK = 0x0007,
    EFFECT_KEYFRAME_ACTION_FLAG_CASTER_ONLY = 0x0008,
    EFFECT_KEYFRAME_ACTION_FLAG_POST_ACTION_DISPLAY = 0x0010,
    EFFECT_KEYFRAME_ACTION_FLAG_UPDATE_DISPLAY = 0x0020,
    EFFECT_KEYFRAME_ACTION_FLAG_TARGET_ANIMATION = 0x0040,
    EFFECT_KEYFRAME_ACTION_ANIMATION_MASK = 0xff00,
    EFFECT_KEYFRAME_ACTION_ANIMATION_SHIFT = 8,
};

/* Color and count header followed by one pointer for each sprite part. */
typedef struct battle_effect_sprite_block {
    struct {
        u8 red;
        u8 green;
        u8 blue;
        u8 sprite_count;
    } color;
    u8 _unused_04[4];
    struct battle_effect_sprite_part* frames[1];
} battle_effect_sprite_block_t;

/* Child timeline counters: twelve active tracks and one extra reset slot. */
typedef union battle_effect_child_track_counters {
    s16 values[13];
    struct {
        s16 particle[5];
        s16 sound[3];
        s16 surroundings;
        s16 caster;
        s16 target;
        s16 screen;
        s16 _unused_18; /* extra halfword cleared by the child reset */
    } tracks;
} battle_effect_child_track_counters_t;

typedef union battle_effect_child_track_steps {
    s16 values[13];
    struct {
        u16 particle[5];     /* loaded unsigned by opcode 28 */
        u8 _unused_0a[0x10]; /* remaining slots are reset but not stepped */
    } tracks;
} battle_effect_child_track_steps_t;

typedef struct battle_effect_child_timeline_state {
    battle_effect_child_track_counters_t keyframe;  /* 0x00; record +0x2a */
    battle_effect_child_track_counters_t remaining; /* 0x1a; record +0x44 */
    battle_effect_child_track_steps_t step;         /* 0x34; record +0x5e */
} battle_effect_child_timeline_state_t;
typedef char
    battle_effect_child_timeline_state_size_must_be_0x4e[sizeof(battle_effect_child_timeline_state_t) == 0x4e ? 1 : -1];

/* Parent counters: ten particle tracks, fourteen colour/sound tracks and
 * two extra halfwords. The reset clears all 26, not just the 24 stepped tracks. */
typedef union battle_effect_parent_track_counters {
    s16 values[26];
    struct {
        s16 particle[2][5];
        s16 surroundings[2];
        s16 caster[2];
        s16 target[2];
        s16 screen[2];
        s16 sound[3][2];
        s16 _unused_30[2]; /* extra halfwords cleared by the parent reset */
    } tracks;
} battle_effect_parent_track_counters_t;

typedef union battle_effect_parent_track_steps {
    s16 values[26];
    struct {
        s16 particle[2][5];
        u8 _unused_14[0x10]; /* reset only */
        /* Six two-byte addresses inside the step array. Opcode 29 passes each
         * high byte to the sound helper's unused argument; no sound helper reads it. */
        u8 _unknown_24[3][2][2]; /* record +0xba; the original purpose is unknown */
        s16 _unused_30[2];       /* extra halfwords cleared by the parent reset */
    } tracks;
} battle_effect_parent_track_steps_t;

typedef struct battle_effect_parent_timeline_state {
    s16 spawn_countdown;                             /* 0x00; record +0x2a */
    u16 spawned_count;                               /* 0x02; record +0x2c */
    battle_effect_parent_track_counters_t keyframe;  /* 0x04; record +0x2e */
    battle_effect_parent_track_counters_t remaining; /* 0x38; record +0x62 */
    battle_effect_parent_track_steps_t step;         /* 0x6c; record +0x96 */
    u8 _unused_a0[6];                                /* record +0xca..0xcf; untouched by both timeline views */
} battle_effect_parent_timeline_state_t;

/* Opcodes 28 and 29 interpret the same record storage differently. */
typedef union battle_effect_timeline_state {
    battle_effect_child_timeline_state_t child;
    battle_effect_parent_timeline_state_t parent;
} battle_effect_timeline_state_t;
typedef char battle_effect_timeline_state_size_must_be_0xa6[sizeof(battle_effect_timeline_state_t) == 0xa6 ? 1 : -1];

/* The target and frame travel with the counters of the active timeline. */
typedef struct battle_effect_record_timeline {
    s16 target_index;                     /* 0x00; index into effect-context targets */
    s16 frame;                            /* 0x02 */
    battle_effect_timeline_state_t state; /* 0x04; opcode 28 child or opcode 29 parent */
} battle_effect_record_timeline_t;
typedef char battle_effect_record_timeline_size_must_be_0xaa[sizeof(battle_effect_record_timeline_t) == 0xaa ? 1 : -1];

/* effect_list_node_t.kind: how battle_effect_render_particle_sprite places the sprite, and the
 * FRAME_CHANGED bit it sets when the sequence picks another frame. */
enum {
    EFFECT_LIST_NODE_FRAME_CHANGED = 0x1,
    EFFECT_LIST_NODE_PLACEMENT_MASK = 0x6,
};

/* Placement subfield of effect_list_node_t.kind. */
typedef enum effect_list_node_placement {
    EFFECT_LIST_NODE_WORLD = 0x0,          /* projected position, scaled by the camera zoom */
    EFFECT_LIST_NODE_SCREEN_OFFSET = 0x2,  /* screen position from the camera translation, scaled by the zoom */
    EFFECT_LIST_NODE_WORLD_UNSCALED = 0x4, /* projected position, no zoom */
    EFFECT_LIST_NODE_SCREEN = 0x6,         /* screen position as stored, no zoom */
} effect_list_node_placement_e;

/* effect_list_node_t.depth_mode: the third byte of a sequence frame, which adjusts the sprite's
 * ordering-table depth. */
typedef enum effect_sprite_depth_mode {
    EFFECT_SPRITE_DEPTH_NEARER_8 = 1,
    EFFECT_SPRITE_DEPTH_FIXED_8 = 2,
    EFFECT_SPRITE_DEPTH_FIXED_FARTHEST = 3, /* EFFECT_SPRITE_OT_DEPTH_MAX */
    EFFECT_SPRITE_DEPTH_FIXED_16 = 4,
    EFFECT_SPRITE_DEPTH_NEARER_16 = 5,
} effect_sprite_depth_mode_e;

/* Opcodes in the low seven bits of a sprite-sequence control byte. */
typedef enum effect_sequence_opcode {
    EFFECT_SEQUENCE_OP_LOOP = 1,       /* back to the first byte */
    EFFECT_SEQUENCE_OP_SET_OFFSET = 2, /* two s16 sprite offsets follow */
    EFFECT_SEQUENCE_OP_ADD_OFFSET = 3, /* two s8 sprite offset deltas follow */
} effect_sequence_opcode_e;

/* The high bit distinguishes control bytes from ordinary frame indices. */
enum {
    EFFECT_SEQUENCE_OP_MASK = 0x7f,
    EFFECT_SEQUENCE_OP_FLAG = 0x80,
};

/* 0x24-byte sprite node at 0x801c00a4: the animated sprite of one particle
 * (effect_work_record_t.data). Links are node indexes into that array; node 0
 * is the sentinel. g_battle_effect_active_list_node_head is the list head and
 * g_battle_effect_free_list_node_head the free-list head. */
typedef struct effect_list_node {
    u16 prev;                  /* 0x00 */
    u16 next;                  /* 0x02 */
    u16 index;                 /* 0x04: this node's own array index */
    s16 kind;                  /* 0x06: EFFECT_LIST_NODE_* */
    s16 sprite_offset_x;       /* 0x08: added to x when drawn */
    s16 sprite_offset_y;       /* 0x0a: added to y when drawn */
    s16 x;                     /* 0x0c: from the owning work record's position */
    s16 y;                     /* 0x0e */
    s16 z;                     /* 0x10 */
    s16 screen_rotation_angle; /* 0x12: battle_effect_update_node_from_work stores the velocity heading here */
    u8 depth_mode;             /* 0x14: effect_sprite_depth_mode_e */
    u8 _padding_15;            /* aligns frame_timer */
    s16 frame_timer;           /* 0x16: counts down by 2; the next opcode runs at <= 0 */
    u8* sequence_data;         /* 0x18: base of the u8 animation sequence */
    u16 sequence_offset;       /* 0x1c: byte offset of the next sequence opcode */
    u8 frame_group_offset;     /* 0x1e: byte offset into the frame-group offset table */
    u8 sprite_frame_index;     /* 0x1f: frame id from the sequence opcode */
    battle_effect_sprite_block_t* sprite_block; /* 0x20 */
} effect_list_node_t;
typedef char assert_effect_list_node_size[sizeof(effect_list_node_t) == 0x24 ? 1 : -1];

/* Motion state of an effect slot (battle_effect_slot_t +0x98) and of a
 * particle (effect_work_record_t +0x08), stepped once a frame by
 * battle_effect_step_motion and battle_effect_integrate_particle_motion:
 * position gains velocity; velocity keeps (inertia -
 * g_battle_effect_inertia_threshold) / inertia of itself, gains
 * acceleration << 12 / inertia and gains g_battle_effect_gravity scaled by
 * weight (4.12 fixed point); acceleration gains acceleration_step. */
typedef struct battle_effect_motion {
    s16 inertia;                 /* 0x00 */
    s16 weight;                  /* 0x02; scales the gravity vector */
    vec3s32_t position;          /* 0x04; 20.12 fixed */
    vec3s32_t velocity;          /* 0x10 */
    vec3s32_t acceleration;      /* 0x1c */
    vec3s32_t acceleration_step; /* 0x28 */
} battle_effect_motion_t;
typedef char battle_effect_motion_size_must_be_0x34[(sizeof(battle_effect_motion_t) == 0x34) ? 1 : -1];

/* 0x58-byte particle on an effect record's work list. The emitter initializes
 * its motion, homing and child-emitter state; the integrator advances it. */
typedef struct effect_work_record {
    struct effect_work_record* prev; /* 0x00 */
    struct effect_work_record* next; /* 0x04 */
    battle_effect_motion_t motion;   /* 0x08 */
    vec3s16_t target;                /* 0x3c */
    s16 lifetime;                    /* 0x42; frames or EFFECT_PARTICLE_LIFETIME_ANIMATION_DRIVEN */
    u8 _unused_44;
    u8 homing_curve_index;         /* 0x45 */
    u8 colour_curves[3];           /* 0x46; RGB animation curves */
    u8 _padding_49;                /* aligns homing_strength */
    s16 homing_strength;           /* 0x4a */
    s16 kind_flags;                /* 0x4c; EFFECT_EMITTER_* placement and EFFECT_WORK_KIND_* */
    u16 flags;                     /* 0x4e; EFFECT_EMITTER_FLAG_* */
    s16 curve_frame;               /* 0x50 */
    u8 child_emitter_on_death;     /* 0x52 */
    u8 child_emitter_mid_life;     /* 0x53 */
    struct effect_list_node* data; /* 0x54; sprite node, or heap work for EFFECT_WORK_KIND_HEAP_BLOCK */
} effect_work_record_t;
typedef char assert_effect_work_record_size[sizeof(effect_work_record_t) == 0x58 ? 1 : -1];

enum { EFFECT_RECORD_FLAG_ALLOCATED = 0x1 };

/* One running effect script: a 0xf8-byte record of g_effect_state_records.
 * The root record runs the effect file's script from its start; the parent
 * timeline (opcode 29) starts one child record per effect target, and opcode
 * 02 starts a child the script keeps track of. Record 0 is the list sentinel,
 * so an index of 0 means none. */
typedef struct effect_record {
    s16 next_index;       /* 0x00: next record on the allocated or the available list */
    s16 record_index;     /* 0x02: this record's array index */
    s16 parent_index;     /* 0x04: the record that started this one; 0 for the root */
    s16 pc;               /* 0x06: byte offset of the current instruction in script */
    u8* script;           /* 0x08: borrowed script byte stream, shared with child records */
    s16 child_indices[4]; /* 0x0c: the child records script opcode 02 started, by child slot */
    s16 counters[4];   /* 0x14: script counters; the counter opcodes select one by their second byte's top two bits */
    s16 work_count;    /* 0x1c: particles on work_head */
    u16 flags;         /* 0x1e: EFFECT_RECORD_FLAG_ALLOCATED */
    s16 frame_counter; /* 0x20: frames the script has run, 0 on its first; the frame of script-spawned emitters */
    u8 callback_phase[4];                     /* 0x22: effect_phase_e of the handler in each callback slot */
    battle_effect_record_timeline_t timeline; /* 0x26; target, frame and timeline counters */
    effect_work_record_t* work_head;          /* 0xd0: its particles */
    battle_effect_callback_t callbacks[4];    /* 0xd4: the native handler bound to each callback slot */
    void* work_slots[4];                      /* 0xe4: the work block each callback slot's handler allocated */
    battle_effect_callback_t file_function;   /* 0xf4: the native function script opcode 07 last called */
} effect_record_t;

/* pc is a signed byte offset into the shared, halfword-aligned script stream. */
static __inline__ battle_effect_script_instruction_t* battle_effect_get_current_script_instruction(
    effect_record_t* record) {
    return (battle_effect_script_instruction_t*)(record->script + record->pc);
}

static __inline__ u16 battle_effect_get_current_script_header(effect_record_t* record) {
    /* Loading the word here preserves the retail base/offset register order. */
    return *(u16*)(record->script + record->pc);
}

typedef char assert_effect_record_size[sizeof(effect_record_t) == 0xf8 ? 1 : -1];
typedef char
    effect_record_timeline_offset_must_be_0x26[((unsigned long)&((effect_record_t*)0)->timeline == 0x26) ? 1 : -1];
typedef char
    effect_record_work_head_offset_must_be_0xd0[((unsigned long)&((effect_record_t*)0)->work_head == 0xd0) ? 1 : -1];

/* One playback step of the animation script referenced by an effect slot:
 * a hold count and the frame id written to the slot. */
typedef struct battle_effect_anim_step {
    u8 hold;  /* 0x00 */
    u8 frame; /* 0x01 */
} battle_effect_anim_step_t;

typedef struct battle_effect_anim_script {
    s16 count;                         /* 0x00 */
    battle_effect_anim_step_t step[1]; /* 0x02 */
} battle_effect_anim_script_t;

/* 0xcc-byte effect slots at 0x801b9278, chained through the leading next/prev
 * byte pair; slot 0 is the null sentinel. g_battle_effect_active_slot_head heads the active chain
 * and g_battle_effect_slot_free_head the free chain. 0x84..0x87 has the battle_effect_sprite_block_t
 * layout (colour plus the frame byte); battle_effect_init_trap_animation resets
 * the colour and writes the halfwords at 0x88 and 0x8e. */

typedef struct battle_effect_slot {
    u8 next;                             /* 0x00 */
    u8 prev;                             /* 0x01 */
    u8 _padding_02[2];                   /* aligns modes */
    DR_MODE modes[2][2];                 /* 0x04; two draw modes per buffer */
    POLY_FT4 quads[2];                   /* 0x34; one quad per buffer */
    u8 red;                              /* 0x84 */
    u8 green;                            /* 0x85 */
    u8 blue;                             /* 0x86 */
    u8 frame;                            /* 0x87 */
    u16 clut;                            /* 0x88 */
    s16 step_index;                      /* 0x8a */
    s16 hold;                            /* 0x8c */
    s16 angle;                           /* 0x8e; billboard quad rotation */
    battle_effect_anim_script_t* script; /* 0x90 */
    s16 life;                            /* 0x94: negative runs until the script wraps */
    u8 _padding_96[0x02];                /* aligns motion */
    battle_effect_motion_t motion;       /* 0x98 */
} battle_effect_slot_t;
typedef char assert_battle_effect_slot_size[sizeof(battle_effect_slot_t) == 0xcc ? 1 : -1];

/* The spawn block at 0x14-0x4b of an emitter record, as the
 * battle_effect_interpolate_emitter_* helpers read it; the native renderers
 * that spawn like the particle emitter embed it. The spread is the ellipsoid
 * or box half-extent around the spawn position and the velocity spread
 * randomizes the base angles. */
typedef struct effect_emitter_geometry_view {
    vec3s16_t spawn_position_start;      /* 0x14 */
    vec3s16_t spawn_position_end;        /* 0x1a */
    vec3s16_t spawn_spread_start;        /* 0x20 */
    vec3s16_t spawn_spread_end;          /* 0x26 */
    vec3s16_t velocity_base_angle_start; /* 0x2c */
    vec3s16_t velocity_base_angle_end;   /* 0x32 */
    vec3s16_t velocity_spread_start;     /* 0x38 */
    vec3s16_t velocity_spread_end;       /* 0x3e */
    s16 inertia_start[2];                /* 0x44: min, max */
    s16 inertia_end[2];                  /* 0x48 */
} effect_emitter_geometry_view_t;
typedef char assert_effect_emitter_geometry_view_size[sizeof(effect_emitter_geometry_view_t) == 0x38 ? 1 : -1];

/* One 0xc4-byte emitter record of the effect file's particle-system section,
 * with the meanings the BATTLE particle emitter
 * (battle_effect_spawn_emitter_particles) gives it. A native EFFECT renderer
 * reuses the record's slots and their curve bindings for its own parameters
 * and reads it through its own view (effect_geometry_table_t.entries).
 *
 * A value given as a start and an end is lerped between them by an animation
 * curve; a min, max pair is the range of the random value each new particle
 * draws. Each curve nibble of curve_indices selects the curve (0 = none,
 * otherwise the curve index + 1) of one such value:
 *   curve_indices[0]: 0 spawn position, 1 spawn spread, 2 velocity base angle,
 *     3 velocity spread, 4 inertia, 5 the pair at 0x4c (lerped and discarded),
 *     6 weight, 7 radial speed;
 *   curve_indices[1]: 0 acceleration, 1 acceleration step, 2 lifetime,
 *     3 target offset, 4 unread, 5 particle count, 6 spawn interval,
 *     bits 28-29 homing strength; bits 30-31 are the particle's homing curve. */
typedef struct effect_geometry_entry {
    u8 _unused_00;
    u8 animation_index;  /* 0x01: the sprite sequence its particles play */
    u16 placement_flags; /* 0x02: EFFECT_EMITTER_* placement bits and EFFECT_WORK_KIND_*; the particle's kind_flags */
    u8 animation_frame_group; /* 0x04: frame group of that sequence's frames */
    u8 _padding_05;           /* aligns flags */
    u16 flags;                /* 0x06: EFFECT_EMITTER_FLAG_* and effect_emitter_velocity_e; the particle's flags */
    u32 curve_indices[2];     /* 0x08 */
    u32 colour_curves;        /* 0x10: nibbles 0-2 = r/g/b curve indexes, used raw */
    vec3s16_t spawn_position_start;      /* 0x14: offset from the origin effect_emitter_origin_e names */
    vec3s16_t spawn_position_end;        /* 0x1a */
    vec3s16_t spawn_spread_start;        /* 0x20: half-extent of the spawn box or ellipsoid */
    vec3s16_t spawn_spread_end;          /* 0x26 */
    vec3s16_t velocity_base_angle_start; /* 0x2c */
    vec3s16_t velocity_base_angle_end;   /* 0x32 */
    vec3s16_t velocity_spread_start;     /* 0x38: random angle added to each base angle; halved when it has no curve */
    vec3s16_t velocity_spread_end;       /* 0x3e */
    s16 inertia_start[2];                /* 0x44: min, max */
    s16 inertia_end[2];                  /* 0x48 */
    s16 _unknown_4c[2];                  /* 0x4c: lerped to _unknown_50 and discarded */
    s16 _unknown_50[2];                  /* 0x50 */
    s16 weight_start[2];                 /* 0x54: min, max; scales gravity */
    s16 weight_end[2];                   /* 0x58 */
    s16 radial_speed_start[2];           /* 0x5c: min, max */
    s16 radial_speed_end[2];             /* 0x60 */
    s16 acceleration_start[6];           /* 0x64: x, y and z min, max pairs */
    s16 acceleration_end[6];             /* 0x70 */
    s16 acceleration_step_start[6];      /* 0x7c: x, y and z min, max pairs */
    s16 acceleration_step_end[6];        /* 0x88 */
    s16 lifetime_start[2];               /* 0x94: min, max, in frames */
    s16 lifetime_end[2];                 /* 0x98 */
    s16 target_offset_start[3];          /* 0x9c: homing target, an offset from what effect_emitter_homing_e names */
    s16 target_offset_end[3];            /* 0xa2 */
    u8 _unused_a8[0x08];
    s16 particle_count_start;     /* 0xb0: particles per spawn */
    s16 particle_count_end;       /* 0xb2 */
    s16 spawn_interval_start;     /* 0xb4: frames between spawns */
    s16 spawn_interval_end;       /* 0xb6 */
    s16 homing_strength_start[2]; /* 0xb8: min, max */
    s16 homing_strength_end[2];   /* 0xbc */
    u8 child_emitter_on_death;    /* 0xc0: see EFFECT_EMITTER_FLAG_CHILD_ON_DEATH_MASK */
    u8 child_emitter_mid_life;    /* 0xc1: see EFFECT_EMITTER_FLAG_CHILD_MID_LIFE_MASK */
    u8 _padding_c2[0x02];         /* tail padding to 4-byte alignment */
} effect_geometry_entry_t;
typedef char assert_effect_geometry_entry_size[sizeof(effect_geometry_entry_t) == 0xc4 ? 1 : -1];

/* The scrolling textured quad grids' (2x2, 4x4 and 8x8) and the wave mesh's
 * view of one emitter record. Curve nibbles: curve_indices[0] 2 scroll angle
 * and 7 scroll speed; the wave mesh adds curve_indices[0] 1 and 3, the phase
 * speeds of its vertical and horizontal waves (the vertical pair is the
 * emitter's spawn spread at 0x20, read through the emitter helper), and
 * curve_indices[1] 0, the waves' shape. */
typedef struct effect_textured_quad_geometry_view {
    u8 _unused_00[0x06];
    u16 flags;            /* 0x06: EFFECT_EMITTER_FLAG_COLOUR_CURVES */
    u32 curve_indices[2]; /* 0x08 */
    u32 colour_curves;    /* 0x10: nibbles 0-2 = r/g/b */
    u16 texture_u;        /* 0x14 */
    u16 texture_v;        /* 0x16 */
    u8 _unused_18[0x16];
    s16 scroll_angle_start; /* 0x2e */
    u8 _unused_30[0x04];
    s16 scroll_angle_end; /* 0x34 */
    u8 _unused_36[0x02];
    s16 wave_phase_speed_start[2]; /* 0x38 */
    u8 _unused_3c[0x02];
    s16 wave_phase_speed_end[2]; /* 0x3e */
    u8 _unused_42[0x0a];
    s16 texture_page_flags; /* 0x4c: EFFECT_TEXTURE_PAGE_* */
    s16 brightness_row;     /* 0x4e */
    u8 _unused_50[0x04];
    s16 ot_depth; /* 0x54 */
    u8 _unused_56[0x06];
    s16 scroll_speed_start; /* 0x5c */
    u8 _unused_5e[0x02];
    s16 scroll_speed_end; /* 0x60 */
    u8 _unused_62[0x02];
    s16 wave_y_start[6]; /* 0x64 */
    s16 wave_y_end[6];   /* 0x70 */
    s16 wave_x_start[6]; /* 0x7c */
    s16 wave_x_end[6];   /* 0x88 */
    u8 _unused_94[0x14];
    s16 texture_width;  /* 0xa8 */
    s16 texture_height; /* 0xaa */
    u8 _unused_ac[0x18];
} effect_textured_quad_geometry_view_t;
typedef char
    assert_effect_textured_quad_geometry_view_size[sizeof(effect_textured_quad_geometry_view_t) == 0xc4 ? 1 : -1];

/* This renderer's view of one effect_geometry_entry_t. */
typedef struct effect_particle_ribbon_geometry_view {
    u8 _unused_00[0x02];
    u16 placement_flags; /* 0x02 */
    u8 _unused_04[0x02];
    u16 flags;                              /* 0x06 */
    u32 curve_indices[2];                   /* 0x08 */
    u32 colour_curves;                      /* 0x10 */
    effect_emitter_geometry_view_t emitter; /* 0x14: read by the battle_effect_interpolate_emitter_* helpers */
    u16 texture_page_flags;                 /* 0x4c */
    s16 brightness_row;                     /* 0x4e */
    u8 _unused_50[0x04];
    s16 ot_depth; /* 0x54 */
    u8 _unused_56[0x06];
    s16 radial_speed_min_start; /* 0x5c */
    s16 radial_speed_max_start; /* 0x5e */
    s16 radial_speed_min_end;   /* 0x60 */
    s16 radial_speed_max_end;   /* 0x62 */
    s16 texture_u;              /* 0x64 */
    u8 _unused_66[0x02];
    s16 texture_v; /* 0x68 */
    u8 _unused_6a[0x02];
    s16 frame_count; /* 0x6c */
    s16 frame_delay; /* 0x6e */
    u8 _unused_70[0x0c];
    s16 target_spread_start[6]; /* 0x7c: x/y/z at even indices */
    s16 target_spread_end[6];   /* 0x88 */
    s16 lifetime_start[2];      /* 0x94 */
    s16 lifetime_end[2];        /* 0x98 */
    s16 target_offset_start[3]; /* 0x9c */
    s16 target_offset_end[3];   /* 0xa2 */
    s16 texture_width;          /* 0xa8 */
    s16 texture_height;         /* 0xaa */
    u8 _unused_ac[0x04];
    s16 particle_count_start; /* 0xb0 */
    s16 particle_count_end;   /* 0xb2 */
    s16 spawn_interval_start; /* 0xb4 */
    s16 spawn_interval_end;   /* 0xb6 */
    u8 _unused_b8[0x08];
    u8 child_emitter_on_death; /* 0xc0 */
    u8 child_emitter_mid_life; /* 0xc1 */
    u8 _padding_c2[0x02];      /* tail padding to 4-byte alignment */
} effect_particle_ribbon_geometry_view_t;
typedef char
    assert_effect_particle_ribbon_geometry_view_size[sizeof(effect_particle_ribbon_geometry_view_t) == 0xc4 ? 1 : -1];

/* This renderer's view of one effect_geometry_entry_t. It keeps the particle
 * emitter's slot-to-curve bindings but gives the slots its own meanings; each
 * field notes the emitter slot it occupies, and offsets it never reads stay
 * unknown. Curve nibbles: curve_indices[0] 0 centre, 1 radius, 2 velocity,
 * 3 acceleration, 4 angle, 7 radius step; curve_indices[1] 0 radius step
 * delta, 1 scroll, 3 angle step, 6 angle step delta. */
typedef struct effect_polar_mesh_geometry_view {
    u8 _unused_00[0x06];
    u16 flags;              /* 0x06: bit 6 = per-channel colour curves */
    u32 curve_indices[2];   /* 0x08 */
    u32 colour_curves;      /* 0x10: nibbles 0-2 = r/g/b */
    vec3s16_t center_start; /* 0x14: spawn position, read by battle_effect_interpolate_emitter_spawn_position */
    vec3s16_t center_end;   /* 0x1a */
    s16 radius_start;       /* 0x20: spawn spread x */
    u8 _unused_22[0x04];
    s16 radius_end; /* 0x26 */
    u8 _unused_28[0x04];
    s16 velocity_angle_start; /* 0x2c: velocity base angle x */
    s16 velocity_speed_start; /* 0x2e: velocity base angle y */
    u8 _unused_30[0x02];
    s16 velocity_angle_end; /* 0x32 */
    s16 velocity_speed_end; /* 0x34 */
    u8 _unused_36[0x02];
    s16 acceleration_angle_start; /* 0x38: velocity spread x */
    s16 acceleration_speed_start; /* 0x3a: velocity spread y */
    u8 _unused_3c[0x02];
    s16 acceleration_angle_end; /* 0x3e */
    s16 acceleration_speed_end; /* 0x40 */
    u8 _unused_42[0x02];
    s16 angle_start; /* 0x44: minimum inertia */
    u8 _unused_46[0x02];
    s16 angle_end; /* 0x48 */
    u8 _unused_4a[0x02];
    u16 texture_page_flags; /* 0x4c: bits 0-1 tpage x, bit 2 clears ABE */
    s16 brightness_row;     /* 0x4e */
    u8 _unused_50[0x04];
    s16 ot_depth; /* 0x54 */
    u8 _unused_56[0x06];
    s16 radius_step_start; /* 0x5c: minimum radial speed */
    u8 _unused_5e[0x02];
    s16 radius_step_end; /* 0x60 */
    u8 _unused_62[0x02];
    s16 radius_step_delta_start; /* 0x64: minimum x acceleration */
    u8 _unused_66[0x0a];
    s16 radius_step_delta_end; /* 0x70 */
    u8 _unused_72[0x0a];
    s16 scroll_u_speed_start; /* 0x7c: minimum x acceleration step */
    u8 _unused_7e[0x02];
    s16 scroll_v_speed_start; /* 0x80: minimum y acceleration step */
    u8 _unused_82[0x06];
    s16 scroll_u_speed_end; /* 0x88 */
    u8 _unused_8a[0x02];
    s16 scroll_v_speed_end; /* 0x8c */
    u8 _unused_8e[0x0e];
    s16 angle_step_start; /* 0x9c: target offset x */
    u8 _unused_9e[0x04];
    s16 angle_step_end; /* 0xa2 */
    u8 _unused_a4[0x04];
    s16 texture_width;  /* 0xa8 */
    s16 texture_height; /* 0xaa */
    u8 _unused_ac[0x08];
    s16 angle_step_delta_start; /* 0xb4: spawn interval */
    s16 angle_step_delta_end;   /* 0xb6 */
    u16 texture_u;              /* 0xb8: minimum start homing strength */
    u16 texture_v;              /* 0xba: maximum start homing strength */
    u8 _unused_bc[0x08];
} effect_polar_mesh_geometry_view_t;
typedef char assert_effect_polar_mesh_geometry_view_size[sizeof(effect_polar_mesh_geometry_view_t) == 0xc4 ? 1 : -1];

/* This renderer's view of one effect_geometry_entry_t. It shares the layout of
 * effect_summon_mesh_geometry_view_t and adds the per-row radius terms at
 * 0x6c/0x78 and 0x84/0x90. */
typedef struct effect_ring_mesh_geometry_view {
    u8 _unused_00[0x02];
    u16 placement_flags; /* 0x02: bits 9-11 select how the origin is placed */
    u8 _unused_04[0x02];
    u16 flags;              /* 0x06: bit 6 = per-channel colour curves */
    u32 scroll_curves;      /* 0x08: nibbles 0 origin, 2 texture, 7 spin speed */
    u32 wave_curves;        /* 0x0c: nibbles 0 growth, 1 growth step, 4 radius */
    u32 colour_curves;      /* 0x10: nibbles 0-2 = r/g/b */
    s16 origin_start[3];    /* 0x14 */
    s16 origin_end[3];      /* 0x1a */
    s16 u_span;             /* 0x20 */
    s16 v_span;             /* 0x22 */
    u16 texture_page_flags; /* 0x24: bits 0-1 blend rate, bit 2 clears ABE */
    u8 _unused_26[0x06];
    s16 u_start;        /* 0x2c */
    s16 v_start;        /* 0x2e */
    u16 v_scroll_speed; /* 0x30 */
    s16 u_end;          /* 0x32 */
    s16 v_end;          /* 0x34 */
    u8 _unused_36[0x06];
    s16 brightness_row; /* 0x3c */
    u8 _unused_3e[0x1e];
    s16 spin_speed_start; /* 0x5c */
    u8 _unused_5e[0x02];
    s16 spin_speed_end; /* 0x60 */
    u8 _unused_62[0x02];
    s16 growth_start; /* 0x64 */
    u8 _unused_66[0x02];
    s16 height_start; /* 0x68 */
    u8 _unused_6a[0x02];
    s16 row_radius_start; /* 0x6c */
    u8 _unused_6e[0x02];
    s16 growth_end; /* 0x70 */
    u8 _unused_72[0x02];
    s16 height_end; /* 0x74 */
    u8 _unused_76[0x02];
    s16 row_radius_end; /* 0x78 */
    u8 _unused_7a[0x02];
    s16 growth_step_start; /* 0x7c */
    u8 _unused_7e[0x02];
    s16 height_step_start; /* 0x80 */
    u8 _unused_82[0x02];
    s16 row_radius_step_start; /* 0x84 */
    u8 _unused_86[0x02];
    s16 growth_step_end; /* 0x88 */
    u8 _unused_8a[0x02];
    s16 height_step_end; /* 0x8c */
    u8 _unused_8e[0x02];
    s16 row_radius_step_end; /* 0x90 */
    u8 _unused_92[0x16];
    s16 radius_start; /* 0xa8 */
    u8 _unused_aa[0x02];
    s16 radius_end; /* 0xac */
    u8 _unused_ae[0x16];
} effect_ring_mesh_geometry_view_t;
typedef char assert_effect_ring_mesh_geometry_view_size[sizeof(effect_ring_mesh_geometry_view_t) == 0xc4 ? 1 : -1];

/* This renderer's view of one effect_geometry_entry_t. The shared entry type
 * names several of these offsets for other effect kinds; the curve nibbles
 * select rows of g_battle_effect_animation_curve_data. */
typedef struct effect_summon_mesh_geometry_view {
    u8 _unused_00[0x02];
    u16 placement_flags; /* 0x02: bits 9-11 select how the origin is placed */
    u8 _unused_04[0x02];
    u16 flags;              /* 0x06: bit 6 = per-channel colour curves */
    u32 scroll_curves;      /* 0x08: nibbles 0 origin, 2 texture, 7 spin speed */
    u32 wave_curves;        /* 0x0c: nibbles 0 growth, 1 growth step, 4 radius */
    u32 colour_curves;      /* 0x10: nibbles 0-2 = r/g/b */
    s16 origin_start[3];    /* 0x14 */
    s16 origin_end[3];      /* 0x1a */
    s16 u_span;             /* 0x20 */
    s16 v_span;             /* 0x22 */
    u16 texture_page_flags; /* 0x24: bits 0-1 blend rate, bit 2 clears ABE */
    u8 _unused_26[0x06];
    s16 u_start;        /* 0x2c */
    s16 v_start;        /* 0x2e */
    u16 v_scroll_speed; /* 0x30 */
    s16 u_end;          /* 0x32 */
    s16 v_end;          /* 0x34 */
    u8 _unused_36[0x06];
    s16 brightness_row; /* 0x3c */
    u8 _unused_3e[0x1e];
    s16 spin_speed_start; /* 0x5c */
    u8 _unused_5e[0x02];
    s16 spin_speed_end; /* 0x60 */
    u8 _unused_62[0x02];
    s16 growth_start; /* 0x64 */
    u8 _unused_66[0x02];
    s16 height_start; /* 0x68 */
    u8 _unused_6a[0x06];
    s16 growth_end; /* 0x70 */
    u8 _unused_72[0x02];
    s16 height_end; /* 0x74 */
    u8 _unused_76[0x06];
    s16 growth_step_start; /* 0x7c */
    u8 _unused_7e[0x02];
    s16 height_step_start; /* 0x80 */
    u8 _unused_82[0x06];
    s16 growth_step_end; /* 0x88 */
    u8 _unused_8a[0x02];
    s16 height_step_end; /* 0x8c */
    u8 _unused_8e[0x1a];
    s16 radius_start; /* 0xa8 */
    u8 _unused_aa[0x02];
    s16 radius_end; /* 0xac */
    u8 _unused_ae[0x16];
} effect_summon_mesh_geometry_view_t;
typedef char assert_effect_summon_mesh_geometry_view_size[sizeof(effect_summon_mesh_geometry_view_t) == 0xc4 ? 1 : -1];

/* These renderers' view of one effect_geometry_entry_t. They spawn segments as
 * the BATTLE particle emitter spawns particles, so the fields they share with
 * it keep its names and curve nibbles: curve_indices[0] nibbles 0 spawn
 * position, 1 spawn spread, 2 velocity base angle, 3 velocity spread and
 * 7 radial speed; curve_indices[1] nibbles 1 tip spread, 2 lifetime, 3 target
 * offset, 5 particle count and 6 spawn interval. The remaining named fields
 * are renderer-specific. */
typedef struct effect_tentacle_mesh_geometry_view {
    u8 _unused_00[0x02];
    u16 placement_flags; /* 0x02: bits 9-11 place the spawn origin, bits 5-7 the tip */
    u8 _unused_04[0x02];
    u16 flags;                              /* 0x06: bits 0-1 and 2-3 enable death and mid-life children */
    u32 curve_indices[2];                   /* 0x08 */
    u32 colour_curves;                      /* 0x10 */
    effect_emitter_geometry_view_t emitter; /* 0x14: read by the battle_effect_interpolate_emitter_* helpers */
    u16 texture_page_flags;                 /* 0x4c */
    s16 brightness_row;                     /* 0x4e */
    u8 _unused_50[0x04];
    s16 ot_depth; /* 0x54: added to each quad's depth */
    u8 _unused_56[0x06];
    s16 radial_speed_min_start; /* 0x5c */
    s16 radial_speed_max_start; /* 0x5e */
    s16 radial_speed_min_end;   /* 0x60 */
    s16 radial_speed_max_end;   /* 0x62 */
    s16 texture_u;              /* 0x64 */
    u8 _unused_66[0x02];
    s16 texture_v; /* 0x68 */
    u8 _unused_6a[0x02];
    s16 frame_count; /* 0x6c */
    s16 frame_delay; /* 0x6e */
    u8 _unused_70[0x0c];
    s16 tip_spread_start_x; /* 0x7c */
    u8 _unused_7e[0x02];
    s16 tip_spread_start_y; /* 0x80 */
    u8 _unused_82[0x02];
    s16 tip_spread_start_z; /* 0x84 */
    u8 _unused_86[0x02];
    s16 tip_spread_end_x; /* 0x88 */
    u8 _unused_8a[0x02];
    s16 tip_spread_end_y; /* 0x8c */
    u8 _unused_8e[0x02];
    s16 tip_spread_end_z; /* 0x90 */
    u8 _unused_92[0x02];
    s16 lifetime_start[2];      /* 0x94: min, max; the segment's length */
    s16 lifetime_end[2];        /* 0x98 */
    s16 target_offset_start[3]; /* 0x9c: the tip, added to the anchor chosen by placement_flags bits 5-7 */
    s16 target_offset_end[3];   /* 0xa2 */
    s16 texture_width;          /* 0xa8 */
    s16 texture_height;         /* 0xaa */
    u8 _unused_ac[0x04];
    s16 particle_count_start; /* 0xb0: segments spawned per interval */
    s16 particle_count_end;   /* 0xb2 */
    s16 spawn_interval_start; /* 0xb4 */
    s16 spawn_interval_end;   /* 0xb6 */
    u8 _unused_b8[0x08];
    u8 child_emitter_on_death; /* 0xc0 */
    u8 child_emitter_mid_life; /* 0xc1: spawned at the trailing point each frame */
    u8 _padding_c2[0x02];      /* tail padding to 4-byte alignment */
} effect_tentacle_mesh_geometry_view_t;
typedef char
    assert_effect_tentacle_mesh_geometry_view_size[sizeof(effect_tentacle_mesh_geometry_view_t) == 0xc4 ? 1 : -1];

/* The E454 and E464 particle handlers use the renderer-specific meanings of
 * the emitter record's 0x4c and 0xa8 fields. */
typedef struct effect_particle_vertex_emitter_view {
    u8 _unused_00[0x4c];
    s16 vertex_group; /* 0x4c: selects a vertex in the effect work buffer */
    u8 _unused_4e[0x5a];
    s16 start_frame; /* 0xa8 */
    s16 end_frame;   /* 0xaa */
    u8 _unused_ac[0x18];
} effect_particle_vertex_emitter_view_t;
typedef char
    assert_effect_particle_vertex_emitter_view_size[sizeof(effect_particle_vertex_emitter_view_t) == 0xc4 ? 1 : -1];

/* The existing 0xc4-byte geometry record, interpreted by its owning renderer.
 * The interpolation helpers read the particle-emitter slots shared by these
 * layouts; renderer-specific fields retain their own names. */
typedef union effect_geometry_record {
    effect_geometry_entry_t emitter;
    effect_textured_quad_geometry_view_t textured_quad;
    effect_particle_ribbon_geometry_view_t particle_ribbon;
    effect_polar_mesh_geometry_view_t polar_mesh;
    effect_ring_mesh_geometry_view_t ring_mesh;
    effect_summon_mesh_geometry_view_t summon_mesh;
    effect_tentacle_mesh_geometry_view_t tentacle_mesh;
    effect_particle_vertex_emitter_view_t particle_vertex_emitter;
} effect_geometry_record_t;
typedef char assert_effect_geometry_record_size[sizeof(effect_geometry_record_t) == 0xc4 ? 1 : -1];

/* Particle-system section addressed by effect-file header word 0x0c. */
typedef struct effect_geometry_table {
    u16 _unused_00;        /* 2, or 1 in seven shipped files */
    u16 entry_count;       /* 0x02 */
    vec3s32_t gravity;     /* 0x04: copied to g_battle_effect_gravity by effect script opcode 0x27 */
    s32 inertia_threshold; /* 0x10: passed to battle_effect_set_inertia_threshold by opcode 0x27 */
    /* One 0xc4-byte record per emitter or mesh. The slots keep their curve
     * bindings, but each native EFFECT renderer gives them its own meanings,
     * so a record is read through the member of the renderer that owns it. */
    effect_geometry_record_t entries[1];
} effect_geometry_table_t;

/* Frames in one animation curve: emitter and particle frame counters wrap here. */
enum { EFFECT_ANIMATION_CURVE_FRAMES = 0xa0 };

/* One interpolation factor per frame; curve indices select emitter and colour tracks. */
typedef u8 effect_animation_curve_t[EFFECT_ANIMATION_CURVE_FRAMES];
typedef char assert_effect_animation_curve_size[sizeof(effect_animation_curve_t) == 0xa0 ? 1 : -1];

/* The effect file has one count followed by one 160-byte curve per entry. */
typedef struct effect_animation_curve_table {
    u32 count;
    effect_animation_curve_t curves[1];
} effect_animation_curve_table_t;
typedef char effect_animation_curve_table_data_offset_must_be_4
    [(unsigned long)&((effect_animation_curve_table_t*)0)->curves == 4 ? 1 : -1];

/* Disc extent of a BATTLE resource file. */
typedef struct battle_file_extent {
    s32 sector;
    s32 size;
} battle_file_extent_t;

extern battle_effect_charge_params_t g_battle_effect_charge_line_params[2];
extern battle_effect_death_poof_state_t g_battle_effect_death_poof_states[];
extern battle_secondary_effect_function_t g_battle_effect_secondary_handlers[23];
extern battle_effect_secondary_animation_t g_battle_effect_secondary_animations[];
extern battle_effect_secondary_data_t g_battle_effect_secondary_data[];
extern u8 g_effect_load_state; /* battle_effect_load_state_e */
/* Projectile secondary animation of each weapon item type: BOW_ARROW for bows, CROSSBOW_BOLT for
 * crossbows, NONE for the rest. */
extern u8 g_battle_effect_weapon_type_extra_flags[];
extern u32 g_battle_effect_camera_rotation_mode;
extern u32 g_battle_effect_camera_position_mode;
extern u32 g_battle_effect_camera_zoom_mode;
extern battle_effect_sprite_section_t g_battle_effect_trap_frame_data;
extern u8 g_battle_effect_ninja_ball_masks[];
extern s32 g_battle_effect_gravity_scale;
extern MATRIX g_battle_effect_particle_heading_matrix;
extern MATRIX g_battle_effect_emitter_matrix;
extern MATRIX g_battle_effect_projectile_matrix;
extern psyq_tmd_object_t* g_battle_effect_model_object;
extern SVECTOR* g_battle_effect_model_vertices;
extern SVECTOR* g_battle_effect_model_normals;
extern psyq_tmd_packet_t* g_battle_effect_model_commands;
extern s32 g_battle_effect_model_command_word_offset;
extern SVECTOR g_battle_effect_camera_rotation_start;
extern SVECTOR g_battle_effect_camera_rotation_target;
extern SVECTOR g_battle_effect_camera_rotation_current;
extern SVECTOR g_battle_effect_camera_rotation_saved;
extern VECTOR g_battle_effect_camera_position_start;
extern VECTOR g_battle_effect_camera_position_target;
extern VECTOR g_battle_effect_camera_position_current;
extern VECTOR g_battle_effect_camera_position_saved;
extern s32 g_battle_effect_camera_zoom_duration;
extern s32 g_battle_effect_camera_zoom_frame;
extern VECTOR g_battle_effect_camera_zoom_start;
extern VECTOR g_battle_effect_camera_zoom_target;
extern VECTOR g_battle_effect_camera_zoom_current;
extern VECTOR g_battle_effect_camera_zoom_saved;
extern s32 g_battle_effect_trajectory_hit_unit_id;
extern battle_effect_sprite_section_t* g_battle_effect_trap_frame_data_ptr;
extern battle_effect_callback_t g_battle_effect_callback_slots[];
extern u8 g_battle_effect_palette_target_misc_ids[];
extern SVECTOR g_battle_effect_trajectory_origin;
extern SVECTOR g_battle_effect_trajectory_destination;
extern s32 g_battle_effect_trajectory_step_count;
extern battle_effect_palette_section_t* g_battle_effect_palette_ptr;

/* Per-group trap emitter records (0x801b8564, stride 0x2e). The secondary-effect
 * handlers spawn while spawn_start_frame <= timer < spawn_end_frame and finish
 * once timer passes spawn_start_frame with no live slots;
 * battle_effect_update_active_slots walks count active slots. The level-up
 * handler seeds group 5's position.y with -8, then lowers it by 3 per frame. */
extern battle_effect_emitter_values_t g_battle_effect_trap_particle_configs[17];
extern s16 g_battle_effect_root_record_index;
extern psyq_tmd_header_t* g_battle_effect_model_files[];
extern battle_file_extent_t g_battle_effect_disc_entries[];
extern battle_effect_secondary_state_t g_battle_effect_secondary_state;
/* Tile target of the current effect: emitter origin CURSOR_TILE, homing CURSOR_TILE and camera source
 * CURSOR_TILE. */
extern battle_effect_target_t g_battle_effect_target_tile;
extern battle_effect_tile_ref_t g_battle_effect_trajectory_tile;
extern s16 g_battle_effect_ability_ids[];
extern effect_animation_curve_table_t* g_battle_effect_animation_curve_data;
extern s16 g_battle_effect_arctan_angle_mod;
extern s32 g_battle_effect_buffer_index;
extern battle_effect_secondary_init_t g_battle_effect_context;
extern u16 g_battle_effect_free_slot_head;
extern struct battle_effect_secondary_data* g_battle_effect_current_secondary;
extern u32 g_battle_effect_current_slot_id;
extern effect_file_header_t* g_battle_effect_data_ptrs[];
extern battle_effect_callback_t g_battle_effect_file_functions[];
extern u8* g_battle_effect_frame_data;
extern u8* g_battle_effect_frame_data_start;
extern s32 g_battle_effect_gravity_modifier;
extern s32 g_battle_effect_inertia_threshold;
extern s16 g_battle_effect_load_countdown;
extern effect_timeline_t* g_battle_effect_timeline;
extern u8* g_battle_effect_script;
/* Timing-curve section of an effect file: a time scale per timeline frame,
 * packed two nibbles to the byte, low nibble first; values 1-9 are applied
 * (battle_state_set_time_scale). The parent timeline's curve comes first and
 * the child timeline's follows at 0x12c (the second extent is taken to equal
 * the first). */
typedef struct battle_effect_timing_curves {
    u8 parent_timeline[0x12c]; /* 0x000 */
    u8 child_timeline[0x12c];  /* 0x12c */
} battle_effect_timing_curves_t;
extern battle_effect_timing_curves_t* g_battle_effect_timing_curves;
extern battle_effect_tick_channels_t* g_battle_effect_timing_channels;
extern u8* g_battle_effect_animation_sequences;
extern u8* g_battle_effect_animation_sequences_start;
extern u32* g_battle_effect_polygon_depth_data;
extern u16 g_battle_effect_active_slot_tail;
extern s32 g_battle_effect_primitive_sizes[];
extern s32 g_battle_effect_saved_inertia_threshold;
extern s32 g_battle_effect_sound_data_base;
extern struct main_sound_resource* g_battle_effect_sound_section_ptr;
extern u8 g_battle_effect_target_id; /* id of the charge secondary effect started with the ability; 0 none */
extern effect_flags_section_t* g_battle_effect_flags_section;
/* Particle gravity; secondary handlers save it while using (0, ONE, 0). */
extern vec3s32_t g_battle_effect_gravity;
extern vec3s32_t g_battle_effect_saved_gravity;
extern s32 g_battle_effect_arc_trajectory_height;
extern s16 g_battle_effect_arrow_arc_caster_relative_yaw;
extern s32 g_battle_effect_arrow_arc_distance;
extern s32 g_battle_effect_arrow_arc_distance_step;
extern s32 g_battle_effect_arrow_arc_height;
extern s16 g_battle_effect_arrow_arc_launch_angle;
extern s32 g_battle_effect_arrow_arc_length;
extern u16 g_battle_effect_arrow_arc_yaw;
extern s32 g_battle_effect_camera_position_duration;
extern s32 g_battle_effect_camera_position_frame;
extern s32 g_battle_effect_camera_rotation_duration; /* angle track duration */

extern s32
    g_battle_effect_camera_rotation_frame; /* angle track counter; reset by battle_camera_execute_angle_command */

extern DVECTOR g_battle_effect_charge_line_direction_offsets[];
extern u8 g_battle_effect_charge_line_fade_levels[];
/* Four-byte alignment preserves the original word copies into x/y. */
extern SVECTOR g_battle_effect_corner_bottom_left __attribute__((aligned(4)));
extern SVECTOR g_battle_effect_corner_bottom_right __attribute__((aligned(4)));
extern SVECTOR g_battle_effect_corner_top_left __attribute__((aligned(4)));
extern SVECTOR g_battle_effect_corner_top_right __attribute__((aligned(4)));
extern u8 g_battle_effect_ending_secondary_function_id; /* function byte to store; 5 = nothing */
extern s32 (*g_battle_effect_instructions[120])(effect_record_t*);
extern MATRIX g_battle_effect_matrix; /* 0x801bc09c */
extern u32* g_battle_effect_otag;

/* Work-kind handlers, indexed by kind_flags >> 12. The int return type is
 * load-bearing even though the result is ignored: the value-returning call is
 * a second set of $2, so sched1 no longer boosts the final `$2 = 1` to the end
 * of its block, and the pc update is allocated $3 as in the target. */
extern s32 (*g_battle_effect_particle_render_handlers[])(effect_record_t* record, effect_work_record_t* work);
extern VECTOR g_battle_effect_projectile_delta;
extern s32 g_battle_effect_projectile_length;
extern s32 g_battle_effect_projectile_progress;
extern s32 g_battle_effect_projectile_speed;
extern s16 g_battle_effect_projectile_spin_passes;
extern s16 g_battle_effect_projectile_step_count;

/* Six RGB byte triples, packed into both CLUT images as 15-bit colours. */
extern rgb24_t g_battle_effect_reflect_palette_colors[6];

/* Secondary-animation id per ability, indexed by the low halfword of the
 * caller's packed argument. */
extern u8 g_battle_effect_secondary_by_charge_animation[];
extern u8 g_battle_effect_sound_call_counts[]; /* per-channel call counts */

/* Brightness (out of 128) of each of a summon-charge ring's ten slots, newest point first. */
extern u8 g_battle_effect_summon_ring_brightness[];
extern SVECTOR g_battle_effect_trajectory_position; /* final projectile position */
extern s32 g_battle_effect_trajectory_source_id;
extern VECTOR g_battle_effect_trajectory_step; /* last step's movement */
extern s32 g_battle_effect_trajectory_tile_flags;

/* Self-relative animation script table: each halfword is a byte offset from
 * the table's own base to a battle_effect_anim_script_t. Declared as bytes
 * because both uses are byte-based -- the stored pointer is the table's own
 * base plus a byte offset -- and the halfword entries are read through a cast
 * at the use site. Declaring it `u16[]` and casting the symbol to `u8*` for
 * the address instead reserves an unused 8-byte frame in
 * battle_effect_init_trap_animation. */
extern u8 g_battle_effect_trap_animation_data[];
extern u8 g_battle_effect_trap_colors[][3];

/* CLUT of the zodiac poof's particles, an index in the weapons row, for each
 * sprite palette (the secondary effect's parameter). */
extern u8 g_battle_effect_zodiac_poof_animations[];

/* Unsigned halfword indexes: BATTLE list traversal loads these with lhu. */
extern s16 g_battle_effect_allocated_record_head;
extern u16 g_battle_effect_available_record_head;
extern s32 g_battle_effect_frame_parity;
extern s32 g_battle_effect_frame_start_prim_buffer_offset;
extern s32 g_battle_effect_phase; /* battle_effect_phase_e */
extern u8* g_battle_effect_prim_buffer;
extern s32 g_battle_effect_prim_buffer_offset;
extern battle_effect_slot_t g_battle_effect_slots[];
extern s32 g_battle_effect_sprite_count;
extern s32 g_battle_effect_sprite_count_peak;
extern effect_geometry_table_t* g_effect_geometry_table;

extern effect_record_t g_effect_state_records[];
extern s16 g_battle_effect_current_record_index;
extern s32 g_battle_effect_work_record_peak;
extern effect_work_record_t* g_battle_effect_free_work_record_head;
extern u8 g_battle_effect_active_slot_head;
extern s32 g_battle_effect_work_record_count;

/* Effect work pool: g_battle_effect_work_records is the 256-record allocation, g_battle_effect_free_work_record_head
 * the free-chain head, g_battle_effect_work_record_count the in-use count and g_battle_effect_work_record_peak its
 * peak. */
extern effect_work_record_t* g_battle_effect_work_records;
extern u8 g_battle_effect_slot_free_head;
extern u16 g_battle_effect_free_list_node_head;
extern effect_list_node_t g_battle_effect_list_nodes[];
extern u16 g_battle_effect_active_list_node_head;
void battle_effect_set_ability_animation(u32 packed, s32 ability, battle_effect_secondary_init_t* source);
s32 battle_effect_init_fall_dust_secondary(battle_effect_secondary_init_t* source, VECTOR* position);
s32 battle_effect_is_item_ability(s32 ability_id);
void battle_effect_set_arrow_trajectory(s32 source_id, SVECTOR* tile_position, s32 target_id);
s32 battle_effect_check_direct_trajectory_to_target(s32 source_id, SVECTOR* tile_position, s32 target_id);
void battle_effect_store_targets_and_display_types(s32 ability_id, battle_unit_misc_data_t* unit);
void battle_effect_free_state_record(s16);

/* parent is the particle a child emitter spawns from, read only for the
 * EFFECT_EMITTER_ORIGIN_PARENT_PARTICLE origin: the particle update passes
 * the particle, the EFFECT overlay's particle handlers a record built on
 * their stack that holds a position, and the timeline none. The emitter index
 * arrives as a full word. The function is declared with an int result it
 * never sets and no caller reads. */
s32 battle_effect_spawn_emitter_particles(s16 record_index, s32 frame, s32 emitter_index, effect_work_record_t* parent);

void battle_effect_build_secondary_init_from_action(
    battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* init);

void battle_move_start_teleport_step(battle_unit_misc_data_t* unit);

void battle_effect_init_altima_teleport_data(
    battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* teleport_data);

s32 battle_effect_init_weapon_hit_secondary(s32 elements, battle_effect_secondary_init_t* source, s32 unused_is_bow);
void battle_effect_start_action_projectile_secondary(battle_unit_misc_data_t* unit);
void battle_effect_set_secondary_death_smoke(battle_unit_misc_data_t* unit);
void battle_effect_set_secondary_venom_trap(battle_unit_misc_data_t* unit);
void battle_effect_set_secondary_zodiac_poof(battle_unit_misc_data_t* unit);
s32 battle_effect_start_altima_teleport_arrival(battle_unit_misc_data_t* unit);
s32 battle_effect_start_altima_teleport_departure(battle_unit_misc_data_t* unit);
void battle_effect_start_altima_teleport_fade_out(battle_unit_misc_data_t* unit);
void battle_effect_start_altima_teleport_white_flash(battle_unit_misc_data_t* unit);
void battle_effect_decode_next_model_command(battle_effect_decoded_model_command_t* out);
s32 battle_effect_init_model_sections(psyq_tmd_header_t* resource, s32 unused_1);
s32 battle_effect_spawn_particle_motion(battle_effect_emitter_values_t* values, battle_effect_motion_t* motion);
void battle_effect_add_random_vector_offsets(const VECTOR* source, const VECTOR* ranges, VECTOR* destination);

void battle_effect_add_random_vector_offsets_and_store_q12(
    const VECTOR* source, const VECTOR* ranges, VECTOR* destination);

void battle_effect_add_vectors(const VECTOR* first, const VECTOR* second, VECTOR* out);

void battle_effect_advance_map_background_track(
    battle_effect_background_track_t* track, s16* keyframe, s16* remaining_frames);

void battle_effect_build_secondary_init(
    struct battle_unit_misc_data* unit, struct battle_effect_secondary_init* effect);

s32 battle_effect_calculate_arc_height_term(s32 distance);
s32 battle_effect_calculate_arcing_trajectory(battle_arc_query_t* query, s32* arc_a, s32* arc_b);
void battle_effect_call_play(void);
void battle_effect_copy_camera_angles_and_screen_coords(void);
void battle_effect_save_gravity(void);
void battle_effect_restore_gravity(void);
void battle_effect_finalize_secondary(u8 secondary_id);
s32 battle_effect_find_next_keyframe(s16 frame, s16 mask, s32* out_index, s32* out_frame);
s32 battle_effect_get_bow_shot_angle(void);
u8 battle_effect_allocate_secondary_slot(void);
s32 battle_effect_get_ninja_ball(s32 elements);
s32 battle_effect_get_on_hit_sound_index(s32 channel);
s16 battle_effect_get_random_between_s16(s16 minimum, s16 maximum);
s32 battle_effect_get_secondary_phase(u8 secondary_id);
void battle_effect_init_flags_section_pointer(s16 effect_id);
void battle_effect_init_camera_and_screen_data(void);
s32 battle_effect_init_projectile_secondary(s32 animation, struct battle_effect_secondary_init* source);
s32 battle_effect_init_secondary(s32 animation_id, s32 parameter, struct battle_effect_secondary_init* source);
void battle_effect_init_system(void);
void battle_effect_interpolate_s16(s16 from, s16 to, s32 t, s16* out);
void battle_effect_interpolate_svector(const SVECTOR* from, const SVECTOR* to, s32 t, SVECTOR* out);
void battle_effect_interpolate_vector_q13(const VECTOR* from, const VECTOR* to, s32 t, VECTOR* out);
s32 battle_effect_lerp_and_store(s32 start, s32 end, s32 factor, s32* out);
void battle_effect_lerp_component_q13(s32 from, s32 to, s32 t, s32* out);
void battle_effect_list_valid_unit_targeting_data(s32 first, s32 second, battle_effect_obstacle_unit_list_t** out);
void battle_effect_mask_vector_low_12_bits(const SVECTOR* src, SVECTOR* dst);
void battle_effect_load_lba_and_size(s32 effect_id, s32* lba, s32* byte_length);
void battle_script_play_effect_and_wait(void);
void battle_effect_remove_secondary(u8 id);
void battle_effect_replace_flags_0006(struct battle_effect_flag_prefix* object, s32 value);
void battle_effect_reset_render_state(void);
void battle_effect_restore_inertia_threshold(void);
void battle_effect_save_inertia_threshold(void);
void battle_effect_set_evade_type_data_and_weapon_element(struct battle_unit_misc_data* unit, s32 is_bow);
void battle_effect_set_inertia_threshold(s32 threshold);
void battle_effect_set_secondary(struct battle_unit_misc_data* unit, s32 effect);
void battle_effect_set_secondary_fall_dust(struct battle_unit_misc_data* unit);
void battle_effect_set_secondary_level_up(struct battle_unit_misc_data* unit);
void battle_effect_set_secondary_splash(struct battle_unit_misc_data* unit);
void battle_effect_set_secondary_teleport(struct battle_unit_misc_data* unit);
void battle_effect_shift_vector_left_12(const VECTOR* src, VECTOR* dst);
void battle_effect_shift_vector_right_12(const VECTOR* src, VECTOR* dst);
void battle_effect_start_group(s32 group, s32 clut_index);

void battle_effect_step_emitter_timeline(
    effect_record_t* state, battle_effect_keyframe_table_t* table, s16* index, s16* remaining, s16* step);

void battle_effect_integrate_particle_motion(effect_work_record_t* list);
void battle_effect_step_motion(battle_effect_motion_t* motion);
void battle_effect_set_gravity(vec3s32_t* src);

s32 battle_effect_trace_arc_trajectory_path(
    SVECTOR* origin, s32* height, s32* distance, SVECTOR* arc, battle_effect_obstacle_unit_list_t* obstacles);

s32 battle_effect_test_position_for_obstacle(
    battle_effect_obstacle_unit_list_t* list, VECTOR* position, s32* out_unit, battle_effect_tile_ref_t* tile_ref);

s32 battle_effect_try_init_data(void);
void battle_effect_update_active_slots(s32 group);
s32 battle_effect_update_secondary_effects(void);
s32 battle_return_zero_801b0fec(void);
s32 battle_effect_arrow_arc_secondary_handler(void);
s32 battle_effect_weapon_hit_secondary_handler(void);
s32 battle_effect_golem_secondary_handler(void);
s32 battle_effect_spell_charge_secondary_handler(void);
s32 battle_effect_null_secondary_handler(void);
s32 battle_effect_red_orbs_secondary_handler(void);
s32 battle_noop_801b2968(void);
s32 battle_effect_fall_dust_secondary_handler(void);
s32 battle_effect_splash_secondary_handler(void);
s32 battle_noop_801b2bfc(void);
s32 battle_noop_801b2c04(void);
s32 battle_effect_venom_trap_secondary_handler(void);
s32 battle_effect_level_up_secondary_handler(void);
s32 battle_noop_801b2e60(void);
s32 battle_effect_update_death_poof_secondary(s32 unused_arg0, s32 passthrough);
s32 battle_noop_801b3118(void);
s32 battle_effect_zodiac_poof_secondary_handler(void);
s32 battle_effect_reflect_secondary_handler(void);
s32 battle_effect_teleport_secondary_handler(void);
s32 battle_effect_linear_projectile_secondary_handler(void);
s32 battle_effect_equip_break_secondary_handler(void);
s32 battle_effect_summon_charge_secondary_handler(void);
void battle_effect_add_svector_to_vector(const SVECTOR* first, const VECTOR* second, VECTOR* out);
void battle_effect_add_svectors(const SVECTOR* first, const SVECTOR* second, SVECTOR* out);

void battle_effect_advance_coord_target_palette_track(
    battle_effect_palette_track_t* track, s16* keyframe, s16* remaining_frames);

void battle_effect_advance_target_palette_track(
    battle_effect_palette_track_t* track, s16* keyframe, s16* remaining_frames);

s32 battle_effect_calculate_launch_velocity(void);
s32 battle_effect_calculate_arc_trajectory_term(s32 a, s32 b);
s32 battle_effect_check_arc_trajectory_between_units(u8 source_id, u8 target_id);
s32 battle_effect_check_direct_trajectory_between_units(u8 source_id, u8 target_id);
void battle_effect_load_vector(vec3s16_t* src, VECTOR* dst);
void battle_effect_convert_tile_coords_to_world_coords(SVECTOR* in, VECTOR* out);
void battle_effect_copy_misc_unit_screen_location(u32 misc_id, VECTOR* destination);
void battle_effect_load_svector(vec3s16_t* src, SVECTOR* dst);
void battle_effect_draw_item_drop_sprite(VECTOR* origin, s32 item_id, s32 angle);

void battle_effect_draw_projectile_model(
    SVECTOR* spin, VECTOR* position, SVECTOR* angles, VECTOR* scale, s32 parameter);

s32 battle_effect_end_secondary_on_current_unit(void);
void battle_effect_find_projectile_xyz_along_trajectory(const VECTOR* src, s32 total, s32 current, VECTOR* dst);
s32 battle_effect_get_animation_timing_curve_value(effect_record_t* record);
s32 battle_effect_get_timing_curve_value(effect_record_t* record);
s32 battle_effect_init_data(void);
void battle_effect_init_animation_sequence(effect_list_node_t* state, s32 frame_group, s32 animation_index);
void battle_effect_init_trap_animation(s32 group, u16 clut, battle_effect_slot_t* slot);
s32 battle_effect_interpolate_s32_cos(s32 start, s32 end, s32 duration, s32 current);
u32 battle_effect_load_ability(s32 ability_id);
void battle_effect_render_particle_sprite(effect_list_node_t* particle, s32 red, s32 green, s32 blue);

void battle_effect_get_screen_location_by_battle_id(s32 adjust_for_tile_height, u32 battle_id,
    battle_effect_secondary_init_t* effect_coordinates, SVECTOR* destination);

void battle_effect_resolve_target_render_coords(
    s32 add_depth, s32 target_index, battle_effect_secondary_init_t* coord_data, SVECTOR* out);

void battle_effect_run_keyframe_actions(u16 flags, u8 target, effect_record_t* state);
void battle_effect_run_keyframe_actions_all_targets(u16 flags, effect_record_t* state);
s32 battle_effect_set_secondary_teleport_by_misc_id(u32 misc_id);
s32 battle_effect_set_and_validate_arc_trajectory(s32 source_id, SVECTOR* tile_position, s32 target_id);
void battle_effect_start_new(s32 ability_id, s32 target_mode, battle_effect_start_target_data_t target);
void battle_effect_store_map_center_coordinates(VECTOR* destination);

void battle_effect_submit_sprite_to_ordering_table(
    battle_effect_sprite_block_t* set, DVECTOR* position, s16 angle, VECTOR* zoom, u32* ot);

/* Steps the projectile along its trajectory, testing each obstacle. */
s32 battle_effect_trace_projectile_path(
    VECTOR* delta, SVECTOR* origin, s32* distance, battle_effect_obstacle_unit_list_t* obstacles);
void battle_effect_update_on_hit_sound_timer(
    battle_effect_sound_track_t* schedule, s16* entry_index, s16* countdown, u8* unused_sound_state);
void battle_effect_add_vector_and_store_q12(const VECTOR* delta, VECTOR* value, VECTOR* out);
s32 battle_effect_get_random_between(s32 value_a, s32 value_b);
void battle_effect_interpolate_svector_cos(
    const SVECTOR* from, const SVECTOR* to, s32 duration, s32 current, SVECTOR* out);
void battle_effect_interpolate_svector_cos_q12(
    const SVECTOR* from, const SVECTOR* to, s32 duration, s32 current, SVECTOR* out);
void battle_effect_interpolate_svector_linear(
    const SVECTOR* from, const SVECTOR* to, s32 duration, s32 current, SVECTOR* out);
void battle_effect_interpolate_vector_q13_cos(
    const VECTOR* from, const VECTOR* to, s32 duration, s32 current, VECTOR* out);
void battle_effect_interpolate_vector_q13_cos_q12(
    const VECTOR* from, const VECTOR* to, s32 duration, s32 current, VECTOR* out);
void battle_effect_interpolate_vector_q13_linear(
    const VECTOR* from, const VECTOR* to, s32 duration, s32 current, VECTOR* out);

void battle_effect_add_random_rotation_offsets(const SVECTOR* source, const SVECTOR* ranges, SVECTOR* destination);

void battle_effect_add_rotation_vectors_mod_q12(const SVECTOR* first, const SVECTOR* second, SVECTOR* out);

void battle_effect_advance_caster_palette_track(
    battle_effect_palette_track_t* track, s16* keyframe, s16* remaining_frames, s32 caster_misc_id);

effect_list_node_t* battle_effect_alloc_list_node(s16 kind);
void* battle_effect_alloc_primitive(s32 primitive_type);
s32 battle_effect_alloc_slot(void);
battle_effect_sprite_block_t* battle_effect_alloc_sprite_block(u8 sprite_count);
s16 battle_effect_alloc_state_record(void);
effect_work_record_t* battle_effect_alloc_work_record(s16 record_index, s16 kind);
void battle_effect_reset_child_timeline(effect_record_t* record, s16 target_index);
void battle_effect_reset_parent_timeline(effect_record_t* record);
void battle_effect_free_list_node(effect_list_node_t* node);
void battle_effect_free_work_record(s16 record_index, effect_work_record_t* work);
void battle_effect_free_slot(u8 id);
void battle_effect_free_sprite_block(battle_effect_sprite_block_t* block);
battle_effect_callback_t battle_effect_get_file_function(s16 function_id);
effect_work_record_t* battle_effect_get_list_node(s16 record_index, s16 requested_index);
s32 battle_effect_get_phase(void);
s32 battle_effect_get_prim_buffer_offset(void);
void battle_effect_init_free_list(void);
void battle_effect_init_record_chain(void);
void battle_effect_init_render_state(void);
void battle_effect_init_state_records(void);
s16 battle_effect_start_script_record(u8* script, s16 start_pc, s16 parent_index);
s32 battle_effect_code_script_04_end(effect_record_t* unused_record);
s32 battle_effect_code_script_2e_invalid(effect_record_t* unused_record);
void battle_effect_interpolate_emitter_spawn_spread(const effect_geometry_record_t* source, s32 factor, VECTOR* out);
s32 battle_effect_lerp_linear(s32 start, s32 end, s32 factor);
void battle_effect_play(void);
s32 battle_effect_run_block(s16 index);
s16 battle_effect_select_larger_magnitude_s16(s16 first, s16 second);
s16 battle_effect_select_largest_magnitude_s16(s16 first, s16 second, s16 third);
void battle_effect_set_phase_none(void);
void battle_effect_start_render_frame(void);
s32 battle_effect_update_cycle(void);
s32 battle_effect_update_slot(s32 id);
s32 battle_effect_update_stage(void);
void battle_effect_interpolate_emitter_spawn_position(const effect_geometry_record_t* source, s32 factor, VECTOR* out);
void battle_effect_interpolate_emitter_velocity_base_angles(
    const effect_geometry_record_t* source, s32 factor, VECTOR* out);
void battle_effect_interpolate_emitter_velocity_direction_spread(
    const effect_geometry_record_t* source, s32 factor, VECTOR* out);
void battle_effect_add_vectors_to_svector(const VECTOR* left, const VECTOR* right, SVECTOR* result);
void battle_effect_add_vectors_to_svector_rot_y_270(const VECTOR* left, const VECTOR* right, SVECTOR* result);
void battle_effect_add_vectors_to_svector_rot_y_180(const VECTOR* left, const VECTOR* right, SVECTOR* result);
void battle_effect_add_vectors_to_svector_rot_y_90(const VECTOR* left, const VECTOR* right, SVECTOR* result);
void battle_effect_draw_billboard_sprite(battle_effect_slot_t* slot);

/* turn */
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

/* battle_action_run_between_turn_control modes. */
typedef enum battle_between_turn_mode {
    BATTLE_BETWEEN_TURN_MODE_RUN = 0,
    BATTLE_BETWEEN_TURN_MODE_RESET = 1,
    BATTLE_BETWEEN_TURN_MODE_FIND_ACTIVE_UNIT = 2,
} battle_between_turn_mode_e;

/* g_battle_between_turn_state and g_battle_between_turn_resume_state: the phases of
 * battle_action_run_between_turn_control. */
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
extern battle_active_unit_t g_battle_active_turn_unit;
extern s32 g_battle_between_turn_resume_state;
extern s16 g_battle_acting_unit_used_ability_id;
extern s32 g_battle_between_turn_state;
extern s32 g_battle_turn_unit_id;

/* unit */
/* Miscellaneous Unit Data 0x130 identifies which role 0x131 links. */
typedef enum battle_misc_mount_state {
    BATTLE_MISC_MOUNT_STATE_NONE = 0,
    BATTLE_MISC_MOUNT_STATE_RIDER = 1,
    BATTLE_MISC_MOUNT_STATE_MOUNT = 2,
} battle_misc_mount_state_e;

/* Battle Stats 0x182 links the two simulation records in a mounted pair. */
typedef enum battle_mount_info_flags {
    BATTLE_MOUNT_INFO_PARTNER_ID_MASK = 0x1f,
    BATTLE_MOUNT_INFO_FLAG_MOUNT = 0x40,
    BATTLE_MOUNT_INFO_FLAG_RIDER = 0x80,
    BATTLE_MOUNT_INFO_ROLE_MASK = 0xc0,
} battle_mount_info_flags_e;

/* Result of battle_unit_get_action_restriction. */
typedef enum battle_unit_action_restriction {
    BATTLE_UNIT_ACTION_RESTRICTION_NONE = 0,
    /* Don't Act status or BATTLE_MOUNT_INFO_FLAG_MOUNT. */
    BATTLE_UNIT_ACTION_RESTRICTION_CANNOT_ACT = 1,
    /* Tile depth 2 or more, unless the unit has Float, Chicken or Frog status,
     * a water movement ability or BATTLE_MOUNT_INFO_FLAG_RIDER. */
    BATTLE_UNIT_ACTION_RESTRICTION_DEEP_WATER = 2,
} battle_unit_action_restriction_e;

/* Lifecycle states stored in Battle Stats 0x183. */
typedef enum battle_unit_existence_state {
    BATTLE_UNIT_EXISTENCE_OFF_FIELD = 0, /* defined but not placed; AddUnit can use the slot */
    BATTLE_UNIT_EXISTENCE_ACTIVE = 1,
    BATTLE_UNIT_EXISTENCE_PENDING_REMOVAL = 2,
    BATTLE_UNIT_EXISTENCE_DISABLED = 0x80,
    BATTLE_UNIT_EXISTENCE_NONE = 0xff, /* empty slot */
} battle_unit_existence_state_e;

/* How battle_gfx_construct_polygon_data_for_units draws a sprite part. The
 * unit's sprite_display_flags, its horizontal_flip_flag, the flags a
 * sequence toggles and each part's own are XORed together. */
typedef enum battle_misc_display_flags {
    BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT = 0x01,
    BATTLE_MISC_DISPLAY_HORIZONTAL_FLIP = 0x02,
    BATTLE_MISC_DISPLAY_VERTICAL_FLIP = 0x04,
    /* A part's flags only: queued nearer the camera than the rest by its pitch and zoom. */
    BATTLE_MISC_DISPLAY_NEARER = 0x80,
} battle_misc_display_flags_e;

/* Two-bit selector in sprite flags and tpage, already shifted to bits 5-6. */
typedef enum battle_misc_display_blend_mode {
    BATTLE_MISC_DISPLAY_BLEND_AVERAGE = 0x00,
    BATTLE_MISC_DISPLAY_BLEND_ADD = 0x20,
    BATTLE_MISC_DISPLAY_BLEND_SUBTRACT = 0x40,
    BATTLE_MISC_DISPLAY_BLEND_ADD_QUARTER = 0x60,
} battle_misc_display_blend_mode_e;

enum { BATTLE_MISC_DISPLAY_BLEND_MASK = 0x60 };

typedef enum battle_unit_equipped_flags {
    BATTLE_UNIT_EQUIPPED_FLAG_MATERIA_BLADE = 0x04,
    BATTLE_UNIT_EQUIPPED_FLAG_SWORD = 0x08,
} battle_unit_equipped_flags_e;

typedef enum battle_unit_height_flags {
    BATTLE_UNIT_HEIGHT_NON_CHOCOBO = 0x20,
    BATTLE_UNIT_HEIGHT_SHARED_TILE = 0x40,
    BATTLE_UNIT_HEIGHT_UNAVAILABLE = 0x80,
} battle_unit_height_flags_e;

/* Selection policy for battle_unit_find_at_tile.  Status bits are inclusive
 * alternatives; the Chocobo bit is an additional requirement. */
typedef enum battle_unit_tile_filter {
    BATTLE_UNIT_TILE_FILTER_TREASURE = 0x01,
    BATTLE_UNIT_TILE_FILTER_DRY_LOW_SLOPE = 0x02,
    BATTLE_UNIT_TILE_FILTER_TRANSPARENT = 0x10,
    BATTLE_UNIT_TILE_FILTER_CRYSTAL = 0x40,
    BATTLE_UNIT_TILE_FILTER_MOUNTABLE_CHOCOBO = 0x80,
    BATTLE_UNIT_TILE_STATUS_FILTERS
    = BATTLE_UNIT_TILE_FILTER_TREASURE | BATTLE_UNIT_TILE_FILTER_TRANSPARENT | BATTLE_UNIT_TILE_FILTER_CRYSTAL,
} battle_unit_tile_filter_e;

/* battle_stats_t.entd_slot value for an unused or removed slot. */
enum {
    BATTLE_ENTD_SLOT_NONE = 0xff,
};

/* An EVTCHR slot's fixed SEQ storage. Its header words are unset and unread;
 * its 0x40 pointers refer into the slot's own 0x400 bytes. */
typedef struct battle_gfx_unit_seq {
    battle_gfx_unit_seq_table_t header; /* 0x000 */
    u8* sequences[0x40];                /* 0x008 */
    u8 sequence_data[0x400];            /* 0x108 */
} battle_gfx_unit_seq_t;
typedef char battle_gfx_unit_seq_size_must_be_0x508[sizeof(battle_gfx_unit_seq_t) == 0x508 ? 1 : -1];

/* Renderer-side Misc-unit status mirror. These packed masks do not use the
 * canonical battle_status_id_e byte order. */
typedef enum battle_misc_status_flags_1_4 {
    /* Misc +0x140. Bit 0x00000001 is tested for animation 0x21 and never set. */
    BATTLE_MISC_STATUS_CONFUSION = 0x00000080,
    BATTLE_MISC_STATUS_SLEEP = 0x00000040,
    BATTLE_MISC_STATUS_PETRIFY = 0x00000020,
    BATTLE_MISC_STATUS_STOP = 0x00000010,
    BATTLE_MISC_STATUS_CRITICAL = 0x00000008,
    BATTLE_MISC_STATUS_DEAD = 0x00000004,
    BATTLE_MISC_STATUS_MOUNTED = 0x00000002,

    /* Misc +0x141. */
    BATTLE_MISC_STATUS_FAITH = 0x00008000,
    BATTLE_MISC_STATUS_BERSERK = 0x00004000,
    BATTLE_MISC_STATUS_UNNAMED_0E = 0x00002000,
    BATTLE_MISC_STATUS_HASTE = 0x00001000,
    BATTLE_MISC_STATUS_SLOW = 0x00000800,
    BATTLE_MISC_STATUS_DEFENDING = 0x00000400,
    BATTLE_MISC_STATUS_CHARGING = 0x00000200,
    BATTLE_MISC_STATUS_PERFORMING = 0x00000100,

    /* Misc +0x142. */
    BATTLE_MISC_STATUS_OIL = 0x00800000,
    BATTLE_MISC_STATUS_POISON = 0x00400000,
    BATTLE_MISC_STATUS_PROTECT = 0x00200000,
    BATTLE_MISC_STATUS_SHELL = 0x00100000,
    BATTLE_MISC_STATUS_DONT_ACT = 0x00080000,
    BATTLE_MISC_STATUS_DONT_MOVE = 0x00040000,
    BATTLE_MISC_STATUS_BLOOD_SUCK = 0x00020000,
    BATTLE_MISC_STATUS_INNOCENT = 0x00010000,

    /* Misc +0x143. Bit 0x80000000 remains unknown. */
    BATTLE_MISC_STATUS_UNDEAD = 0x40000000,
    BATTLE_MISC_STATUS_REGEN = 0x20000000,
    BATTLE_MISC_STATUS_RERAISE = 0x10000000,
    BATTLE_MISC_STATUS_DARKNESS = 0x08000000,
    BATTLE_MISC_STATUS_DEATH_SENTENCE = 0x04000000,
    BATTLE_MISC_STATUS_SILENCE = 0x02000000,
    BATTLE_MISC_STATUS_CHARM = 0x01000000,

    BATTLE_MISC_STATUS_PALETTE_MOD_MASK = 0x60c26020,
    BATTLE_MISC_STATUS_BUBBLE_MASK = 0x1fffc2c4,
    BATTLE_MISC_STATUS_PREVENT_FACING_MASK = 0x00080074,
    BATTLE_MISC_STATUS_ANIMATION_SELECTION_MASK = 0x00003fff,
} battle_misc_status_flags_1_4_e;

typedef enum battle_misc_status_flags_5_6 {
    /* Misc +0x144. */
    BATTLE_MISC_STATUS_CRYSTAL = 0x00000001,
    BATTLE_MISC_STATUS_CHICKEN = 0x00000002,
    BATTLE_MISC_STATUS_FROG = 0x00000004,
    BATTLE_MISC_STATUS_TREASURE = 0x00000008,
    BATTLE_MISC_STATUS_TRANSPARENT = 0x00000010,
    BATTLE_MISC_STATUS_FLOAT = 0x00000020,
    BATTLE_MISC_STATUS_JUMP = 0x00000040,
    BATTLE_MISC_STATUS_POACHED = 0x00000080,

    /* Misc +0x145. Other bits remain unknown. */
    BATTLE_MISC_STATUS_MORBOL = 0x00000100,
    BATTLE_MISC_STATUS_JUMP_HEIGHT_ACTIVE = 0x00000200,

    BATTLE_MISC_STATUS_TRANSFORMATION_MASK = 0x0000000f,
} battle_misc_status_flags_5_6_e;

/* Kept while source files migrate to the more specific type name. */
/* In-use words at 0x800b7304, stride 0x440: each sits one word ahead of its
 * g_battle_unit_misc_data record, in the previous record's last word. */
typedef struct battle_unit_misc_slot_flag {
    s32 in_use;
    u8 _unused_004[0x43c];
} battle_unit_misc_slot_flag_t;
typedef char battle_misc_slot_flag_size_must_be_0x440[(sizeof(battle_unit_misc_slot_flag_t) == 0x440) ? 1 : -1];

/* Provisional: war-trophy search result filled by
 * battle_unit_find_war_trophies_and_bonus_money. REQUIRE links the same routine
 * as battle_unit_find_war_trophies_and_bonus_money into
 * g_require_reward_war_trophy_search_result. */
typedef struct battle_war_result {
    u8 item_ids[BATTLE_UNIT_SLOT_COUNT]; /* 0x00 */
    u8 item_count;                       /* 0x15 */
    u8 _padding_16[2];                   /* 0x16; aligns bonus_money */
    s32 bonus_money;                     /* 0x18; sum of bonus-money modifiers * 100 */
    s32 level_sum_money;                 /* 0x1c; g_battle_enemy_level_sum * 100 */
    s32 level_bonus_money;               /* 0x20; (highest enemy level - lowest level) * 100 */
} battle_war_result_t;
typedef char battle_war_result_size_must_be_0x24[(sizeof(battle_war_result_t) == 0x24) ? 1 : -1];

/* Seven result bytes; assigning elevation preserves the unassigned high bit. */
typedef struct battle_unit_height_data {
    u8 x;
    u8 y;
    u8 elevation : 7;
    u8 _unused_02 : 1; /* not read or assigned */
    u8 unit_flags;
    u8 unit_height;
    u8 walking_height;
    u8 total_height;
} battle_unit_height_data_t;
typedef char battle_unit_height_data_size_must_be_7[sizeof(battle_unit_height_data_t) == 7 ? 1 : -1];

/* Sprite lookup result filled in by battle_gfx_get_unit_sprite_frame_and_vram_ids. */
typedef struct {
    s16 clut;             /* 0x00; the unit's CLUT of MAP_CLUT_ROW_UNITS */
    s16 tpage;            /* 0x02 */
    s16 graphic_height;   /* 0x04 */
    s16 graphic_y_offset; /* 0x06 */
} battle_unit_sprite_query_t;

/* Queued unit graphics load (g_main_gfx_unit_graphics_load_queue.descriptors, 16 entries),
 * appended by battle_gfx_append_unit_graphics_load_descriptor and consumed by
 * the per-unit graphics loader at 0x80088904. */
typedef struct battle_unit_graphics_load_descriptor {
    u8 map_x;
    u8 map_y;
    u8 map_level;
    u8 _padding_03;     /* aligns facing */
    u16 facing;         /* a quarter turn per unit */
    s16 spritesheet_id; /* below BATTLE_SPRITESHEET_ID_COUNT */
    u16 palette_id;
    u16 misc_id;
    struct battle_stats* battle_stats;
    u32 flags; /* nonzero: the unit starts undrawn */
} battle_unit_graphics_load_descriptor_t;
typedef char battle_unit_graphics_load_descriptor_size_must_be_0x14
    [(sizeof(battle_unit_graphics_load_descriptor_t) == 0x14) ? 1 : -1];

/* The load queue at 0x80049c18: battle_gfx_append_unit_graphics_load_descriptor
 * indexes the descriptors by the count through one base register. */
typedef struct battle_unit_graphics_load_queue {
    s32 count;                                              /* 0x00 */
    battle_unit_graphics_load_descriptor_t descriptors[16]; /* 0x04 */
} battle_unit_graphics_load_queue_t;

typedef enum battle_unit_spell_quote_skillset_flags {
    BATTLE_UNIT_SPELL_QUOTE_SECONDARY_SKILLSET = 1,
    BATTLE_UNIT_SPELL_QUOTE_PRIMARY_SKILLSET = 2,
} battle_unit_spell_quote_skillset_flags_e;

/* Facing values shifted to position bit 8 for the evasion comparisons. */
enum {
    BATTLE_UNIT_POSITION_FACING_SOUTH = 0x0000,
    BATTLE_UNIT_POSITION_FACING_WEST = 0x0100,
    BATTLE_UNIT_POSITION_FACING_NORTH = 0x0200,
    BATTLE_UNIT_POSITION_FACING_EAST = 0x0300,
};

typedef void (*battle_distortion_animation_handler_t)(battle_unit_misc_data_t* unit);

extern const u32 g_battle_misc_status_mask_by_handler_index[BATTLE_STATUS_COUNT + 1];
extern POLY_FT4* g_battle_gfx_sprite_prim_pool;
extern battle_unit_misc_data_t g_battle_unit_misc_data[16];
extern battle_distortion_animation_handler_t
    g_battle_unit_distortion_animation_handlers[BATTLE_DISTORTION_ANIMATION_COUNT];
extern battle_unit_misc_slot_flag_t g_battle_unit_misc_slot_flags[];
extern unit_status_staging_t* g_battle_unit_status_staging_data;
extern s32 g_battle_unit_palette_animation_offset;
extern s32 g_battle_unit_palette_animation_timer;
extern u16 g_battle_unit_view_started_battle_id;
extern u16 g_battle_unit_view_started_comparison_id;
extern world_unit_view_thread_set_t g_battle_unit_view_thread_sets[];
extern s32 g_battle_unit_view_mode;
extern u16 g_battle_unit_status_bounce_step;
extern world_gfx_image_load_parameters_t g_battle_unit_editor_sprite_image_params_a[19];
extern world_gfx_image_load_parameters_t g_battle_unit_editor_sprite_image_params_b[19];
extern world_gfx_image_load_parameters_t g_battle_unit_editor_row_image_params_a[4];
extern world_gfx_image_load_parameters_t g_battle_unit_editor_row_image_params_b[4];
extern world_gfx_image_load_parameters_t g_battle_unit_editor_item_icon_image_params[5];
extern u16 g_battle_unit_editor_sprite_clut_pairs[19][2];
extern world_menu_palette_slot_t g_battle_unit_summary_palettes[];
extern u8 g_battle_unit_summary_digit_images[];
extern world_menu_palette_primitives_t g_battle_unit_editor_palette;
extern s32 g_battle_companion_overlay_primitives;

/* The statuses a rider can be inflicted with, one mask per status byte. */
extern u8 g_battle_rider_status_infliction_mask[BATTLE_STATUS_BYTE_COUNT];
extern s32 g_battle_entd_selection_mode; /* battle_entd_selection_e */

/* Count and coordinates of the tiles already handed out as dismount
 * destinations this action. */
extern s32 g_battle_relocated_unit_count;
extern battle_tile_coordinates_t g_battle_relocated_unit_coords[];
extern RECT g_battle_unit_editor_cursor_rect; /* cursor source */
extern RECT g_battle_unit_editor_panel_rect;  /* panel box */
extern RECT g_battle_unit_editor_text_rect_a; /* first text image */
extern RECT g_battle_unit_editor_text_rect_b; /* second text image */
extern s32 g_battle_unit_graphics_load_complete;
extern battle_unit_misc_data_t* g_battle_unit_misc_list_head;

/* WalkToAnim: the animation each unit takes once it has moved and stopped
 * (battle_unit_update_facing_and_pending_animations); 0 none. */
extern u16 g_battle_unit_pending_animation_ids[];
extern battle_stats_t g_battle_unit_stats[BATTLE_UNIT_SLOT_COUNT];
extern u16 g_battle_unit_status_bounce_offsets[];
extern u8 g_battle_unit_status_first_page_by_mode[];
extern world_unit_status_page_t g_battle_unit_status_panel_pages[];
extern CVECTOR g_battle_unit_status_gauge_colors[];
extern world_gfx_image_load_parameters_t g_battle_unit_status_icon_image_params[];
extern u16 g_battle_unit_status_quad_clut_pairs[][2];
/* Live caster-panel image parameters followed by their defaults. */
extern world_gfx_image_load_parameters_t g_battle_menu_attack_caster_quad_image_params[2][5];
extern world_gfx_image_load_parameters_t* g_battle_unit_status_quad_image_params_by_mode[];
extern u16 g_battle_unit_status_sprite_clut_pairs[][2];
extern world_gfx_image_load_parameters_t* g_battle_unit_status_sprite_image_params_by_mode[];
extern RECT g_battle_unit_summary_panel_rect;
extern world_gfx_image_load_parameters_t g_battle_unit_summary_portrait_image_params;
extern world_gfx_image_load_parameters_t g_battle_unit_summary_sprite_image_params[];
extern u16 g_battle_unit_was_moving_latch[]; /* a unit with a pending animation was moving on the last update */
extern s32 g_battle_companion_overlay_state;

extern s8 g_battle_unit_float_bob_offsets[24];

/* BATTLE and WORLD use the same seven-byte per-unit rotation state. */
extern world_unit_animation_state_t g_battle_unit_misc_rotation_data[];

/* 0x8016604e: result of battle_gfx_poll_unit_graphics_load, 0 done / 1 pending. */
extern u16 g_battle_unit_graphics_load_pending;
extern battle_menu_status_panel_slot_storage_t g_battle_menu_comparison_billboard_data;
extern u8 g_battle_unit_editor_line_colors[8 * WORLD_UNIT_EDITOR_LINE_SIZE];
extern battle_unit_graphics_load_queue_t g_main_gfx_unit_graphics_load_queue;
battle_unit_height_data_t* battle_unit_calculate_height_data(battle_unit_height_data_t* out, s32 unit_id);
battle_stats_t* battle_unit_find_stats_by_entd_unit_id(s32 unit_id, s32* out_battle_id);
battle_stats_t* battle_unit_get_existing_pointer(u32 unit_id);
s32 battle_script_get_misc_id(s32 unit_id);
void battle_unit_load_entd_units(entd_encounter_t* entd_data, s32 mode);
s32 battle_unit_find_at_tile(s32 x, s32 y, u32 level, s32 filters);
s32 battle_unit_draw_and_activate_by_misc_id(u32 misc_id);
void battle_unit_apply_poach_morbol_transformation(battle_unit_misc_data_t* unit);
void battle_unit_apply_staged_status_data(s32 unit_id, s32 misc_id);
u8 battle_unit_calculate_move_exp_jp_up_to_earn(battle_stats_t* unit);
void battle_unit_restore_all_facings_after_attack(void);
void battle_unit_clear_distortion_animation_id_10(battle_unit_misc_data_t* unit);
void battle_unit_clear_distortion_animation_id_11(battle_unit_misc_data_t* unit);
void battle_unit_clear_facing_update_suppression(u32 misc_id);
void battle_unit_clear_status_staging_data(void);
void battle_unit_copy_misc_screen_location(u32 misc_id, VECTOR* destination);
void battle_unit_copy_rider_data_to_mount(battle_unit_misc_data_t* unit, battle_unit_misc_data_t* rider);
s32 battle_unit_count_crystal_misc_units(void);
void battle_unit_decide_facing_direction(battle_unit_misc_data_t* unit, s32 facing);
void battle_unit_dismount_rider(battle_unit_misc_data_t* unit);
s32 battle_unit_animate_teleport_distortion(battle_unit_misc_data_t* unit);
void battle_unit_dispatch_distortion_animation(battle_unit_misc_data_t* unit);
void battle_unit_face_toward_cursor(battle_unit_misc_data_t* unit);
battle_unit_misc_data_t* battle_unit_find_other_chocobo_at_map_coords(s32 map_x, s32 map_y, s32 map_z);
void battle_unit_find_war_trophies_and_bonus_money(battle_war_result_t* result);
s32 battle_unit_disable_remove(s32 misc_id);
void battle_unit_set_facing(s32 misc_id, s32 value);
void battle_unit_project_misc_to_screen(u32 misc_id, SVECTOR* screen_coordinates);
s32 battle_unit_set_enemy_level_data_by_battle_id(u32 battle_id);
s32 battle_unit_set_target_anim_by_misc_id(s32 misc_id);
void battle_unit_set_facing_update_suppression(u32 misc_id);
s32 battle_unit_get_battle_index_by_entd_unit_id(s32 unit_id);
s32 battle_unit_get_battle_index_by_misc_id(u32 misc_id);
battle_stats_t* battle_unit_get_by_unit_id(s32 unit_id);
battle_unit_misc_data_t* battle_unit_get_casting_misc_data(void);
u8 battle_unit_get_effective_height(battle_stats_t* unit);
s16 battle_unit_get_facing_quadrant_by_misc_id(u32 misc_id);
battle_unit_misc_data_t* battle_unit_get_selectable_misc_data_at_map_coords(s32 map_x, s32 map_y, s32 map_z);
battle_unit_misc_data_t* battle_unit_get_crystal_or_treasure_at_map_coords(s32 map_x, s32 map_y, s32 map_z);
battle_unit_misc_data_t* battle_unit_get_misc_data_by_battle_id(u16 battle_id);
battle_unit_misc_data_t* battle_unit_get_misc_data_by_misc_id(u16 misc_id);
s32 battle_unit_get_misc_id_by_battle_id(u32 battle_id);
battle_unit_misc_data_t* battle_unit_get_source_misc_data(void);
battle_stats_t* battle_unit_get_stats_from_battle_id(u32 id);
s32 battle_unit_has_misc_id(u32 misc_id);
void battle_unit_increase_blue_team_brave_by_10(void);
s32 battle_unit_init_party_member_battle_data(s32 unused0, s32 unused1, s32 unit_index);
void battle_unit_learn_used_ability(battle_stats_t* unit);
void battle_unit_transform_into_morbol(battle_stats_t* unit);
void battle_unit_mount_rider_onto_unit(battle_unit_misc_data_t* rider, battle_unit_misc_data_t* mount);
s32 battle_unit_move_misc_unit_to_head(u32 misc_id);
s32 battle_unit_place_in_new_location(u32 misc_id, unit_tile_coordinates_t* tile, s32 facing);
s32 battle_unit_poll_and_load_entd_units(void);
s32 battle_unit_remove_misc_by_id(u32 misc_id);
void battle_unit_reset_animation_states(void);
void battle_unit_save_previous_state(battle_unit_misc_data_t* unit);
void battle_unit_set_animation_based_on_mounted_state(battle_unit_misc_data_t* unit);
void battle_unit_set_bow_hardcoding(battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target);
s32 battle_unit_set_cannot_exist(s32 unit_index);
void battle_unit_set_enemy_level_data(battle_stats_t* unit);
void battle_unit_set_animation_based_on_status(battle_unit_misc_data_t* unit);
void battle_unit_set_idle_animation_for_movement(battle_unit_misc_data_t* unit);
void battle_unit_set_level_up_animation(battle_unit_misc_data_t* unit);
void battle_unit_set_map_coords_after_death_dismount(battle_unit_misc_data_t* unit);
void battle_unit_set_real_coords_from_map_coords(battle_unit_misc_data_t* unit);
void battle_unit_restore_facing_after_attack(battle_unit_misc_data_t* unit);
void battle_unit_set_animation(s32 misc_id, u32 animation);
void battle_unit_snap_all_facings_to_quadrants(void);
void battle_unit_start_ability_charge_animation(battle_unit_misc_data_t* unit);
void battle_unit_start_ability_charge_hold_animation(battle_unit_misc_data_t* unit);
void battle_unit_store_animation_facing(u32 animation, s32 facing, battle_unit_misc_data_t* unit);
void battle_unit_store_coordinate_mount_animation_data(battle_unit_misc_data_t* unit);
s32 battle_unit_try_get_misc_data_by_unit_id(u16* unit_id, s32* state);
void battle_unit_save_facing_before_attack(battle_unit_misc_data_t* unit);
void battle_unit_update_and_animate_units(void);
void battle_unit_update_anim_display_for_all_targets(battle_unit_misc_data_t* unit);
void battle_unit_update_attacker_earned_experience(s32 outcome);
void battle_unit_update_display(battle_unit_misc_data_t* unit);
void battle_unit_update_display_by_misc_id(s32 misc_id);
s32 battle_unit_update_fade_out_removal(battle_unit_misc_data_t* unit);
void battle_unit_update_float_and_jump_height(battle_unit_misc_data_t* unit);
void battle_unit_update_graphics_by_misc_id_wrapper(u32 misc_id);
void battle_unit_set_tile_position_with_mount_partner(battle_unit_misc_data_t* unit);
void battle_unit_update_post_command_animation_display(battle_unit_misc_data_t* unit);
void battle_unit_update_staged_status_data(u16 unit_id, u16 restore);
s32 battle_unit_set_placement_and_validate(s32 unit_index, unit_deployment_coordinates_t* src);
s32 battle_unit_validate_placement(s32 unit_index);
void battle_unit_view_supervisor_thread(void);
s32 battle_unit_save_staged_status_data(s32 unit_id);
s32 battle_unit_find_relocation_tile(s32 unit_index, unit_tile_coordinates_t* destination);
void battle_unit_update_animation_for_status_changes(battle_unit_misc_data_t* unit);
void battle_unit_move_toward_action_target(battle_unit_misc_data_t* unit, s32 allow_height_change);
s32 battle_unit_apply_action_rewards(s32 unit_index, battle_action_reward_display_t* display);
s32 battle_unit_init_deployed_units_data(unit_deployment_coordinates_t* formation, s32 is_red_team_debug);
s32 battle_unit_set_tile_position(s32 misc_unit_id, u8 map_x, u8 map_y, u8 map_z, u8 facing);

void battle_unit_set_target_animation_from_attack_type(
    battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target);

s16 battle_unit_get_camera_facing_quadrant_by_battle_id(u32 battle_id);
void battle_unit_shift_down_or_up(battle_unit_misc_data_t* unit, s8 delta);
s32 battle_unit_apply_level_up_down_ability(void);
s32 battle_unit_apply_stat_increment_decrement(s32 mod, u8* stat, u8 max, u8 min);
s32 battle_unit_build_deployed_units_data(unit_deployment_coordinates_t* formation, s32 unused_1);
s32 battle_unit_build_gained_exp_jp_level_job_level(battle_stats_t* unit, battle_action_reward_display_t* rewards);
void battle_unit_call_bow_hardcoding_by_misc_id(u16 attacker_id, u16 target_id);
void battle_unit_call_set_animation_based_on_status(struct battle_unit_misc_data* unit);
s32 battle_unit_get_chocobo_mount_state(battle_stats_t* unit);
s32 battle_unit_check_facing_relative_to_camera(s32 unit_id);
battle_unit_misc_data_t* battle_unit_claim_misc_slot(s16 unit_id);
void battle_unit_clear_horizontal_flip_flag(u32 misc_id);
void battle_unit_clear_movement_effect_suppression(u32 misc_id);
void battle_unit_clear_palette_update_suppression(u32 misc_id);
void battle_unit_copy_map_coords_from_misc_id(u32 misc_id, SVECTOR* coords);
void battle_unit_copy_misc_data_xyz_values(SVECTOR* destination, const battle_unit_misc_data_t* misc_data);
void battle_unit_copy_misc_screen_coords(u32 misc_id, SVECTOR* screen_coords);
void battle_unit_dismount_rider_and_update_display(battle_unit_misc_data_t* rider);
void battle_unit_face_towards_action_target(battle_unit_misc_data_t* unit, battle_unit_misc_data_t* target);
battle_stats_t* battle_unit_find_active_data_pointer(void);
battle_action_command_t* battle_unit_get_action_command_by_battle_id(s32 unit_id);
SVECTOR* battle_unit_get_event_offset_ptr_by_misc_id(u32 misc_id);
battle_stats_t* battle_unit_get_attacker_data_pointer(void);
s32 battle_unit_get_facing_nibble_by_misc_id(u32 misc_id);
s32 battle_unit_get_map_coords_from_misc_id(u32 misc_id, SVECTOR* coords);
battle_unit_misc_data_t* battle_unit_find_any_misc_data_at_map_coords(s32 map_x, s32 map_y, s32 map_z);

battle_unit_misc_data_t* battle_unit_get_overlapping_misc_data_pointer(
    battle_unit_misc_data_t* self, s32 x, s32 y, s32 z);

battle_action_command_t* battle_unit_get_misc_action_command_by_battle_id(u32 battle_id);
s32 battle_unit_get_tile_center_and_height_by_battle_id(u32 battle_id, SVECTOR* coords);
s32 battle_unit_add_event_offset_by_misc_id(u32 misc_id, const SVECTOR* amount);
void battle_unit_increment_or_decrement_height_mod(battle_unit_misc_data_t* unit);
void battle_move_update_teleport_start(struct battle_unit_misc_data* unit);
void battle_move_start_teleport_animation(battle_unit_misc_data_t* unit);
void battle_unit_init_deployed_units_data_for_debug_red_team(unit_deployment_coordinates_t* formation, s32 unused_1);

battle_unit_misc_data_t* battle_unit_init_misc_data(s32 map_x, s32 map_y, s32 map_level, s16 facing, s32 spritesheet_id,
    s16 palette, s16 misc_id, battle_stats_t* stats, u32 flags, const battle_gfx_unit_sprite_file_t* data);

s32 battle_unit_learn_from_crystal(battle_stats_t* unit, s32 flags);
s32 battle_unit_load_job_level(battle_stats_t* unit, s32 job_id, s32* out_slot);
void battle_unit_mount_onto_pending_mount(battle_unit_misc_data_t* unit);
s32 battle_unit_prepare_rider_dismount(u32 misc_id, s32 direction, s32 jump_length);
void battle_unit_remove_misc(battle_unit_misc_data_t* unit);
s32 battle_unit_select_and_open_entd(void);

void battle_unit_select_attack_animation_for_ability(
    battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target);

void battle_unit_select_weapon_attack_animation(battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target);
void battle_unit_set_animation_for_item_abilities(battle_unit_misc_data_t* unit, battle_unit_misc_data_t* target);
s32 battle_unit_set_animation_from_facing_nibble(u32 misc_id, s32 facing_nibble);

void battle_unit_set_attacker_animation_for_shield_block(
    battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target);

void battle_unit_set_horizontal_flip_flag(u32 misc_id);
void battle_unit_set_mounted_animation_by_misc_id(u32 misc_id);
void battle_unit_set_move_and_screen_coords(battle_unit_misc_data_t* unit);
void battle_unit_set_movement_effect_suppression(u32 misc_id);
void battle_unit_set_palette_update_suppression(u32 misc_id);
void battle_unit_set_specific_animation_by_misc_id(u32 misc_id, u32 animation);
void battle_unit_set_status_animation_by_misc_id(u32 misc_id);
void battle_unit_shift_forward_or_backward(battle_unit_misc_data_t* unit, s8 delta);
void battle_unit_shift_right_or_left(battle_unit_misc_data_t* unit, s8 delta);
void battle_unit_store_animation_facing_movement_data(s32 animation, s32 facing, battle_unit_misc_data_t* unit);
void battle_unit_store_entd_flags_without_control_flag(battle_unit_misc_data_t* unit);
s32 battle_unit_store_jp_and_calculate_unlocked_jobs(s32 unit_index, s32 jp, s32 team, s32 slot);
void battle_unit_update_and_animate_units_three_times(void);
void battle_unit_update_attack_result_animation(battle_unit_misc_data_t* unit);
void battle_unit_update_facing_and_pending_animations(void);
void battle_unit_start_post_attack_animation_display(
    battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target);
u8 battle_unit_generate_treasure(battle_stats_t* unit);
SVECTOR* battle_copy_misc_unit_screen_coords(u32 misc_id);
struct battle_unit_misc_data* battle_unit_get_mount_misc_data(struct battle_unit_misc_data* unit);
SVECTOR* battle_unit_get_screen_data_ptr_by_misc_id(u32 misc_id);
void battle_unit_set_real_coords_from_screen_coords(struct battle_unit_misc_data* unit);
void battle_unit_set_screen_coords_from_real_coords(struct battle_unit_misc_data* unit);
void battle_unit_clear_misc_units(void);

/* gfx */
/* Serialized SHP/SEQ values are little-endian byte sequences. These file
 * views have byte alignment; their decoded RAM tables remain separate. */

/* A unit SHP frame set: primary at file +8; an optional submerged set is
 * addressed by the file header. Only the first 0xd0 offsets are installed. */
typedef struct battle_gfx_packed_unit_shp_frame_set {
    u8 frame_offsets[0x100][4]; /* 0x000; signed words, -1 resolves to offset zero */
    u8 frame_data_size[2];      /* 0x400; unsigned halfword */
    u8 frame_data[0];           /* 0x402; variable tail */
} battle_gfx_packed_unit_shp_frame_set_t;
typedef char battle_gfx_packed_unit_shp_frame_set_size_must_be_0x402
    [sizeof(battle_gfx_packed_unit_shp_frame_set_t) == 0x402 ? 1 : -1];

typedef struct battle_gfx_packed_unit_shp_record {
    u8 submerged_frame_set_offset[4]; /* 0x000; 8 means both native tables share the primary set */
    u8 first_attack_frame[2];         /* 0x004; unsigned halfword */
    u8 first_sp2_frame[2];            /* 0x006; unsigned halfword */
    battle_gfx_packed_unit_shp_frame_set_t primary_frame_set; /* 0x008 */
} battle_gfx_packed_unit_shp_record_t;
typedef char
    battle_gfx_packed_unit_shp_record_size_must_be_0x40a[sizeof(battle_gfx_packed_unit_shp_record_t) == 0x40a ? 1 : -1];

/* WEP/EFF SHP has a different header and twice as many frame offsets. */
typedef struct battle_gfx_packed_weapon_shp_record {
    u8 _unused_000[4];          /* 0x000; loader skips this word */
    u8 first_frame[0x20][2];    /* 0x004; unsigned zero-frame indices by weapon type */
    u8 frame_offsets[0x200][4]; /* 0x044; signed words, -1 resolves to offset zero */
    u8 frame_data_size[2];      /* 0x844; unsigned halfword */
    u8 frame_data[0];           /* 0x846; variable tail */
} battle_gfx_packed_weapon_shp_record_t;
typedef char
    battle_gfx_packed_weapon_shp_record_size_must_be_0x846[sizeof(battle_gfx_packed_weapon_shp_record_t) == 0x846 ? 1
                                                                                                                  : -1];

/* WEP/EFF SEQ stores 256 offsets followed by a variable sequence payload. */
typedef struct battle_gfx_packed_offset_table {
    u8 _unused_000[4];  /* 0x000; WEP/EFF loader skips the header word */
    u8 entry[0x100][4]; /* 0x004; signed sequence offsets, -1 resolves to zero */
    u8 payload_size[2]; /* 0x404; unsigned halfword */
    u8 payload[0];      /* 0x406; variable sequence data */
} battle_gfx_packed_offset_table_t;
typedef char
    battle_gfx_packed_offset_table_size_must_be_0x406[sizeof(battle_gfx_packed_offset_table_t) == 0x406 ? 1 : -1];

/* One of the 137 fixed-size blocks in EVTCHR.BIN. No native pointer tables
 * or widened unit-animation header words are present in the file. */
typedef struct battle_gfx_packed_evtchr_record {
    u8 sequence_offsets[0x40][4]; /* 0x0000; signed words, -1 resolves to zero */
    u8 sequence_data[0x400];      /* 0x0100 */
    u8 frame_offsets[0x28][4];    /* 0x0500; signed words, -1 resolves to zero */
    u8 frame_data[0x1e0];         /* 0x05a0; copied into both native frame sets */
    u8 cluts[16 * 16 * 2];        /* 0x0780; 16 CLUTs, 16 little-endian colours each */
    u8 pixels[200 * 128];         /* 0x0980; 256x200, 4bpp */
    u8 _unused_6d80[0xa80];       /* unread tail of the 0x7800-byte block */
} battle_gfx_packed_evtchr_record_t;
typedef char
    battle_gfx_packed_evtchr_record_size_must_be_0x7800[sizeof(battle_gfx_packed_evtchr_record_t) == 0x7800 ? 1 : -1];

/* OTHER.SPR: the palette helper copies only the first 21 of 32 CLUTs. */
typedef struct battle_gfx_other_sprite_archive {
    u8 palettes[32 * 16 * 2]; /* 0x0000; 32 CLUTs, 16 little-endian colours each */
    u8 pixels[256 * 128];     /* 0x0400; uncompressed 256x256, 4bpp */
} battle_gfx_other_sprite_archive_t;
typedef char
    battle_gfx_other_sprite_archive_size_must_be_0x8400[sizeof(battle_gfx_other_sprite_archive_t) == 0x8400 ? 1 : -1];

/* First 21 OTHER.SPR palettes copied together; the other three groups also
 * retain their independently loaded symbol aliases. */
typedef struct battle_gfx_other_palettes {
    palette_t chicken[5];  /* 0x000 */
    palette_t frog[5];     /* 0x0a0 */
    palette_t crystal[6];  /* 0x140 */
    palette_t treasure[5]; /* 0x200 */
} battle_gfx_other_palettes_t;
typedef char battle_gfx_other_palettes_size_must_be_0x2a0[sizeof(battle_gfx_other_palettes_t) == 0x2a0 ? 1 : -1];

/* WEP.SPR contains three CLUT/image pairs: weapons, trails and particles. */
typedef struct battle_gfx_weapon_sprite_archive {
    clut_t weapon_palettes;       /* 0x00000 */
    u8 weapon_pixels[256][128];   /* 0x00200; 4bpp */
    clut_t trail_palettes;        /* 0x08200 */
    u8 trail_pixels[256][128];    /* 0x08400; 4bpp */
    clut_t particle_palettes;     /* 0x10400 */
    u8 particle_pixels[144][128]; /* 0x10600; 4bpp */
} battle_gfx_weapon_sprite_archive_t;

/* Commands of 0x800e77b8, which stores RGB and controls GTE background color. */
typedef enum battle_background_color_command {
    BATTLE_BACKGROUND_COLOR_SET = 9,
    BATTLE_BACKGROUND_COLOR_GET = 10,
    BATTLE_BACKGROUND_COLOR_DISABLE = 11,
    BATTLE_BACKGROUND_COLOR_RESTORE = 12
} battle_background_color_command_e;

/* Texture pages stored in a sprite display record's tpage. */
enum {
    /* The images of g_battle_gfx_vram_slots[0] and [1]. */
    BATTLE_GFX_TPAGE_EVTCHR_SLOT_0 = 4,
    BATTLE_GFX_TPAGE_EVTCHR_SLOT_1 = 5,
    /* The effect and the weapon sheets of WEP.SPR. */
    BATTLE_GFX_TPAGE_EFFECT = 8,
    BATTLE_GFX_TPAGE_WEAPON = 9,
    /* The sheets from BATTLE_SPRITESHEET_ID_CRYSTAL up. */
    BATTLE_GFX_TPAGE_OTHER = 11,
    /* Plus the unit's spritesheet slot. */
    BATTLE_GFX_TPAGE_SPRITESHEET_SLOT_0 = 0x14,
    BATTLE_GFX_TPAGE_ITEM = 0x1e,  /* ITEM.BIN */
    BATTLE_GFX_TPAGE_FRAME = 0x1f, /* FRAME.BIN */
};

/* Spritesheet ids index g_battle_gfx_spritesheet_data and
 * g_battle_gfx_spritesheet_files. An id below 0x80 is the character_identity_e
 * value it is loaded for (main_unit_get_spritesheet_palette returns those
 * unchanged) and has that identity's name; a numbered name loads the file of
 * the sheet with the plain name. From 0x60 the sheets are the generic jobs',
 * male then female: the generic selectors resolve to
 * 0x60 + (job - JOB_ID_SQUIRE) * 2, plus one for a female unit, with Dancer
 * and Mime moved down one job. Then comes one sheet per monster family, named
 * by the first job whose record has it (job_data_t.spritesheet_id), and the
 * four status sheets, which have no file of their own. */
typedef enum battle_spritesheet_id {
    BATTLE_SPRITESHEET_ID_RAMZA_CHAPTER_1 = 0x01,
    BATTLE_SPRITESHEET_ID_RAMZA_CHAPTER_2_3 = 0x02,
    BATTLE_SPRITESHEET_ID_RAMZA_CHAPTER_4 = 0x03,
    BATTLE_SPRITESHEET_ID_DELITA_CHAPTER_1 = 0x04,
    BATTLE_SPRITESHEET_ID_DELITA_CHAPTER_2_3 = 0x05,
    BATTLE_SPRITESHEET_ID_DELITA_CHAPTER_4 = 0x06,
    BATTLE_SPRITESHEET_ID_ALGUS = 0x07,
    BATTLE_SPRITESHEET_ID_ZALBAG = 0x08,
    BATTLE_SPRITESHEET_ID_DYCEDARG = 0x09,
    BATTLE_SPRITESHEET_ID_LARG = 0x0a,
    BATTLE_SPRITESHEET_ID_GOLTANA = 0x0b,
    BATTLE_SPRITESHEET_ID_OVELIA = 0x0c,
    BATTLE_SPRITESHEET_ID_ORLANDU = 0x0d,
    BATTLE_SPRITESHEET_ID_FUNERAL = 0x0e,
    BATTLE_SPRITESHEET_ID_REIS_HUMAN = 0x0f,
    BATTLE_SPRITESHEET_ID_ZALMO = 0x10,
    BATTLE_SPRITESHEET_ID_GAFGARION_ENEMY = 0x11,
    BATTLE_SPRITESHEET_ID_MALAK_12 = 0x12,
    BATTLE_SPRITESHEET_ID_SIMON = 0x13,
    BATTLE_SPRITESHEET_ID_ALMA_BATTLE = 0x14,
    BATTLE_SPRITESHEET_ID_OLAN = 0x15,
    BATTLE_SPRITESHEET_ID_MUSTADIO_JOIN = 0x16,
    BATTLE_SPRITESHEET_ID_GAFGARION_GUEST = 0x17,
    BATTLE_SPRITESHEET_ID_DRACLAU = 0x18,
    BATTLE_SPRITESHEET_ID_RAFA_GUEST = 0x19,
    BATTLE_SPRITESHEET_ID_MALAK = 0x1a,
    BATTLE_SPRITESHEET_ID_ELMDOR = 0x1b,
    BATTLE_SPRITESHEET_ID_TETA = 0x1c,
    BATTLE_SPRITESHEET_ID_BARINTEN = 0x1d,
    BATTLE_SPRITESHEET_ID_AGRIAS_JOIN = 0x1e,
    BATTLE_SPRITESHEET_ID_BEOWULF = 0x1f,
    BATTLE_SPRITESHEET_ID_WIEGRAF_CHAPTER_1 = 0x20,
    BATTLE_SPRITESHEET_ID_BALMAFULA = 0x21,
    BATTLE_SPRITESHEET_ID_MUSTADIO_GUEST = 0x22,
    BATTLE_SPRITESHEET_ID_RUDVICH = 0x23,
    BATTLE_SPRITESHEET_ID_VORMAV = 0x24,
    BATTLE_SPRITESHEET_ID_ROFEL = 0x25,
    BATTLE_SPRITESHEET_ID_IZLUDE = 0x26,
    BATTLE_SPRITESHEET_ID_KLETIAN = 0x27,
    BATTLE_SPRITESHEET_ID_WIEGRAF_CHAPTER_2_3 = 0x28,
    BATTLE_SPRITESHEET_ID_RAFA_JOIN = 0x29,
    BATTLE_SPRITESHEET_ID_MELIADOUL_JOIN = 0x2a,
    BATTLE_SPRITESHEET_ID_BALK = 0x2b,
    BATTLE_SPRITESHEET_ID_ALMA_2C = 0x2c,
    BATTLE_SPRITESHEET_ID_CELIA = 0x2d,
    BATTLE_SPRITESHEET_ID_LEDE = 0x2e,
    BATTLE_SPRITESHEET_ID_MELIADOUL_ENEMY = 0x2f,
    BATTLE_SPRITESHEET_ID_ALMA_EVENTS = 0x30,
    BATTLE_SPRITESHEET_ID_AJORA = 0x31,
    BATTLE_SPRITESHEET_ID_CLOUD = 0x32,
    BATTLE_SPRITESHEET_ID_ZALBAG_ZOMBIE = 0x33,
    BATTLE_SPRITESHEET_ID_AGRIAS_GUEST = 0x34,
    BATTLE_SPRITESHEET_ID_FEMALE_CHEMIST_35 = 0x35,
    BATTLE_SPRITESHEET_ID_FEMALE_PRIEST_36 = 0x36,
    BATTLE_SPRITESHEET_ID_MALE_WIZARD_37 = 0x37,
    BATTLE_SPRITESHEET_ID_MALE_ORACLE_38 = 0x38,
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE_39 = 0x39,
    BATTLE_SPRITESHEET_ID_CELIA_3A = 0x3a,
    BATTLE_SPRITESHEET_ID_LEDE_3B = 0x3b,
    BATTLE_SPRITESHEET_ID_VELIUS = 0x3c,
    BATTLE_SPRITESHEET_ID_MALE_KNIGHT_3D = 0x3d,
    BATTLE_SPRITESHEET_ID_ZALERA = 0x3e,
    BATTLE_SPRITESHEET_ID_MALE_ARCHER_3F = 0x3f,
    BATTLE_SPRITESHEET_ID_HASHMALUM = 0x40,
    BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM = 0x41,
    BATTLE_SPRITESHEET_ID_MALE_WIZARD_42 = 0x42,
    BATTLE_SPRITESHEET_ID_QUEKLAIN = 0x43,
    BATTLE_SPRITESHEET_ID_FEMALE_TIME_MAGE_44 = 0x44,
    BATTLE_SPRITESHEET_ID_ADRAMELK = 0x45,
    BATTLE_SPRITESHEET_ID_MALE_ORACLE_46 = 0x46,
    BATTLE_SPRITESHEET_ID_FEMALE_SUMMONER_47 = 0x47,
    BATTLE_SPRITESHEET_ID_REIS_DRAGON = 0x48,
    BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM = 0x49,
    BATTLE_SPRITESHEET_ID_BOY = 0x4a,
    BATTLE_SPRITESHEET_ID_GIRL = 0x4b,
    BATTLE_SPRITESHEET_ID_YOUNG_MAN = 0x4c,
    BATTLE_SPRITESHEET_ID_YOUNG_WOMAN = 0x4d,
    BATTLE_SPRITESHEET_ID_MIDDLE_AGED_MAN = 0x4e,
    BATTLE_SPRITESHEET_ID_MIDDLE_AGED_WOMAN = 0x4f,
    BATTLE_SPRITESHEET_ID_OLD_MAN = 0x50,
    BATTLE_SPRITESHEET_ID_OLD_WOMAN = 0x51,
    BATTLE_SPRITESHEET_ID_OLD_FUNERAL_MAN = 0x52,
    BATTLE_SPRITESHEET_ID_OLD_FUNERAL_WOMAN = 0x53,
    BATTLE_SPRITESHEET_ID_FUNERAL_MAN = 0x54,
    BATTLE_SPRITESHEET_ID_FUNERAL_WOMAN = 0x55,
    BATTLE_SPRITESHEET_ID_FUNERAL_PRIEST = 0x56,
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE_57 = 0x57,
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE_58 = 0x58,
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE_59 = 0x59,
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE_5A = 0x5a,
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE_5B = 0x5b,
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE_5C = 0x5c,
    BATTLE_SPRITESHEET_ID_DAMI = 0x5d,    /* named by its file, DAMI.SPR; no ENTD unit has it */
    BATTLE_SPRITESHEET_ID_KASANEK = 0x5e, /* named by its file, KASANEK.SPR; no ENTD unit has it */
    BATTLE_SPRITESHEET_ID_KASANEM = 0x5f, /* named by its file, KASANEM.SPR; no ENTD unit has it */
    BATTLE_SPRITESHEET_ID_MALE_SQUIRE = 0x60,
    BATTLE_SPRITESHEET_ID_FEMALE_SQUIRE = 0x61,
    BATTLE_SPRITESHEET_ID_MALE_CHEMIST = 0x62,
    BATTLE_SPRITESHEET_ID_FEMALE_CHEMIST = 0x63,
    BATTLE_SPRITESHEET_ID_MALE_KNIGHT = 0x64,
    BATTLE_SPRITESHEET_ID_FEMALE_KNIGHT = 0x65,
    BATTLE_SPRITESHEET_ID_MALE_ARCHER = 0x66,
    BATTLE_SPRITESHEET_ID_FEMALE_ARCHER = 0x67,
    BATTLE_SPRITESHEET_ID_MALE_MONK = 0x68,
    BATTLE_SPRITESHEET_ID_FEMALE_MONK = 0x69,
    BATTLE_SPRITESHEET_ID_MALE_PRIEST = 0x6a,
    BATTLE_SPRITESHEET_ID_FEMALE_PRIEST = 0x6b,
    BATTLE_SPRITESHEET_ID_MALE_WIZARD = 0x6c,
    BATTLE_SPRITESHEET_ID_FEMALE_WIZARD = 0x6d,
    BATTLE_SPRITESHEET_ID_MALE_TIME_MAGE = 0x6e,
    BATTLE_SPRITESHEET_ID_FEMALE_TIME_MAGE = 0x6f,
    BATTLE_SPRITESHEET_ID_MALE_SUMMONER = 0x70,
    BATTLE_SPRITESHEET_ID_FEMALE_SUMMONER = 0x71,
    BATTLE_SPRITESHEET_ID_MALE_THIEF = 0x72,
    BATTLE_SPRITESHEET_ID_FEMALE_THIEF = 0x73,
    BATTLE_SPRITESHEET_ID_MALE_MEDIATOR = 0x74,
    BATTLE_SPRITESHEET_ID_FEMALE_MEDIATOR = 0x75,
    BATTLE_SPRITESHEET_ID_MALE_ORACLE = 0x76,
    BATTLE_SPRITESHEET_ID_FEMALE_ORACLE = 0x77,
    BATTLE_SPRITESHEET_ID_MALE_GEOMANCER = 0x78,
    BATTLE_SPRITESHEET_ID_FEMALE_GEOMANCER = 0x79,
    BATTLE_SPRITESHEET_ID_MALE_LANCER = 0x7a,
    BATTLE_SPRITESHEET_ID_FEMALE_LANCER = 0x7b,
    BATTLE_SPRITESHEET_ID_MALE_SAMURAI = 0x7c,
    BATTLE_SPRITESHEET_ID_FEMALE_SAMURAI = 0x7d,
    BATTLE_SPRITESHEET_ID_MALE_NINJA = 0x7e,
    BATTLE_SPRITESHEET_ID_FEMALE_NINJA = 0x7f,
    BATTLE_SPRITESHEET_ID_MALE_CALCULATOR = 0x80,
    BATTLE_SPRITESHEET_ID_FEMALE_CALCULATOR = 0x81,
    BATTLE_SPRITESHEET_ID_BARD = 0x82,
    BATTLE_SPRITESHEET_ID_DANCER = 0x83,
    BATTLE_SPRITESHEET_ID_MALE_MIME = 0x84,
    BATTLE_SPRITESHEET_ID_FEMALE_MIME = 0x85,
    BATTLE_SPRITESHEET_ID_CHOCOBO = 0x86,
    BATTLE_SPRITESHEET_ID_GOBLIN = 0x87,
    BATTLE_SPRITESHEET_ID_BOMB = 0x88,
    BATTLE_SPRITESHEET_ID_RED_PANTHER = 0x89,
    BATTLE_SPRITESHEET_ID_PISCO_DEMON = 0x8a,
    BATTLE_SPRITESHEET_ID_SKELETON = 0x8b,
    BATTLE_SPRITESHEET_ID_GHOUL = 0x8c,
    BATTLE_SPRITESHEET_ID_FLOTIBALL = 0x8d,
    BATTLE_SPRITESHEET_ID_JURAVIS = 0x8e,
    BATTLE_SPRITESHEET_ID_URIBO = 0x8f,
    BATTLE_SPRITESHEET_ID_WOODMAN = 0x90,
    BATTLE_SPRITESHEET_ID_BULL_DEMON = 0x91,
    BATTLE_SPRITESHEET_ID_MORBOL = 0x92,
    BATTLE_SPRITESHEET_ID_BEHEMOTH = 0x93,
    BATTLE_SPRITESHEET_ID_DRAGON = 0x94, /* also Reis as a Holy Dragon */
    BATTLE_SPRITESHEET_ID_HYUDRA = 0x95,
    BATTLE_SPRITESHEET_ID_BYBLOS = 0x96, /* and Apanda */
    BATTLE_SPRITESHEET_ID_SERPENTARIUS = 0x97,
    BATTLE_SPRITESHEET_ID_DRAGON_98 = 0x98,     /* loads the Dragon file again; no job has it */
    BATTLE_SPRITESHEET_ID_ARCHAIC_DEMON = 0x99, /* and Ultima Demon */
    BATTLE_SPRITESHEET_ID_STEEL_GIANT = 0x9a,
    BATTLE_SPRITESHEET_ID_CRYSTAL = 0x9b,
    BATTLE_SPRITESHEET_ID_CHICKEN = 0x9c,
    BATTLE_SPRITESHEET_ID_FROG = 0x9d,
    BATTLE_SPRITESHEET_ID_TREASURE = 0x9e,
} battle_spritesheet_id_e;

enum {
    BATTLE_SPRITESHEET_ID_RAMZA_END = 0x04,
    BATTLE_SPRITESHEET_ID_JOB_FIRST = 0x60, /* the generic job sheets start here; the monsters' follow */
    BATTLE_SPRITESHEET_ID_COUNT = 0x9f,
};

enum {
    BATTLE_SHP_FRAME_EVTCHR_FIRST = 0xd2, /* frame 0 of an EVTCHR slot */
};

/* The SHP frames of an EVTCHR slot, laid out as battle_gfx_unit_shp_frame_tables_t
 * so that frame ids from BATTLE_SHP_FRAME_EVTCHR_FIRST up index it
 * (battle_gfx_select_unit_shp_frame). The file has 0x28 frames; their data
 * is stored twice, once for each frame table, with the same contents. */
typedef struct battle_gfx_evtchr_shp {
    battle_gfx_unit_shp_frame_tables_t tables; /* 0x000; header words remain unset and unread */
    u8 data[0x1e0];                            /* 0x688 */
    u8 submerged_data[0x1e0];                  /* 0x868 */
} battle_gfx_evtchr_shp_t;

/*
 * The two EVTCHR slots: each owns a 256x200 texture page (image_rect) for the
 * pictures that do not fit a unit's spritesheet slot, and a buffer to build
 * them in. A page shows a unit's attack image (decompressed on demand by
 * battle_gfx_load_unit_into_evtchr_slot), a unit's SP2 file, or the image of
 * an EVTCHR.BIN block: the event instruction LoadEVTCHR unpacks a block into
 * seq, shp, cluts and image_data and SaveEVTCHR uploads the image. Every
 * target access uses a 0x7564-byte stride.
 */
typedef struct battle_gfx_vram_slot {
    u32 owner;                   /* 0x0000; misc id with owner flags, or battle_gfx_vram_slot_owner_marker_e marker */
    u32 image_buffer_state;      /* 0x0004; upload countdown or battle_gfx_vram_slot_buffer_marker_e marker */
    u32 evtchr_load_state;       /* 0x0008; set to 0 free, 1 a unit's attack image, 3 an EVTCHR block; never read */
    RECT image_rect;             /* 0x000c */
    battle_gfx_unit_seq_t seq;   /* 0x0014 */
    battle_gfx_evtchr_shp_t shp; /* 0x051c */
    clut_t cluts;                /* 0x0f64; filled by battle_gfx_set_evtchr_palette_by_misc_id */
    u8 image_data[0x6400];       /* 0x1164 */
} battle_gfx_vram_slot_t;
typedef char battle_vram_slot_size_must_be_0x7564[(sizeof(battle_gfx_vram_slot_t) == 0x7564) ? 1 : -1];

/* Per-slot summary written by battle_gfx_get_vram_slot_summary. */
typedef struct battle_gfx_vram_slot_summary {
    u8 spritesheet_id; /* 0xff free, 0xfe saved, 0xfd no unit, else owner's sheet */
    u8 evtchr_state;   /* 0xff none, 0xfe saved, 0xfd loaded */
} battle_gfx_vram_slot_summary_t;

typedef enum battle_gfx_vram_slot_owner_marker {
    BATTLE_GFX_VRAM_SLOT_OWNER_EVTCHR = 0xfe, /* an EVTCHR image (battle_gfx_save_evtchr_slot) */
    BATTLE_GFX_VRAM_SLOT_OWNER_NONE = 0xff,
} battle_gfx_vram_slot_owner_marker_e;

/* Otherwise owner is a misc id combined with one of these image-kind bits. */
typedef enum battle_gfx_vram_slot_owner_flags {
    BATTLE_GFX_VRAM_SLOT_OWNER_SP2 = 0x40,
    BATTLE_GFX_VRAM_SLOT_OWNER_ATTACK_IMAGE = 0x80,
} battle_gfx_vram_slot_owner_flags_e;

enum { BATTLE_GFX_VRAM_SLOT_OWNER_ID_MASK = 0x1f };

typedef enum battle_gfx_vram_slot_buffer_marker {
    BATTLE_GFX_VRAM_SLOT_BUFFER_FREE = 0,
    BATTLE_GFX_VRAM_SLOT_BUFFER_EVTCHR = 0xfe, /* an unpacked EVTCHR block, not yet saved to VRAM or cleared */
} battle_gfx_vram_slot_buffer_marker_e;

/* Other positive buffer values are upload countdowns decremented each frame. */
enum { BATTLE_GFX_VRAM_SLOT_UPLOAD_FRAMES = 2 };

/* 0x32d6-byte spritesheet VRAM slot records at 0x800c7ce8 (nine slots);
 * g_battle_gfx_spritesheet_ids_by_vram_slot aliases the first record's byte 1. */
typedef struct battle_gfx_spritesheet_slot {
    u8 in_use;                          /* 0x00; cleared when no live unit uses the slot */
    u8 spritesheet_id;                  /* 0x01 */
    u8 compressed_attack_image[0x30d4]; /* 0x02; image data copied from SPR data +0x9200 by 0x80087704 */
    clut_t palettes;                    /* 0x30d6; 16 CLUTs with the STP bit set (0x80087704) */
} battle_gfx_spritesheet_slot_t;
typedef char battle_spritesheet_slot_size_must_be_0x32d6[(sizeof(battle_gfx_spritesheet_slot_t) == 0x32d6) ? 1 : -1];

/* Four-byte lookup record for the 0x9f ordinary unit spritesheets. */
typedef struct battle_gfx_spritesheet_data {
    u8 shp_id;         /* battle_shp_type_e */
    u8 seq_id;         /* battle_seq_type_e */
    u8 flying_flag;    /* nonzero: the sheet idles with the BATTLE_UNIT_ANIMATION_IDLE_FLYING animations */
    u8 graphic_height; /* height of the standing sprite in pixels */
} battle_gfx_spritesheet_data_t;
typedef char battle_gfx_spritesheet_data_size_must_be_4[(sizeof(battle_gfx_spritesheet_data_t) == 4) ? 1 : -1];

/* battle_gfx_spritesheet_data_t.shp_id: index of the sheet's frame table file in
 * g_battle_gfx_sprite_shp_files (BATTLE/TYPE1.SHP ...). Id 5 has no file. */
typedef enum battle_shp_type {
    BATTLE_SHP_TYPE1 = 0,
    BATTLE_SHP_TYPE2 = 1,
    BATTLE_SHP_CYOKO = 2,
    BATTLE_SHP_MON = 3,
    BATTLE_SHP_OTHER = 4,
    BATTLE_SHP_ARUTE = 6,
    BATTLE_SHP_KANZEN = 7,
} battle_shp_type_e;

/* battle_gfx_spritesheet_data_t.seq_id: index of the sheet's animation file in
 * g_battle_gfx_sprite_seq_files (BATTLE/TYPE1.SEQ ...). */
typedef enum battle_seq_type {
    BATTLE_SEQ_TYPE1 = 0,
    BATTLE_SEQ_TYPE3 = 1,
    BATTLE_SEQ_CYOKO = 2,
    BATTLE_SEQ_MON = 3,
    BATTLE_SEQ_OTHER = 4,
    BATTLE_SEQ_RUKA = 5,
    BATTLE_SEQ_ARUTE = 6,
    BATTLE_SEQ_KANZEN = 7,
} battle_seq_type_e;

/* One of the two 0xee28-byte battle render buffers. The primitive pools are
 * pre-initialised once here; the swap helper at 0x8005a0cc reads the ordering
 * table pointer at +0xe5b4. */
typedef struct battle_render_buffer {
    POLY_GT3 gt3[360]; /* 0x00000 */
    POLY_GT4 gt4[710]; /* 0x03840 */
    POLY_F3 f3[64];    /* 0x0c878 */
    POLY_F4 f4[256];   /* 0x0cd78 */
    POLY_G4 overlay;   /* 0x0e578 */
    DR_MODE overlay_modes[2] /* 0x0e59c */;
    u32* otag;             /* 0x0e5b4 */
    u8 _unused_e5b8[0x70]; /* 0x0e5b8 */
    POLY_FT3 ft3[64];      /* 0x0e628 */
} battle_render_buffer_t;
typedef char battle_render_buffer_size_must_be_0xee28[(sizeof(battle_render_buffer_t) == 0xee28) ? 1 : -1];

/* g_battle_gfx_unit_graphics_load_state.phase: the phase of the file being loaded. */
typedef enum battle_gfx_unit_load_phase {
    BATTLE_GFX_UNIT_LOAD_PHASE_START_READ = 0,
    BATTLE_GFX_UNIT_LOAD_PHASE_WAIT = 1,
    BATTLE_GFX_UNIT_LOAD_PHASE_INSTALL = 2,
    BATTLE_GFX_UNIT_LOAD_PHASE_FINISH = 3,
} battle_gfx_unit_load_phase_e;

/* g_battle_gfx_unit_graphics_load_state.file_kind, the file kind, counts through one of two lists:
 * battle_gfx_step_queued_unit_graphics_load loads three files per queued unit and
 * battle_gfx_step_load_unit_shp_seq_data the shared sprite, weapon and effect files. */
typedef enum battle_gfx_queued_load_kind {
    BATTLE_GFX_QUEUED_LOAD_SHP = 0,
    BATTLE_GFX_QUEUED_LOAD_SEQ = 1,
    BATTLE_GFX_QUEUED_LOAD_SPR = 2,
} battle_gfx_queued_load_kind_e;

typedef enum battle_gfx_shared_load_kind {
    BATTLE_GFX_SHARED_LOAD_SPRITE_SHP = 0,
    BATTLE_GFX_SHARED_LOAD_SPRITE_SEQ = 1,
    BATTLE_GFX_SHARED_LOAD_WEAPON_SHP = 2,
    BATTLE_GFX_SHARED_LOAD_WEAPON_SEQ = 3,
    BATTLE_GFX_SHARED_LOAD_EFFECT_SHP = 4,
    BATTLE_GFX_SHARED_LOAD_EFFECT_SEQ = 5,
    BATTLE_GFX_SHARED_LOAD_WEAPON_SPR = 6,
    BATTLE_GFX_SHARED_LOAD_OTHER_SPR = 7,
} battle_gfx_shared_load_kind_e;

/* Both load-kind lists start at zero. */
enum { BATTLE_GFX_UNIT_LOAD_KIND_FIRST = 0 };

/* Shared by the resident-file and queued-unit loaders. The file kind selects
 * the buffer's format, and index selects a file or a queued descriptor. */
typedef struct battle_gfx_unit_graphics_load_state {
    s32 file_kind; /* 0x00; battle_gfx_shared_load_kind_e or battle_gfx_queued_load_kind_e */
    s32 phase;     /* 0x04; battle_gfx_unit_load_phase_e */
    s32 index;     /* 0x08 */
    void* buffer;  /* 0x0c; allocated until the FINISH phase */
} battle_gfx_unit_graphics_load_state_t;
typedef char
    battle_gfx_unit_graphics_load_state_size_must_be_16[sizeof(battle_gfx_unit_graphics_load_state_t) == 16 ? 1 : -1];

extern u16 g_battle_gfx_screen_color_modulation_fade_frames;
extern palette_t g_battle_gfx_wait_direction_arrow_palette;
extern palette_t g_battle_gfx_map_selection_cursor_palette;
extern palette_t g_battle_gfx_shadow_palette;
extern palette_t g_battle_gfx_status_bubble_palette;
extern s32 g_battle_gfx_status_bubble_delay;
extern rgb24_t g_battle_gfx_screen_color_modulation_target;
extern u16 g_battle_gfx_screen_color_modulation_value[3];
extern u16 g_battle_gfx_screen_color_modulation_step[3];
extern RECT g_battle_gfx_screen_modulation_tex_window;

extern battle_gfx_unit_graphics_load_state_t g_battle_gfx_unit_graphics_load_state;
extern palette_t g_battle_gfx_cycled_deployed_palettes[2];
extern u8 g_battle_gfx_unit_animation_data[];
extern clut_t g_battle_gfx_item_palettes; /* the first 0x200 bytes of WEP.SPR */
extern clut_t g_battle_gfx_eff_palettes;
extern palette_t g_battle_gfx_frog_palettes[5];
extern palette_t g_battle_gfx_crystal_palettes[6];
extern palette_t g_battle_gfx_treasure_palettes[5];
extern u8 g_battle_gfx_shp_frame_data_buffer[];
extern POLY_F4 g_battle_gfx_screen_modulation_polygons[2];
extern s32 g_battle_gfx_tpage7_free_rect_pixels[4];
extern s32 g_battle_gfx_tpage7_free_rect_cells[4];
extern MATRIX g_battle_gfx_numeric_display_matrix;
extern SVECTOR g_battle_gte_rtps_vxy0;
extern SVECTOR g_battle_gte_rtpt_vxy0;
extern SVECTOR g_battle_gte_rtpt_vxy1;
extern SVECTOR g_battle_gte_rtpt_vxy2;
extern u32 g_main_gfx_battle_otags[2][0x180];
extern battle_file_extent_t g_battle_gfx_sprite_seq_files[];
extern battle_file_extent_t g_battle_gfx_sprite_shp_files[];
extern battle_file_extent_t g_battle_gfx_weapon_seq_files[];
extern battle_file_extent_t g_battle_gfx_weapon_shp_files[];
extern battle_file_extent_t g_battle_gfx_effect_seq_files[];
extern battle_file_extent_t g_battle_gfx_effect_shp_files[];
extern battle_file_extent_t g_battle_gfx_spritesheet_files[];
extern battle_file_extent_t g_battle_gfx_sp2_files[];
extern battle_file_extent_t g_battle_gfx_worker8_sp2_files[];
/* Sprite part width/height in 8-pixel tiles, indexed by SHP size bits. */
extern size32_t g_battle_gfx_part_sizes[];
extern battle_gfx_wep_eff_graphics_t g_battle_gfx_effect_graphics[1];
extern u8* g_battle_gfx_compressed_data;
extern s32 g_battle_gfx_compressed_nibble_phase;
extern s32 g_battle_gfx_compressed_offset;
extern rgb24_t g_battle_gfx_screen_color_modulation;
extern rgb24_t g_battle_gfx_screen_color_modulation_backup;
extern battle_gfx_native_unit_seq_table_t g_battle_gfx_unit_seq_tables[5];
extern battle_gfx_native_unit_seq_table_t g_battle_gfx_boss_seq_tables;
extern battle_gfx_spritesheet_data_t g_battle_gfx_spritesheet_data[BATTLE_SPRITESHEET_ID_COUNT];
extern battle_gfx_unit_shp_frame_tables_t g_battle_gfx_boss_shp_frame_tables;
extern u8 g_battle_gfx_spritesheet_ids_by_vram_slot[];
extern battle_gfx_unit_shp_frame_tables_t g_battle_gfx_unit_shp_frame_tables[5];
extern battle_gfx_wep_eff_graphics_t g_battle_gfx_weapon_graphics[2];
extern battle_gfx_spritesheet_slot_t g_battle_gfx_spritesheet_slots[];
extern u8 g_battle_gfx_tpage7_vram_allocation_grid[15][16];
extern battle_gfx_vram_slot_t g_battle_gfx_vram_slots[];
extern battle_render_buffer_t g_battle_render_buffers[2];

/* The buffer being built this frame; main_gfx_swap_and_clear_otag alternates
 * it between the two g_battle_render_buffers entries. */
extern battle_render_buffer_t* g_battle_gfx_current_render_buffer;
/* The 24 orders in which battle_gfx_draw_unit_sprite_layers queues a unit's
 * four layers: 0 is the unit's own sprite, n its sprite block n - 1. */
extern s32 g_battle_gfx_animation_layer_priorities[][4];

/* Where a weapon or item graphic sits in WEP.SPR, per item graphic id. */
typedef struct battle_gfx_item_graphic {
    u8 palettes;    /* 0x00; g_battle_gfx_item_palettes rows: high nibble for MAP_CLUT_ROW_ITEM_A, low for _B */
    u8 texture_row; /* 0x01; top of the graphic on its texture page, in steps of 8 pixels */
} battle_gfx_item_graphic_t;

extern battle_gfx_other_palettes_t g_battle_gfx_other_palettes;
extern s16 g_battle_gfx_counter;
extern u8 g_battle_gfx_fallback_seq_data[];
extern u8 g_battle_gfx_fallback_shp_frame_data[];
extern battle_gfx_item_graphic_t g_battle_gfx_item_graphic_data[0x92];
extern s32 g_battle_gfx_last_loaded_seq_id;
extern s32 g_battle_gfx_last_loaded_shp_id;

/* Byte-granular bump-allocation cursors: the consuming files do raw byte
 * arithmetic on these, so the element type must stay u8. */
extern u8* g_battle_gfx_seq_data_cursor;
extern u8* g_battle_gfx_shp_frame_data_cursor;
extern s16 g_battle_gfx_previous_counter;
extern battle_gfx_fade_overlay_t g_battle_gfx_screen_fade_overlays[]; /* one fade overlay per packet buffer */

/* Overlay draw-mode primitives, one per screen polarity. */
extern DR_MODE g_battle_gfx_screen_modulation_draw_modes[2];

/* Frame counter and frame index of the selection cursor's bob animation. */
extern s32 g_battle_gfx_selection_cursor_bob_timer;
extern s32 g_battle_gfx_selection_cursor_bob_frame;
extern u32* g_battle_gfx_sp2_data;
/* The display rotations that the upper five bits of a frame's first byte select, ONE to the turn. */
extern u16 g_battle_gfx_sprite_y_rotations[];

/* Status flag shown by each status bubble id (0x16 words at 0x80093cb4).
 * Bubbles 0 (KO), 0x14 (Death Sentence) and 0x15 (casting) hold 0 and are
 * tested separately by battle_gfx_determine_status_bubble_parameters. */
extern s32 g_battle_gfx_status_bubble_status_masks[];
/* By battle_shp_type_e: the frame a unit is left on when its sequence ends
 * or its attack image is taken away, seen from behind and from the front. */
extern u8 g_battle_gfx_targeted_frame_back[];
extern u8 g_battle_gfx_targeted_frame_front[];

/* Advanced by g_main_system_animation_speed once per call and sampled with bit 5 to blink
 * the highlighted arrow. */
extern u32 g_battle_gfx_wait_arrow_blink_timer;

/* Separate triggers, command arguments, and completion status. Argument
 * domains remain provisional: map commands 0x80/0x83, polls 0x81/0x84.
 * The u16 requests and arguments are read with lhu by the consumer
 * battle_script_process_pending_requests; every other access is a store. */
extern u16 g_battle_3d_object_use_request; /* 0x80165fe2 */
extern u16 g_battle_3d_object_wait_status; /* 0x8016606e */
extern battle_render_buffer_t g_battle_buffer_a[2];
void battle_display_menu_number_entry(s32 value, u8 digit_count, u8* image, glyph_blit_t* origin);
s32 battle_gfx_get_vram_slot_summary(battle_gfx_vram_slot_summary_t* summary);
void battle_gfx_draw_status_bubble(battle_unit_misc_data_t* unit, const DVECTOR* unused_screen_coords);
void battle_gfx_unpack_unit_seq_data(
    battle_gfx_native_unit_seq_table_t* destination, const battle_gfx_packed_seq_record_t* source);
void battle_gfx_run_unit_seq_script(battle_unit_misc_data_t* unit, battle_unit_anim_state_t* state, s32 advance);

void battle_gfx_run_wep_eff_seq_script(
    battle_unit_misc_data_t* unit, battle_unit_sprite_block_t* sprite, s32 index, s32 advance);

void battle_gfx_build_cursor_tile_glow(
    s32 palette, s32 shape, SVECTOR* v0, SVECTOR* v1, SVECTOR* v2, SVECTOR* v3, u32* ot);

void battle_gfx_draw_map_selection_cursor(SVECTOR* base, u32* ot);
void battle_gfx_draw_wait_direction_arrows(SVECTOR* vector, u32* ot);
s32 battle_gfx_build_next_action_result_display(battle_unit_misc_data_t* unit);
void battle_gfx_determine_status_bubble_parameters(battle_unit_misc_data_t*);
s32 battle_gfx_claim_spritesheet_slot(s16 spritesheet_id);
void battle_gfx_update_and_animate_unit_wep_eff(battle_unit_misc_data_t* unit);
void battle_gfx_draw_unit_sprite_layers(battle_unit_misc_data_t* unit);
void battle_menu_init_panel_fade_tiles(void);
void battle_gfx_prepare_post_action_display(battle_unit_misc_data_t* unit);
void battle_gfx_set_thrown_item_graphic_palette(s32 item_id, battle_unit_misc_data_t* unit);
void battle_gfx_update_all_unit_rotation_and_vectors(void);
void battle_gfx_tint_unit_palette_for_weather_and_tile(battle_unit_misc_data_t* unit, s32 level, s32 final_value);
void battle_gfx_animate_post_action_text(battle_unit_misc_data_t* unit, const SVECTOR* screen_coords);
void battle_gfx_apply_status_spritesheet_change(battle_unit_misc_data_t* unit, s32 tile_effect_level);
s32 battle_gfx_build_next_special_action_result_display(battle_unit_misc_data_t* unit);
void battle_gfx_calculate_sprite_shadow_from_tile_slope(battle_unit_misc_data_t* unit);
void battle_gfx_draw_unit_shadow(battle_unit_misc_data_t* unit, u32* ot);
void battle_gfx_draw_unit_sprite_position_copies(battle_unit_misc_data_t* unit);
u32 battle_gfx_get_unit_spritesheet_height(battle_unit_misc_data_t* unit);
void battle_gfx_init_item_get_rendering(struct battle_unit_misc_data* unit);
battle_gfx_sprite_display_data_t* battle_gfx_init_item_sprite_display(s32 unit_index);

battle_gfx_sprite_display_data_t* battle_gfx_init_numeric_sprite_display(s32 unit_index, s32 display_section);

void battle_gfx_init_status_bubble(battle_unit_misc_data_t* unit);
battle_gfx_sprite_display_data_t* battle_gfx_init_status_bubble_sprite_display(s32 unit_index);
battle_gfx_sprite_display_data_t* battle_gfx_init_weapon_sprite_display(s32 unit_index, s32 display_type);

void battle_gfx_load_trap_and_unit_frame_parts(
    battle_unit_misc_data_t* unit, battle_unit_anim_state_t* state, u16 frame, u16 sequence_id);

void battle_gfx_load_unit_frame_parts(battle_unit_misc_data_t* unit, battle_gfx_sprite_display_data_t* display,
    battle_gfx_source_frame_t* src, s32 tpage);

void battle_gfx_load_unit_into_evtchr_slot(battle_unit_misc_data_t* unit, s32 slot, s32 image_slot);

void battle_gfx_modify_misc_unit_palette(battle_unit_misc_data_t* misc, s32 mode, s32 clut_row, s32 clut_index,
    s32 preset, s16 red, s16 green, s16 blue, s32 final_value);

s32 battle_gfx_open_sp2_file(battle_unit_misc_data_t* unit, u32* destination);

void battle_gfx_select_unit_shp_frame(battle_unit_misc_data_t* unit, battle_unit_anim_state_t* state, u16 frame,
    u16 sequence_id, battle_gfx_unit_shp_frame_tables_t** out_table, battle_gfx_source_frame_t** out_frame);

void battle_gfx_set_sprite_part_semi_transparent(
    battle_gfx_sprite_display_data_t* display, s32 part_index, s32 enabled);
void battle_gfx_shift_and_fill_position_copies(struct battle_unit_misc_data* unit);
void battle_gfx_store_item_display_data(struct battle_unit_misc_data* unit, u32 item_id);
void battle_gfx_invalidate_sp2_vram_slot(battle_unit_misc_data_t* unit);
void battle_gfx_unpack_unit_shp_data(
    battle_gfx_unit_shp_frame_tables_t* dst, const battle_gfx_packed_unit_shp_record_t* src);
void battle_gfx_update_misc_unit_status_palette(battle_unit_misc_data_t* misc, s32 tile_effect_level, s32 final_value);
s32 battle_gfx_prepare_post_action_display_by_misc_id(s32 misc_id);
s32 battle_gfx_calculate_screen_z_from_misc_battle_map_data(struct battle_unit_misc_data* unit);
s32 battle_gfx_calculate_screen_z_from_misc_move_data(struct battle_unit_misc_data* unit);

/* Returns (s16), but an s16 prototype makes callers such as
 * battle_move_interpolate_height_toward_resting_height re-extend the result; keep s32. */
s32 battle_gfx_calculate_screen_z_from_misc_screen_data(struct battle_unit_misc_data* unit);

s32 battle_gfx_calculate_screen_z_with_caller_data(struct battle_unit_misc_data* unit, const SVECTOR* coords);

void battle_gfx_init_render_buffers(void);
void battle_gfx_append_gpu_primitive_to_secondary_otag(u32* primitive);

void battle_gfx_append_unit_graphics_load_descriptor(s32 map_x, s32 map_y, s32 map_level, s16 facing,
    s16 spritesheet_id, s16 palette_id, s16 misc_id, battle_stats_t* unit, u32 flags);

s32 battle_gfx_calculate_screen_z_from_misc_map_data(struct battle_unit_misc_data* unit);

void battle_gfx_check_tile_status_palette_mod(
    struct battle_unit_misc_data* unit, s32 tile_effect_level, s32 final_value);

void battle_gfx_clear_unit_graphics_load_state(s32 unused_map_id);
s32 battle_gfx_clear_shadow_graphic_trigger_by_misc_id(u32 misc_id);
void battle_gfx_clear_tpage7_vram_allocation_grid(void);
void battle_gfx_configure_misc_unit_palette_modulation_1f(s32 unit_id);

void battle_gfx_construct_polygon_data_for_units(battle_gfx_sprite_display_data_t* display, s32 end, s32 start,
    SVECTOR* position, s16 angle, s16 mode, SVECTOR* scale, u32* ot);

void battle_gfx_decompress_attack_spritesheet(u8* source, u8* destination);
void battle_gfx_draw_or_append_gpu_primitive(void* primitive);
void battle_gfx_draw_screen_color_modulation_overlay(void);
void battle_gfx_draw_wait_direction_unit(void);
void battle_gfx_free_tpage7_vram(RECT* rect);
u32 battle_gfx_get_spritesheet_flying_flag(u32 spritesheet_id);
battle_gfx_unit_seq_table_t* battle_gfx_get_unit_seq_by_spritesheet_id(u32 spritesheet_id);
battle_gfx_unit_shp_frame_tables_t* battle_gfx_get_unit_shp_by_spritesheet_id(u32 spritesheet_id);
s32 battle_gfx_get_spritesheet_vram_by_battle_id(u32 battle_id);
void battle_gfx_tint_all_units_by_team(void);
void battle_gfx_init_default_poly_ft4(POLY_FT4* primitive);
void battle_gfx_init_earned_exp_jp_display(struct battle_unit_misc_data* unit);
s32 battle_gfx_init_evtchr_vram_slots(void);
void battle_gfx_init_misc_unit_palette_modulation(s32 misc_id, s32 mode, s16 red, s16 green, s16 blue);
void battle_gfx_init_position_vector_copies(struct battle_unit_misc_data* unit);
void battle_gfx_init_render_frame(void);
void battle_gfx_init_render_state(void);
void battle_gfx_load_casting_unit_graphics(void);
void battle_gfx_load_spritesheet_into_vram_slot(const battle_gfx_unit_sprite_file_t* data, u32 index, s32 unused,
    struct battle_unit_misc_data* unit, s16 unused_palette);
s32 battle_gfx_load_unit_graphics_by_battle_id(s32, s32);
void battle_gfx_modify_palette_based_on_team(struct battle_unit_misc_data* unit, s32 amount);
u32 battle_gfx_read_compressed_nibble(void);
void battle_gfx_reset_jumping_unit_graphic_triggers(void);
s32 battle_gfx_reset_unit_graphic_trigger(u32 misc_id);
void battle_gfx_set_evtchr_palette_by_misc_id(s32 misc_id, s32 slot_index, s32 clut_index);
s32 battle_gfx_set_shadow_graphic_trigger_by_misc_id(u32 misc_id);
void battle_gfx_set_thrown_item_palette_by_misc_id(u32 item_id, u32 misc_id);
void battle_gfx_submit_screen_fade_overlay(battle_gfx_fade_overlay_t* overlay, s32 level);
void battle_gfx_update_screen_color_modulation_fade(void);
void battle_gfx_update_sprite_transparency_flag(struct battle_unit_misc_data* unit);
void battle_gfx_update_status_bubble_display(struct battle_unit_misc_data* unit);
void battle_gfx_update_status_bubble_graphic_trigger(struct battle_unit_misc_data* unit);
void battle_gfx_update_status_bubbles_and_graphics(void);
void battle_gfx_update_unit_palette_animation(void);
void battle_gfx_update_unit_palettes(void);
void battle_gfx_alloc_tpage7_vram(RECT* rect, world_texture_prim_t* prim, u32* image);
battle_gfx_source_frame_t* battle_gfx_get_unit_sprite_frame_and_vram_ids(u32 misc_id, battle_unit_sprite_query_t* out);
void battle_gfx_apply_weather_time_tile_palette_mod_by_misc_id(u32 misc_id);
s32 battle_gfx_clear_loaded_evtchr_slot(s32 slot_index);
s32 battle_gfx_clear_saved_evtchr_slot(s32 slot_index);
void battle_gfx_configure_misc_unit_palette_modulation(s32 misc_id, s32 mode, s16 red, s16 green, s16 blue);
void battle_gfx_extract_deployed_unit_palettes(void);
void battle_gfx_flip_poly_ft4_uv_if_negative(POLY_FT4* primitive, s32 direction);
void battle_gfx_copy_shp_data_to_ram(battle_gfx_weapon_shp_t* dst, const battle_gfx_packed_weapon_shp_record_t* src);
void battle_gfx_resolve_packed_offset_table(u8** entries, const battle_gfx_packed_offset_table_t* record);
u8** battle_gfx_get_effect_seq_data(u32 unused_spritesheet_id);
battle_gfx_weapon_shp_t* battle_gfx_get_effect_shp_data(u32 unused_spritesheet_id);
u8** battle_gfx_get_weapon_seq_by_spritesheet_id(u32 spritesheet_id);
battle_gfx_weapon_shp_t* battle_gfx_get_weapon_shp_by_spritesheet_id(u32 spritesheet_id);
s32 battle_gfx_get_unit_spritesheet_height_by_misc_id(u32 misc_id);
s32 battle_gfx_get_unit_spritesheet_height_by_unit_id(u32 unit_id);
void battle_gfx_highlight_all_units_blue_or_red(s32 mode);
s16 battle_gfx_increment_counter(void);
void battle_gfx_init_deployment_and_reset_unit_graphics_state(void);
battle_gfx_sprite_display_data_t* battle_gfx_init_unit_sprite_display(s32 unit_index);
void battle_gfx_set_sprite_part_flip(battle_gfx_sprite_display_data_t* display, s32 part_index, u32 flags);
s32 battle_gfx_is_spritesheet_id_loaded(s16 spritesheet_id);
s32 battle_gfx_load_misc_unit_into_evtchr_slot(s32 misc_id, s32 slot);

void battle_gfx_modify_vram_palette(s32 mode, const rgba5551_t* source, s32 clut_row, s32 clut_index, s32 preset,
    s16 red, s16 green, s16 blue, s32 final_value);

s32 battle_gfx_release_reserved_vram_slot(s32 slot_index);
void battle_gfx_release_unit_vram_slot(struct battle_unit_misc_data* unit);
void battle_gfx_reset_unit_graphics_load_state(void);
s32 battle_gfx_save_evtchr_slot(s32 slot_index);
void battle_gfx_set_draw_mode_for_texture_page(DR_MODE* mode, s32 page);
void battle_gfx_set_draw_mode_from_origin(DR_MODE* mode, const point16_t* origin);
void battle_gfx_set_primitive_brightness(P_TAG* primitive, s32 brightness);
void battle_gfx_start_screen_color_modulation_fade(u16 blend_mode, u16 red, u16 green, u16 blue, u16 duration);
s32 battle_gfx_step_load_unit_shp_seq_data(void);

void battle_gfx_release_unit_sprite_display_stub(battle_gfx_sprite_display_data_t* unused_display);

void battle_gfx_store_sprite_display_data(battle_gfx_sprite_display_data_t* display, s32 index, s16 x_shift,
    s16 y_shift, s16 u, s16 v, s16 width, s16 height, s16 flags);

s32 battle_gfx_unpack_evtchr_file_to_vram_slot(s32 index, const battle_gfx_packed_evtchr_record_t* file);

void battle_gfx_init_image_loading(world_texture_prim_t* primitive, const point16_t* base_load, const RECT* base_screen,
    const world_gfx_image_load_parameters_t* parameters);

void battle_gfx_copy_other_spr_palette_data_to_ram(const battle_gfx_other_sprite_archive_t* archive);
void battle_gfx_load_wep_spr_data(battle_gfx_weapon_sprite_archive_t* archive);

void battle_gfx_apply_misc_unit_palette_modulation(
    s32 preset_color, s32 time, s32 misc_id, s16 red, s16 green, s16 blue);

void battle_gfx_start_misc_unit_palette_modulation(
    s32 preset_color, s32 time, s32 misc_id, s16 red, s16 green, s16 blue);

void battle_gfx_load_evtchr_thread(void);

/* 0x8008d104..0x8008d138: drains immediate loader steps (2), returns
 * 0 when finished or 1 while work remains pending (including allocation retry). */
s32 battle_gfx_poll_unit_graphics_load(void);

/* 0x80088904: 0 finished, 1 pending/deferred, 2 immediate progress. */
s32 battle_gfx_step_queued_unit_graphics_load(void);

/* action */
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
    BATTLE_ACTION_MISS_TYPE_HIT = 0x00,
    BATTLE_ACTION_MISS_TYPE_ACCESSORY_EVADE = 0x01,
    BATTLE_ACTION_MISS_TYPE_RIGHT_HAND_EVADE = 0x02,
    BATTLE_ACTION_MISS_TYPE_LEFT_HAND_EVADE = 0x03,
    BATTLE_ACTION_MISS_TYPE_CLASS_EVADE_OR_ARROW_GUARD = 0x04,
    BATTLE_ACTION_MISS_TYPE_NULLIFIED = 0x05,
    BATTLE_ACTION_MISS_TYPE_ACCURACY_MISS = 0x06,
    BATTLE_ACTION_MISS_TYPE_FORCED_FAILURE = 0x07,
    BATTLE_ACTION_MISS_TYPE_CANCELLED = 0x08,
    BATTLE_ACTION_MISS_TYPE_REFLECTED = 0x09,
    BATTLE_ACTION_MISS_TYPE_GOLEM_GUARD = 0x0a,
    BATTLE_ACTION_MISS_TYPE_BLADE_GRASP_OR_FINGER_GUARD = 0x0b,
    BATTLE_ACTION_MISS_TYPE_BUGGED_REFLECTION = 0x0c,
    BATTLE_ACTION_MISS_TYPE_CATCH = 0x0d,
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
    BATTLE_ACTION_SPECIAL_EFFECT_NULLIFIED = 0x0200,
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

/* Pending post-action displays are queued as one little-endian word in Misc
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
    BATTLE_ACTION_DISPLAY_IMAGE_DEAD = 0x00,
    BATTLE_ACTION_DISPLAY_IMAGE_UNDEAD = 0x01,
    BATTLE_ACTION_DISPLAY_IMAGE_PETRIFY = 0x02,
    BATTLE_ACTION_DISPLAY_IMAGE_INVITE = 0x03,
    BATTLE_ACTION_DISPLAY_IMAGE_DARKNESS = 0x04,
    BATTLE_ACTION_DISPLAY_IMAGE_CONFUSION = 0x05,
    BATTLE_ACTION_DISPLAY_IMAGE_SILENCE = 0x06,
    BATTLE_ACTION_DISPLAY_IMAGE_BLOOD_SUCK = 0x07,
    BATTLE_ACTION_DISPLAY_IMAGE_OIL = 0x08,
    BATTLE_ACTION_DISPLAY_IMAGE_FLOAT = 0x09,
    BATTLE_ACTION_DISPLAY_IMAGE_RERAISE = 0x0a,
    BATTLE_ACTION_DISPLAY_IMAGE_TRANSPARENT = 0x0b,
    BATTLE_ACTION_DISPLAY_IMAGE_BERSERK = 0x0c,
    BATTLE_ACTION_DISPLAY_IMAGE_POISON = 0x0d,
    BATTLE_ACTION_DISPLAY_IMAGE_REGEN = 0x0e,
    BATTLE_ACTION_DISPLAY_IMAGE_PROTECT = 0x0f,
    BATTLE_ACTION_DISPLAY_IMAGE_SHELL = 0x10,
    BATTLE_ACTION_DISPLAY_IMAGE_HASTE = 0x11,
    BATTLE_ACTION_DISPLAY_IMAGE_SLOW = 0x12,
    BATTLE_ACTION_DISPLAY_IMAGE_STOP = 0x13,
    BATTLE_ACTION_DISPLAY_IMAGE_FAITH = 0x14,
    BATTLE_ACTION_DISPLAY_IMAGE_INNOCENT = 0x15,
    BATTLE_ACTION_DISPLAY_IMAGE_CHARM = 0x16,
    BATTLE_ACTION_DISPLAY_IMAGE_SLEEP = 0x17,
    BATTLE_ACTION_DISPLAY_IMAGE_DONT_MOVE = 0x18,
    BATTLE_ACTION_DISPLAY_IMAGE_DONT_ACT = 0x19,
    BATTLE_ACTION_DISPLAY_IMAGE_REFLECT = 0x1a,
    BATTLE_ACTION_DISPLAY_IMAGE_DEATH_SENTENCE = 0x1b,
    BATTLE_ACTION_DISPLAY_IMAGE_STOLEN = 0x1c,
    BATTLE_ACTION_DISPLAY_IMAGE_BROKEN = 0x1d,
    BATTLE_ACTION_DISPLAY_IMAGE_QUICK = 0x24,
    BATTLE_ACTION_DISPLAY_IMAGE_HP = 0x25,
    BATTLE_ACTION_DISPLAY_IMAGE_MP = 0x26,
    BATTLE_ACTION_DISPLAY_IMAGE_CT = 0x27,
    BATTLE_ACTION_DISPLAY_IMAGE_SPEED = 0x28,
    BATTLE_ACTION_DISPLAY_IMAGE_BRAVE = 0x29,
    BATTLE_ACTION_DISPLAY_IMAGE_FAITH_STAT = 0x2a,
    BATTLE_ACTION_DISPLAY_IMAGE_PHYSICAL_ATTACK = 0x2b,
    BATTLE_ACTION_DISPLAY_IMAGE_MAGICAL_ATTACK = 0x2c,
    BATTLE_ACTION_DISPLAY_IMAGE_LEVEL = 0x2d,
    BATTLE_ACTION_DISPLAY_IMAGE_GIL = 0x2e,
    BATTLE_ACTION_DISPLAY_IMAGE_EXP = 0x2f,
    BATTLE_ACTION_DISPLAY_IMAGE_FROG = 0x30,
    BATTLE_ACTION_DISPLAY_IMAGE_NONE = 0xff,
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

/* Five-byte range/targeting work entry at 0x80192dd8, 256 entries per level.
 * Movement-cost propagation uses remaining_range as the residual budget;
 * the AI reads it at 0x801995f4 and 0x801996ac. */
typedef struct battle_target_panel {
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
} battle_target_panel_t;
typedef char battle_targeting_panel_size_must_be_5[(sizeof(battle_target_panel_t) == 5) ? 1 : -1];

/* Current ability data at 0x801938c0: the attacker/target pair, weapons, formula
 * inputs and results of the strike being resolved, with copies of the ability's
 * secondary data, the weapon's data and the status-infliction record that
 * battle_action_run_pre_formula_setup loads. */
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
typedef char battle_current_ability_size_must_be_0x50[(sizeof(battle_current_ability_t) == 0x50) ? 1 : -1];

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
 * battle_formula_calculate_knockback, with PENDING set until it is applied. */
typedef enum battle_knockback_kind {
    BATTLE_KNOCKBACK_FALL_DAMAGE = 0x01,    /* a grounded target: the fall height can hurt */
    BATTLE_KNOCKBACK_NO_FALL_DAMAGE = 0x02, /* a flying target */
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
 * battle_action_set_current_attacker_data and read by
 * battle_action_set_mimic_ability. The AI saves and restores it whole as 30
 * bytes (battle_ai_store_considered_action_data). */
typedef struct battle_current_attacker_data {
    u8 facing;                      /* 0x00 */
    u8 mimic_pending;               /* 0x01: a non-Mime action was recorded; state 13 queues the Mimes */
    u8 right_hand_weapon_id;        /* 0x02 */
    u8 left_hand_weapon_id;         /* 0x03 */
    u8 team;                        /* 0x04 */
    u8 _padding_05;                 /* 0x05; aligns target_delta_x */
    s16 target_delta_x;             /* 0x06 */
    s16 target_delta_y;             /* 0x08 */
    battle_action_command_t action; /* 0x0a: copy of battle_stats_t +0x16e */
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

/* battle_action_calculate_at_list modes. */
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

/* Directions of battle_target_calculate_linear_attack_tiles. */
typedef enum battle_target_line_direction {
    BATTLE_TARGET_LINE_EAST = 0,  /* +x */
    BATTLE_TARGET_LINE_WEST = 2,  /* -x */
    BATTLE_TARGET_LINE_SOUTH = 4, /* -y */
    BATTLE_TARGET_LINE_NORTH = 6, /* +y */
} battle_target_line_direction_e;

/* Result of battle_action_prepare_attack, kept in battle_unit_misc_data_t.attack_phase_state. */
typedef enum battle_attack_preparation {
    BATTLE_ATTACK_PREPARATION_INVALID = -1,
    BATTLE_ATTACK_PREPARATION_CHARGE = 0, /* the action has a charge time; also Defend */
    BATTLE_ATTACK_PREPARATION_IMMEDIATE = 1,
    BATTLE_ATTACK_PREPARATION_CHANGE_EQUIPMENT = 2,
    BATTLE_ATTACK_PREPARATION_JUMP = 3,
} battle_attack_preparation_e;

/* Result of battle_target_set_panels_for_action, kept in
 * battle_unit_misc_data_t.ability_preview_phase: how the action's target is chosen. */
typedef enum battle_target_panels {
    BATTLE_TARGET_PANELS_INVALID = -1,
    BATTLE_TARGET_PANELS_UNIT_OR_TILE = 0, /* the ability can follow a unit */
    BATTLE_TARGET_PANELS_TILE_ONLY = 1,    /* ABILITY_SECONDARY_FLAG_2_NO_FOLLOW_TARGET */
    BATTLE_TARGET_PANELS_NO_TARGETING = 2, /* nothing to choose: the action is centred on the acting unit */
    BATTLE_TARGET_PANELS_NONE = 3,         /* the panel routine marked no tile */
} battle_target_panels_e;

/* battle_target_set_tile_background_color modes and the g_battle_target_tile_mark_modes states:
 * one layer per map tile mark (move destination, ability range, targeted). A SHOW or HIDE
 * request recolours the current render buffer; battle_map_draw_mesh_and_weather then does the
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

/* g_battle_action_post_action_display_phase: the reports shown one after another once an action
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

extern s32 g_battle_action_turn_event_type;
extern s32 g_battle_action_at_list_active;
extern s32 g_battle_action_at_list_id;
extern battle_action_display_image_rect_t g_battle_action_display_image_rects[BATTLE_ACTION_DISPLAY_IMAGE_COUNT];
extern const u8 g_battle_action_display_numeric_selectors[16];
extern u16 g_battle_action_display_values[16];

/* Eight per status_infliction/status_removal byte, indexed from the byte's high bit. */
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
extern s16 g_battle_current_reaction_ability_id;
extern s32 g_battle_distribute_target_count;
extern s32 g_battle_unit_source_misc_id;
extern s32 g_battle_casting_unit_id;
extern u8 g_battle_target_ability_targets_list[16];
extern battle_target_panel_t g_battle_target_panel_data[0x200];
extern battle_current_ability_t g_battle_action_current_ability;
extern s32 g_battle_action_hamedo_flag;
extern s32 g_battle_script_loaded_event_id;
extern s32 g_battle_state_wait_direction_facing;
extern battle_action_command_t g_battle_action_saved_command;
extern battle_stats_t* g_battle_action_target;
extern battle_post_effect_msg_t g_battle_action_post_effect_msgs[];
extern u8 g_battle_action_menu_skillsets[];

/* Separate link name for g_battle_action_saved_command.ability_id. */
extern s16 g_battle_reaction_unit_last_ability_id;
extern battle_current_attacker_data_t g_battle_action_current_attacker;
extern battle_current_attacker_data_t g_battle_ai_current_attacker_backup;
extern battle_action_data_t g_battle_action_attacker_action_data;
extern battle_stats_t* g_battle_action_attacker;
extern u8 g_battle_action_menu_row_types[]; /* per-row skillset kind; 13 keeps the row active */
extern s32 g_battle_action_learn_on_hit_unit_id;
extern void (*g_battle_formula_handlers[])(void); /* by formula id less 1: formula 0 has no handler */
extern u16 g_battle_target_color_tile_x;
extern s16 g_battle_target_color_tile_y;
extern s16 g_battle_target_tile_mark_modes[3]; /* battle_tile_mark_mode_e, one per tile mark */
extern u8 g_battle_action_canceled_statuses[];

/* Second name of g_battle_target_panel_data, unsized: 512 map panels followed
 * by 16 auxiliary panels, all initialized at 0x80174e84. */
extern battle_target_panel_t g_battle_target_panels[];
extern SVECTOR g_battle_current_misc_screen_coords;
extern VECTOR g_battle_current_vector;
extern u32* g_battle_gfx_current_otag_entry;
extern battle_render_buffer_t* g_battle_target_tile_color_buffer;
extern void* g_battle_effect_current_work;
extern u16 g_battle_current_music_track_index; /* 0x80165fd8 */
extern entd_unit_t* g_main_entd_current_unit;
extern s32 g_battle_current_thread_id;
s32 battle_action_add_poached_item_to_fur_shop_inventory(void);
s32 battle_action_calculate_at_list(battle_at_entry_t* list, s32 mode);
u32 battle_action_get_number_of_turns_to_resolve(s32 unit_index, s32 ct, battle_at_entry_t* entries);
s32 battle_action_calculate_at_list_and_get_specific_unit_id(s32 unit_idx);
s32 battle_action_calculate_chance_to_react(const battle_stats_t* unit);
void battle_action_call_attack_preparation(battle_action_command_t* action);
void battle_action_store_used_weapon(battle_action_command_t* action);
s32 battle_action_can_unit_react(const battle_stats_t* unit);

/* Provisional: the definition falls off the end after the tail call to
 * battle_unit_get_action_restriction, so v0 carries that result; every caller reads
 * it as an s32 status. */
s32 battle_action_is_reaction_blocked(battle_stats_t* unit);
s32 battle_action_check_and_consume_mp(battle_stats_t* unit);
void battle_action_check_arrow_guard_usability(void);
void battle_action_check_between_turn_events(void);
void battle_action_check_blade_grasp_usability(void);
void battle_action_check_counter_reaction_usability(u16 reaction_id, u32 mask);
s32 battle_action_check_change_of_turn(s32 unit_id);
void battle_action_check_face_up_and_absorb_used_mp_usability(s16 reaction_id);
void battle_action_check_critical_quick_hp_restore_mp_restore_meatbone_slash_usability(s16 reaction_id);
void battle_action_check_damage_split_usability(void);
void battle_action_check_distribute_usability(void);
void battle_action_check_mp_switch_usability(void);
void battle_action_check_pa_save_ma_save_speed_save_regenerator_auto_potion_gilgame_heart_usability(s16 reaction_id);
s32 battle_action_check_reaction(battle_stats_t* unit);
void battle_action_check_reflect_reaction(void);
void battle_action_choose_facing_for_wait(void);
void battle_action_choose_wait(void);
void battle_action_clear_at_list_active(void);
void battle_action_clear_at_list_id(void);
void battle_action_open_status_menu_at_list_start(void);
void battle_action_clear_current_data(battle_action_data_t* action);
void battle_action_clear_data(void);
void battle_action_clear_knockback_flag(void);
void battle_action_clear_status_changes(battle_action_data_t* action);
void battle_action_confirm(void);
void battle_menu_copy_billboard_data_in(const battle_unit_status_record_t* banner, const battle_active_unit_t* unit,
    const battle_menu_status_panel_slot_storage_t* billboard);
s32 battle_action_end_turn(s32 unit_id);
void battle_action_execute_ability(void);
void battle_action_finalize_target_current_action(void);
void battle_action_get_next_acting_unit(s32 arg);
void battle_action_handle_move_command(void);
void battle_action_handle_post_action_xp_jp_ability(void);
void battle_action_increment_at_list_id(void);
s32 battle_action_increment_item_quantity_for_steal_break(battle_stats_t* unit, u8 item);
void battle_action_init_learn_ability_on_hit(void);
s32 battle_action_init_movement_ability_benefit(battle_stats_t* unit);
void battle_action_is_at_list_unit_charging_ability(void);
s32 battle_action_report_job_level_up(battle_unit_misc_data_t* misc);
s32 battle_action_report_level_up(battle_unit_misc_data_t* misc);
s32 battle_action_resume_attack_phase_control(void);
void battle_action_run_between_turn_events(void);
void battle_action_set_at_list_active(void);
void battle_action_set_casting_unit_id_ff(void);
void battle_action_set_current_attacker_data(battle_stats_t* unit);
void battle_action_set_damage_display_type_based_on_ability(void);
void battle_action_set_item_throw_stone_ability_display(void);
s32 battle_action_set_move_act_flags(s32 unit_id, s32 move_flag, s32 act_flag);
void battle_action_set_only_action_taken(s32 unit_id);
void battle_action_set_only_movement_taken(s32 unit_id);
void battle_action_set_target_variables(battle_stats_t* unit);
s32 battle_action_should_end_unit_turn(battle_stats_t* unit);
void battle_state_enter_move_range_exception(void);
void battle_action_show_caster_post_effect_messages(void);
void battle_action_switch_ability_to_default_attack(void);
void battle_formula_3c_damage_caster_max_hp_one_fifth_heal_target_two_fifths(void);
void battle_formula_43_damage_caster_missing_hp(void);
void battle_formula_add_brave_x_stats_y(void);
void battle_formula_apply_attack_up_and_martial_arts(void);
void battle_formula_apply_attacker_berserk_frog(void);
void battle_formula_apply_catch(void);
void battle_formula_apply_charge(void);
void battle_formula_apply_damage_and_knockback(void);
void battle_formula_apply_dance_abilities(void);
void battle_formula_apply_defense_up(void);
void battle_formula_apply_elemental(void);
void battle_formula_apply_elemental_absorption(void);
s32 battle_formula_apply_elemental_absorption_and_status(void);

/* Provisional: the definition falls off the end after
 * battle_formula_roll_conditional_status_proc, so v0 carries that result and
 * every caller tests it. */
s32 battle_formula_apply_elemental_absorption_and_status_proc(void);
void battle_formula_apply_elemental_strengthen(void);
void battle_formula_apply_evasion_changes_due_to_statuses(void);
void battle_formula_apply_full_hp_mp_heal(void);
void battle_formula_apply_hp_absorption(void);
void battle_formula_apply_magic_attack_up(void);
void battle_formula_apply_magic_defense_up(void);
void battle_formula_apply_magical_support_status_compatibility(void);
void battle_formula_apply_maintenance(void);
void battle_formula_apply_mp_recovery(void);
void battle_formula_apply_physical_attack_supports(void);
void battle_formula_apply_physical_status_support_compatibility(void);
void battle_formula_apply_physical_xa_modifying_statuses_support(void);
void battle_formula_apply_quick_effect(void);
void battle_formula_apply_song_abilities(void);
void battle_formula_apply_status(void);
void battle_formula_apply_status_to_action(void);
s32 battle_formula_apply_status_and_check_undead(void);
s32 battle_formula_apply_steal_break_might_sword_hardcoding(void);
void battle_formula_apply_target_magical_status_xa_modifiers(void);
void battle_formula_apply_target_xa_affecting_statuses(void);
void battle_formula_apply_undead_absorb_attack(void);
void battle_formula_apply_undead_reversal(void);
void battle_formula_apply_weapon_element_strengthen(void);
void battle_formula_apply_weather_effects_on_bows(void);
void battle_formula_apply_weather_elemental_effects(void);
void battle_formula_set_brave_loss_y(void);
void battle_formula_calculate_abandon(void);
s32 battle_formula_calculate_attack_evaded(u16 numerator, u16 denominator, u8 miss_type, u8 item_lost);
void battle_formula_calculate_base_xa(void);
void battle_formula_calculate_concentrate(void);
void battle_formula_calculate_critical_hit(void);
void battle_formula_calculate_dark_confuse(void);
void battle_formula_calculate_facing_evade(void);
void battle_formula_calculate_faith(void);
void battle_formula_calculate_final_hit_percent(void);
void battle_formula_calculate_gravi2_damage(void);
void battle_formula_calculate_hp_percent_damage(void);
void battle_formula_calculate_katana_break_chance(void);
s32 battle_formula_calculate_magic_accuracy_without_faith(void);
s32 battle_formula_calculate_magical_evade(void);
void battle_formula_calculate_magical_xa_times_ya(void);
void battle_formula_calculate_mp_percent_damage(void);
void battle_formula_calculate_physical_damage(void);
s32 battle_formula_calculate_physical_evade(void);
void battle_formula_calculate_stolen_gil(void);
void battle_formula_calculate_transparent(void);
void battle_formula_calculate_truth_damage(void);
void battle_formula_calculate_truth_magical_damage(void);
s32 battle_unit_get_action_restriction(battle_stats_t* unit);
s32 battle_formula_calculate_hit(void);

/* Provisional: falls off the end after battle_formula_calculate_hit, whose
 * result stays in v0 for the caller's test. */
s32 battle_formula_calculate_physical_evade_charge(void);
void battle_formula_modify_damage_for_element(s32 element);
void battle_formula_nullify_action(void);
void battle_formula_cause_action_miss(void);
void battle_formula_check_dragon(void);
void battle_formula_clear_nullify_flags(void);
void battle_formula_convert_hp_damage_to_mp_recovery(void);
void battle_formula_determine_reduced_stat(void);
void battle_formula_force_attack_miss(void);
void battle_formula_force_sleeping_target_miss(void);
s32 battle_formula_get_random_0_7fff(void);
void battle_formula_heal_mp_z_times_ten(void);
void battle_formula_init_weapon_xa_ya(void);
void battle_formula_modify_elemental_damage(void);
void battle_formula_roll_conditional_status_proc_inner(void);
s32 battle_formula_roll_conditional_status_proc(void);
s32 battle_formula_roll_random_at_least(s32 limit, s32 threshold);
void battle_formula_select_magic_gun_ability(void);
void battle_formula_set_exp_stolen(void);
void battle_formula_store_ma_and_ma_plus_y_divided_by_two(void);
void battle_formula_store_ma_and_pa_plus_y_divided_by_two(void);
void battle_formula_store_ma_and_x(void);
void battle_formula_store_ma_and_y(void);
void battle_formula_store_magical_evade_values(void);
void battle_formula_store_pa_and_pa_plus_y_divided_by_two(void);
void battle_formula_store_pa_and_weapon_power(void);
void battle_formula_store_pa_and_weapon_power_plus_y(void);
void battle_formula_store_pa_and_x(void);
void battle_formula_store_pa_and_y(void);
void battle_formula_store_reaction_proc_id_and_target(void);
void battle_formula_store_speed_and_x(void);
void battle_formula_store_xa_plus_ya_status_damage(void);
void battle_formula_store_xa_times_ya_damage(void);
void battle_formula_use_hp_damage_as_action_hit_percent(void);
s32 battle_target_set_panels_for_action(const battle_action_command_t* action);
s32 battle_action_build_reaction_targets(s32 actor_id, battle_strike_work_t* work, s32* target_count, u8* target_ids);
void battle_target_apply_unit_team_eligibility(s32 raw_unit_id, u8 allow_allies, u8 allow_enemies, u8 aoe_is_0xff);
void battle_target_calculate_aoe_vertical_tolerance(s32 x, s32 y, s32 lo, s32 hi);
void battle_target_calculate_arc_range(battle_stats_t* unit, u8 range, u8 flags);
void battle_target_calculate_linear_attack_tiles(s32 dir, s32 x, s32 y);
s32 battle_target_calculate_map_for_action(battle_action_command_t* action, u8* flags_3);
void battle_target_calculate_strike_lunge_range(battle_stats_t* unit, u8 flags);
void battle_target_calculate_tile_coords_and_glow_from_at_list(void);
void battle_target_calculate_tile_coords_with_cursor_glow(void);
void battle_target_calculate_weapon_range(battle_stats_t* unit);
void battle_target_can_select_tile(void);
void battle_target_clear_panel_data(void);
void battle_target_clear_panel_spread_flags(void);
void battle_target_disable_green_panel_flags(void);
s32 battle_target_disable_green_panel_on_all_but_target_tile(const battle_action_command_t* source);
void battle_target_gather_x_y_data_for_attacks(battle_unit_misc_data_t* unit);
s32 battle_target_get_unit_id_if_tile_targetable(s32 a0, s32 a1, s32 a2);
void battle_target_move_cursor_to_unit(battle_unit_misc_data_t* unit);
void battle_target_remove_close_range(s32 x, s32 y, s32 range);
void battle_target_select_random_tile_for_random_fire_abilities(void);
void battle_target_select_tile(void);
void battle_target_show_move_range_in_free_cursor(void);
void battle_target_sort_list(s32 target_battle_id);
void battle_target_spread_panels(u8 passes, s32 unused);
void battle_target_store_cursor_casting_unit_name_and_data(void);
void battle_target_store_cursor_unit_name_and_data(void);
s32 battle_target_set_boxes_red(void);
void battle_target_store_cursor_unit_as_preview_target(void);
void battle_target_update_free_cursor_selection(battle_unit_misc_data_t* unit, battle_unit_misc_data_t* selected_unit);

s32 battle_target_validate_height_overlap(
    s32 attacker_id, s32 target_x, s32 target_y, s32 target_elevation, s32 target_id);

void battle_action_queue_post_effect_messages_for_unit(battle_unit_misc_data_t* unit);
s32 battle_target_calculate_cursor_tile_polygon(SVECTOR* coords, u8 layer, SVECTOR* quad);
s32 battle_target_set_green_panels_for_action(battle_action_command_t* action);
s32 battle_target_is_unit_untargetable_and_store_tile(s32 unit_id, s32* tile_index);
s32 battle_action_prepare_attack(battle_action_command_t* action, battle_action_command_t* target, s32 flag);
s32 battle_action_resolve_ability_strike(s32 misc_unit_id, battle_strike_work_t* work);
void battle_action_init_current_ability_strike_data(battle_stats_t* unit);
s32 battle_reaction_prepare_next(s16* reaction_id);
s32 battle_action_run_between_turn_control(s32 mode);
void battle_formula_calculate_knockback(void);
void battle_formula_apply_zodiac_compatibility(void);
void battle_formula_store_physical_evade_values(void);
s32 battle_action_run_pre_formula_setup(const battle_action_command_t* source, u8 target_id);
s32 battle_action_finalize_attack_and_flag_reactions(s32 misc_id);
s32 battle_target_move_cursor_by_input(void);
void battle_target_set_tile_background_color(s32 mode, s32 unused);
void battle_action_set_target_coords_and_attacker_anim(void);
void battle_menu_copy_billboard_data_out(battle_unit_status_record_t* banner, battle_active_unit_t* unit,
    battle_menu_status_panel_slot_storage_t* billboard, map_selected_tile_t* cursor_tile);
s32 battle_action_add_war_funds(battle_stats_t* unit, s32 total, u8 item_id);
s32 battle_action_apply_tile_trap(battle_stats_t* unit);
void battle_action_build_targets_post_action_message(void);

/* Preview action calculation entry run on the main stack. */
s32 battle_action_calculate_projected_effect(
    battle_stats_t* attacker, battle_stats_t* target, battle_action_command_t* command);

s32 battle_action_call_attack_preparation_at_preview(battle_action_command_t* command);
s32 battle_action_check_at_list_for_unit_battle_id(battle_stats_t* unit);
s32 battle_action_check_battle_outcome(void);
void battle_action_check_mp_switch_distribute_and_damage_split_usability(void);
void battle_action_check_reflect_blade_grasp_and_arrow_guard(void);
void battle_action_check_stat_save_and_restore_reaction_usability(void);
s32 battle_action_decrement_player_item_quantity(battle_stats_t* unit, s32 item_id, s32 always_one);
void battle_action_dispatch_target_reaction_ability(void);
s32 battle_action_get_elemental_ability_id(battle_stats_t* unit);
void battle_action_finalize_draw_out_katana_result(battle_stats_t* attacker, battle_strike_work_t* work, u8 hit_count);
void battle_action_handle_steal_exp(battle_stats_t* unit, u8 amount);
s32 battle_action_perform_reaction_ability(void);
s32 battle_action_preview_at_list(battle_stats_t* unit, battle_action_command_t* action, battle_at_entry_t* at_list);
s32 battle_action_remove_broken_or_stolen_equipment(void);
void battle_action_apply_jp_change(battle_stats_t* unit);
s32 battle_action_select_auto_potion_item(battle_stats_t* unit);
void battle_action_set_casting_unit_id_ff_and_init(void);
s32 battle_action_set_mimic_ability(battle_stats_t* unit);
s32 battle_action_sort_at_list(s32 unit_id, s32 mode, s32 sort_key, battle_at_entry_t* list);
void battle_action_store_target_ids(u8* src);
s32 battle_action_store_counter_ability(battle_stats_t* unit, s8 skillset_id, s16 ability_id, s32 validate_target);
void battle_formula_apply_poach_and_train(void);
s32 battle_formula_calculate_dance_song_hit(void);
s32 battle_formula_calculate_elemental_xa_times_ya(void);
s32 battle_formula_calculate_friendly_magic_accuracy(void);
s32 battle_formula_calculate_golem_accuracy(void);
s32 battle_formula_calculate_magic_accuracy(void);
s32 battle_formula_calculate_magic_accuracy_no_elemental(void);
s32 battle_formula_calculate_physical_accuracy(void);
s32 battle_formula_calculate_physical_status_accuracy(void);
s32 battle_formula_calculate_weapon_damage(void);
s32 battle_reaction_prepare_hamedo_for_pending_action(s32 id, s16* out_ability);
s32 battle_target_apply_reflect(battle_stats_t* unit);
void battle_target_apply_vertical_fixed(s32 x, s32 y);
void battle_target_apply_vertical_tolerance(u8 ref_height, u8 tolerance, s32 single_layer);
s32 battle_target_calculate_ability_range_with_map_parameters(battle_stats_t* unit, u8 range);
s32 battle_target_calculate_for_menu_types(const battle_action_command_t* source);
s32 battle_target_count_hit_by_ability(u8* out);
s32 battle_target_list_units_on_panels(u8* list, battle_stats_t* origin);
s32 battle_target_move_cursor_to_battle_id(u32 battle_id);
void battle_target_project_cursor_tile_to_screen(VECTOR* projected, SVECTOR* position, SVECTOR* raised);
s32 battle_target_run_calculator(const battle_action_command_t* source);
s32 battle_target_set_ability_panels(const battle_action_command_t* source);
s32 battle_target_set_all_panels_targeted_if_targetable(void);
void battle_target_set_coordinates_for_ability(battle_stats_t* acting, battle_strike_work_t* out);
s32 battle_target_set_weapon_attack_panels(battle_action_command_t* source);
s32 battle_target_set_jump_ability_panels(const battle_action_command_t* source);
s32 battle_target_validate_lunging_target(s32 unit_id, s32 x, s32 y, s32 elevation, s32 target_id);
s32 battle_target_validate_weapon_target(const battle_action_command_t* source);
void battle_target_build_directional_attack_panels(const battle_action_command_t* source, u8 mode);
void battle_target_check_moldball_virus_depth(s16 ability_id);
s32 battle_action_init_current_data(s32 unit_id);
void battle_action_store_acting_unit_data(battle_stats_t* unit);
s32 battle_action_store_target_stats_pointer_data(s32 unit_id);

/* move */
/* Eight-byte result copied whole into the post-movement item display. */
typedef struct battle_move_found_item {
    s32 _unknown_00;   /* 0x00; reset when an item is selected */
    u8 item_id;        /* 0x04 */
    u8 _padding_05[3]; /* tail padding to four-byte alignment */
} battle_move_found_item_t;
typedef char battle_move_found_item_size_must_be_8[sizeof(battle_move_found_item_t) == 8 ? 1 : -1];

typedef enum battle_trap_id {
    BATTLE_TRAP_ID_DEGENERATOR = 0,
    BATTLE_TRAP_ID_DEATHTRAP = 1,
    BATTLE_TRAP_ID_SLEEPING_GAS = 2,
    BATTLE_TRAP_ID_STEEL_NEEDLE = 3,
    BATTLE_TRAP_ID_NONE = 0xff,
} battle_trap_id_e;

typedef enum battle_move_find_result_flags {
    BATTLE_MOVE_FIND_RESULT_ITEM = 0x01,
    BATTLE_MOVE_FIND_RESULT_TRAP = 0x02,
    BATTLE_MOVE_FIND_RESULT_OCCUPIED = 0x04,
} battle_move_find_result_flags_e;

typedef struct battle_move_find_result_data {
    u8 flags; /* battle_move_find_result_flags_e */
    u8 entry_index;
    u8 rare_item_id;
    u8 common_item_id;
    u8 trap_id; /* battle_trap_id_e */
    s8 occupying_unit_id;
} battle_move_find_result_data_t;
typedef char battle_move_find_result_data_size_must_be_6[(sizeof(battle_move_find_result_data_t) == 6) ? 1 : -1];

/* Ordered class selection at 0x80174630–0x80174700, not movement ability
 * flags or the separate cost-table selector. Water names describe standing
 * height. The BATTLE_MOVEMENT_SET_3 bits carry the retail ability names, which
 * are the other way round: "Move in Water" (0x1f6) stands on the surface and
 * "Walk on Water" (0x1f7) one level below it. */
typedef enum battle_move_class {
    BATTLE_MOVEMENT_CLASS_FLY = 0,
    BATTLE_MOVEMENT_CLASS_TELEPORT = 1,
    BATTLE_MOVEMENT_CLASS_FLOAT = 2,
    BATTLE_MOVEMENT_CLASS_LAVA = 3,
    BATTLE_MOVEMENT_CLASS_WATER_SURFACE = 4,
    BATTLE_MOVEMENT_CLASS_WATER_DEPTH_ONE = 5,
    BATTLE_MOVEMENT_CLASS_UNDERWATER = 6,
    BATTLE_MOVEMENT_CLASS_NORMAL = 7,
} battle_move_class_e;

typedef enum battle_move_effective_flags {
    BATTLE_EFFECTIVE_MOVEMENT_SILENT_WALK = 0x02,
    BATTLE_EFFECTIVE_MOVEMENT_TELEPORT = 0x08,
    BATTLE_EFFECTIVE_MOVEMENT_IN_WATER = 0x10, /* BATTLE_MOVEMENT_SET_3_WALK_ON_WATER: one level below the surface */
    BATTLE_EFFECTIVE_MOVEMENT_ON_WATER = 0x20, /* BATTLE_MOVEMENT_SET_3_MOVE_IN_WATER: on the surface */
    BATTLE_EFFECTIVE_MOVEMENT_FLOAT = 0x40,
    BATTLE_EFFECTIVE_MOVEMENT_FLY = 0x80,
} battle_move_effective_flags_e;

/* Acting-unit scratch block at 0x1f800000, pointer at 0x8018f4e0.
 * Only target-proven bytes are named. */
typedef struct battle_move_pathfind_scratch {
    u16 target_index;  /* 0x00; target tile index: (level << 8) + y * map_max_x + x */
    u8 jump_times_two; /* 0x02; 0x3e when flying / ignoring height */
    u8 _unused_03;
    u8 jump_or_1f;     /* 0x04 */
    u8 jump_half;      /* 0x05 */
    u8 move;           /* 0x06 */
    u8 x;              /* 0x07; battle_stats_t 0x47 */
    u8 y;              /* 0x08; battle_stats_t 0x48 */
    u8 high_elevation; /* 0x09 */
    u8 target_x;       /* 0x0a */
    u8 target_y;       /* 0x0b */
    u8 target_level;
    u8 unit_id;              /* 0x0d; mount id when riding */
    u8 move_type;            /* 0x0e; battle_move_class_e */
    u8 cost_table;           /* 0x0f; main_terrain_cost_table_e */
    u8 movement_3;           /* 0x10 */
    u8 cannot_enter_water;   /* 0x11 */
    u8 will_drown;           /* 0x12 */
    u8 will_sink;            /* 0x13 */
    u8 cannot_stay_on_water; /* 0x14 */
    u8 mountable_chocobo;    /* 0x15 */
    u8 can_ride;             /* 0x16 */
    u8 _unused_17;
    u8 map_max_x;       /* 0x18 */
    u8 map_max_y;       /* 0x19 */
    u8 _unknown_1a;     /* 0x1a; byte at 0x8018f4fc */
    u8 unit_size;       /* 0x1b */
    u8 movement_set_3;  /* 0x1c */
    u8 fly_or_teleport; /* 0x1d */
    u8 source_tile_occupied;
    u8 destination_tile_occupied;
    u8 _unused_20[2];
    u8 stepping_stone;      /* 0x22 */
    u8 ai_propagation_mode; /* 0x23; set only by AI target propagation (0x80178224 clears it) */
    u8 movement_set_2;      /* 0x24 */
    u8 can_pass_lava;       /* 0x25 */
} battle_move_pathfind_scratch_t;

enum {
    BATTLE_MOVEMENT_SAVED_SOURCE_HEIGHT = 1,
    BATTLE_MOVEMENT_SAVED_DESTINATION_HEIGHT = 2,
};

/* Per-tile frontier flags produced by the movement spread pass. These are
 * distinct from map_tile_t::ceiling_depth_and_marks and are consumed by the
 * reachable-tile and path-propagation helpers. */
enum {
    FRONTIER_FLAG_QUEUED = 0x01, /* waiting to be spread from in the current pass */
    FRONTIER_FLAG_LOW_CORNER = 0x02,
    FRONTIER_FLAG_UNIT_ON_TILE = 0x04,
    FRONTIER_FLAG_STAND_ON_UNIT = 0x08,  /* the unit on the tile can be stood on */
    FRONTIER_FLAG_ENTERABLE = 0x10,      /* the terrain lets the mover in and no enemy stands there */
    FRONTIER_FLAG_MOUNTABLE_UNIT = 0x20, /* a unit of the mover's team that can be mounted stands there */
    FRONTIER_FLAG_STEEP = 0x40,          /* a slope higher than the mover's jump: no entry from its low side */
    FRONTIER_FLAG_BLOCKED = 0x80,        /* a blocked tile, or the tile of Altima's second form */
};

/* battle_move_set_tile_flags_for_pathfinding modes. Bit 0 clears the target panels' ranges. */
typedef enum battle_move_tile_flags_mode {
    BATTLE_MOVE_TILE_FLAGS_MODE_START_SPREAD = 1, /* mark the units and queue the mover's own tile */
    BATTLE_MOVE_TILE_FLAGS_MODE_KEEP_RANGES = 2,  /* mark the units only */
    BATTLE_MOVE_TILE_FLAGS_MODE_TERRAIN_ONLY = 3, /* leave the units out */
} battle_move_tile_flags_mode_e;

/* Composite event work produced after movement. This is separate from the
 * tile-query flags in battle_move_find_result_data_t and the learned movement
 * ability bitsets. Bit 0x0200 has readers but no proven producer. */
typedef enum battle_move_post_event_flags {
    BATTLE_MOVE_POST_EVENT_CRYSTAL_OR_TREASURE = 0x0001,
    BATTLE_MOVE_POST_EVENT_MOVEMENT_BENEFIT = 0x0002,
    BATTLE_MOVE_POST_EVENT_ITEM_FOUND = 0x0004,
    BATTLE_MOVE_POST_EVENT_TRAP_TRIGGERED = 0x0008,
    BATTLE_MOVE_POST_EVENT_CHARGING_CANCEL = 0x0010,
    BATTLE_MOVE_POST_EVENT_MOVE_HP_UP = 0x0020,
    BATTLE_MOVE_POST_EVENT_MOVE_MP_UP = 0x0040,
    BATTLE_MOVE_POST_EVENT_MOVE_GET_EXP = 0x0080,
    BATTLE_MOVE_POST_EVENT_MOVE_GET_JP = 0x0100,
    BATTLE_MOVE_POST_EVENT_SOURCE_DISPLAY = 0x0200, /* runs the source unit's post-action display */
    BATTLE_MOVE_POST_EVENT_MOUNT_STATUS_CHANGE = 0x0400,
} battle_move_post_event_flags_e;

/* Shared movement work state addressed through 0x8018f4e4.
 *
 * Propagation at 0x80178ca4 and jump spreading at 0x80177c08 use the same
 * base. Coordinate halfwords have both signed and unsigned readers; cast
 * explicitly for signed comparisons. This is a verified prefix, not a claim
 * about the allocation's full size.
 */
typedef struct battle_move_spread_state {
    map_tile_t* current_tile;                 /* 0x00 */
    map_tile_t* destination_tile;             /* 0x04 */
    battle_target_panel_t* current_panel;     /* 0x08 */
    battle_target_panel_t* destination_panel; /* 0x0c */
    battle_target_panel_t* source_panel;      /* 0x10; selected at 0x8017567c */
    /* 0x14; base-grid or 512 + record panel, set at 0x80177614 */
    battle_target_panel_t* effective_destination_panel;
    s16 reachable_count;           /* 0x18 */
    s16 tile_index;                /* 0x1a */
    s16 destination_index;         /* 0x1c */
    s16 source_panel_index;        /* 0x1e; terrain index or 512 + compact record index */
    s16 destination_panel_index;   /* 0x20; terrain index or 512 + compact record index */
    s16 candidate_remaining_range; /* 0x22 */
    s16 jump_remaining_range;      /* 0x24; source remaining range minus pass at 0x80176594 */
    s16 tile_x;                    /* 0x26 */
    s16 tile_y;                    /* 0x28 */
    s16 tile_level;                /* 0x2a */
    u16 level_offset;              /* 0x2c */
    u16 row_offset;                /* 0x2e */
    s16 work_x;                    /* 0x30 */
    s16 work_y;                    /* 0x32 */
    s16 work_level;                /* 0x34; signed at 0x8017742c, low byte copied at 0x80177fdc */
    /* Signed side differences before water adjustment. Source is exit minus
     * opposite at 0x80175864, reversed at 0x80177358. Destination is opposite
     * minus entry at 0x80175b7c, reversed at 0x801776b0. */
    s16 source_side_height_delta;      /* 0x36 */
    s16 destination_side_height_delta; /* 0x38 */
    s16 x_step;                        /* 0x3a */
    s16 y_step;                        /* 0x3c */
    u8 _unused_3e[2];
    /* 0x40-0x43: whether the source/destination is a unit's compact record
     * (a path above a unit) and that record's index, from panel byte 0x02. */
    u8 source_unit_record_flag;       /* 0x40 */
    u8 source_unit_record_index;      /* 0x41 */
    u8 destination_unit_record_flag;  /* 0x42 */
    u8 destination_unit_record_index; /* 0x43; also the per-step on-record flag in path encoding */
    u8 source_side_shift;
    u8 destination_side_shift;            /* 0x45; slope shift of the destination entry side */
    u8 source_ceiling_height;             /* 0x46; half-height units */
    u8 destination_ceiling_height;        /* 0x47; half-height units */
    u8 source_exit_height;                /* 0x48; half-height units */
    u8 source_opposite_height;            /* 0x49; extended samples at 0x80177180 leave it unchanged */
    u8 destination_entry_height;          /* 0x4a; half-height units */
    u8 destination_opposite_height;       /* 0x4b */
    u8 source_base_height_times_two;      /* 0x4c */
    u8 destination_base_height_times_two; /* 0x4d */
    u8 _unused_4e[2];
    /* 0x50; height scratch: a side height compared with the exit/entry height,
     * a panel max_height_delta, or a unit record index in the jump check. */
    u8 work_height;
    u8 jump_half_height;        /* 0x51; intermediate-panel half height at 0x8017637c */
    u8 jump_slope;              /* 0x52; intermediate-panel packed slope at 0x80176388 */
    u8 jump_unit_size;          /* 0x53; acting-unit body size copied at 0x80176260 */
    u8 outer_count;             /* 0x54 */
    u8 inner_count;             /* 0x55 */
    u8 source_half_height;      /* 0x56; tile byte 3, low five bits */
    u8 destination_half_height; /* 0x57 */
    u8 source_slope;            /* 0x58; packed two-bit side multipliers */
    u8 destination_slope;       /* 0x59 */
    u8 budget_matches;          /* 0x5a; candidate budget equals predecessor budget minus step cost */
    u8 candidate_saved;         /* 0x5b; set by the save-selected-candidate step at 0x8017808c */
    u8 _unused_5c[2];
    u8 path_length;                  /* 0x5e; trace pass count, also each panel mark */
    u8 source_climb;                 /* 0x5f; step bit 2 */
    u8 destination_climb;            /* 0x60; step bit 3 */
    u8 step_distance;                /* 0x61; axis distance, encoded minus one */
    u8 frontier_max_remaining_range; /* 0x62; maximum residual queued for the next pass */
    u8 _unused_63;
    /* 0x64 is |exit - entry| (0 when flying), stored into panel max_height_delta;
     * 0x65-0x67 are the best candidate's tie-breakers (0x80177e64). */
    u8 height_delta;          /* 0x64 */
    u8 best_height_delta;     /* 0x65 */
    u8 best_fly_height_delta; /* 0x66 */
    u8 best_extra_span;       /* 0x67 */
    u8 saved_x;               /* 0x68 */
    u8 saved_y;               /* 0x69 */
    u8 saved_level;           /* 0x6a */
    u8 saved_unit_record_flag;
    u8 saved_unit_record_index;
    u8 saved_remaining_range;
    u8 saved_destination_height_offset;
    u8 saved_source_height_offset;
    u8 saved_height_offset_flags;
    u8 placement_failed; /* 0x71 */
    u8 selected_source_side_shift;
    u8 previous_source_side_shift;
    u8 clearance_height; /* 0x74; ceilings below this plus unit size block the step (0x801777cc) */
} battle_move_spread_state_t;
typedef char
    battle_move_spread_tile_x_must_be_26[((unsigned long)&((battle_move_spread_state_t*)0)->tile_x == 0x26) ? 1 : -1];
typedef char battle_move_spread_frontier_must_be_62
    [((unsigned long)&((battle_move_spread_state_t*)0)->frontier_max_remaining_range == 0x62) ? 1 : -1];
typedef char battle_move_spread_placement_must_be_71
    [((unsigned long)&((battle_move_spread_state_t*)0)->placement_failed == 0x71) ? 1 : -1];

/* battle_move_record_t.unit_id_flags. */
enum {
    BATTLE_MOVE_RECORD_UNIT_ID_MASK = 0x1f,
    BATTLE_MOVE_RECORD_FLAG_NOT_MOUNTABLE = 0x20,
    BATTLE_MOVE_RECORD_FLAG_OTHER_TEAM = 0x40, /* team bits differ from the mover's */
    BATTLE_MOVE_RECORD_NONE = 0xff,
};

/* Sixteen seven-byte records reached through 0x8018f4e8. AI propagation
 * invalidates byte 3 at 0x80178d50–0x80178d60. The ordinary movement producer
 * at 0x80174430 stores heights in half-height units: body height includes a
 * rider adjustment, standing height includes terrain/water/Float adjustments,
 * and top height is their sum (0x80174a28–0x80174b44). */
typedef struct battle_move_record {
    u8 x;                    /* 0x00 */
    u8 y;                    /* 0x01 */
    u8 higher_elevation : 7; /* 0x02 */
    u8 stepping_stone : 1;
    u8 unit_id_flags;   /* 0x03; BATTLE_MOVE_RECORD_* */
    u8 body_height;     /* 0x04 */
    u8 standing_height; /* 0x05 */
    u8 top_height;      /* 0x06 */
} battle_move_record_t;
typedef char battle_movement_record_size_must_be_7[(sizeof(battle_move_record_t) == 7) ? 1 : -1];

/* Contiguous result bytes returned as battle_move_find_result_data_t. Keep
 * distinct symbols: the target reloads their absolute addresses separately. */
extern battle_move_found_item_t g_battle_move_find_display_item;
/* The scalar first-byte alias retains the retail absolute read/write addresses. */
extern battle_move_find_result_data_t g_battle_move_find_tile_result;
extern u8 g_battle_move_find_result_flags;
extern u8 g_battle_move_find_entry_index;
extern u8 g_battle_move_find_rare_item_id;
extern u8 g_battle_move_find_common_item_id;
extern u8 g_battle_move_find_trap_id;
extern s8 g_battle_move_find_occupying_unit_id;
extern u8 g_battle_move_current_edge_height;
extern u8 g_battle_move_destination_edge_height;
extern s32 g_battle_move_jump_gravity;
extern battle_move_step_t g_battle_move_step_value;
extern s16 g_battle_move_target_screen_z;
/* battle_move_step_phase_e by step direction: the phase each kind of step enters. */
extern u8 g_battle_move_float_to_centre_phases[];
extern u8 g_battle_move_walk_to_centre_phases[];
extern u8 g_battle_move_climb_to_centre_phases[];
extern u8 g_battle_move_jump_descent_phases[];
extern battle_unit_misc_data_t* g_battle_move_tile_occupant;
extern s32 g_battle_move_displacement_direction;
extern u8* g_battle_move_terrain_costs_ptr;
extern s32 g_battle_move_reachable_tiles_valid;
extern s32 g_battle_move_reachable_unit_id;
extern battle_walk_path_t g_battle_move_path;
extern u8 g_battle_move_pathing_scratch;
extern battle_move_pathfind_scratch_t* g_battle_move_config_ptr;

/* The two map tiles of the step in progress, refreshed by battle_move_get_current_and_destination_tiles from
 * the unit's movement value. Callers that only forward them to the step
 * starters take them as opaque words; the geometry readers use the tile
 * fields (height at 0x02, depth_half_height at 0x03, slope_type, flags). */
extern map_tile_t* g_battle_move_current_tile;
extern map_tile_t* g_battle_move_destination_tile;

/* Pending battle_move_post_event_flags_e bits, dispatched in priority order. */
extern s32 g_battle_move_find_result;
extern u8* g_battle_move_frontier_flags_ptr;

/* Seven proven half-height offsets at 0x8018f4d8; the following byte is not
 * established as another element. Extended callers select indices 0..6. */
extern u8 g_battle_move_height_offsets[7];
extern battle_move_record_t* g_battle_move_records_ptr;
extern battle_move_spread_state_t* g_battle_move_scratch_pad_ptr;
extern u8 g_battle_move_path_height_offsets;
extern u8 g_battle_move_destination_unit_record;
extern u8 g_battle_move_effective_flags;
extern u8 g_battle_move_jump_landing_phases[];
extern u8* g_battle_move_pathing_frontier;
extern s32 g_battle_move_pathing_resume_pass;
extern s32 g_battle_move_pathing_tile_index;
/* Zeroed by battle_move_calculate_pathing and never read. That pass keeps in statics what
 * battle_move_store_unit_movement_to_scratchpad keeps in locals; these three stand where it sets
 * its team, ignore_terrain and count locals (the generic unit has no team, terrain rule or units). */
extern u8 g_battle_move_pathing_team;
extern u8 g_battle_move_pathing_ignore_terrain;
extern s32 g_battle_move_pathing_unit_record_count;
extern void (*g_battle_move_spread_preset_table[])(void);
extern u8 g_battle_move_terrain_cost;

/* Height scratch record at 0x80096238; battle_unit_calculate_height_data fills
 * it in place. */
extern battle_unit_height_data_t g_battle_move_tile_occupant_height;
extern u8 g_battle_move_weather_terrain_cost;
s32 battle_move_check_horizontal_jump(s32, u8, u8, u8);

battle_walk_path_t* battle_move_calculate_pathing(s32 flags, s32 jump, s32 x, s32 y, u32 level, s32 target_x,
    s32 target_y, u32 target_level, s32 initialize, s32* suspended, s32 check_budget);

void battle_move_calculate_pathing_time_sliced(s32 flags, s32 jump, s32 x, s32 y, s32 level, s32 target_x, s32 target_y,
    s32 target_level, s32 initialize, s32* suspended);

void battle_move_set_unit_step_slope_scales(battle_unit_misc_data_t* unit);
void battle_move_step_unit_to_map_tile_center(battle_unit_misc_data_t* unit, s32 allow_height_change);
void battle_move_undo_unit_move(battle_unit_misc_data_t* unit);

void battle_move_set_velocity_for_contiguous_steps_with_final_tile_height(
    battle_unit_misc_data_t* unit, const battle_walk_path_t* path, const battle_move_step_t* step);

void battle_move_apply_unit_step_velocity(battle_unit_misc_data_t* unit);
void battle_move_displace_unit_at_destination_tile(battle_unit_misc_data_t* unit, s32 direction);
void battle_move_finish_unit_step_at_tile_edge(battle_unit_misc_data_t* unit);
void battle_move_update_walking_step_at_tile_edge(battle_unit_misc_data_t* unit);
s32 battle_move_start_next_post_movement_step(void);
void battle_move_accelerate_unit_to_destination_with_height_change(battle_unit_misc_data_t* unit);
void battle_move_accelerate_unit_to_destination_no_height_change(battle_unit_misc_data_t* unit);
void battle_move_store_unit_movement_to_scratchpad(s32 unit_id);
s32 battle_move_set_reachable_tiles(s32 unit_id, s32 map_x, s32 map_y, s32 map_z);
void battle_move_set_tile_flags_for_pathfinding(s32 mode);
void battle_move_update_path_step(battle_unit_misc_data_t* unit);

void battle_move_get_current_and_destination_tiles(
    battle_unit_misc_data_t* unit, map_tile_t** current_tile, map_tile_t** destination_tile);

void battle_move_check_occupied_tile_standing_height(s32 record_index, s32 extra_span);
void battle_move_advance_display_unit_step(battle_unit_misc_data_t* unit);
void battle_move_apply_selected_candidate(battle_move_spread_state_t* state);
void battle_move_calculate_spread(void);
u8 battle_move_calculate_tile_ceiling(s32 x, s32 y, s32 exit_slope_shift, u8 exit_height);
s32 battle_move_calculate_tile_layer_step_offset(s32 x, s32 y, u32 layer);
s32 battle_move_check_spreading_tile_coordinates(void);
void battle_move_clear_enterable_flags_under_dead_or_jumping_units(void);
void battle_move_displace_overlapping_unit(battle_unit_misc_data_t* unit, s32 direction);
void battle_move_displace_unit_along_step_direction(battle_unit_misc_data_t* unit, s32 x, s32 y, s32 delta);
void battle_move_finalize_path_after_animation(battle_unit_misc_data_t* unit);
battle_move_effective_flags_e battle_move_get_effective_flags(const battle_stats_t* unit);
s32 battle_move_get_support_flags(battle_stats_t* unit);
s32 battle_move_has_reached_current_tile_exit_edge(s32 direction, battle_unit_misc_data_t* unit);
s32 battle_move_has_reached_destination_tile_center(s32 direction, battle_unit_misc_data_t* unit);
s32 battle_move_has_reached_destination_tile_entry_edge(s32 direction, battle_unit_misc_data_t* unit);
s32 battle_move_init_destination_geometry(s32 candidate);
void battle_move_init_knockback(battle_unit_misc_data_t* unit);
void battle_move_init_post_movement_display(void);
s32 battle_move_init_source_geometry(s32 direction);
s32 battle_move_init_source_panel(s32 direction);
void battle_move_init_spread_scratch(void);

s32 battle_move_calculate_jump_arc_velocity(const SVECTOR* from, const SVECTOR* to, VECTOR* out);

battle_walk_path_t* battle_move_calculate_walkto_pathing(
    s32 a, s32 b, s32 x, s32 y, s32 z, s32 destination_x, s32 destination_y, s32 destination_z);

/* Scales the three velocity words at misc-unit offsets 0x28/0x2c/0x30; the
 * definition indexes them as a word array. */
void battle_move_interpolate_partial(struct battle_unit_misc_data* unit, s32 scale);
u32 battle_move_set_target_for_mounted_unit_and_find_item(battle_stats_t* unit);
void battle_move_start_unit_step(battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* unused_to);
void battle_move_start_float_step(battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* to);
void battle_move_start_unit_step_at_climb_speed(
    battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* unused_to);
s32 battle_move_is_unit_moving_by_misc_id(u32 misc_id);
s32 battle_move_propagate_destination(s32 candidate, s32 extra_span);
void battle_move_return_to_previous_map_square(battle_unit_misc_data_t* unit);
void battle_move_save_selected_candidate(s32 direction, s32 candidate, s32 extra_span);
void battle_move_set_reachable_tile_flags(void);
void battle_move_set_spreading_tile_x_and_y_coordinates(void);
void battle_move_snap_axis_to_current_tile_exit_edge(s32 direction, battle_unit_misc_data_t* unit);
void battle_move_snap_axis_to_destination_tile_center(s32 direction, battle_unit_misc_data_t* unit);
void battle_move_spread_to_neighbors(void);
s32 battle_move_start_unit_walk_to(s32 misc_id, unit_tile_coordinates_t* destination, s32 flags, s32 jump, s32 speed);
void battle_move_step_unit_to_map_tile_center_no_height_change(battle_unit_misc_data_t* unit);
void battle_move_step_unit_to_map_tile_center_with_height_change(battle_unit_misc_data_t* unit);
void battle_move_transfer_tiles_height_halves_and_slope_to_scratch_pad(void);
void battle_move_update_airborne_ascent_phase(battle_unit_misc_data_t* unit);
s32 battle_move_update_candidate(s32 extra_span);
void battle_move_update_knockback_after_animation(battle_unit_misc_data_t* unit);
void battle_move_update_unit_step_to_destination_tile_center(battle_unit_misc_data_t* unit);
u8 battle_move_get_water_depth(battle_unit_misc_data_t* unit);
void battle_move_apply_knockback(void);
s32 battle_move_calculate_teleport_chances(void);
void battle_move_check_and_spread_one_tile_around(void);
s32 battle_move_check_knockback_destination(s32 direction, s32 x, s32 y);
void battle_move_encode_path_steps(void);
s32 battle_move_get_direction(const battle_unit_misc_data_t* unit);
void battle_move_set_unit_path_flag(battle_unit_misc_data_t* unit);

void battle_move_set_unit_step_delta_center_to_edge(
    battle_unit_misc_data_t* unit, const battle_move_step_t* step, const map_tile_t* tile, s32 edge_height);

void battle_move_set_float_step_delta_center_to_edge(
    battle_unit_misc_data_t* unit, const battle_move_step_t* step, const map_tile_t* tile, s32 edge_height);

void battle_move_set_unit_step_delta_edge_to_center(
    battle_unit_misc_data_t* unit, const battle_move_step_t* step, const map_tile_t* tile, s32 edge_height);

void battle_move_set_float_step_delta_edge_to_center(
    battle_unit_misc_data_t* unit, const battle_move_step_t* step, const map_tile_t* tile, s32 edge_height);

void battle_move_set_velocity_for_contiguous_clamped_steps(
    battle_unit_misc_data_t* unit, const battle_walk_path_t* path, const battle_move_step_t* step);

void battle_move_spread_horizontal_jump(void);
void battle_move_start_unit_climb_hop_step(battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* to);

void battle_move_start_unit_climb_jump_step(
    battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* to);

void battle_move_start_float_climb_jump_step(
    battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* to);

void battle_move_start_unit_jump_step(
    battle_unit_misc_data_t* unit, const map_tile_t* unused_from, const map_tile_t* unused_to);
void battle_move_start_fly_step(
    battle_unit_misc_data_t* unit, const map_tile_t* unused_current_tile, const map_tile_t* unused_destination_tile);
void battle_move_transfer_target_coordinates(battle_unit_misc_data_t* src, battle_unit_misc_data_t* dst);
void battle_move_update_knockback_step(battle_unit_misc_data_t* unit);
void battle_move_update_unit_by_misc_id(void);
void battle_move_update_unit_step_to_current_tile_exit_edge(battle_unit_misc_data_t* unit);
void battle_move_update_float_step_to_current_tile_exit_edge(battle_unit_misc_data_t* unit);
void battle_move_update_float_step_to_destination_tile_center(battle_unit_misc_data_t* unit);
void battle_move_update_unit_step_to_destination_tile_entry_edge(battle_unit_misc_data_t* unit);
void battle_move_update_unit_vertical_step_to_destination_tile_center(battle_unit_misc_data_t* unit);
battle_walk_path_t* battle_move_build_path_to_tile(s32 unit_id, s32 x, s32 y, s32 elevation);
s32 battle_move_clamp_z_to_tile_headroom(s32 value, s32 x, s32 y, u32 layer);

/* map */
/* Four serialized move-find item/trap entries per map. */
typedef enum map_move_find_item_entry_flags {
    MAP_MOVE_FIND_ENTRY_TRAP_DEGENERATOR = 0x01,
    MAP_MOVE_FIND_ENTRY_TRAP_DEATHTRAP = 0x02,
    MAP_MOVE_FIND_ENTRY_TRAP_SLEEPING_GAS = 0x04,
    MAP_MOVE_FIND_ENTRY_TRAP_STEEL_NEEDLE = 0x08,
    MAP_MOVE_FIND_ENTRY_NO_TRAP = 0x10,
    MAP_MOVE_FIND_ENTRY_NO_ITEM = 0x20,
    MAP_MOVE_FIND_ENTRY_UPPER_LAYER = 0x80,
} map_move_find_item_entry_flags_e;

typedef struct map_move_find_item_entry {
    u8 position; /* X in the high nibble, Y in the low nibble. */
    u8 flags;    /* map_move_find_item_entry_flags_e */
    u8 rare_item_id;
    u8 common_item_id;
} map_move_find_item_entry_t;
typedef char map_move_find_item_entry_size_must_be_4[(sizeof(map_move_find_item_entry_t) == 4) ? 1 : -1];

typedef struct map_move_find_item_data {
    map_move_find_item_entry_t entries[4];
} map_move_find_item_data_t;
typedef char map_move_find_item_data_size_must_be_16[(sizeof(map_move_find_item_data_t) == 16) ? 1 : -1];

/* Terrain chunk: two dimensions followed by two fixed 256-tile layers. */
typedef struct battle_map_terrain_resource {
    u8 width;                   /* 0x0000; map x dimension */
    u8 depth;                   /* 0x0001; map y dimension */
    map_tile_t tiles[2][0x100]; /* 0x0002; tile levels 0 and 1 */
} battle_map_terrain_resource_t;
typedef char battle_map_terrain_resource_size_must_be_0x1002[sizeof(battle_map_terrain_resource_t) == 0x1002 ? 1 : -1];

/*
 * Script variable 0x23, set by event instruction {3C} Weather.  The same
 * strength scale covers rain and snow; the map's palette-mod flag word says
 * which one is falling.  battle_map_get_effective_weather folds that flag in by
 * adding 3, so callers that compare against it see the snow values.
 */
typedef enum battle_map_weather {
    BATTLE_WEATHER_NONE = 0,
    BATTLE_WEATHER_NONE_ALT = 1,
    BATTLE_WEATHER_RAIN = 2,
    BATTLE_WEATHER_STORM = 3,
    BATTLE_WEATHER_STRONG_STORM = 4,
    BATTLE_WEATHER_SNOW = 5,
    BATTLE_WEATHER_SNOWSTORM = 6,
    BATTLE_WEATHER_STRONG_SNOWSTORM = 7,
} battle_map_weather_e;

/* Added to a rain value to give its snow counterpart. */
enum {
    BATTLE_WEATHER_SNOW_OFFSET = 3,
};

/* Bits of the map weather flag word at 0x800b6698 ("weather palette mods"). */
typedef enum battle_map_weather_flags {
    BATTLE_MAP_WEATHER_FLAG_SNOW = 0x01,
    BATTLE_MAP_WEATHER_FLAG_IGNORE_WEATHER = 0x02,
} battle_map_weather_flags_e;

/* The signed palette modifier shares the word above its seven low flag bits. */
enum {
    BATTLE_MAP_WEATHER_MODIFIER_SHIFT = 7,
    BATTLE_MAP_WEATHER_LOW_FLAGS_MASK = 0x7f,
};

/* Eight CLUT colour-cycle slots at 0x800911f4. Each holds a row and a colour
 * range to rotate; they are separate from the mesh's 32 texture-animation
 * instructions and their handle table. */
typedef struct map_texture_animation_state {
    u8 active;
    u8 elapsed_frames;
    u8 frame_duration;
    u8 _padding_03;  /* aligns clut_row */
    u16 clut_row;    /* 0x04; map_clut_row_e */
    u16 first_color; /* 0x06; colour index in the row: the colours first..last rotate one place */
    u16 last_color;  /* 0x08; every frame_duration frames, towards last */
} map_texture_animation_state_t;
typedef char map_texture_animation_state_size_must_be_0xa[(sizeof(map_texture_animation_state_t) == 0xa) ? 1 : -1];

/* Weather particle positions at 0x800fc358: weather 0x8f (snow) uses all 64,
 * 0x90 (rain) fills two sets of 32. */
typedef struct battle_weather_particle {
    s16 x;
    s16 y;
    s16 z;
    /* g_battle_map_weather_fall_speed, plus the sprite index for 0x8f; rain
     * reuses it as splash state (0/1/2) and the splash set as a countdown. */
    s16 fall_speed;
} battle_weather_particle_t;

typedef enum gns_map_state_comparison {
    GNS_MAP_STATE_EQUAL = 1,
    GNS_MAP_STATE_REQUESTED_LESS_THAN_RECORD = 2,
    GNS_MAP_STATE_REQUESTED_GREATER_THAN_RECORD = 3,
} gns_map_state_comparison_e;

/* IDs shared by GNS row tags and mesh-file offset slots. The dispatcher also
 * accepts the separate map_resource_apply_command_e operations below. */
typedef enum gns_resource_type {
    GNS_RESOURCE_NONE = 0x00, /* empty record slot; also ends the command records */
    /* Mesh-file slots use these IDs as word indices; a file row may load a whole mesh or texture. */
    GNS_RESOURCE_PRIMARY_GEOMETRY = 0x10,
    GNS_RESOURCE_PALETTES = 0x11,          /* kept pending until MAP_RESOURCE_APPLY_PALETTES */
    GNS_RESOURCE_UNSUPPORTED_12 = 0x12,    /* raises exception 0x3ca */
    GNS_RESOURCE_TEXTURE_QUARTER_0 = 0x13, /* 64x256 texture strip at VRAM x 0x300 */
    GNS_RESOURCE_TEXTURE_QUARTER_1 = 0x14,
    GNS_RESOURCE_TEXTURE_QUARTER_2 = 0x15,
    GNS_RESOURCE_TEXTURE_QUARTER_3 = 0x16,
    GNS_RESOURCE_TEXTURE = 0x17,
    GNS_RESOURCE_LIGHTING = 0x19, /* kept pending until MAP_RESOURCE_APPLY_LIGHTING */
    GNS_RESOURCE_TERRAIN = 0x1a,
    GNS_RESOURCE_TEXTURE_ANIMATIONS = 0x1b,
    GNS_RESOURCE_PALETTE_ANIMATION_FRAMES = 0x1c,
    GNS_RESOURCE_MARKED_TILE_PALETTES = 0x1f, /* the sixteen CLUTs of MAP_CLUT_ROW_MARKED_TILES */
    GNS_RESOURCE_STATE_RECORDS = 0x23,        /* battle_map_state_record_t list */
    GNS_RESOURCE_PART_GEOMETRY_1 = 0x24,      /* geometry of mesh parts 1-8 */
    GNS_RESOURCE_PART_GEOMETRY_2 = 0x25,
    GNS_RESOURCE_PART_GEOMETRY_3 = 0x26,
    GNS_RESOURCE_PART_GEOMETRY_4 = 0x27,
    GNS_RESOURCE_PART_GEOMETRY_5 = 0x28,
    GNS_RESOURCE_PART_GEOMETRY_6 = 0x29,
    GNS_RESOURCE_PART_GEOMETRY_7 = 0x2a,
    GNS_RESOURCE_PART_GEOMETRY_8 = 0x2b,
    GNS_RESOURCE_POLYGON_FLAGS = 0x2c,
    GNS_RESOURCE_MESH_PRIMARY = 0x2e,
    GNS_RESOURCE_MESH_OVERRIDE = 0x2f,
    GNS_RESOURCE_MESH_ALTERNATE = 0x30,
    GNS_RESOURCE_FILLER = 0x31,              /* repeated file-section rows; the loader still visits them */
    GNS_RESOURCE_SET_CAMERA_POSITION = 0x83, /* 0x83 up: command records */
    GNS_RESOURCE_SET_CAMERA_ROTATION = 0x84,
    GNS_RESOURCE_SET_INDOOR = 0x85,
    GNS_RESOURCE_SET_OUTDOOR = 0x86,
    GNS_RESOURCE_UNHANDLED_87 = 0x87, /* rows of this type are in 14 maps' files; the dispatcher has no case for it */
    GNS_RESOURCE_CLEAR_SNOW = 0x88,
    GNS_RESOURCE_SET_SNOW = 0x89,
    GNS_RESOURCE_UNHANDLED_8A = 0x8a, /* 28-byte rows; forwarded unconditionally and ignored by the dispatcher */
    GNS_RESOURCE_SET_WEATHER_MODIFIER = 0x8b,
} gns_resource_type_e;

/* Structural bounds and the tag separating file rows from command records. */
enum {
    GNS_RESOURCE_TABLE_COUNT = 49, /* mesh offset words and pending/loaded resource slots */
    GNS_RECORD_COMMAND_SEPARATOR = 0x80,
};

/* Private dispatcher operations issued after a texture load, not serialized IDs. */
typedef enum map_resource_apply_command {
    MAP_RESOURCE_APPLY_LIGHTING = 0x32,
    MAP_RESOURCE_APPLY_PALETTES = 0x33,
} map_resource_apply_command_e;

/* Commands of battle_map_dispatch_map_data_command, which switches on command
 * - 0x6a. This is its own id space: event Use3DObject (0x54) reaches command
 * 0x80. Names come from the case bodies. */
typedef enum map_data_command {
    MAP_DATA_COMMAND_DISABLE_TEXTURE_ANIMATION = 0x6a,
    MAP_DATA_COMMAND_RESTORE_TEXTURE_ANIMATION = 0x6b,
    MAP_DATA_COMMAND_DISABLE_ALL_TEXTURE_ANIMATIONS = 0x71,
    MAP_DATA_COMMAND_RESTORE_ALL_TEXTURE_ANIMATIONS = 0x72,
    MAP_DATA_COMMAND_DISABLE_OVERLAY_DITHER = 0x7a, /* SetDrawMode dtd 0 */
    MAP_DATA_COMMAND_ENABLE_OVERLAY_DITHER = 0x7b,  /* SetDrawMode dtd 1 */
    MAP_DATA_COMMAND_SET_3D_OBJECT_STATE = 0x80,
    MAP_DATA_COMMAND_GET_3D_OBJECT_STATE = 0x81,
    MAP_DATA_COMMAND_GET_TEXTURE_ANIMATION_ACTIVE = 0x82,
    MAP_DATA_COMMAND_START_TEXTURE_ANIMATION = 0x83,
    MAP_DATA_COMMAND_SPLIT_MESH_PARTS = 0x98,
    MAP_DATA_COMMAND_RELEASE_GNS_HOLD = 0x9a,
    MAP_DATA_COMMAND_SET_TEXTURE_ANIMATION_DURATION = 0x9d,
} map_data_command_e;

/* Commands of battle_map_light_matrix_command. A START queues its STEP in
 * g_battle_map_light_transition_command, which the map draw loop issues every frame. */
typedef enum map_light_matrix_command {
    MAP_LIGHT_MATRIX_COMMAND_SET = 0x5e,
    MAP_LIGHT_MATRIX_COMMAND_SAVE = 0x5f,
    MAP_LIGHT_MATRIX_COMMAND_RESTORE = 0x60,
    MAP_LIGHT_MATRIX_COMMAND_GET_COLOR = 0x61,
    MAP_LIGHT_MATRIX_COMMAND_GET_LIGHT = 0x62,
    MAP_LIGHT_MATRIX_COMMAND_START_BLEND = 0x66, /* towards the given matrices over the duration */
    MAP_LIGHT_MATRIX_COMMAND_STEP_BLEND = 0x67,
    MAP_LIGHT_MATRIX_COMMAND_START_DELTA = 0x68, /* add a frame-scaled delta */
    MAP_LIGHT_MATRIX_COMMAND_STEP_DELTA = 0x69,
} map_light_matrix_command_e;

/* Commands of battle_map_run_weather_command and battle_map_init_weather_particles. */
typedef enum map_weather_command {
    MAP_WEATHER_COMMAND_SNOW = 0x53,
    MAP_WEATHER_COMMAND_CLEAR = 0x54,
    MAP_WEATHER_COMMAND_RAIN = 0x55,
    MAP_WEATHER_COMMAND_LIGHT_RAIN = 0x7c,
    MAP_WEATHER_COMMAND_HEAVY_RAIN = 0x7d,
    MAP_WEATHER_COMMAND_LIGHT_SNOW = 0x7e,
    MAP_WEATHER_COMMAND_HEAVY_SNOW = 0x7f,
    MAP_WEATHER_COMMAND_THUNDER = 0x89,              /* one lightning flash with its sound */
    MAP_WEATHER_COMMAND_LIGHTNING = 0x8a,            /* random lightning from now on */
    MAP_WEATHER_COMMAND_APPLY_SCRIPT_WEATHER = 0x8b, /* issue the commands for the weather script variable */
    MAP_WEATHER_COMMAND_RESTORE_LIGHTING = 0x8c,
    MAP_WEATHER_COMMAND_INIT_SNOW_PARTICLES = 0x8f, /* battle_map_init_weather_particles */
    MAP_WEATHER_COMMAND_INIT_RAIN_PARTICLES = 0x90,
} map_weather_command_e;

/* Stored renderer selector at g_battle_map_weather_effect_mode. The ordinary
 * modes reuse command IDs; 0x96 runs E458's timed particle transform. */
typedef enum battle_map_weather_effect_mode {
    BATTLE_MAP_WEATHER_EFFECT_SNOW = MAP_WEATHER_COMMAND_SNOW,
    BATTLE_MAP_WEATHER_EFFECT_CLEAR = MAP_WEATHER_COMMAND_CLEAR,
    BATTLE_MAP_WEATHER_EFFECT_RAIN = MAP_WEATHER_COMMAND_RAIN,
    BATTLE_MAP_WEATHER_EFFECT_TIMED_PARTICLE_TRANSFORM = 0x96,
} battle_map_weather_effect_mode_e;

/* g_battle_map_lightning_mode. */
typedef enum map_lightning_mode {
    MAP_LIGHTNING_OFF = 0,
    MAP_LIGHTNING_RANDOM = 1,
    MAP_LIGHTNING_ONCE = 2, /* back to OFF when the flash ends */
} map_lightning_mode_e;

/* Commands of battle_map_load_data, read only while the GNS phase is idle. */
typedef enum map_load_command {
    MAP_LOAD_COMMAND_LOAD_GNS = 0x73, /* always read the GNS; no current caller issues this */
    MAP_LOAD_COMMAND_START = 0x75,
    MAP_LOAD_COMMAND_POLL = 0x76,       /* an unhandled command only advances the load */
    MAP_LOAD_COMMAND_START_HELD = 0x99, /* as START, but hold dispatch until MAP_DATA_COMMAND_RELEASE_GNS_HOLD */
    MAP_LOAD_COMMAND_POLL_HELD = 0x9c,
} map_load_command_e;

/* g_battle_map_resource_load_state; these are not the loader's normal return values. */
typedef enum map_resource_load_state {
    MAP_LOAD_STATE_IDLE = 0x00,
    MAP_LOAD_STATE_GNS_FILE = 0x74, /* also accepted as a no-op input command */
    MAP_LOAD_STATE_DONE = 0x77,
    MAP_LOAD_STATE_RESOURCE_FILE = 0x78,
    MAP_LOAD_STATE_MESH_RECORDS = 0x79, /* rebuild render records once per buffer */
    MAP_LOAD_STATE_MESH_RECORDS_SECOND = 0x91,
    MAP_LOAD_STATE_APPLY_LIGHTING = 0x92,
    MAP_LOAD_STATE_APPLY_TEXTURE = 0x93,
} map_resource_load_state_e;

/* The loader otherwise returns g_battle_map_gns_load_phase. */
enum { MAP_LOAD_RESULT_HELD = 0x9b };

/* g_battle_map_gns_load_phase and the normal return of battle_map_load_data. */
typedef enum map_gns_load_phase {
    MAP_GNS_LOAD_PHASE_IDLE = 0,
    MAP_GNS_LOAD_PHASE_RESOURCES = 1, /* loading the pending resource files one by one */
    MAP_GNS_LOAD_PHASE_START = 0xf,   /* choose the resources once the GNS is in */
} map_gns_load_phase_e;

/* Commands of battle_map_polygon_flag_command. */
typedef enum map_polygon_flag_command {
    MAP_POLYGON_FLAG_COMMAND_RESET_ACTIVE = 0x46,    /* copy LIT into LIT_ACTIVE on every polygon */
    MAP_POLYGON_FLAG_COMMAND_CLEAR_ACTIVE = 0x47,    /* clear LIT_ACTIVE everywhere */
    MAP_POLYGON_FLAG_COMMAND_RESTORE_AMBIENT = 0x94, /* repaint the polygons without LIT in the ambient colour */
} map_polygon_flag_command_e;

/* polygon_flags of the textured map polygons (GNS_RESOURCE_POLYGON_FLAGS).
 * The four battle_map_queue_ routines skip a polygon whose hide bit for the
 * camera yaw is set. No mesh file sets bits 0, 1 or 14. */
enum {
    /* Working copy of LIT, the bit the queue routines test: the polygon's
     * colours come from its normals this frame. Cleared on a polygon a tile
     * mark tints. */
    MAP_POLYGON_FLAG_LIT_ACTIVE = 0x0001,
    /* Hidden while the yaw is in 45-degree band n: bit 2 + n, where
     * n = ((yaw - 1) & 0xe00) >> 9. Tested every frame. */
    MAP_POLYGON_FLAG_HIDDEN_IN_YAW_BAND_0 = 0x0004,
    /* Hidden while the camera rests on diagonal n: bit 10 + n, where
     * n = yaw >> 10. Tested only when (yaw + 0x200) & 0x3ff is 0. */
    MAP_POLYGON_FLAG_HIDDEN_AT_RESTING_YAW_0 = 0x0400,
    MAP_POLYGON_FLAG_HIDDEN_MASK = 0x3ffc,
    /* Lit from its normals each frame; a polygon without it keeps the flat
     * ambient colour. */
    MAP_POLYGON_FLAG_LIT = 0x8000,
};

/* Commands for the map's background gradient, ambient and darkness colours. */
typedef enum map_light_command {
    MAP_LIGHT_COMMAND_SET_BACKGROUND_GRADIENT = 0x56,
    MAP_LIGHT_COMMAND_GET_BACKGROUND_GRADIENT = 0x57,
    MAP_LIGHT_COMMAND_SAVE_BACKGROUND_GRADIENT = 0x58,
    MAP_LIGHT_COMMAND_RESTORE_BACKGROUND_GRADIENT = 0x59,
    MAP_LIGHT_COMMAND_SET_AMBIENT_COLOR = 0x5a,
    MAP_LIGHT_COMMAND_SAVE_AMBIENT_COLOR = 0x5b,
    MAP_LIGHT_COMMAND_RESTORE_AMBIENT_COLOR = 0x5c,
    MAP_LIGHT_COMMAND_GET_AMBIENT_COLOR = 0x5d,
    MAP_LIGHT_COMMAND_SET_DARKNESS_COLOR = 0x63,
    MAP_LIGHT_COMMAND_RESET_DARKNESS_COLOR = 0x64,
    MAP_LIGHT_COMMAND_GET_DARKNESS_COLOR = 0x65,
} map_light_command_e;

/* Commands of battle_map_configure_effect_data_state: the ordering-table
 * handling of untextured map polygons while an effect draws. */
typedef enum map_effect_data_command {
    MAP_EFFECT_DATA_END = 0x87,
    MAP_EFFECT_DATA_BEGIN = 0x88,
} map_effect_data_command_e;

/* Map color-transition commands: the source color and treatment of RGB offsets.
 * battle_gfx_modify_vram_palette honors modes 5-7; every other value adds the
 * offsets to its supplied colors, including RESTORE. */
typedef enum map_color_mode {
    MAP_COLOR_MODE_CURRENT = 0,           /* the colour in transition plus the offsets */
    MAP_COLOR_MODE_CURRENT_HALF = 1,      /* half of it plus the offsets */
    MAP_COLOR_MODE_CURRENT_GREY = 2,      /* its grey (2R + 3G + B) / 6 plus the offsets */
    MAP_COLOR_MODE_CURRENT_GREY_HALF = 3, /* half that grey plus the offsets */
    MAP_COLOR_MODE_BASE = 4,              /* the last applied colour plus the offsets */
    MAP_COLOR_MODE_BASE_HALF = 5,
    MAP_COLOR_MODE_BASE_GREY = 6,
    MAP_COLOR_MODE_BASE_GREY_HALF = 7,
    MAP_COLOR_MODE_RESTORE = 8,      /* map transitions restore the last applied colour and ignore offsets */
    MAP_COLOR_MODE_BASE_REPEAT = 9,  /* as BASE, and record the offsets so that the fade alternates with RESTORE */
    MAP_COLOR_MODE_STOP_REPEAT = 10, /* only clear the repeat record */
} map_color_mode_e;

/* The repeat member of the map colour transitions and of the CLUT animations. */
typedef enum map_color_repeat {
    MAP_COLOR_REPEAT_OFF = 0,
    MAP_COLOR_REPEAT_TINTED = 1,   /* fading to the recorded offsets; a fade back to the base follows */
    MAP_COLOR_REPEAT_RESTORED = 2, /* fading back to the base; the offsets follow again */
} map_color_repeat_e;

/* A CLUT row of the battle map: one VRAM line of sixteen 16-colour CLUTs,
 * at y 0x1e0 through 0x1ed (battle_map_upload_clut_rows uploads a 256x14 image). Index into
 * map_palette_state_t.clut_rows and g_battle_map_palette_modified_colors. */
typedef enum map_clut_row {
    MAP_CLUT_ROW_MAP = 0,
    MAP_CLUT_ROW_MARKED_TILES = 1, /* the map's CLUTs for the polygons of marked tiles */
    MAP_CLUT_ROW_SPRITES = 2,      /* weather particles, deploy portraits, arrow, cursor, shadow, status bubble */
    MAP_CLUT_ROW_UNITS = 3,        /* one CLUT per unit, as in rows 4 to 7 and 10 */
    MAP_CLUT_ROW_UNIT_SHADOWS = 4,
    MAP_CLUT_ROW_ITEM_A = 5, /* the two palettes of the item a unit holds or throws */
    MAP_CLUT_ROW_ITEM_B = 6,
    MAP_CLUT_ROW_ACTION_RESULTS = 7,
    MAP_CLUT_ROW_UNITS_SECOND = 10, /* the second eight palettes of each unit's sheet */
    MAP_CLUT_ROW_WEAPONS = 11,
    MAP_CLUT_ROW_EFFECT = 12,
    MAP_CLUT_ROW_EFFECT_SECOND = 13,
} map_clut_row_e;

enum {
    MAP_CLUT_ROW_COUNT = 14,
    MAP_CLUTS_PER_ROW = 16,
    MAP_CLUT_COLOR_COUNT = 16,
    MAP_CLUT_ROW_COLOR_COUNT = 256, /* MAP_CLUTS_PER_ROW * MAP_CLUT_COLOR_COUNT */
    MAP_CLUT_ROWS_Y = 0x1e0,        /* VRAM line of MAP_CLUT_ROW_MAP; the rows follow one line apart */
};

/* The clut word of CLUT clut_index of a map CLUT row: the value of
 * GetClut(clut_index * 16, MAP_CLUT_ROWS_Y + clut_row), as a constant. */
#define MAP_CLUT(clut_row, clut_index) (((MAP_CLUT_ROWS_Y + (clut_row)) << 6) | (clut_index))

/* Eight runtime slots rotate a range of colours of a CLUT row for map texture animations.
 * They are separate from the 32 serialized 0x14-byte animation instructions
 * a map's mesh file carries. */
enum {
    MAP_CLUT_CYCLE_SLOT_COUNT = 8,
    MAP_CLUT_CYCLE_SLOT_BYTE_COUNT = 0x0a,
};

/* Unscaled 5:5:5:1 PlayStation color components. */
typedef struct map_palette_color_components {
    u8 red_5bit;
    u8 green_5bit;
    u8 blue_5bit;
    u8 semi_transparent;
    u8 red_delta_biased;   /* target - current + 0x1f */
    u8 green_delta_biased; /* target - current + 0x1f */
    u8 blue_delta_biased;  /* target - current + 0x1f */
} map_palette_color_components_t;
typedef char map_palette_color_components_size_must_be_7[(sizeof(map_palette_color_components_t) == 7) ? 1 : -1];

typedef struct map_palette_animation_state {
    u8 active;
    u8 blend_step;    /* 0x01; index into g_battle_map_palette_blend_steps_32/_8 */
    u8 delay_counter; /* 0x02; one blend step each time it reaches mode >> 2 */
    u8 mode;
    u8 repeat;            /* 0x04; map_color_repeat_e */
    rgb24_t repeat_color; /* 0x05 */
} map_palette_animation_state_t;
typedef char map_palette_animation_state_size_must_be_8[(sizeof(map_palette_animation_state_t) == 8) ? 1 : -1];

/* The 0x982-byte runtime state of one CLUT row (map_clut_row_e), based at 0x800995f4:
 * an animation per CLUT, then the 256 colours of the row. Components and
 * packed_colors are runtime state/mirror, not the separate 0x200-byte-per-row
 * upload staging at 0x800e4ea4 (g_battle_map_palette_modified_colors). Palette extraction fills
 * both; 0x80092f98 uploads staging and clears the pending word at 0x800995ec. */
typedef struct map_clut_row_state {
    u8 enabled;
    u8 updates_disabled;
    map_palette_animation_state_t animations[MAP_CLUTS_PER_ROW];
    map_palette_color_components_t components[MAP_CLUT_ROW_COLOR_COUNT];
    u16 packed_colors[MAP_CLUT_ROW_COLOR_COUNT];
} map_clut_row_state_t;
typedef char map_clut_row_state_size_must_be_0x982[(sizeof(map_clut_row_state_t) == 0x982) ? 1 : -1];

/* Palette runtime state block at 0x800995ec: battle_map_update_animations
 * addresses the CLUT rows (0x800995f4) from this base. The upload/move words are
 * the pending flags battle_map_upload_clut_rows (0x80092f98) services. */
typedef struct map_palette_state {
    s32 upload_pending;
    s32 move_pending;
    map_clut_row_state_t clut_rows[MAP_CLUT_ROW_COUNT];
} map_palette_state_t;
typedef char map_palette_state_size_must_be_0x8524[(sizeof(map_palette_state_t) == 0x8524) ? 1 : -1];

/*
 * A 20-byte row from the file section of MAPnnn.GNS, before the 0x80
 * separator. Resource type 0x31 marks repeated filler rows. For mesh records,
 * the loader compares map_state against a request assembled from the low 12
 * bits of the script variable at offset 0, weather in bits 12-14, and
 * time-of-day in bit 15.
 *
 * The two bytes at 4 also form the conventional little-endian GNS record type
 * (for example, 0x2e01 for a primary mesh).  The game reads them separately:
 * the low byte selects the state comparison and the high byte selects the
 * resource type. In all 2,987 pre-separator rows of the USA disc, +0x06 is
 * 0x3333 and +0x10..+0x13 are 55 66 77 88. The loader compares those bytes
 * when checking whether a file changed, and tests +0x13 for a nonzero marker.
 */
typedef struct gns_file_record {
    u16 script_variable_id;  /* 0x00 */
    s16 map_state;           /* 0x02 */
    u8 map_state_comparison; /* 0x04; gns_map_state_comparison_e */
    u8 resource_type;        /* 0x05; gns_resource_type_e */
    u16 _padding_06;         /* 0x06; aligns disc_lba */
    u32 disc_lba;            /* 0x08; absolute, including the high halfword at +0x0a */
    u32 byte_length;         /* 0x0c; sector-rounded */
    u8 _unused_10[3];        /* 0x10 */
    u8 file_present_marker;  /* 0x13; 0x88 on disc, tested for nonzero */
} gns_file_record_t;
typedef char gns_file_record_size_must_be_0x14[(sizeof(gns_file_record_t) == 0x14) ? 1 : -1];

/* Command rows after the 0x80 separator reuse the 20-byte prefix, but +0x08
 * is a byte stride (0x14 or 0x1c), not a disc LBA. The 0x1c rows have eight
 * more payload bytes after this prefix. +0x06/+0x09..+0x0b are zero in the
 * USA disc's command rows. */
typedef struct gns_command_record_prefix {
    u16 script_variable_id;  /* 0x00 */
    s16 map_state;           /* 0x02 */
    u8 map_state_comparison; /* 0x04; gns_map_state_comparison_e */
    u8 resource_type;        /* 0x05; gns_resource_type_e, or GNS_RECORD_COMMAND_SEPARATOR */
    u16 _unused_06;
    u8 record_byte_length; /* 0x08 */
    u8 _unused_09;
    u16 _unused_0a;
    SVECTOR payload; /* 0x0c; followed by more bytes for 0x1c rows */
} gns_command_record_prefix_t;
typedef char gns_command_record_prefix_size_must_be_0x14[(sizeof(gns_command_record_prefix_t) == 0x14) ? 1 : -1];

/* Serialized GNS primary/animated mesh geometry. Counts are unsigned; the
 * variable tail stores unpadded XYZ halfwords, normals, materials and terrain
 * coordinates. Native renderer position/normal records have different strides. */
typedef struct battle_map_mesh_geometry {
    u16 textured_triangle_count;   /* 0x00 */
    u16 textured_quad_count;       /* 0x02 */
    u16 untextured_triangle_count; /* 0x04 */
    u16 untextured_quad_count;     /* 0x06 */
    u16 payload[0];                /* 0x08; count-dependent sections */
} battle_map_mesh_geometry_t;
typedef char battle_map_mesh_geometry_prefix_must_be_8[sizeof(battle_map_mesh_geometry_t) == 8 ? 1 : -1];

/* Mesh files begin with 49 32-bit byte offsets indexed by resource ID (slot = ID * 4).
 * The dispatcher visits IDs 0x10..0x30; slots 0..0x0f and the other reserved
 * words are zero in the USA disc's mesh files. A zero offset means the chunk
 * is absent. MAP000.5 alone has an embedded texture quarter at 0x4c. Each
 * member is named after its gns_resource_type_e id. */
typedef struct map_mesh_file_header {
    u32 _unused_00[16];                  /* 0x00 */
    u32 primary_geometry_offset;         /* 0x40 */
    u32 palettes_offset;                 /* 0x44 */
    u32 _unused_48;                      /* 0x48 */
    u32 texture_quarter_0_offset;        /* 0x4c */
    u32 _unused_50[5];                   /* 0x50 */
    u32 lighting_offset;                 /* 0x64 */
    u32 terrain_offset;                  /* 0x68 */
    u32 texture_animations_offset;       /* 0x6c */
    u32 palette_animation_frames_offset; /* 0x70 */
    u32 _unused_74[2];                   /* 0x74 */
    u32 marked_tile_palettes_offset;     /* 0x7c */
    u32 _unused_80[3];                   /* 0x80 */
    u32 state_records_offset;            /* 0x8c */
    u32 part_geometry_offsets[8];        /* 0x90 */
    u32 polygon_flags_offset;            /* 0xb0 */
    u32 _unused_b4[4];                   /* 0xb4 */
} map_mesh_file_header_t;
typedef char map_mesh_file_header_size_must_be_0xc4[(sizeof(map_mesh_file_header_t) == 0xc4) ? 1 : -1];

/* Polygon position and normal records copied from the mesh geometry block.
 * Each XYZ vector occupies eight bytes; the first position's spare halfword
 * stores the packed terrain tile. */
typedef struct battle_map_mesh_triangle_positions {
    s16 x0, y0, z0;
    s16 terrain_tile;
    s16 x1, y1, z1;
    s16 polygon_flags;
    s16 x2, y2, z2;
    u16 _unused_16;
} battle_map_mesh_triangle_positions_t;

typedef struct battle_map_mesh_quad_positions {
    s16 x0, y0, z0;
    s16 terrain_tile;
    s16 x1, y1, z1;
    s16 polygon_flags;
    s16 x2, y2, z2;
    u16 _unused_16;
    s16 x3, y3, z3;
    u16 _unused_1e;
} battle_map_mesh_quad_positions_t;

typedef struct battle_map_mesh_triangle_normals {
    SVECTOR vertex[3];
} battle_map_mesh_triangle_normals_t;

typedef struct battle_map_mesh_quad_normals {
    SVECTOR vertex[4];
} battle_map_mesh_quad_normals_t;

/* File-load payload at +0x06 of a GNS row. The loader compares all fourteen
 * bytes, including the fixed markers, before deciding whether to reload.
 * Word fields remain byte arrays so enclosing rows retain halfword alignment. */
typedef union gns_load_info {
    struct {
        u16 _unknown_00; /* 0x3333 in file rows */
        u8 disc_lba_le[4];
        u8 byte_length_le[4];
        u8 _unknown_0a[3];      /* 55 66 77 in file rows */
        u8 file_present_marker; /* 0x88 in file rows; zero means no file */
    } fields;
    u8 bytes[14];
} gns_load_info_t;
typedef char gns_load_info_size_must_be_14[sizeof(gns_load_info_t) == 14 ? 1 : -1];

/* Halfword-aligned view of a 20-byte GNS record: the target copies these
 * records with lwl/lwr pairs, so the loader's type has no word member. */
typedef struct gns_record_view {
    s16 script_variable_id;
    s16 map_state;
    u8 map_state_comparison; /* 0x04; gns_map_state_comparison_e */
    u8 resource_type;        /* 0x05; gns_resource_type_e, or GNS_RECORD_COMMAND_SEPARATOR */
    gns_load_info_t load_info;
} gns_record_view_t;
typedef char gns_record_view_size_must_be_0x14[(sizeof(gns_record_view_t) == 0x14) ? 1 : -1];

/* Runtime colour fades stepped by update_map_animations (0x800912a4):
 * g_battle_map_background_gradient_transition at 0x800a1b10, the ambient light at
 * 0x800a1b50 and the darkness at 0x800a1b74. Channels are 16.16 fixed point;
 * the byte fields follow the updater's use. */
typedef struct map_color_transition_channels {
    s32 red;
    s32 green;
    s32 blue;
    s32 red_step;
    s32 green_step;
    s32 blue_step;
} map_color_transition_channels_t;
typedef char
    map_color_transition_channels_size_must_be_0x18[(sizeof(map_color_transition_channels_t) == 0x18) ? 1 : -1];

typedef struct map_color_transition {
    u8 active;
    u8 phase;
    u8 tick; /* 0x02; frame counter, reset when it reaches period >> 2 */
    u8 period;
    u8 repeat;            /* 0x04; map_color_repeat_e */
    rgb24_t repeat_color; /* 0x05 */
    map_color_transition_channels_t channels;
} map_color_transition_t;
typedef char map_color_transition_size_must_be_0x20[(sizeof(map_color_transition_t) == 0x20) ? 1 : -1];

/* The background gradient fade: one channel set per gradient colour. colors[]
 * at 0x38 (0x800a1b48/0x800a1b4c) is the applied gradient pair;
 * battle_map_blend_background_gradient_color indexes it from the same base. */
typedef struct map_gradient_transition {
    u8 active;
    u8 phase;
    u8 tick; /* 0x02; frame counter, reset when it reaches period >> 2 */
    u8 period;
    u8 repeat;            /* 0x04; map_color_repeat_e */
    rgb24_t repeat_color; /* 0x05 */
    map_color_transition_channels_t channels[2];
    CVECTOR colors[MAP_BACKGROUND_GRADIENT_COLOR_COUNT];
} map_gradient_transition_t;
typedef char map_gradient_transition_size_must_be_0x40[(sizeof(map_gradient_transition_t) == 0x40) ? 1 : -1];

/* 0x98-byte map mesh part records at 0x800fbe00. Each of the rotation,
 * translation and scale channels has its current value, the start of the
 * running keyframe and its target or step. */
typedef struct battle_map_mesh_part {
    SVECTOR rotation_start;    /* 0x00 */
    SVECTOR rotation;          /* 0x08 */
    SVECTOR rotation_step;     /* 0x10 */
    SVECTOR translation_start; /* 0x18 */
    SVECTOR translation;       /* 0x20 */
    SVECTOR translation_step;  /* 0x28 */
    VECTOR scale_start;        /* 0x30; the scales reset to {0x1000, 0x1000, 0x1000} */
    VECTOR scale;              /* 0x40 */
    VECTOR scale_step;         /* 0x50 */
    MATRIX matrix;             /* 0x60 */
    s16 elapsed;               /* 0x80; time in the keyframe */
    s16 duration;              /* 0x82; tween length */
    u8 speed;                  /* 0x84; 4.4, 0x10 = 1 */
    u8 next_instruction;       /* 0x85 */
    u8 parent;                 /* 0x86; parent part, from mesh properties byte 0 */
    u8 keyframe;               /* 0x87; 0 = fetch next */
    /* 0x88: per-part start indices and counts of its render records, written
     * by battle_map_append_mesh_geometry (0x800f4dd4). */
    u16 textured_triangle_start;
    u16 textured_quad_start;
    u16 untextured_triangle_start;
    u16 untextured_quad_start;
    u16 textured_triangle_count; /* 0x90 */
    u16 textured_quad_count;
    u16 untextured_triangle_count;
    u16 untextured_quad_count;
} battle_map_mesh_part_t;
typedef char battle_map_mesh_part_size_must_be_0x98[(sizeof(battle_map_mesh_part_t) == 0x98) ? 1 : -1];

/* One 0x50-byte keyframe from a mesh animation record of type 1.
 * Keyframe ids are 1-based; entry 0 is never loaded. Each of the nine channels
 * (rotation, translation and scale of a mesh part) has a target value, a
 * start/end angle for the sine modes and a mode flag word whose bits 0 and 1
 * are TweenTo and TweenBy. */
typedef struct battle_map_mesh_keyframe {
    SVECTOR rotation;    /* 0x00; target */
    SVECTOR translation; /* 0x08; target */
    s16 scale_x;         /* 0x10; ONE = 1.0 */
    s16 scale_y;         /* 0x12 */
    s16 scale_z;         /* 0x14 */
    s16 _unused_16;      /* 0x16 */
    s16 angle_start[9];  /* 0x18 */
    s16 angle_end[9];    /* 0x2a */
    u16 tween_flags[9];  /* 0x3c; per channel */
    u16 _unused_4e;      /* 0x4e */
} battle_map_mesh_keyframe_t;
typedef char battle_map_mesh_keyframe_size_must_be_0x50[(sizeof(battle_map_mesh_keyframe_t) == 0x50) ? 1 : -1];

/* battle_map_mesh_keyframe_t.tween_flags: how one channel reaches its keyframe value. */
typedef enum battle_map_mesh_tween_flags {
    BATTLE_MAP_MESH_TWEEN_TO = 0x01, /* delta = target - start */
    BATTLE_MAP_MESH_TWEEN_BY = 0x02, /* delta = the keyframe value */
    BATTLE_MAP_MESH_TWEEN_LINEAR = 0x04,
    BATTLE_MAP_MESH_TWEEN_SINE = 0x08,        /* delta * sin */
    BATTLE_MAP_MESH_TWEEN_RAISED_SINE = 0x10, /* delta * (sin + 1) */
} battle_map_mesh_tween_flags_e;

/* One instruction from a mesh animation record of type 2. A duration of
 * 0 stops the part's animation when it is reached. */
typedef struct battle_map_mesh_instruction {
    u8 keyframe;  /* 1-based keyframe id */
    u8 next;      /* index of the following instruction */
    u16 duration; /* frames */
} battle_map_mesh_instruction_t;

/* A mesh part's 8 animation states of 17 instructions (index 0 unused).
 * States are loaded state-major; row 0 is also the running copy that event
 * command 0x80 overwrites with the selected state. */
typedef struct battle_map_mesh_instructions {
    battle_map_mesh_instruction_t states[8][17];
} battle_map_mesh_instructions_t;
typedef char
    battle_map_mesh_instructions_size_must_be_0x220[(sizeof(battle_map_mesh_instructions_t) == 0x220) ? 1 : -1];

/* battle_map_texture_animation_t.mode: the technique of a map texture animation row and, for the
 * values from 0x80, its playback state. */
typedef enum map_texture_animation_mode {
    MAP_TEXTURE_ANIMATION_MODE_NONE = 0x00, /* an unused row */
    MAP_TEXTURE_ANIMATION_MODE_UV_LOOP = 0x01,
    MAP_TEXTURE_ANIMATION_MODE_UV_PING_PONG = 0x02,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_LOOP = 0x03,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_PING_PONG = 0x04,
    /* 5-0x1c play once when a map data command starts them; the eight ids of a group behave alike */
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_0 = 0x05,
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_1 = 0x06,
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_2 = 0x07,
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_3 = 0x08,
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_4 = 0x09,
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_5 = 0x0a,
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_6 = 0x0b,
    MAP_TEXTURE_ANIMATION_MODE_UV_ONCE_7 = 0x0c,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_0 = 0x0d,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_1 = 0x0e,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_2 = 0x0f,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_3 = 0x10,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_4 = 0x11,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_5 = 0x12,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_6 = 0x13,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_ONCE_7 = 0x14,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_0 = 0x15,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_1 = 0x16,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_2 = 0x17,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_3 = 0x18,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_4 = 0x19,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_5 = 0x1a,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_6 = 0x1b,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_7 = 0x1c,
    /* starts a battle_map_start_texture_animation over colours canvas_x .. canvas_x + width - 1 of MAP_CLUT_ROW_MAP */
    MAP_TEXTURE_ANIMATION_MODE_COLOR_CYCLE = 0x1d,
    /* Playback states written and dispatched by battle_map_update_texture_animations. */
    MAP_TEXTURE_ANIMATION_MODE_UV_PING_PONG_BACK = 0x80,
    MAP_TEXTURE_ANIMATION_MODE_PALETTE_PING_PONG_BACK = 0x81,
    MAP_TEXTURE_ANIMATION_MODE_UV_REVERSE_ONCE_PLAYING = 0x82,
} map_texture_animation_mode_e;

/* Provisional runtime copy of one 0x14-byte map texture animation
 * instruction (32 rows).
 * UV animations move a canvas-sized frame through VRAM with the DR_MOVE
 * packet of the same index; palette animations reload CLUT
 * canvas_x / 16 of MAP_CLUT_ROW_MAP from the palette frames at g_battle_map_palette_animation_frames. */
typedef struct battle_map_texture_animation {
    s16 canvas_x;   /* 0x00 */
    s16 canvas_y;   /* 0x02 */
    s16 width;      /* 0x04 */
    s16 height;     /* 0x06 */
    s16 frame_x;    /* 0x08 */
    s16 frame_y;    /* 0x0a */
    u8 column;      /* 0x0c */
    u8 row;         /* 0x0d */
    u8 mode;        /* 0x0e; map_texture_animation_mode_e */
    u8 frame_count; /* 0x0f */
    u8 frame;       /* 0x10 */
    u8 duration;    /* 0x11 */
    u8 timer;       /* 0x12 */
    u8 active;      /* 0x13 */
} battle_map_texture_animation_t;
typedef char battle_map_texture_animation_size_must_be_0x14[(sizeof(battle_map_texture_animation_t) == 0x14) ? 1 : -1];

/* Lightning flash state at 0x800f79c0 (g_battle_map_lightning_state).
 * battle_map_set_light_matrices fills color_matrices[0] and [2]; the lightning weather commands clear the rotation part
 * of color_matrices[1] and start the 60-frame flash timer. */
typedef struct battle_lightning_state {
    u32 timer;                                                /* 0x00 */
    MATRIX color_matrices[3];                                 /* 0x04 */
    u8 _unused_64[0x20];                                      /* 0x64 */
    CVECTOR base_colors[MAP_BACKGROUND_GRADIENT_COLOR_COUNT]; /* 0x84 */
    CVECTOR colors[MAP_BACKGROUND_GRADIENT_COLOR_COUNT];      /* 0x8c */
} battle_lightning_state_t;
typedef char battle_lightning_state_size_must_be_0x94[(sizeof(battle_lightning_state_t) == 0x94) ? 1 : -1];

/* Nested records in GNS_RESOURCE_STATE_RECORDS, a mesh-file chunk at slot 0x23. */
typedef enum map_mesh_animation_record_type {
    MAP_MESH_ANIMATION_RECORD_END = 0,
    MAP_MESH_ANIMATION_RECORD_KEYFRAMES = 1,
    MAP_MESH_ANIMATION_RECORD_INSTRUCTIONS = 2,
    MAP_MESH_ANIMATION_RECORD_PART_PROPERTIES = 3,
} map_mesh_animation_record_type_e;

/* Record header parsed by the map-state loader at 0x800f26bc: a type word,
 * an entry count and the packed entries. */
typedef struct battle_map_state_record {
    u32 type;  /* map_mesh_animation_record_type_e */
    u32 count; /* types 1/3: entry count; type 2: packed instruction/state dimensions */
    u8 data[4];
} battle_map_state_record_t;

/* Packed lighting: two nine-halfword matrices followed by three RGB colors. */
typedef struct battle_map_light_resource {
    s16 colour[3][3];      /* 0x00 */
    s16 direction[3][3];   /* 0x12 */
    u8 ambient[3];         /* 0x24 */
    u8 gradient_top[3];    /* 0x27 */
    u8 gradient_bottom[3]; /* 0x2a */
} battle_map_light_resource_t;

/* Polygon-render properties: the 0x380-byte prefix is not consumed here. */
typedef struct battle_map_polygon_flags_resource {
    u8 _unused_0000[0x380];
    u16 textured_triangles[512];  /* 0x0380 */
    u16 textured_quads[768];      /* 0x0780 */
    u16 untextured_triangles[64]; /* 0x0d80 */
    u16 untextured_quads[256];    /* 0x0e00 */
} battle_map_polygon_flags_resource_t;
typedef char battle_map_polygon_flags_resource_size_must_be_0x1000[sizeof(battle_map_polygon_flags_resource_t) == 0x1000
        ? 1
        : -1];

/* resource_type selects a serialized prefix or block; file offsets locate
 * variable-length chunks within the loaded resource. */
typedef union battle_map_resource {
    battle_map_mesh_geometry_t geometry;
    battle_map_polygon_flags_resource_t polygon_flags;
    map_mesh_file_header_t mesh_file;
    battle_map_light_resource_t lighting;
    battle_map_terrain_resource_t terrain;
    battle_map_texture_animation_t texture_animations[32];
    battle_map_state_record_t state_record;
    gns_record_view_t gns_records[150];
    gns_command_record_prefix_t command;
    s16 command_halfwords[10]; /* Halfword view of the 20-byte command prefix. */
    clut_t palette_colors;
    u32 texture_words[0]; /* Variable-length image data consumed by LoadImage. */
    u8 bytes[0];          /* Byte offsets in files and variable-length command payloads. */
} battle_map_resource_t;

extern u8 g_battle_map_background_color[];
extern u8 g_battle_map_background_color_backup[];
extern battle_map_resource_t* g_battle_map_data_load_buffer;
extern s32 g_battle_map_data_load_complete;
extern s32 g_battle_map_deep_dungeon_load_in_progress;
extern char g_battle_map_directory_path[];
extern u32 g_battle_map_gns_lba[MAP_ID_COUNT]; /* 0 where the map id has no file */
extern u32 g_battle_map_gns_record_cursor;
extern gns_record_view_t g_battle_map_gns_records[150];
extern gns_record_view_t g_battle_map_selected_mesh_record;
extern s32 g_battle_map_id;
extern u8 g_battle_map_max_x;
extern u8 g_battle_map_max_y;
extern battle_render_buffer_t* g_battle_map_mesh_last_render_buffer;
extern battle_map_resource_t* g_battle_map_mesh_load_buffer;
extern s32 g_battle_map_mesh_load_state;
extern map_move_find_item_data_t g_battle_map_move_find_item_data[MAP_ID_COUNT];
extern map_move_find_item_data_t g_battle_current_map_move_find_item_data;
extern u16 g_battle_map_palette_modified_colors[][MAP_CLUT_ROW_COLOR_COUNT]; /* [map_clut_row_e][colour] */
extern s32 g_battle_map_resource_load_state;                                 /* map_resource_load_state_e */
extern s32 g_battle_map_texture_animation_handles[];
extern map_texture_animation_state_t g_battle_map_texture_animation_states[];
extern s32 g_battle_map_textured_quad_count;
extern s32 g_battle_map_textured_quad_render_record_count;
extern s32 g_battle_map_textured_triangle_count;
extern s32 g_battle_map_textured_triangle_render_record_count;
extern s32 g_battle_map_tilt_target;
extern s32 g_battle_map_untextured_quad_count;
extern s32 g_battle_map_untextured_quad_render_record_count;
extern s32 g_battle_map_untextured_triangle_count;
extern s32 g_battle_map_untextured_triangle_render_record_count;
extern s32 g_battle_map_zoom_target;
extern palette_t g_battle_map_weather_particle_palette;
extern MATRIX g_battle_map_light_color_matrix;
extern MATRIX g_battle_map_light_matrix;
extern SVECTOR g_battle_map_light_rotation;
extern s32 g_battle_map_back_color_red;
extern s32 g_battle_map_back_color_green;
extern s32 g_battle_map_back_color_blue;
extern s16 g_battle_map_weather_effect_mode; /* battle_map_weather_effect_mode_e */
extern VECTOR g_battle_map_camera_zoom;
extern u8 g_battle_map_back_color_disabled;
extern MATRIX g_battle_map_camera_matrix;
extern s32 g_battle_map_command_0x96_frame;
extern MATRIX g_battle_map_light_rotation_matrix;

/* Untextured map polygons go to OT slot (otz & mask) | fixed index: {0, 4}
 * normally, {-1, 0} while map effect data is active (command 0x88), so a
 * non-zero mask also marks that state. */
extern s32 g_battle_map_untextured_fixed_ot_index;
extern s32 g_battle_map_untextured_otz_mask;
extern s32 g_battle_map_command_0x96_duration;
extern s16 g_battle_map_destruction_wait_state;
extern u16 g_battle_map_refresh_pending;
extern battle_move_found_item_t g_battle_map_rare_item_result;
extern map_selected_tile_t g_battle_map_selected_tile_data;
extern RECT g_battle_tile_info_image_location;
extern world_gfx_image_load_parameters_t g_battle_tile_info_image_params[2];
extern CVECTOR g_battle_map_white_cvector;

/* Default vectors shared with battle_process_map_gns_record, which emits them. */
extern SVECTOR g_battle_map_zero_svector;
extern VECTOR g_battle_map_unit_scale_vector;
extern u8 g_battle_map_weather_primary_speed;
extern u8 g_battle_map_rain_secondary_fall_speed;
extern battle_render_buffer_t* g_battle_map_ambient_restored_buffer;
extern battle_weather_particle_t g_battle_map_rain_splash_particles[32];
extern RECT g_battle_map_weather_drop_uv_rects[3];
extern RECT g_battle_map_weather_splash_uv_rects[4];
extern battle_weather_particle_t g_battle_map_weather_particles[64];
extern u16 g_battle_map_append_state; /* 0x80165fe0 */

/* 1 starts, 2 polls, >=3 is reset to zero by the outer update. */
extern u16 g_battle_map_reload_state; /* 0x80165fde */

/* Battle terrain: two layers of 256 records, indexed
 * (layer * 256 + y * width + x). Callers that walk it with raw byte offsets
 * cast to u8*; the record shape is map_tile_t. */
extern map_tile_t g_battle_map_tile_data[];
extern map_palette_state_t g_battle_map_palette_state;

/* GNS records selected for loading and the ones currently loaded, indexed by
 * resource type. */
extern gns_record_view_t g_battle_map_pending_gns_records[GNS_RESOURCE_TABLE_COUNT];
extern gns_record_view_t g_battle_map_loaded_gns_records[GNS_RESOURCE_TABLE_COUNT];

/* The applied background gradient pair at 0x801251c8. The copies at
 * 0x800e81d0, 0x800e8228 and 0x800e827c move each colour as an independent
 * unaligned word (lwl/lwr + swl/swr). */
extern map_background_gradient_colors_t g_battle_map_background_gradient_colors;
extern CVECTOR g_battle_map_ambient_light_color;
extern CVECTOR g_battle_map_darkness_color;
extern map_gradient_transition_t g_battle_map_background_gradient_transition;
extern map_color_transition_t g_battle_map_ambient_light_transition;
extern map_color_transition_t g_battle_map_darkness_transition;
extern battle_map_mesh_part_t g_battle_map_mesh_parts[9];
extern battle_map_mesh_keyframe_t g_battle_map_mesh_animation_keyframes[];
extern battle_map_mesh_instructions_t g_battle_map_mesh_animation_instructions[9];
extern battle_map_texture_animation_t g_battle_map_texture_animations[32];
extern DR_MOVE g_battle_map_texture_animation_moves[32];
extern SVECTOR g_battle_map_camera_rotation;
extern battle_lightning_state_t g_battle_map_lightning_state;
extern rgb24_t g_battle_map_ambient_polygon_color;
extern CVECTOR g_battle_map_cached_back_color;
extern SVECTOR g_battle_map_command_0x96_rotation;
extern u8 g_battle_map_gns_dispatch_held;
extern s32 g_battle_map_gns_load_phase; /* map_gns_load_phase_e */
extern CVECTOR g_battle_map_current_darkness_color;
extern u16 g_battle_map_light_transition_command;               /* pending per-frame light command, 0 when idle */
extern u16 g_battle_map_light_transition_duration;              /* transition duration in frames */
extern u16 g_battle_map_light_transition_frame;                 /* transition frame counter */
extern MATRIX g_battle_map_light_transition_start_color_matrix; /* transition start colour matrix */
extern MATRIX g_battle_map_light_transition_start_light_matrix; /* transition start light matrix */

extern MATRIX
    g_battle_map_light_transition_target_color_matrix; /* transition target (0x66) or delta (0x68) colour matrix */

extern MATRIX
    g_battle_map_light_transition_target_light_matrix; /* transition target (0x66) or delta (0x68) light matrix */

extern u8 g_battle_map_lighting_pending;
extern s32 g_battle_map_lightning_mode;
extern s32 g_battle_map_loaded_id;
extern MATRIX g_battle_map_local_light_matrix;
extern s32 g_battle_map_lowest_surface_y;
extern s32 g_battle_map_mesh_part_animation_states[];
extern const battle_map_mesh_geometry_t* g_battle_map_mesh_part_geometry[9];
extern clut_t g_battle_map_palette_animation_frames;
extern s8 g_battle_map_palette_blend_steps_32[][32];
extern s8 g_battle_map_palette_blend_steps_8[][8];
extern u8 g_battle_map_palette_pending;
extern battle_map_mesh_triangle_positions_t* g_battle_map_part_textured_triangle_positions;
extern battle_map_mesh_quad_positions_t* g_battle_map_part_textured_quad_positions;
extern battle_map_mesh_triangle_positions_t* g_battle_map_part_untextured_triangle_positions;
extern battle_map_mesh_quad_positions_t* g_battle_map_part_untextured_quad_positions;
extern battle_map_mesh_triangle_normals_t* g_battle_map_part_textured_triangle_normals;
extern battle_map_mesh_quad_normals_t* g_battle_map_part_textured_quad_normals;
extern s32 g_battle_map_part_textured_quad_count;
extern s32 g_battle_map_part_textured_triangle_count;
extern s32 g_battle_map_part_untextured_quad_count;
extern s32 g_battle_map_part_untextured_triangle_count;
extern battle_map_mesh_triangle_positions_t g_battle_map_textured_triangle_positions[];
extern battle_map_mesh_quad_positions_t g_battle_map_textured_quad_positions[];
extern battle_map_mesh_triangle_positions_t g_battle_map_untextured_triangle_positions[];
extern battle_map_mesh_quad_positions_t g_battle_map_untextured_quad_positions[];
extern battle_map_mesh_triangle_normals_t g_battle_map_textured_triangle_normals[];
extern battle_map_mesh_quad_normals_t g_battle_map_textured_quad_normals[];
extern u8 g_battle_map_pending_lighting_data[0x2d];
extern u8 g_battle_map_pending_palette_data[0x200];
extern u8 g_battle_map_polygon_flag_clear_countdown;
extern s32 g_battle_map_primary_textured_quad_count;
extern s32 g_battle_map_primary_textured_triangle_count;
extern s32 g_battle_map_primary_untextured_quad_count;
extern s32 g_battle_map_primary_untextured_triangle_count;
extern s32 g_battle_map_saved_back_color_blue;
extern s32 g_battle_map_saved_back_color_green;
extern s32 g_battle_map_saved_back_color_red;
extern CVECTOR g_battle_map_saved_background_gradient_first;
extern CVECTOR g_battle_map_saved_background_gradient_second;
extern MATRIX g_battle_map_saved_light_color_matrix; /* saved colour matrix (command 0x5f/0x60) */
extern MATRIX g_battle_map_saved_light_matrix;       /* saved light matrix (command 0x5f/0x60) */
extern u8 g_battle_map_saved_texture_animation_modes[];
extern u8 g_battle_map_texture_pending;
extern u8 g_battle_map_texture_quarter_0_pending;
extern u8 g_battle_map_texture_quarter_1_pending;
extern u8 g_battle_map_texture_quarter_2_pending;
extern u8 g_battle_map_texture_quarter_3_pending;
extern map_tile_t* g_battle_map_tile_data_ptr;
extern u8 g_battle_map_tile_depth;
extern u8 g_battle_map_tile_width;
extern RECT g_battle_map_vram_load_rect;
extern u8 g_battle_map_weather_fall_speed;

/* Map weather flags: bit 1 suppresses weather, bit 0 marks snow. */
extern s32 g_battle_map_weather_flags;
extern MATRIX g_battle_map_weather_matrix;
extern SVECTOR g_battle_map_weather_rotation;
extern VECTOR g_battle_map_weather_scale;
extern s32 g_battle_map_weather_sprite_scale;
extern VECTOR g_battle_map_weather_translation;
void battle_map_blend_darkness_color(s32 mode, s32 frame_duration, s32 red, s32 green, s32 blue);
s32 battle_map_calculate_location(battle_stats_t* unit);
s32 battle_map_calculate_move_find_item_flag(s32 map_id, s32 tile_index, s32 mode);
s32 battle_map_get_tile_data_value(s32 field, s32 x, s32 y, s32 layer);
void battle_map_init_ambient_light(void);
void battle_map_init_background_gradient(void);
void battle_map_start_darkness_blend(s32 first, s32 second, s16 third, s16 fourth, s16 fifth);
s32 battle_map_check_deep_dungeon_crystal_and_init_state(void);
void battle_map_color_field(s32 first, s32 second, s16 red, s16 green, s16 blue);

/* Only SET/GET define a result (stored RGB pointer, or null for SET while
 * disabled). Other command paths leave the target return register unspecified. */
s32* battle_map_control_gte_background_color(s32 command, s32 red, s32 green, s32 blue);
void battle_map_polygon_flag_command(s32 command);
MATRIX* battle_map_light_matrix_command(s32 command, MATRIX* color, MATRIX* light, s32 duration);
void battle_map_data_load_complete_thread(void);
s32 battle_map_dispatch_gns_resource(u8 resource_type, battle_map_resource_t* data);
void battle_map_extract_palette_colors(const palette_t palette, s32 clut_row, s32 clut_index);
s32 battle_map_get_effective_weather(void);
map_tile_t* battle_map_get_tile_data_pointer(s32 x, s32 y, u32 layer);
s32 battle_map_set_tile_data_value(s32 field, s32 x, s32 y, s32 layer, s32 value);
map_tile_t* battle_map_get_tile_data_ptr_from_misc_id(u32 misc_id);
void battle_map_init_palette_state(void);
s32 battle_map_start_data_load(void);
s32 battle_map_poll_data_load(void);
void battle_map_load_move_find_item_data(map_move_find_item_data_t* map_data);
void battle_map_mark_data_load_complete(void);
void battle_map_modify_background_gradient(s32 mode, s32 frame_duration, s16 red, s16 green, s16 blue);
void battle_map_reset_texture_animations(void);
void battle_map_set_background_gradient(s32 frame_duration, const map_background_gradient_colors_t* colors);
void battle_map_set_command_0x96_duration(s32 id, s32 value);
void battle_map_start_ambient_light_blend(s32 mode, s32 frame_duration, s16 red, s16 green, s16 blue);
void battle_map_set_darkness_color(s32 frame_duration, const CVECTOR* color);
s32 battle_map_step_init_sequence(s32 map_id, s32 step);
s32 battle_map_stop_texture_animation(s32 handle);
void battle_map_store_modified_palette_colors(s32 clut_row, s32 clut_index, s32 store_all);
void battle_map_store_selected_tile_coordinates(SVECTOR* destination);
void battle_map_store_selected_tile_data(void);
s32 battle_map_try_start_deep_dungeon_mesh_load(void);
s32 battle_map_update_deep_dungeon_and_animation(void);
void battle_map_wait_for_refresh(void);

void battle_map_modify_palette(
    s32 command, s32 amount, s32 clut_row, s32 clut_index, s32 all_colors, s16 red, s16 green, s16 blue);

s32 battle_map_calculate_slope_height(const SVECTOR* coords, s32 map_z);
s32 battle_map_resolve_rotated_offset_layer(battle_stats_t*, s32, s32, s32, s32*, s32*, s32*);
s32 battle_map_load_data(s32 map_id, s32 command);
void battle_map_blend_background_gradient_color(s32 mode, s32 frame_duration, s32 red, s32 green, s32 blue);
void battle_map_blend_ambient_light_color(s32 mode, s32 frame_duration, s32 red, s32 green, s32 blue);
void battle_map_update_palette_colors_inner(s32 mode, s32 clut_row, s32 clut_index, const palette_t colors);
battle_move_found_item_t* battle_map_determine_rare_common_item(battle_stats_t* unit);
battle_move_find_result_data_t* battle_map_set_item_trap_data(s32 x, u8 y, u8 layer);
s32 battle_process_map_gns_record(s32 phase, battle_map_resource_t* record);
void battle_map_update_animations(void);
void battle_map_upload_clut_rows(void);
void battle_map_draw_mesh_and_weather(MATRIX* camera_matrix);
void battle_map_freeze(void);
void battle_map_unfreeze(void);
s32 battle_map_dispatch_map_data_command(s32 command, u32 index, s32 object_state, s32 flag);
void battle_map_light_thread(void);
void battle_map_transition_ambient_light_color(s32 frame_duration, const CVECTOR* color);

void battle_map_transition_and_store_background_gradient(
    s32 frame_duration, const map_background_gradient_colors_t* colors);

void battle_map_transition_background_gradient(s32 frame_duration, const map_background_gradient_colors_t* colors);
void battle_map_transition_darkness_color(s32 frame_duration, const CVECTOR* color);

/* Map data, palette and mesh interfaces implemented in the battle overlay. */
void battle_map_init_darkness(void);
s16 battle_map_is_gns_record_matching_state(s32 comparison, s16 record_state, s16 requested_state);
void battle_map_update_all_palettes_from_color_data(s32 mode, const clut_t color_data);
void battle_map_update_palette_colors_by_index(s32 mode, s32 clut_index, const palette_t* color_data);
void battle_map_update_and_store_palette_colors(
    s32 mode, s32 clut_row, s32 clut_index, s32 update_all, const palette_t* colors);
void battle_map_load_palette_data(const palette_t* palettes, s32 clut_row, s32 clut_index, s32 load_all);
void* battle_map_start_file_load(
    const char* unused_directory_path, gns_load_info_t* load_info, void* destination, s32 unused_resource_type);
s32 battle_map_stop_texture_animations_and_weather_sfx(void);
void battle_map_store_max_coordinates(SVECTOR* destination);

void battle_map_update_palette_colors(s32 mode, s32 clut_row, s32 clut_index, s32 update_all, const palette_t* colors);

battle_map_state_record_t* battle_map_load_saved_state_record(battle_map_state_record_t* record);
void* battle_map_light_state_command(s32 command, void* data);
void battle_map_append_mesh_geometry(const battle_map_mesh_geometry_t* geometry_data, battle_map_mesh_part_t* metadata);
/* A typed terrain parameter loses the first retail byte-cursor increment. */
void battle_map_copy_xy_coords_and_tile_data(u8* data);
map_tile_t* battle_map_get_tile_data_ptr_from_battle_id(u32 battle_id);
map_tile_t* battle_map_get_tile_data_ptr_from_misc_screen_coords(u32 misc_id);
s32 battle_move_get_weather_terrain_cost(void);
s32 battle_map_init_units_sprites_event_and_music(s32 map_id, s32 step, s32 deployment_running);
s32 battle_map_load_mesh_variant(s32 palette_mode);
void battle_map_queue_textured_quads(u32* otag, POLY_GT4* prims, s32 yaw, s32* unused_count);
void battle_map_queue_textured_triangles(u32* otag, POLY_GT3* prims, s32 yaw, s32* unused_count);
void battle_map_queue_untextured_quads(u32* otag, POLY_F4* prims, s32 yaw, s32* unused_count);
void battle_map_queue_untextured_triangles(u32* otag, POLY_F3* prims, s32 yaw, s32* unused_count);
s32 battle_map_refresh_deep_dungeon(void);
s32 battle_map_command_set_3d_object_state(s32 value_a, s32 value_b);
s32 battle_map_command_start_texture_animation(s32 value_a, s32 value_b);
void battle_map_set_light_matrices(MATRIX* first, MATRIX* second);
void battle_map_run_weather_command(s32 command);
s32 battle_map_start_texture_animation(s32 frame_duration, s32 clut_row, s32 first_color, s32 last_color);
void battle_map_update_mesh_part_animations(void);

void battle_map_update_palette_from_other(
    const rgba5551_t* source, s32 clut_row, s32 clut_index, s32 preset, s16 red, s16 green, s16 blue, s32 final_value);

void battle_map_update_texture_animations(void);
void battle_map_init_mesh_render_records(const battle_map_mesh_geometry_t* geometry_data);
void battle_map_init_weather_particles(s32 command);
gns_record_view_t* battle_map_load_gns_and_move_find_items(s32 map_id);
void battle_map_reset_mesh_render_state(void);

/* menu */
/* One EVTFACE.BIN portrait block; both GPU uploads use 64 halfword columns. */
typedef struct battle_menu_portrait_image_file {
    u32 pixels[48][32]; /* 0x0000 */
    u32 palette[2][32]; /* 0x1800 */
    u8 _unknown_1900[0x700];
} battle_menu_portrait_image_file_t;

/* BATTLE scratch at 0x801cc074, in the event-overlay area past BATTLE.BIN:
 * battle_menu_build_sprite_page lays it out as two sprite pages and the
 * scrolling ability list saves its five 16-colour CLUTs there. WORLD keeps
 * its own g_world_menu_panel_buffer at the same address. */
typedef union battle_menu_scratch_buffer {
    menu_sprite_page_t sprite_pages[2];
    u16 clut_save[5 * 16];
} battle_menu_scratch_buffer_t;

/* The numeric editor's two adjacent rectangles: the first is the panel frame
 * and the screen origin handed to image loading, the second places the
 * portrait. */
typedef struct battle_menu_status_panel_numeric_geometry {
    RECT texture_rect;   /* 0x00 */
    RECT draw_area_rect; /* 0x08 */
} battle_menu_status_panel_numeric_geometry_t;

/* Origin and row geometry used by the EVENT menu text image renderers. */
typedef struct battle_menu_text_image_bounds {
    u16 x;
    u16 y;
    u16 row_stride;
    s16 line_height;
} battle_menu_text_image_bounds_t;

/* Pen of the BUNIT, CARD, EQUIP and JOBSTTS *_text_render_glyph_to_4bpp_image: x in pixels (two per byte),
 * y in rows, and the image's row stride in halfwords. */
typedef struct bunit_text_image_position {
    s16 x;          /* 0x00 */
    s16 y;          /* 0x02 */
    s16 row_stride; /* 0x04 */
} bunit_text_image_position_t;
typedef char bunit_text_image_position_size_must_be_6[(sizeof(bunit_text_image_position_t) == 6) ? 1 : -1];

extern battle_menu_allocation_t g_battle_menu_allocator_slot_records[16];
extern u8 g_battle_menu_buffer_arena[53][0x100];
extern u8 g_battle_menu_buffer_allocation_map[53];
extern s32 g_battle_menu_companion_executable_loading;
extern s32 g_battle_menu_companion_executable_request_index;
extern u8 g_battle_menu_calculator_skillsets[4];
extern s32 g_battle_menu_current_id; /* battle_menu_id_e */
extern s32 g_battle_menu_help_open;
extern s32 g_battle_menu_help_opening;
extern menu_number_entry_t g_battle_menu_hovered_unit_stats_setup[10];
extern RECT g_battle_menu_hovered_unit_stats_display;
extern world_menu_entry_t g_battle_menu_idle_action_entries[];
extern u16 g_battle_menu_input_mode; /* menu_input_mode_e */
extern u8 g_battle_menu_numeric_display_frame_offsets[12][8];
extern u8 g_battle_menu_numeric_display_palette_indices[12];
extern world_panel_record_t g_battle_menu_numeric_display_panel_records[2];
extern RECT g_battle_menu_numeric_display_texture_window;
extern s32 g_battle_menu_packet_buffer_index;
extern menu_palette_t g_battle_menu_palette_colors;
extern s32 g_battle_menu_panel_fade_intensity;
extern s32 g_battle_menu_panel_fade_mode; /* menu_panel_fade_mode_e */
extern RECT g_battle_menu_portrait_palette_rect;
extern RECT g_battle_menu_portrait_pixel_rect;
extern u8 g_battle_menu_restore_pending;
extern menu_number_entry_t g_battle_menu_right_unit_stats_setup[10];
extern RECT g_battle_menu_right_unit_stats_display;
extern s32 g_battle_menu_companion_data_file_request_index;
extern s32 g_battle_menu_selected_skill_ct;
extern s32 g_battle_menu_slot_owner_thread_ids[3];
extern s32 g_battle_menu_slot_states[3];
extern s32 g_battle_menu_status_requested;
extern s32 g_battle_menu_status_screen_selected;
extern u16 g_battle_menu_transition_state;
extern menu_selection_remap_t g_battle_menu_selection_remaps[8];
extern battle_menu_record_t g_battle_menu_unit_selection_records[BATTLE_UNIT_SLOT_COUNT];
extern menu_text_state_t g_menu_text_state;
extern battle_unit_status_record_t g_battle_menu_active_turn_banner;

/* 0x40-byte billboard block at 0x8014d0a0 saved and restored whole with the AT
 * banner: unit stats indexed as halfwords by
 * battle_menu_load_selected_unit. */
extern battle_menu_status_panel_slot_storage_t g_battle_menu_billboard_data;
extern u8 g_battle_menu_billboard_skillset_name[];
extern u8 g_battle_menu_billboard_secondary_skillset_name[];
extern u8 g_battle_menu_billboard_unit_name[];
extern u8 g_battle_menu_billboard_job_name[];
extern u16 g_battle_menu_ability_display_flags[80];
extern s32 g_battle_menu_used_skillset_id;
extern s32 g_battle_menu_used_item_id;
extern s32 g_battle_menu_system_function_command;
extern s32 g_battle_menu_system_function_option;
extern u8 g_battle_menu_restore_depth;
extern u8 g_battle_menu_restore_unit_id;
extern battle_menu_id_record_t g_battle_menu_id_records[31];
extern menu_glyph_sheet_t g_battle_menu_glyph_image_data;
extern menu_palette_t g_battle_menu_default_palette_colors;
extern RECT g_battle_menu_disabled_texture_window;
extern s32 g_battle_menu_scroll_list_depth;
extern u16 g_battle_menu_status_visible_rows;
extern u16 g_battle_menu_status_scroll_rows;

/* Row actions of the auto-battle setting menu (menu entry 0x80166b4c, +0x24). */
extern s16 g_battle_menu_auto_battle_row_actions[8];
extern s16 g_battle_menu_action_slot_row_actions[6];
extern s16 g_battle_menu_wait_row_actions[4];
extern u16 g_battle_menu_status_list_row_actions[8];
extern u16 g_battle_menu_system_function_row_actions[];
extern s16 g_battle_menu_confirm_stored_unit_row_actions;
extern s16 g_battle_menu_set_auto_battle_row_actions;
extern u16* g_battle_menu_window_tween_easing_tables[];
extern u16 g_battle_menu_hovered_stats_frame_count;
extern RECT g_battle_menu_sprite_page_texture_windows[5];
extern world_gfx_image_load_parameters_t g_battle_menu_sprite_page_image_params[30];
/* Entries 5..7 above; a separate alias preserves the original template base. */
extern world_gfx_image_load_parameters_t g_battle_menu_sprite_page_image_param_bases[];
extern RECT g_battle_menu_attack_caster_stats_display;
extern RECT g_battle_menu_attack_target_stats_display;
extern menu_number_entry_t g_battle_menu_attack_caster_stats_setup[8];
extern menu_number_entry_t g_battle_menu_attack_target_stats_setup[8];
/* The final two entries of g_battle_menu_attack_caster_stats_setup. */
extern menu_number_entry_t g_battle_menu_projected_value_panel[2];
extern s32 g_battle_menu_companion_executable_lbas[];
extern s32 g_battle_menu_companion_executable_sizes[];
extern s32 g_battle_menu_companion_executable_offsets[];
extern s32 g_battle_menu_companion_data_file_lbas[];
extern s32 g_battle_menu_companion_data_file_sizes[];
extern s32 g_battle_menu_companion_data_file_loading;
extern u8 g_battle_menu_idle_action_entry_labels[];
extern s32 g_battle_menu_building_thread_started;
extern u8 g_battle_menu_panel_fade_modes[];
extern u8 g_battle_menu_idle_action_flag_clear_ids[];
extern s16 g_battle_menu_status_left_text_ids[20];
extern s16 g_battle_menu_status_right_text_ids[20];
extern s32 g_battle_menu_action_menu_build_result;
extern s16 g_battle_menu_selected_option;
extern battle_action_data_t* g_battle_menu_preview_target_action;
extern world_fade_tile_frame_t g_battle_menu_panel_fade_frames[];
extern battle_menu_scratch_buffer_t g_battle_menu_scratch_buffer;
extern u16* const* g_battle_help_active_text_id_tables;
extern u16 g_battle_menu_anything_ability_id;
extern s16 g_battle_menu_action_slot_selected_option;
extern s32 g_battle_active_menu; /* active menu */
/* Handed to the dead-unit panel thread as its second parameter. Nothing stores to it and the thread
 * does not read the parameter. */
extern s32 g_battle_menu_dead_unit_panel_parameter;
extern RECT g_battle_menu_ability_list_clut_rect;
extern s16 g_battle_menu_ability_scroll_offset;
/* row_offset of the status list's scroll layout, which starts at g_battle_menu_status_visible_rows */
extern s16 g_battle_menu_status_scroll_offset;
extern s16 g_battle_menu_action_type_modes[][2];
extern menu_window_buffer_t* g_battle_menu_buffer_slots[6];
extern world_menu_quad_page_t g_battle_menu_height_display_pages[2];
extern u16 g_battle_menu_height_display_suppressed;
extern s32 g_battle_menu_hide_numeric_values; /* non-zero: draw digits as blanks (0x48, 0x10) */
extern world_gfx_image_load_parameters_t g_battle_menu_icon_strip_image_params[20];
extern u16 g_battle_menu_input_block_frames;
extern glyph_blit_t g_battle_menu_stacked_glyph_lower;
extern glyph_blit_t g_battle_menu_stacked_glyph_upper;
extern glyph_blit_t g_battle_menu_window_sprite_glyph;
extern glyph_blit_t g_battle_menu_window_sprite_position;
extern glyph_blit_t g_battle_menu_number_glyph_rect; /* glyph source rectangle in the number/sign font */
extern s16 g_battle_menu_pending_selection[4];       /* 4 shorts */
extern u8 g_battle_menu_quad_group_starts[];         /* first piece index per group; [n + 1] ends group n */
extern s32 g_battle_menu_resolved_command;
extern u8 g_battle_menu_restriction_message_ids[];
extern s16 g_battle_menu_result;            /* menu result: -4 pending, -1 cancelled */
extern s16 g_battle_menu_row_message_ids[]; /* per-row message id */

extern battle_ai_command_t g_battle_menu_selected_command;
/* Action command staged by the battle menu and passed whole to
 * battle_action_preview_at_list; the WORLD twin is g_world_menu_preview_action. */
extern battle_action_command_t g_battle_menu_selected_action;
extern world_gfx_image_load_parameters_t g_battle_menu_single_icon_image_params[5];
extern u8 g_battle_menu_skillset_disabled_flags[]; /* per-row greyed flag */
extern battle_unit_status_record_t g_battle_menu_status_billboard;

/* Twelve crop percentages, indexed by step. */
extern u16 g_battle_menu_zoom_percentages[];
extern s32 g_battle_menu_text_param_22; /* 0x80166020: record +0x22 */
extern s32 g_battle_menu_text_param_24; /* 0x80166024: record +0x24 */
extern point16_t g_battle_menu_texture_location;
extern u16 g_option_menu_open;
extern u16 g_battle_menu_suspend_state;
extern u16 g_option_menu_transition_finished;
extern s32 g_battle_menu_status_enabled;

/* Source image used when battle overlays render menu glyphs. */
extern menu_glyph_sheet_t* g_battle_menu_glyph_image;
extern s32 g_battle_menu_text_section_offsets[];
extern world_menu_scroll_text_layout_t g_battle_menu_at_list_layout;
extern world_menu_quad_piece_t g_battle_menu_quad_pieces[]; /* quad piece table */
extern world_menu_quad_colors_t g_battle_menu_quad_colors;  /* vertex colours */
extern point16_t g_battle_menu_quad_origin;                 /* screen origin */
extern glyph_blit_t g_battle_menu_text_pen;
extern world_unit_editor_frame_t g_battle_menu_unit_editor_frames[2];

/* Full record bases, not the legacy aliases biased to thread_id. */
extern menu_window_buffer_t g_battle_menu_window_buffers[6]; /* 0x8016e43c */
extern s32 g_battle_menu_text_palette_offset;

/* BATTLE's current menu-entry table pointer at 0x80173cb8 (menu threads are
 * started on &table[i]): battle_menu_init_subsystem_pointers points it at
 * g_battle_menu_idle_action_entries, and the event overlays that run over
 * BATTLE swap in their own tables and restore it. WORLD's twin is
 * g_world_menu_thread_menu_data. */
extern world_menu_entry_t* g_battle_menu_thread_menu_data;

void battle_menu_display_hovered_unit_stats(
    RECT* frame_rect, s32 mode, menu_number_entry_t* entries, battle_unit_status_record_t* status);

void battle_handle_menu_cancel_input(world_menu_entry_t* menu);
void* battle_menu_alloc_buffer(s32 bytes);
void* battle_menu_alloc_memory(s32 size);
u8* battle_menu_build_and_upload_window_frame_image(s32 width, s32 height, RECT* rect, s32 mode);
void battle_menu_build_idle_action_menu(s32 menu_id);
s32 battle_menu_build_quad_pieces(s32 group, POLY_GT4* poly);
void battle_menu_build_skillset_entries(void);
void battle_menu_build_unit_status_list(void);
void battle_menu_build_window_image_row(const u16* tiles, u16* row, s32 count, s32 index);
void battle_menu_run_helpmenu_thread(void);
s32 battle_menu_can_accept_input(void);
void battle_menu_check_action_restrictions(void);
void battle_menu_clear_entry_selections(void);
void battle_menu_close_move_help(void);
void battle_menu_dispatch_idle_action_menu(void);
void battle_menu_display_triangle_selection(void);
void battle_menu_draw_text_columns_narrow(world_menu_entry_t* entry, s32* row_offset, u8* buffer);
void battle_menu_enter_status_screen_selection(void);
void battle_menu_fade_out_thread(void);
void battle_menu_free_high_overlay(void);
extern menu_cursor_bob_entry_t g_battle_cursor_bob_thresholds[];
extern menu_cursor_bob_entry_t g_battle_cursor_bob_active_thresholds[];
s32 battle_menu_get_cursor_bob_offset(s32 mode);
battle_ai_command_t* battle_menu_get_selected_command_address(void);
void battle_menu_icon_linked_entry_thread(void);
void battle_menu_size_entry_to_text(world_menu_entry_t* entry, s16* width, s16* height, s32* out_pad, s32 pad_short);
s32 battle_state_try_enter_scenario_event(battle_unit_misc_data_t* unit);
void battle_menu_init_buffer_allocator(void);
s32 battle_menu_init_monster_skill_check(battle_stats_t* unit);
s32 battle_menu_has_monster_skill_supporter_at_tile(s32 team_flags, s32 x, s32 y, s32 total_height);
void battle_menu_init_sprite_array(SPRT* sprites, s32 count, s32 clut);
s32 battle_menu_get_id_based_on_mount_moveable_actable(s32 misc_id);
void battle_menu_init_subsystem_pointers(void);
void battle_menu_init_subsystems(void);
s32 battle_menu_init_system_function(s32 type, s32 option, s32 unit_id, s32 parameter, s32 enabled);
s32 battle_menu_is_still_building(void);
u16 battle_menu_load_elemental_abilities(s32 skillset_slot, u8 unit_id);
void battle_menu_leave_status_screen_selection(void);
s32 battle_menu_lerp_fixed12(s32 start, s32 end, s32 factor);
void battle_menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, RECT* from, RECT* to, s32 factor);
void battle_menu_open_active_unit_idle_action_menu(void);
void battle_menu_open_companion_executable(void);
void battle_script_dark_screen_thread(void);
void battle_menu_open_debugchr_unit_editor(void);
void battle_menu_open_free_cursor_help(void);
void battle_menu_open_free_cursor_mini_menu(void);
void battle_menu_open_mini_menu(void);
void battle_menu_open_move_confirm(void);
void battle_menu_open_non_active_unit_idle_action_menu(void);
void battle_menu_open_option_dead_unit_panel(void);
void battle_menu_run_jobstts_thread(void);
void battle_menu_refresh_hovered_unit_stats_display(void);
void battle_menu_request_open_small(void);
void battle_menu_request_open_companion_executable(s32 request_index);
void battle_menu_request_open_companion_data_file(s32 request_index);
void battle_menu_reset_buffer_slots(void);
void battle_menu_reset_unit_record(s32 record_index);
void battle_menu_reserve_high_overlay(u32 byte_length);
void battle_menu_reset_subsystems(void);
void battle_menu_run_numeric_display_panel_thread(void);
void battle_menu_run_skillset_thread(void);
void battle_menu_run_system_function_thread(void);
void battle_menu_selected_tile_info_display_thread(void);
void battle_menu_set_next_script_action_menus(void);
void battle_menu_set_option_transition_finished(void);
void battle_menu_start_building_thread(void);
void battle_menu_start_entry_thread(void);
void battle_menu_start_mini_menu_display_thread(void);
void battle_menu_start_system_function_thread(s32 system_function);
void battle_menu_load_selected_unit(void);
void battle_menu_update_panel_fade(void);
void battle_menu_draw_active_window_frames(void);
void battle_gfx_build_scaled_draw_area_pair_swapped(
    world_gfx_scaled_draw_area_pair_t* packets, const RECT* geometry, s32 step, s32 flag);
void battle_menu_build_ability_preview_at_list(void);
void battle_menu_run_scrolling_ability_list_thread(void);
void battle_menu_select_unit_action_slots_thread(void);
s32 battle_menu_resolve_selection(void);
s32 battle_menu_get_unit_action_menu_id(battle_stats_t* unit);
s32 battle_menu_load_item_abilities(s32 unit_id, u8 skillset, u8* out_item_ids);

s32 battle_menu_get_unit_skillset_ability_data(
    s32 unit_id, u8 skillset, s16* ability_ids, u8* mp_out, u8* ct_out, s32 unused, u8* flags_out, u8* turns_out);

s32 battle_menu_collect_throwable_items(s32 unit_id, u8 ability, u8* items);
void battle_menu_select_icon_cluts(struct world_menu_icon_record* frame);
void battle_clear_menu_render_buffer(void* buffer, s32 bytes);
menu_window_buffer_t* battle_menu_alloc_window_buffer_pair(void);
void battle_menu_build_ability_list(s32 mode);
void battle_menu_build_and_upload_window_image(s32 width, s32 height, RECT* rect, s32 mode, s32 tail_offset);
void battle_menu_build_sprite_page(s32 page_index, RECT* base_screen);
s32 battle_menu_build_unit_action_menus(s32 unit_id, u8* entries, u8* empty, u8* menu_types);
void battle_menu_build_unit_portrait_poly(POLY_FT4* poly, u32 battle_id);
u8* battle_menu_build_window_image(s32 width, s32 height, RECT* rect, s32 mode, s32 tail_offset, s32 keep);
s32 battle_menu_call_load_charge_skillset(s32 unit_id, u8 skillset_id, s16* ability_ids);
void battle_menu_check_action_slot_restrictions(s32 menu, s32 value, s32 check_unit);
void battle_menu_clear_status_menu_state(void);
s32 battle_menu_collect_calculator_abilities(s32 unit_id, u8 skillset, s16* out);
s32 battle_menu_collect_math_skill_abilities_by_flags(s32 unit_id, u8 skillset, s16* out, u8 mask);
s32 battle_menu_collect_monster_skill_abilities(s32 unit_id, u8 skillset, s16* out, s32 unused, u8* flags_out);
void battle_menu_confirm_action_silently(world_menu_entry_t* menu);
s32 battle_menu_copy_ff_terminated_bytes_to_halfwords(u16* dst, u8* src);
void battle_menu_copy_palette_colors(const menu_palette_t* source);
void battle_menu_copy_unit_data_to_status_billboard(battle_stats_t* unit, battle_unit_status_record_t* output);

s32 battle_menu_dispatch_system_function(
    s32 command, s32 option, s32 unit_id, s32 parameter, s32 enabled, battle_action_command_t* action);

s32 battle_menu_display_item_inventory_ability(s32 unit_id, s32 skillset, u8* out_item_ids);
s32 battle_menu_load_math_skill_attributes(s32 unit_id, s32 skillset, s16* out_ability_ids);

void battle_menu_draw_stacked_glyph_pair(u8* pixels, glyph_blit_t* position);
void battle_menu_draw_numeric_display_entries(
    u8* buffer, menu_number_entry_t* entries, glyph_blit_t* position, s32 count);

void battle_menu_release_window_buffer_pair(menu_window_buffer_t* buffer);
s32 battle_menu_get_dead_unit_selection(s32 battle_id);
s32 battle_unit_has_any_current_status(s32 unit_id);
s32 battle_menu_init_attack_resources_and_threads(void);
void battle_menu_init_numeric_display_frame_primitives(RECT* bounds, world_menu_palette_primitives_t* menu);
void battle_menu_init_primitive_colors_normal(world_menu_palette_primitives_t* menu);
void battle_menu_init_primitive_colors_dimmed(world_menu_palette_primitives_t* menu);
void battle_menu_init_semitransparent_sprt(SPRT* sprite);
void battle_menu_run_icon_selection_loop(void);
void battle_menu_select_sprite_clut(SPRT* sprite);
void battle_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* pair);
void battle_menu_submit_icon_primitives(struct world_menu_icon_record* frame);
void battle_menu_step_wrapping_cursor_on_scroll_buttons(world_menu_entry_t* ctx, s32* sel);

void battle_menu_update_icon_cursor_sprites(
    world_menu_entry_t* param, world_menu_icon_sprites_t* record, s32 frame, s32 cursor);

s32 battle_menu_load_charge_skillset(s32 unit_id, s32 skillset, s16* abilities, u8* ability_ct, u8* turns);
s32 battle_menu_load_draw_out_abilities(s32 unit_id, s32 skillset, u8* out_item_ids);
s32 battle_menu_collect_draw_out_katanas(s32 unit_id, u8 skillset, u8* out_item_ids);
s32 battle_menu_load_math_skill_multiples(s32 unit_id, s32 skillset, s16* out_ability_ids);
void battle_menu_init_palette_and_text(s32 unused_0);
s32 battle_menu_load_skillset(s32 unit_id, u8 skillset, u8 menu);
s32 battle_menu_load_throw_abilities(s32 unit_id, s32 skillset, u8* out_item_ids);
void battle_menu_open_help(void);
void battle_menu_open_mini_menu_help(void);
void battle_menu_open_companion_data_file(void);
void battle_menu_preview_ability_turn_at_list(void);
void battle_menu_release_current_thread_slot(void);
void battle_menu_restore_selection_for_selected_unit(s32 menu_id);
void battle_menu_run_equip_thread(void);
void battle_menu_store_auto_battle_main_target_id(s32 target_id, s32 battle_id);
void battle_menu_submit_numeric_display_frame_primitives(world_menu_palette_primitives_t* display);
void battle_menu_copy_bytes_to_halfwords(u16* dst, u8* src, s32 count);
void battle_menu_zoom_cursor_frame(const void* unused_source, SPRT* sprite, s32 step);
void battle_gfx_copy_sprite_position_uv_and_size(const world_menu_entry_t* param, SPRT* sprite, s32 unused_frame);
void battle_menu_select_icon_sprite_clut(SPRT* sprite);
void battle_menu_open_illegal_move_help(void);
s32 battle_menu_update_menus_and_event_speed(u32* otag, s32 controller_input);
void battle_menu_render_text_image_at_record_origin(world_text_dialog_record_t* record);

void battle_menu_build_window_sprites(RECT* rect, world_menu_entry_t* param, world_menu_icon_record_t* record);

s32 battle_menu_display_projected_action_effect(
    POLY_FT4* prim, const point16_t* base_load, const RECT* base_screen, world_gfx_image_load_parameters_t* params);

void battle_menu_clear_option_menu_open(void);
void battle_menu_open_bunit(void);
void battle_menu_run_bunit_transition_thread(void);
void battle_menu_set_option_menu_open(void);
void battle_draw_menu_number_glyphs(s32 value, s32 digits, u8* pixels, glyph_blit_t* pos);
void battle_menu_display_text(u32 text_id, u8* image, glyph_blit_t* origin, u8* text);

u8* battle_menu_redraw_text_page_on_scroll_in_rect(
    world_menu_entry_t* entry, s32* row_offset, s32* redraw, s32 unused, RECT* area);

void battle_menu_tween_window_quads(menu_window_buffer_t* buffers, RECT* to, RECT* to2, s32 table);
void battle_menu_animate_window_quad_crop(menu_window_buffer_t* buffers, RECT* rect, RECT* unused_to);
void battle_menu_free_memory(void* buffer);
void battle_menu_display_text_entry(s32 text_id, u8* image, glyph_blit_t* origin);
void battle_menu_display_text_entry_past_choice_marks(s32 text_id, u8* image, glyph_blit_t* origin);
void battle_menu_display_text_string(u8* image, glyph_blit_t* origin, u8* text);
void battle_menu_handle_action(world_menu_entry_t* menu, s32 selected_option);
void battle_menu_set_text_origin(s32 x, s32 y);
s32 battle_menu_should_close_thread(u32* input);
void battle_unit_start_view_thread(s32 mode, s32 unit_a, s32 unit_b);

/* text */
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

/* Text variables $27 and $28 (rows of glyph pixels drawn, 0x10 by default),
 * bound separately: battle_menu_display_text also takes the table base, and
 * element spellings let GCC address them from it. */
extern s32 g_battle_text_substitution_value_27;
extern s32 g_battle_text_substitution_value_28;
extern char g_battle_text_wait_for_allocation_message[];
extern char g_battle_text_wait_for_file_allocation_message[];
extern u8 g_battle_text_glyph_bitmaps[];
extern u16 g_battle_text_speaker_swap_unit_a;
extern u16 g_battle_text_speaker_swap_unit_b;
extern RECT g_battle_numeric_display_frame_rect;
extern RECT g_battle_numeric_display_texture_window;
extern RECT g_battle_numeric_display_text_rect_a;
extern RECT g_battle_numeric_display_text_rect_b;
extern RECT g_battle_numeric_display_text_rect_c;
extern world_gfx_image_load_parameters_t g_battle_numeric_display_image_params[18];
extern menu_number_entry_t g_battle_numeric_display_entries[15];
extern world_menu_palette_slot_t g_battle_numeric_display_palettes[];
extern u8 g_battle_player_unit_fallen;

/* Player name buffer substituted for text command 0xE0, 0xFE-terminated;
 * 0x801720cf is its last byte. */
extern u8 g_battle_player_name[20];
extern glyph_blit_t g_battle_text_large_number_glyph_rect; /* glyph source rectangle in the large number font */

extern menu_text_state_t
    g_battle_text_typewriter_state; /* origin/stride passed to battle_text_render_glyph_to_4bpp_image */

extern RECT g_battle_text_typewriter_vram_rect; /* VRAM destination of the finished glyph */
extern const u8* g_text_glyph_bitmap_data;

extern battle_text_glyph_state_t g_battle_text_typewriter_glyph;
extern const char g_battle_text_decimal_format[];
extern u16 g_battle_text_message_duration_frames;
extern u8* g_battle_text_pointer_backup[32];
extern s32 g_battle_text_pointer_backup_active;

/* BATTLE text IDs use their upper five bits to select one of these sections
 * (text_section_e). Overlay loaders replace individual entries as their text
 * becomes active. */
extern u8* g_battle_text_section_pointers[32];

/* Glyph pixel widths, 0xD0 entries per 0xDx prefix page. */
extern const u8 g_text_glyph_widths[];
extern s32 g_battle_text_awaiting_input;
extern s32 g_battle_text_current_line;
extern u32 g_battle_text_decimal_divisors[];
extern s32 g_battle_text_glyphs_per_wait;             /* glyphs drawn before the typewriter waits */
extern const char g_battle_text_heap_dp_format[];     /* "DP %x\n" */
extern const char g_battle_text_heap_remain_format[]; /* "HEAP REMAIN %x MAX %x\n" */
extern const char g_battle_text_heap_task_format[];   /* "TASK %d %x\n" */

/* Pixel width of each line measured by the last call. */
extern s32 g_battle_text_line_widths[];
extern u8* g_battle_text_message_buffer;
extern const char g_battle_text_min_heap_format[];     /* "MIN HEAP %x " */
extern char g_battle_text_reading_character_message[]; /* "reading charactor%d \n" */
extern u8 g_battle_text_slot_row_pitches[];            /* per-slot source row pitch in bytes */
extern u8 g_battle_text_slot_source_columns[];         /* per-slot source column */

/* Text variables $00-$1F. TEXT_PRINT_INDEXED_VALUE indexes them with a raw
 * text byte, so a larger index reads the globals that follow. */
extern s32 g_battle_text_substitution_values[32];
extern s16 g_battle_script_opcode_dc_value; /* EVENT_OPCODE_UNKNOWN_DC stores 13; nothing reads it */

/* Text substitutions $7E/$7F (0x801660ec, table index 0x7e from 0x80165ef4)
 * as the four halfwords battle_text_set_substitution_values_7e_7f stores. */
extern RECT g_battle_text_substitution_rect;

extern u8 g_battle_text_typewriter_column_scratch[14];       /* one-column scratch used by the phase shift */
extern u8 g_battle_text_typewriter_glyph_images[3][8][0x54]; /* eight glyph images per slot */
extern u8 g_battle_text_typewriter_previous_images[3][0x54]; /* previous image per slot */
void battle_text_run_spell_quote_thread(void);
const u8* battle_text_skip_to_entry(const u8* text_section, s32 entry_id);
void battle_text_measure_pixels(s16* width, s16* rows, const u8* text);
u8* battle_text_advance_cursor_with_backreferences(world_text_backreference_state_t* state, u8* cursor);
void battle_text_render_glyph_to_4bpp_image(
    const u8* glyph_bitmap, u8* image, glyph_blit_t* origin, s32 palette_offset);
void battle_text_build_display_message(s32 offset, s32 flags, s32* out_width);
void battle_text_build_unit_name_list(void);
u8* battle_text_find_entry(u32 text_id);
void battle_text_init_menu_section_pointers(void);
void battle_text_set_message_duration_frames(u16 frames);
void battle_text_relocate_pointer_table(const u32* offsets);
void battle_text_restore_section_pointers(void);
void battle_text_save_section_pointers(void);
void battle_text_draw_large_number_glyphs(s32 value, s32 digits, u8* pixels, glyph_blit_t* pos);
void battle_text_character_handling_thread(void);
void battle_text_configure_sprite_vram(
    RECT* rect, s16 width, s16 height, world_texture_prim_t* primitive, s32 clut_offset);
u8* battle_text_advance_cursor_with_separate_backreference_state(s32* remaining_bytes, u8* cursor, u8** return_cursor);
s32 battle_text_count_decimal_digits(s32 value);
void battle_text_determine_spell_quote(battle_action_command_t* action, s32 unit_id, s32 enabled);
void battle_text_draw_glyph_with_typewriter_delay(s32 pen_x, s32 pen_y, s32 delay);
void battle_text_print_fnt_value_and_next(s32 value);
s32 battle_text_read_packed_nibble(u8* data, s32 index, s32 row, s32 stride);
s32 battle_text_resolve_format_string_id(s32 format_code, s32 value);

/* The caller leaves 0x16f in $a1, but the body never reads $a1. */
void battle_text_fill_field_with_terminators(u8* text);
void battle_text_mirror_packed_nibble_region_horizontal(u8* data, glyph_blit_t* region);
u8* battle_menu_text_advance_cursor_with_backreferences(world_text_backreference_state_t* reader, u8* p);

/* other */
extern s32 g_battle_destination_camera_yaw;
extern u8 g_battle_job_level_gained_flag;
extern u8 g_battle_math_source_skillsets[4];
extern VECTOR g_battle_offset_screen_coords;
extern s16 g_battle_unit_entd_spritesheet_ids[];
extern s32 g_battle_unit_entd_spritesheet_count;
extern u16 g_battle_enemy_level_sum;
extern u8 g_battle_highest_enemy_level;
extern s32 g_battle_previous_game_state;
extern game_options_t g_battle_saved_game_options;

/* Unit coordinates shared by camera tile tests and dialogue positioning. */
extern SVECTOR g_battle_scratch_coords;
extern s32 g_battle_sort_targets_nearest_first;
extern s32 g_battle_post_battle_unit_changes_active;
extern s16 g_battle_auto_battle_option_colors[5]; /* text colour per option */
extern s32 g_battle_formation_display_mode;
extern u8 g_battle_mimic_pending_flags[21];
extern s16 g_battle_preview_target_unit_id;
extern u16 g_battle_saved_animation_speed; /* animation speed saved while an event thread forces speed 2 */

extern u16 g_battle_team_golem[4]; /* 0x8018f5f4 */
extern s32 g_battle_formation_screen_active;

/* BATTLE flag at 0x80165ff2, set by etc_graphic_open while a full-screen ETC
 * graphic is shown; BATTLE then draws primitives immediately instead of
 * linking them into the ordering table. */
extern u16 g_battle_etc_graphics_enabled;
extern u16 g_battle_field_object_use_request; /* 0x80165fe4 */
extern u16 g_battle_field_object_wait_status; /* 0x80166070 */
extern u8 g_main_terrain_geomancy_abilities[];
extern job_data_t* g_main_job_data_pointer;
extern s32 g_main_system_battle_frame_hsync_count_max;
s32 battle_unit_find_slot_by_character_identity(u32 character_identity);
s32 battle_map_tile_data_15_stub(void);
s32 battle_is_skillset_in_spell_quote_exception_list(s32 skillset_id);
void battle_script_process_inflict_status_commands(void);
s32 battle_menu_poll_small_load(void);
s32 battle_fixed_cross_product_q12(s32 a, s32 b, s32 c, s32 d);
s32 battle_status_prepare_terrain_poison(battle_stats_t* unit);
s32 battle_return_one(s32 unused_misc_id);
void battle_write_packed_nibble(u8* data, s32 index, s32 row, s32 stride, s32 value);
s32 battle_target_spread_panel_to_neighbors(s32 y, s32 x);
s32 battle_return_zero(void);
void blit_text_glyph(const u8* source, u8* pixels, const glyph_blit_t* glyph, glyph_blit_t* position);

/* unnamed */
extern s16 D_800b628c;
extern s16 D_800b6290;
extern s32 D_800F668C;

/* Write-only: zeroed with the map render state and never read anywhere on
 * the disc. The 150 x 14-byte shape suggests per-instruction parameter
 * storage (an event opcode takes at most 14 parameters), but no code proves
 * it, so the names stay provisional. */
extern u8 D_800F70B4[150][14];
extern u8 D_800F78E8[150];
extern u8 D_8018F4FC;
extern u8 D_8019387C;
extern s32 D_801B8B18;
extern s32 D_801B8B1C;
extern s32 D_801B8B98;
extern s32 D_801BACC4;
extern s32 D_801BC0C4;
extern s32 D_801BC0D4;
extern s32 D_801BC0D8;

/* Written by battle_effect_code_script_05 but never read. */
extern u16 D_801BF000;

#endif
