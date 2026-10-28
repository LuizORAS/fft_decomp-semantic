#ifndef BATTLE_H
#define BATTLE_H

/* BATTLE.BIN: the battle engine. Event and effect overlays run alongside it. */

#include "fft/battle_action.h"
#include "fft/battle_ai.h"
#include "fft/battle_camera.h"
#include "fft/battle_effect.h"
#include "fft/battle_event.h"
#include "fft/battle_formula.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/battle_menu.h"
#include "fft/battle_move.h"
#include "fft/battle_script.h"
#include "fft/battle_status.h"
#include "fft/battle_target.h"
#include "fft/battle_text.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/sound.h"
#include "fft/thread.h"
#include "psx/libc.h"

/* heap */
/* The battle heap: a K&R-style free list in 8-byte units from the effect palette buffer up to MAIN's game heap. */

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

extern battle_heap_node_t* g_battle_heap_base;
extern s16 g_battle_heap_block_count;
extern battle_heap_owner_list_t g_battle_heap_owner_lists[];
extern battle_heap_node_t* g_battle_heap_rover;
extern u32 g_battle_heap_end_address;
extern s32 g_battle_heap_min_largest_free; /* Lowest largest-free_node-block size seen, in 8-byte units. */
extern s32 D_801BACC4;
extern s32 D_801BC0D8;
void* battle_heap_alloc_block(u32 size, s32 kind);
void battle_heap_free_block(void* ptr);
void battle_heap_init(battle_heap_node_t* buffer, u32 buffer_size);
void battle_heap_print_stats(void);

/* input */
/* The map cursor: its tile and screen position, the tile before its last step, its input mode and
 * repeat counter. */

extern s32 g_battle_cursor_x;
extern s32 g_battle_cursor_y;
extern s32 g_battle_cursor_z;
extern u16 g_battle_cursor_screen_x;
extern s32 g_battle_cursor_previous_x;     /* cursor X before this step */
extern s32 g_battle_cursor_previous_y;     /* cursor Y before this step */
extern s32 g_battle_cursor_previous_z;     /* cursor level before this step */
extern u32 g_battle_cursor_repeat_counter; /* free-cursor repeat counter */
extern s16 g_battle_cursor_screen_y;

/* ability */
/* Abilities in battle: their animation data, the announcement (name window or spell quote), the
 * ability-list layout and learning on hit (docs/mechanics/Abilities.md). */

extern s16 g_battle_ability_effect_id;
extern u8 g_battle_loaded_ability_flags_1;
s32 battle_ability_find_learn_on_hit_unit(s32 attacker_id, u16* ability_id);

/* deploy */
/* The deployment before a battle: the party's start rows, the deployed units' palettes and the flag
 * that ends it, set by ATTACK's deployment screen or, when the scenario has no first squad to place, by
 * deployment_skip_thread. */

extern const RECT g_battle_deployment_clear_rect;
extern palette_t g_battle_deployed_unit_palettes[2];
extern s32 g_battle_deployment_skipped;

/* state */
/* The battle state machine: battle_state_run_game_loop runs the handler of g_battle_game_state each frame. */

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
    BATTLE_GAME_STATE_AI_COMMAND = 5,
    BATTLE_GAME_STATE_ACTION_HELP_MENU = 6,
    BATTLE_GAME_STATE_DISPLAY_MOVE_AREA = 7,
    BATTLE_GAME_STATE_MINI_MENU = 8,
    BATTLE_GAME_STATE_MINI_MENU_HELP = 9,
    BATTLE_GAME_STATE_ACTIVE_TURN = 10,
    BATTLE_GAME_STATE_STATUS_EXECUTE = 11,
    BATTLE_GAME_STATE_UNIT_MOVE = 12,
    BATTLE_GAME_STATE_SELECT_MOVE_TILE = 13,
    BATTLE_GAME_STATE_MOVE_RANGE_EXCEPTION = 14,
    BATTLE_GAME_STATE_ILLEGAL_MOVE_MENU = 15,
    BATTLE_GAME_STATE_UNIT_MOVING_SETUP = 16,
    BATTLE_GAME_STATE_UNIT_MOVING = 17,
    BATTLE_GAME_STATE_MOVE_CONFIRM_MENU = 18,
    BATTLE_GAME_STATE_WAIT_DIRECTION = 19,
    BATTLE_GAME_STATE_WAIT_MENU = 20,
    BATTLE_GAME_STATE_CRYSTAL_LEARN = 21,
    BATTLE_GAME_STATE_TARGETING_MESSAGE = 22,
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
    BATTLE_GAME_STATE_CONTINUE_TURN = 38,
    BATTLE_GAME_STATE_CHANGE_TURN = 39,
    BATTLE_GAME_STATE_LEARN_ABILITY_ON_HIT = 40,
    BATTLE_GAME_STATE_ANNOUNCE_ABILITY = 41,
    BATTLE_GAME_STATE_OPEN_SP2_FILES = 42,
    BATTLE_GAME_STATE_START_EFFECT_FILE_OPEN = 43,
    BATTLE_GAME_STATE_START_ACTION_EXECUTE = 44,
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

extern s32 g_battle_state_map_init_step;
extern s32 g_battle_state_animation_continue_check;
extern s32 g_battle_state_screen_fade_step;
extern s32 g_main_system_slowdown_frames;
void battle_state_clear_min_vsync_interval(void);
extern s32 g_main_system_slowdown_suppressed;
extern s32 g_main_system_min_vsync_interval;
extern s32 g_battle_game_state;
extern s32 g_battle_frame_counter;
extern s32 g_battle_previous_game_state;
s32 battle_state_announce_next_ability(void);
void battle_state_enter_action_cast(void);
void battle_state_enter_after_command(void);
void battle_state_enter_ai_command(void);
void battle_state_enter_battle_message_display(void);
void battle_state_enter_close_battle(s32 transition_step, s32 close_flow_state);
void battle_state_clear_slowdown(void);
void battle_state_enter_commence_attack_phase(void);
void battle_state_enter_continue_turn(void);
void battle_state_enter_free_cursor(void);
void battle_state_enter_highlight_units_by_team(void);
void battle_state_enter_illegal_range(void);
void battle_state_enter_move_range_exception(void);
void battle_state_enter_open_sp2_files(void);
void battle_state_enter_pre_attack_animation(void);
void battle_state_enter_status_execute(void);
void battle_state_enter_target_display(void);
void battle_state_enter_target_display_start(void);
void battle_state_enter_target_select(void);
void battle_state_enter_target_select_confirm(void);
void battle_state_enter_target_select_denied(void);
void battle_state_enter_target_select_start(void);
void battle_state_enter_targeting_message(void);
void battle_state_enter_unit_moving(void);
void battle_state_enter_unit_moving_setup(void);
s32 battle_state_get_animation_continue_check(void);
s32 battle_state_get_vsync_interval(void);
void battle_state_init_display(s32 width, s32 height, s32 projection, u8 red, u8 green, u8 blue);
void battle_state_run_battle_setup(void);
void battle_state_run_game_loop(void);
void battle_state_set_min_vsync_interval(s32 value);
s32 battle_state_set_vsync_interval(s32 speed);
void battle_state_start_change_map_jump_in(s32 duration);
void battle_state_start_change_map_jump_out(s32 map_id, s32 duration);
void battle_state_start_close_battle(s32 duration);
void battle_state_start_map_jump_out(s32 map_id, s32 duration);
void battle_state_stop_map_animations(void);
s32 battle_state_sync_frame(u32* ordering_table);
s32 battle_state_sync_setup_frame(u32* ordering_table);
s32 battle_state_update_controller_input(void);
s32 battle_state_update_deployment_controller_input(void);
void battle_state_update_units(void);
void battle_state_handle_ability_preview_help_state(void);
void battle_state_handle_ability_preview_state(void);
void battle_state_handle_action_cast_state(void);
void battle_state_handle_action_execute_state(void);
void battle_state_handle_action_help_menu_state(void);
void battle_state_handle_active_turn_state(void);
void battle_state_handle_after_command_state(void);
void battle_state_handle_ai_command_state(void);
void battle_state_handle_announce_ability_state(void);
void battle_state_handle_battle_message_display_state(void);
void battle_state_handle_change_map_init_state(void);
void battle_state_handle_change_map_jumping_in_state(void);
void battle_state_handle_change_map_jumping_out_state(void);
void battle_state_handle_change_turn_state(void);
void battle_state_handle_close_battle_state(void);
void battle_state_handle_commence_attack_phase_state(void);
void battle_state_handle_confirm_action_state(void);
void battle_state_handle_continue_turn_state(void);
void battle_state_handle_crystal_learn_state(void);
void battle_state_handle_deep_dungeon_mesh_finish_state(void);
void battle_state_handle_deep_dungeon_mesh_load_state(void);
void battle_state_handle_display_move_area_state(void);
void battle_state_handle_effect_state(void);
void battle_state_handle_event_state(void);
void battle_state_handle_free_cursor_help_state(void);

/* Per-frame handlers dispatched by battle_state_run_game_loop. */
void battle_state_handle_free_cursor_state(void);

void battle_state_handle_highlight_units_state(void);
void battle_state_handle_idling_action_menus_state(void);
void battle_state_handle_illegal_move_menu_state(void);
void battle_state_handle_illegal_range_state(void);
void battle_state_handle_learn_ability_on_hit_state(void);
void battle_state_handle_map_init_state(void);
void battle_state_handle_map_jumping_in_state(void);
void battle_state_handle_map_jumping_out_state(void);
void battle_state_handle_mini_menu_help_state(void);
void battle_state_handle_mini_menu_state(void);
void battle_state_handle_move_confirm_menu_state(void);
void battle_state_handle_move_range_exception_state(void);
void battle_state_handle_open_action_menus_state(void);
void battle_state_handle_open_sp2_files_state(void);
void battle_state_handle_pre_attack_animation_state(void);
void battle_state_handle_resume_attack_phase_state(void);
void battle_state_handle_select_move_tile_state(void);
void battle_state_handle_start_action_execute_state(void);
void battle_state_handle_start_effect_file_open_state(void);
void battle_state_handle_status_execute_state(void);
void battle_state_handle_target_display_start_state(void);
void battle_state_handle_target_display_state(void);
void battle_state_handle_target_select_confirm_state(void);
void battle_state_handle_target_select_denied_state(void);
void battle_state_handle_target_select_start_state(void);
void battle_state_handle_target_select_state(void);
void battle_state_handle_targeting_message_state(void);
void battle_state_handle_targeting_range_state(void);
void battle_state_handle_unit_move_state(void);
void battle_state_handle_unit_moving_setup_state(void);
void battle_state_handle_unit_moving_state(void);
void battle_state_handle_wait_direction_state(void);
void battle_state_handle_wait_menu_state(void);
void battle_state_deployment_frame_stub(void);
s32 battle_state_try_enter_scenario_event(battle_unit_t* unit);

/* unit */
/* The battle units: the battle stats of the 21 slots (g_battle_unit_stats) and the display records of
 * the misc unit list (g_battle_units), and what reads and changes them by misc or battle id:
 * positions, facings and mounts, animation requests and reactions, the ENTD load and the deployment,
 * staged statuses for events, the status pictures of the unit panels and the unit view thread. */

/* Unit coordinates shared by camera tile tests and dialogue positioning. */
extern SVECTOR g_battle_scratch_coords;

/* effect */
/* The ability effect files (EFFECT/E*.BIN) and the engine that plays them: the request and the load
 * (battle_effect_request_ability_effect, battle_effect_update_file_load), the file's section header and the
 * setters that bind its sections, the script records and their interpreter (battle_effect_run_block,
 * g_battle_effect_instructions), the parent and child timelines with their color, sound and camera tracks,
 * and the native callbacks the scripts bind. The work record types are here because battle_effect_record_t embeds
 * them. */

extern s32 D_801BC0D4;

/* effect particle */
/* The particles of an effect file: the emitters and their spawn (battle_effect_spawn_emitter_particles),
 * the work records and sprite nodes, gravity and inertia, the animation curves, each renderer's view of a
 * geometry table record, and the sprite and primitive renderer with its ring buffer. */

extern s32 D_801BC0C4;

/* effect secondary */
/* The secondary effects resident in BATTLE.BIN: weapon hits, arrows and other projectiles, the spell and
 * summon charges, level up, the landing dust and splash, the trap, the zodiac poof and death smoke, Reflect,
 * teleport and equip break. An animation id picks a handler of g_battle_effect_secondary_handlers, run once
 * a frame through the phases of battle_effect_secondary_phase_e; the projectile models are drawn here. */

extern s32 D_801B8B98;

/* effect camera */
/* The effect camera: the rotation, position and zoom moves an effect's timeline asks for (the
 * battle_camera_* track routines step them), the snapshots of the live camera they start from, and where a
 * target or a tile lies in camera space. */

extern s32 D_801B8B18;
extern s32 D_801B8B1C;

/* action */
/* The action pipeline: the ability and charge time a command resolves to, the action record, each
 * strike's targets and formula setup, the result applied to each unit, the rewards and the effect
 * messages. */

extern u8 D_8019387C;

/* move */
/* Movement (docs/mechanics/Movement.md): the tiles and paths a unit's Move, Jump and movement abilities
 * reach over the terrain, the Move-Find items and traps, each step's animation (walk, climb, jump, float,
 * fly, teleport) and landing, knockback, and the events after a move. */

extern u8 D_8018F4FC;

/* gfx */
/* Unit sprites and what is drawn around them: the SHP frames and SEQ animations of each sheet and the
 * loader that reads them, the nine spritesheet slots and the two EVTCHR slots in VRAM, the sprite primitive
 * ring and the two render buffers, unit palettes and tints and the fade-out removal, the numbers and
 * messages after an action, status bubbles, shadows, the map cursor, the wait arrows and the screen color
 * modulation. */

extern s16 D_800b628c;
extern s16 D_800b6290;

/* map */
/* The battle map: the GNS file of each map and the resources it picks by map state (layout, weather, time
 * of day): mesh, textures, palettes, lighting, terrain, texture and mesh animations; the loader and the two
 * map load sequences; the per-frame draw of the mesh, the weather and the tile marks; the color fades and
 * CLUT rows; the tiles, the Move-Find tiles and the Deep Dungeon reload. */

extern s32 D_800F668C;

/* Write-only: zeroed with the map render state and never read anywhere on
 * the disc. The 150 x 14-byte shape suggests per-instruction parameter
 * storage (an event opcode takes at most 14 parameters), but no code proves
 * it, so the names stay provisional. */
extern u8 D_800F70B4[150][14];

extern u8 D_800F78E8[150];
extern u8 g_battle_random_battle_music_tracks[];

/* menu */
/* The battle menus (battle_menu_*): menu entries run as threads in slots 3 to 8, the turn menus and
 * their lists, the system-function prompts, the windows, text images and cursors they draw, the unit
 * panels and the companion overlays they open. WORLD carries a copy (world_menu_*). */

/* Array view: as a scalar, GCC hoists the menu-index reload above its store
 * in battle_menu_run_system_function_thread. */
extern battle_menu_system_function_t g_battle_system_function_table[];

extern s32 g_battle_system_function_thread_busy;
extern game_options_t g_battle_saved_game_options;

/* util */
/* BATTLE's hand-written helpers: a forward byte copy, a signed multiply-divide through a 64-bit product
 * and a Q12 cross product that flags overflow (the camera tracks' crossing points). */

s32 battle_util_mul_div_s64(s32 a, s32 b, s32 c);
extern u8 g_battle_job_level_gained_flag;

/* A random value between `min` and `max`, in either order. Spelled with rand()
 * first, unlike RAND_BETWEEN; both spellings occur in the target. */
#define RANDOM_BETWEEN(min, max)                                                                                       \
    ((min) == (max) ? (min) : (max) < (min) ? rand() % ((min) - (max)) + (max) : rand() % ((max) - (min)) + (min))

s32 battle_util_cross_product_q12(s32 a, s32 b, s32 c, s32 d);

/* return */
/* Routines that return a constant where the code expects a test: battle_return_one is the event wait
 * instruction's unit-ready test, so it never waits, and battle_return_zero answers
 * main_overlay_call_battle_return_zero, which nothing calls. */

s32 battle_return_one(s32 unused_runtime_id);
s32 battle_return_zero(void);

/* added from build-2 (merge dry run) */
/* Option and item value meaning "no selection" in the packed action-menu
 * result; the option byte test (x & 0xfe) == 0xfe also accepts 0xff. */
enum {
    MENU_SELECTION_NONE = 0xfffe,
};

/* One step of a movement path, as battle_move_encode_path_steps builds it. */
typedef enum battle_move_step_bits {
    BATTLE_MOVE_STEP_DISTANCE_MASK = 0x03, /* distance - 1, for jumps across a gap */
    BATTLE_MOVE_STEP_SOURCE_CLIMB = 0x04,
    BATTLE_MOVE_STEP_DESTINATION_CLIMB = 0x08,
    BATTLE_MOVE_STEP_ON_UNIT = 0x10,
    BATTLE_MOVE_STEP_HIGH_LEVEL = 0x20,
    BATTLE_MOVE_STEP_DIRECTION_SHIFT = 6, /* 0 +x, 1 -x, 2 -y, 3 +y */
} battle_move_step_bits_e;

/* Tile slots are indexed level * 256 + y * width + x
 * (battle_map_calculate_location): two levels of 256 slots. */
enum {
    MAP_LEVEL_TILE_SLOT_COUNT = 0x100,
    MAP_TILE_SLOT_COUNT = 0x200,
};

#endif
