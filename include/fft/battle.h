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
    BATTLE_ABILITY_ANIMATION_COUNT = 454,
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
extern menu_text_layout_t g_battle_ability_menu_layout;
extern u8 g_battle_spell_quote_exception_skillsets[];
s32 battle_ability_find_learn_on_hit_unit(s32 attacker_id, u16* ability_id);

#include "fft/battle_ai.h"

#include "fft/battle_unit.h"

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
 * target's switch table.  The target has no explicit handler for value 50.
 */
typedef enum battle_game_state {
    BATTLE_GAME_STATE_FREE_CURSOR = 0,
    BATTLE_GAME_STATE_FREE_CURSOR_HELP = 1,
    BATTLE_GAME_STATE_HIGHLIGHT_UNITS = 2,
    BATTLE_GAME_STATE_OPEN_ACTION_MENUS = 3,
    BATTLE_GAME_STATE_IDLING_ACTION_MENUS = 4,
    BATTLE_GAME_STATE_MENU_TO_TARGETING = 5,
    BATTLE_GAME_STATE_ACTION_HELP_MENU = 6,
    BATTLE_GAME_STATE_DISPLAY_MOVE_AREA = 7,
    BATTLE_GAME_STATE_MINI_MENU = 8,
    BATTLE_GAME_STATE_MINI_MENU_HELP = 9,
    BATTLE_GAME_STATE_ACTIVE_TURN = 10,
    BATTLE_GAME_STATE_STATUS_EXECUTE = 11,
    BATTLE_GAME_STATE_UNIT_MOVE = 12,
    BATTLE_GAME_STATE_CLOSE_MOVE_HELP = 13,
    BATTLE_GAME_STATE_MOVE_RANGE_EXCEPTION = 14,
    BATTLE_GAME_STATE_ILLEGAL_MOVE_MENU = 15,
    BATTLE_GAME_STATE_UNIT_MOVING_SETUP = 16,
    BATTLE_GAME_STATE_UNIT_MOVING = 17,
    BATTLE_GAME_STATE_MOVE_CONFIRM_MENU = 18,
    BATTLE_GAME_STATE_WAIT_DIRECTION = 19,
    BATTLE_GAME_STATE_WAIT_MENU = 20,
    BATTLE_GAME_STATE_CRYSTAL_LEARN = 21,
    BATTLE_GAME_STATE_ACTION_EXECUTE_SETUP = 22,
    BATTLE_GAME_STATE_TARGETING_RANGE = 23,
    BATTLE_GAME_STATE_ILLEGAL_RANGE = 24,
    BATTLE_GAME_STATE_ABILITY_PREVIEW_HANDLING = 25,
    BATTLE_GAME_STATE_ABILITY_PREVIEW_HELP = 26,
    BATTLE_GAME_STATE_CONFIRM_ACTION = 27,
    BATTLE_GAME_STATE_PRE_ATTACK_ANIMATION = 28,
    BATTLE_GAME_STATE_ACTION_CAST = 29,
    BATTLE_GAME_STATE_COMMENCE_ATTACK_PHASE = 30,
    BATTLE_GAME_STATE_TARGET_SELECT_START = 31,
    BATTLE_GAME_STATE_TARGET_SELECT = 32,
    BATTLE_GAME_STATE_TARGET_SELECT_DENIED = 33,
    BATTLE_GAME_STATE_TARGET_SELECT_CONFIRM = 34,
    BATTLE_GAME_STATE_TARGET_DISPLAY_START = 35,
    BATTLE_GAME_STATE_TARGET_DISPLAY = 36,
    BATTLE_GAME_STATE_AFTER_COMMAND = 37,
    BATTLE_GAME_STATE_JP_EXP_GAIN = 38,
    BATTLE_GAME_STATE_CHANGE_TURN = 39,
    BATTLE_GAME_STATE_LEARN_ABILITY_ON_HIT = 40,
    BATTLE_GAME_STATE_ACTION_ANNOUNCEMENT = 41,
    BATTLE_GAME_STATE_OPEN_SP2_FILES = 42,
    BATTLE_GAME_STATE_START_EFFECT_FILE_OPEN = 43,
    BATTLE_GAME_STATE_SECONDARY_EFFECT = 44,
    BATTLE_GAME_STATE_ACTION_EXECUTE = 45,
    BATTLE_GAME_STATE_BATTLE_MESSAGE_DISPLAY = 46,
    BATTLE_GAME_STATE_RESUME_ATTACK_PHASE = 47,
    BATTLE_GAME_STATE_DEEP_DUNGEON_MESH_LOAD = 48,
    BATTLE_GAME_STATE_DEEP_DUNGEON_MESH_FINISH = 49,
    BATTLE_GAME_STATE_EFFECT = 51,
    BATTLE_GAME_STATE_EVENT = 52,
    BATTLE_GAME_STATE_MAP_JUMPING_OUT = 53,
    BATTLE_GAME_STATE_MAP_INITIALIZE = 54,
    BATTLE_GAME_STATE_MAP_JUMPING_IN = 55,
    BATTLE_GAME_STATE_MAP_JUMPING_OUT_2 = 56,
    BATTLE_GAME_STATE_MAP_INITIALIZE_2 = 57,
    BATTLE_GAME_STATE_MAP_JUMPING_IN_2 = 58,
    BATTLE_GAME_STATE_CLOSE_BATTLE = 59,
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
extern menu_system_function_t g_battle_system_function_table[];
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
extern s32 g_battle_frame_pacing;
extern s32 g_battle_frame_pacing_suppressed;
extern s32 g_battle_frame_pacing_timer;
void battle_state_enter_action_cast(void);
void battle_state_enter_commence_attack_phase(void);
void battle_action_post_action_facing_stub(battle_stats_t* unused_unit, s32 unused_facing);
void battle_state_enter_open_sp2_files(void);
void battle_bin_entrypoint(void);
s32 battle_mul_div_s64(s32 a, s32 b, s32 c);
void battle_menu_init_stub(void);
void battle_script_add_unit_stub(void);
void battle_state_enter_pre_attack_animation(void);
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

/* camera */
enum {
    BATTLE_CAMERA_PITCH_STEEP_MASK_THRESHOLD = 0x178,
};

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
extern u16 g_battle_camera_script_variable_indices[EVENT_CAMERA_COMPONENT_COUNT];
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
s32 battle_camera_step_track(camera_track_t* track, s32 component);
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

#include "fft/battle_effect.h"

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
extern menu_active_unit_t g_battle_active_turn_unit;
extern s32 g_battle_between_turn_resume_state;
extern s16 g_battle_acting_unit_used_ability_id;
extern s32 g_battle_between_turn_state;
extern s32 g_battle_turn_unit_id;

#include "fft/battle_gfx.h"

#include "fft/battle_action.h"

#include "fft/battle_move.h"

#include "fft/battle_map.h"

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

extern menu_allocation_t g_battle_menu_allocator_slot_records[16];
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
extern menu_entry_t g_battle_menu_idle_action_entries[];
extern u16 g_battle_menu_input_mode; /* menu_input_mode_e */
extern u8 g_battle_menu_numeric_display_frame_offsets[12][8];
extern u8 g_battle_menu_numeric_display_palette_indices[12];
extern menu_panel_record_t g_battle_menu_numeric_display_panel_records[2];
extern RECT g_battle_menu_numeric_display_texture_window;
extern s32 g_battle_menu_packet_buffer_index;
extern menu_palette_pair_t g_battle_menu_palette_colors;
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
extern menu_unit_selection_record_t g_battle_menu_unit_selection_records[BATTLE_UNIT_SLOT_COUNT];
extern menu_text_state_t g_menu_text_state;
extern menu_unit_status_record_t g_battle_menu_active_turn_banner;

/* 0x40-byte billboard block at 0x8014d0a0 saved and restored whole with the AT
 * banner: unit stats indexed as halfwords by
 * battle_menu_load_selected_unit. */
extern menu_status_panel_slot_storage_t g_battle_menu_billboard_data;
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
extern menu_palette_pair_t g_battle_menu_default_palette_colors;
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
extern image_load_parameters_t g_battle_menu_sprite_page_image_params[30];
/* Entries 5..7 above; a separate alias preserves the original template base. */
extern image_load_parameters_t g_battle_menu_sprite_page_image_param_bases[];
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
extern menu_gradient_panel_prims_t g_battle_menu_panel_fade_frames[];
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
extern menu_quad_page_t g_battle_menu_height_display_pages[2];
extern u16 g_battle_menu_height_display_suppressed;
extern s32 g_battle_menu_hide_numeric_values; /* non-zero: draw digits as blanks (0x48, 0x10) */
extern image_load_parameters_t g_battle_menu_icon_strip_image_params[20];
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
extern image_load_parameters_t g_battle_menu_single_icon_image_params[5];
extern u8 g_battle_menu_skillset_disabled_flags[]; /* per-row greyed flag */
extern menu_unit_status_record_t g_battle_menu_status_billboard;

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
extern menu_scroll_text_layout_t g_battle_menu_at_list_layout;
extern menu_quad_piece_t g_battle_menu_quad_pieces[]; /* quad piece table */
extern menu_quad_colors_t g_battle_menu_quad_colors;  /* vertex colours */
extern point16_t g_battle_menu_quad_origin;           /* screen origin */
extern glyph_blit_t g_battle_menu_text_pen;
extern menu_unit_editor_frame_t g_battle_menu_unit_editor_frames[2];

/* Full record bases, not the legacy aliases biased to thread_id. */
extern menu_window_buffer_t g_battle_menu_window_buffers[6]; /* 0x8016e43c */
extern s32 g_battle_menu_text_palette_offset;

/* BATTLE's current menu-entry table pointer at 0x80173cb8 (menu threads are
 * started on &table[i]): battle_menu_init_subsystem_pointers points it at
 * g_battle_menu_idle_action_entries, and the event overlays that run over
 * BATTLE swap in their own tables and restore it. WORLD's twin is
 * g_world_menu_thread_menu_data. */
extern menu_entry_t* g_battle_menu_thread_menu_data;

void battle_menu_display_hovered_unit_stats(
    RECT* frame_rect, s32 mode, menu_number_entry_t* entries, menu_unit_status_record_t* status);

void battle_handle_menu_cancel_input(menu_entry_t* menu);
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
void battle_menu_draw_text_columns_narrow(menu_entry_t* entry, s32* row_offset, u8* buffer);
void battle_menu_enter_status_screen_selection(void);
void battle_menu_fade_out_thread(void);
void battle_menu_free_high_overlay(void);
extern menu_cursor_bob_entry_t g_battle_cursor_bob_thresholds[];
extern menu_cursor_bob_entry_t g_battle_cursor_bob_active_thresholds[];
s32 battle_menu_get_cursor_bob_offset(s32 mode);
battle_ai_command_t* battle_menu_get_selected_command_address(void);
void battle_menu_icon_linked_entry_thread(void);
void battle_menu_size_entry_to_text(menu_entry_t* entry, s16* width, s16* height, s32* out_pad, s32 pad_short);
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
    scaled_draw_area_pair_t* packets, const RECT* geometry, s32 step, s32 flag);
void battle_menu_build_ability_preview_at_list(void);
void battle_menu_run_scrolling_ability_list_thread(void);
void battle_menu_select_unit_action_slots_thread(void);
s32 battle_menu_resolve_selection(void);
s32 battle_menu_get_unit_action_menu_id(battle_stats_t* unit);
s32 battle_menu_load_item_abilities(s32 unit_id, u8 skillset, u8* out_item_ids);

s32 battle_menu_get_unit_skillset_ability_data(
    s32 unit_id, u8 skillset, s16* ability_ids, u8* mp_out, u8* ct_out, s32 unused, u8* flags_out, u8* turns_out);

s32 battle_menu_collect_throwable_items(s32 unit_id, u8 ability, u8* items);
void battle_menu_select_icon_cluts(struct menu_icon_record* frame);
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
void battle_menu_confirm_action_silently(menu_entry_t* menu);
s32 battle_menu_copy_ff_terminated_bytes_to_halfwords(u16* dst, u8* src);
void battle_menu_copy_palette_colors(const menu_palette_pair_t* source);
void battle_menu_copy_unit_data_to_status_billboard(battle_stats_t* unit, menu_unit_status_record_t* output);

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
void battle_menu_init_numeric_display_frame_primitives(RECT* bounds, menu_palette_primitives_t* menu);
void battle_menu_init_primitive_colors_normal(menu_palette_primitives_t* menu);
void battle_menu_init_primitive_colors_dimmed(menu_palette_primitives_t* menu);
void battle_menu_init_semitransparent_sprt(SPRT* sprite);
void battle_menu_run_icon_selection_loop(void);
void battle_menu_select_sprite_clut(SPRT* sprite);
void battle_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* pair);
void battle_menu_submit_icon_primitives(struct menu_icon_record* frame);
void battle_menu_step_wrapping_cursor_on_scroll_buttons(menu_entry_t* ctx, s32* sel);

void battle_menu_update_icon_cursor_sprites(menu_entry_t* param, menu_icon_sprites_t* record, s32 frame, s32 cursor);

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
void battle_menu_submit_numeric_display_frame_primitives(menu_palette_primitives_t* display);
void battle_menu_copy_bytes_to_halfwords(u16* dst, u8* src, s32 count);
void battle_menu_zoom_cursor_frame(const void* unused_source, SPRT* sprite, s32 step);
void battle_gfx_copy_sprite_position_uv_and_size(const menu_entry_t* param, SPRT* sprite, s32 unused_frame);
void battle_menu_select_icon_sprite_clut(SPRT* sprite);
void battle_menu_open_illegal_move_help(void);
s32 battle_menu_update_menus_and_event_speed(u32* otag, s32 controller_input);
void battle_menu_render_text_image_at_record_origin(text_dialog_record_t* record);

void battle_menu_build_window_sprites(RECT* rect, menu_entry_t* param, menu_icon_record_t* record);

s32 battle_menu_display_projected_action_effect(
    POLY_FT4* prim, const point16_t* base_load, const RECT* base_screen, image_load_parameters_t* params);

void battle_menu_clear_option_menu_open(void);
void battle_menu_open_bunit(void);
void battle_menu_run_bunit_transition_thread(void);
void battle_menu_set_option_menu_open(void);
void battle_draw_menu_number_glyphs(s32 value, s32 digits, u8* pixels, glyph_blit_t* pos);
void battle_menu_display_text(u32 text_id, u8* image, glyph_blit_t* origin, u8* text);

u8* battle_menu_redraw_text_page_on_scroll_in_rect(
    menu_entry_t* entry, s32* row_offset, s32* redraw, s32 unused, RECT* area);

void battle_menu_tween_window_quads(menu_window_buffer_t* buffers, RECT* to, RECT* to2, s32 table);
void battle_menu_animate_window_quad_crop(menu_window_buffer_t* buffers, RECT* rect, RECT* unused_to);
void battle_menu_free_memory(void* buffer);
void battle_menu_display_text_entry(s32 text_id, u8* image, glyph_blit_t* origin);
void battle_menu_display_text_entry_past_choice_marks(s32 text_id, u8* image, glyph_blit_t* origin);
void battle_menu_display_text_string(u8* image, glyph_blit_t* origin, u8* text);
void battle_menu_handle_action(menu_entry_t* menu, s32 selected_option);
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
extern image_load_parameters_t g_battle_numeric_display_image_params[18];
extern menu_number_entry_t g_battle_numeric_display_entries[15];
extern menu_palette_slot_t g_battle_numeric_display_palettes[];
extern u8 g_battle_player_unit_fallen;

/* Player name buffer substituted for text command 0xE0, 0xFE-terminated;
 * 0x801720cf is its last byte. */
extern u8 g_battle_player_name[20];
extern glyph_blit_t g_battle_text_large_number_glyph_rect; /* glyph source rectangle in the large number font */

extern menu_text_state_t
    g_battle_text_typewriter_state; /* origin/stride passed to battle_text_render_glyph_to_4bpp_image */

extern RECT g_battle_text_typewriter_vram_rect; /* VRAM destination of the finished glyph */
extern const u8* g_text_glyph_bitmap_data;

extern text_glyph_state_t g_battle_text_typewriter_glyph;
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
u8* battle_text_advance_cursor_with_backreferences(text_backreference_state_t* state, u8* cursor);
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
    RECT* rect, s16 width, s16 height, texture_primitive_t* primitive, s32 clut_offset);
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
u8* battle_menu_text_advance_cursor_with_backreferences(text_backreference_state_t* reader, u8* p);

/* other */
extern s32 g_battle_destination_camera_yaw;
extern u8 g_battle_job_level_gained_flag;
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
