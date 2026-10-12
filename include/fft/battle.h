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
extern u8 g_battle_loaded_ability_flags_1;

/* Ability-list window layout at 0x80166994. Column modes: 0 text, 1 number,
 * 2 hidden, 3 alternate number. The columns read the work buffer's u16 text
 * ids and values through the layout's s16 pointers. */
extern menu_text_layout_t g_battle_ability_menu_layout;
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
extern u16 g_battle_cursor_screen_x;
extern s32 g_battle_cursor_previous_x;     /* cursor X before this step */
extern s32 g_battle_cursor_previous_y;     /* cursor Y before this step */
extern s32 g_battle_cursor_previous_z;     /* cursor level before this step */
extern u32 g_battle_cursor_repeat_counter; /* free-cursor repeat counter */
extern s16 g_battle_cursor_screen_y;

/* item */
/* battle_unit_animation_e of each item type's attack at a higher, a level and a lower target. */
extern u8 g_battle_weapon_attack_animations[][3];
extern s16 g_battle_item_icon_clut_base_x;
extern s16 g_battle_item_icon_clut_base_y;
extern point16_t g_battle_item_icon_image_location;
void battle_gfx_set_item_icon_sprite(SPRT* sprite, s32 item_id);

#include "fft/battle_event.h"

#include "fft/battle_status.h"

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
void battle_state_clear_frame_pacing(void);
void battle_state_reset_frame_pacing_timer(void);
s32 battle_state_try_enter_scenario_event(battle_unit_misc_data_t* unit);
void battle_state_enter_action_cast(void);
void battle_state_enter_commence_attack_phase(void);
void battle_action_post_action_facing_stub(battle_stats_t* unused_unit, s32 unused_facing);
void battle_state_enter_open_sp2_files(void);
void battle_bin_entrypoint(void);
s32 battle_mul_div_s64(s32 a, s32 b, s32 c);
void battle_menu_init_stub(void);
void battle_state_enter_pre_attack_animation(void);
void battle_state_deployment_frame_stub(void);
void battle_menu_run_companion_overlay_stub(void);

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

#include "fft/battle_thread.h"

#include "fft/battle_camera.h"

#include "fft/battle_sound.h"

#include "fft/battle_effect.h"

#include "fft/battle_gfx.h"

#include "fft/battle_action.h"

#include "fft/battle_move.h"

#include "fft/battle_map.h"

#include "fft/battle_menu.h"

#include "fft/battle_text.h"

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

extern s32 g_battle_formation_screen_active;

/* BATTLE flag at 0x80165ff2, set by etc_graphic_open while a full-screen ETC
 * graphic is shown; BATTLE then draws primitives immediately instead of
 * linking them into the ordering table. */
extern u16 g_battle_etc_graphics_enabled;
extern u16 g_battle_field_object_use_request; /* 0x80165fe4 */
extern u16 g_battle_field_object_wait_status; /* 0x80166070 */
s32 battle_unit_find_slot_by_character_identity(u32 character_identity);
s32 battle_map_tile_data_15_stub(void);
s32 battle_fixed_cross_product_q12(s32 a, s32 b, s32 c, s32 d);
s32 battle_return_one(s32 unused_misc_id);
s32 battle_target_spread_panel_to_neighbors(s32 y, s32 x);
s32 battle_return_zero(void);

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

#endif
