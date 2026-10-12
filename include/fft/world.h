#ifndef WORLD_H
#define WORLD_H

/* WORLD/WORLD.BIN: world map, menus and event scripts. */

#include "fft/main.h"
#include "psx/libc.h"
#include "psx/libetc.h"
#include "psx/libgs.h"

struct battle_ai_command_action;

/* system */
/* Slots that the WORLD screens use for threads of their own; thread.h has the
 * maps shared with BATTLE, wldcore.h and open.h the WLDCORE and OPENING ones. */
typedef enum world_thread_slot {
    WORLD_THREAD_SLOT_MENU_GROUP = 2,           /* world_menu_thread_group_supervisor_thread */
    WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL = 10, /* the equipment (change banner) or the ability panel */
    WORLD_THREAD_SLOT_PREVIEW_STATS = 11,       /* the numeric editor thread of the preview stats window */
    WORLD_THREAD_SLOT_UPPER_MENU_CHILD = 14,    /* the child of the menu in slot 15, or a wait-forever thread
                                                 * standing in for it */
    WORLD_THREAD_SLOT_UPPER_MENU = 15,          /* a second menu beside the one in slot 6 */
    WORLD_THREAD_SLOT_TUTORIAL_TEXT = 16,       /* tutorial script text: the slot past the sixteen reset ones */
} world_thread_slot_e;

/* world_build_at_list runs its window frame thread in the upper menu's slot. */
enum {
    WORLD_AT_LIST_THREAD_SLOT_WINDOW_FRAME = 15,
};

/* System-function table. Declared as an array: as a scalar, GCC hoists
 * world_menu_run_system_function_thread's menu-index reload above its store. */
extern battle_system_function_t g_world_system_function_table[];
extern s32 g_world_bin_load_request;
extern s32 g_world_primary_bit_read_index;
extern s32 g_world_primary_bit_write_index;
extern u8 g_world_primary_bit_cursor_reset;
extern u8* g_world_primary_bit_cursor;
extern s32 g_world_pending_bin_load_index;
extern s32 g_world_pending_bin_load_destination;
extern s32 g_world_system_function_thread_busy;
extern s32 g_world_bin_load_sectors[];         /* 0x80156388, indexed by request */
extern s32 g_world_bin_load_sizes[];           /* 0x801563c8 */
extern s32 g_world_bin_load_address_offsets[]; /* 0x80156408, relative to the overlay load address */
extern s32 g_world_pending_bin_load_sectors[];
extern s32 g_world_pending_bin_load_sizes[];
extern s32 g_world_pending_bin_load_in_progress;
extern s32 g_world_bin_load_in_progress; /* 0x801564cc */
extern s32 g_world_game_option_original_values[14];
extern s32 g_world_game_option_values[14];
void world_menu_request_open_companion_executable(s32 request);
s32 world_bits_read_primary(s32 bit_count);
s32 world_bits_read_secondary(s32 bit_count);
void world_bit_cursor_set_primary(u8* ptr);
void world_bit_cursor_set_secondary(u8* ptr);
void world_bits_write_primary(u8 bit);
void world_script_clamp_s32_to_range(s32* value, s32 min, s32 max);
void world_menu_copy_bytes_to_halfwords(u16* destination, const u8* source, s32 count);
s32 world_mul_div_64(s32 a, s32 b, s32 c);
void world_game_reset(void);
void world_menu_init_stub(void);
void world_menu_display_projected_action_effect(POLY_FT4* unused_primitive,
    const world_image_location_t* unused_base_load, const RECT* unused_base_screen,
    const world_gfx_image_load_parameters_t* unused_parameters);
void world_text_run_spell_quote_thread(void);
void world_menu_run_companion_overlay_stub(void);
void world_script_frame_after_threads_stub(void);
void world_map_reset_script_light_transform(void);
void world_script_process_pending_requests(void);
void world_camera_load_state_from_script_variables(void);
void world_camera_store_state_to_script_variables(void);
void world_script_add_unit_stub(void);
void world_script_frame_after_requests_stub(void);
void world_card_close_stub(void);
void world_menu_open_companion_executable(void);
void world_menu_open_companion_data_file(void);
void world_script_report_missing_unit_stub(s32 unused_unit_id);
void world_script_opcode_74_event_instruction(const u8* unused_parameters);

/* thread */
/* WORLD's cooperative threads, as in BATTLE but with equipment 1-16, and state some of its threads share. */

extern u16 g_world_thread_change_cooldown;

/* The unspecified argument list is intentional for the same stack-switching
 * callback bridge used by BATTLE. */
extern void (*g_world_thread_call_target)(void);
/* Inner-subroutine continuation invoked by the WORLD thread scheduler. Five call
 * sites agree on this shape; world_script_teleportin_event_instruction reuses the slot with an
 * argument and keeps a local extern for that. */
extern u8 g_world_thread_status_snapshot[16];

/* Nonzero while a modal task owns the menus (0x8015330c): the announce and
 * message threads and event opcode LOCK_MENU_INPUT set it, and
 * world_input_update_with_message_state clears it once thread 1 stops. Menus drop
 * input and dim their palette while it is set. */
extern s32 g_world_thread_task_active;
extern s32 g_world_thread_task_ids[][256];
extern battle_menu_status_panel_indicator_prims_t g_world_thread_indicator_packets[2][2];
extern main_crystal_pickup_result_t* g_world_thread_inner_subroutine_result; /* the crystal pickup */

extern native_thread_t g_world_thread_contexts[]; /* 17 equipment: g_world_gfx_texture_allocation_grid follows */
extern s32 g_world_thread_current_id;

/* WORLD's pointer slot and fixed-array binding refer to the same scheduler
 * storage through different symbols. */
extern native_thread_t* g_world_threads;
s32 world_menu_get_cursor_bob_offset(s32 mode);
s32 world_thread_find_running_4_to_8(void);
void world_formation_item_menu_idle_thread(void);
void world_formation_ability_menu_idle_thread(void);
void world_thread_idle_yield_forever(void);
s32 world_thread_is_running_by_id(s32 thread_id);
s32 world_thread_is_task_active(s32 unused_0);
void world_script_dark_screen_thread(void);
void world_input_update_with_message_state(void);
void world_card_input_update_with_message_state(void);
void world_thread_wait_for_unit_view_panels(void);
void world_thread_yield_forever(void);
void world_thread_request_redraw(s32 thread_id);
s32 world_thread_call_on_main_stack();
void world_thread_exit_current(void);
s32 world_thread_find_running_by_task(s32 task_id);
void* world_thread_get_current_global_pointer(void);
void* world_thread_get_current_parameter_1(void);
s32 world_thread_get_current_parameter_2(void);
s32 world_thread_get_current_parameter_3(void);
s32 world_thread_get_current_task_id(void);
s32 world_thread_is_previous_running(void);
s32 world_thread_is_running(s32 thread_id);
s32 world_thread_is_running_asm(s32 thread_id);
void world_thread_reset_scheduler(void);
s32 world_thread_resolve_id(s32 requested_thread_id);
s32 world_thread_resolve_id_after_current(s32 requested_thread_id);
void world_thread_resume(s32 thread_id);
void world_thread_set_current_task_id(s32 task_id);
void world_thread_set_parameters(s32 thread_id, s32 first, s32 second, s32 third);
void world_thread_set_four_parameters(s32 thread_id, s32 first, s32 second, s32 third, s32 fourth);
void world_thread_request_stop(s32 thread_id);
void world_thread_start(s32 thread_id, void (*function)(void));
void world_thread_suspend(s32 thread_id);
void world_thread_wait_frames(s32 frames);
void world_thread_wait_until_inactive(s32 thread_id);
/* Hand-written (0x800fff50): as battle_thread_yield, calling world_script_update_event_input_state
 * and running equipment 1-16 before slot 0. */
void world_thread_yield(void);

/* input */
extern u8 g_world_input_activation_timer;
extern u32 g_world_input_frame_current;
extern u32 g_world_input_frame_initial_repeat_counter;
extern u32 g_world_input_frame_new;
extern u32 g_world_input_frame_previous;
extern u32 g_world_input_frame_repeat_counter;
extern u32 g_world_input_frame_secondary_repeat_counter;

/* Countdown maintained by world_input_update_lockout_timer. Volatile:
 * that routine reloads it after the zero test and again before the running
 * check instead of reusing the first load. */
extern volatile u8 g_world_input_lockout_timer;
extern u16 g_world_input_previous, g_world_input_latched_button, g_world_input_newly_pressed;
extern s32 g_world_input_repeat_counters[16];
extern u32 g_world_input_primary_repeat, g_world_input_secondary_repeat;
extern u32 g_world_input_unfiltered_controller;
extern world_input_timing_profile_t g_world_input_timing_profiles[];
extern u8 g_world_input_triangle_latched;
extern u8 g_world_input_triangle_latch;
extern u8 g_world_input_blocked_state;
extern u32* g_world_input_frame_controller_input;
extern s32 g_world_input_primary_repeat_latched;
extern s32 g_world_input_secondary_repeat_latched;
void world_input_clear_state(void);
s32 world_input_filter_menu(void);
s32 world_input_get_lockout_timer(void);
void world_input_update_lockout_timer(void);
u32* world_input_get_menu_controller(s32 player);
s32 world_input_get_menu_repeat_counter(void);
s32 world_input_get_tutorial_buttons(void);
s32 world_input_read_controller(s32 unused);
s32 world_input_read_menu_scroll_repeat(void);
s32 world_input_read_tutorial_or_controller(s32 unused_0);
void world_script_load_event_input_state(const world_menu_input_state_t* state);
void world_input_update_controller(void);
void world_input_update_menu_repeat(u32 buttons);
void world_input_update_menu_state(void);

/* camera */
extern s32 g_world_camera_speed_curve;
extern u16 g_world_camera_script_variable_indices[7];
extern u16 g_world_camera_position_locked;
extern u16 g_world_camera_shake_z_offset;
extern s32 g_world_camera_yaw_band;
extern s32 g_world_camera_yaw_unwrap_request;
extern s16 g_world_camera_candidate_yaws[4];
s32 world_camera_wrap_yaw_angle(void);
void world_camera_init_tilt_and_zoom(void);
void world_camera_init_yaw_angle(void);
s32 world_camera_step_track(battle_camera_track_t* track, s32 component);
void world_camera_store_yaw_band(s32 value);
s16 world_camera_pick_nearest_unblocked_yaw(s32 misc_id_a, s32 misc_id_b);
void world_camera_fusion_thread(void);
void world_camera_wait_until_idle(void);
void world_camera_thread(void);

/* gs */

/* screen */
extern s32 g_world_screen_fade;
extern RECT g_world_screen_clear_rect;
extern battle_gfx_fade_overlay_t g_world_screen_tint_tiles[]; /* one tint tile per packet buffer */

/* sound */
extern s16 g_world_sound_current_music_track_index;
extern s32 g_world_sound_effect_id_to_play; /* main_sfx_e; -1 while the event frame has no request */
extern s16 g_world_sound_music_switch_request;
extern s16 g_world_sound_music_track_1_id;
extern s16 g_world_sound_music_track_2_id;
extern s16 g_world_sound_music_unload_slot_request;
extern s32 g_world_sound_music_volume_transition_request;
extern u8 g_world_sound_release_held_loop;
void world_sound_dispatch_effect(void);
void world_sound_play_music_tracks(s32 slot_1_music_id, s32 slot_2_music_id);
void world_sound_set_effect_to_cancel(void);
void world_sound_set_effect_to_confirm(void);
void world_sound_set_effect_to_confirm_checked(void);
void world_sound_set_effect_to_invalid(void);
void world_sound_set_scroll_effect(void);
void world_sound_wait_for_music_stub(void);
void world_sound_bg_thread(void);
void world_sound_edit_bg_thread(void);

/* name */
/* Editing keys of world_name_apply_entry_key; any other key is the character
 * written at the cursor. */
typedef enum world_name_entry_key {
    WORLD_NAME_ENTRY_KEY_BACKSPACE = -1,
    WORLD_NAME_ENTRY_KEY_DELETE = -2,
    WORLD_NAME_ENTRY_KEY_INSERT_SPACE = -3,
    WORLD_NAME_ENTRY_KEY_CURSOR_LEFT = -4,
    WORLD_NAME_ENTRY_KEY_CURSOR_RIGHT = -5,
} world_name_entry_key_e;

extern u8 g_world_name_entry_character_rows[];
extern u8 g_world_name_entry_display_script[];
extern u8 g_world_name_entry_keyboard_script[];
extern s8 g_world_name_entry_started; /* name entry started */
extern u16 g_world_name_keyboard_text_cursor_anim;
extern u16 g_world_name_entry_text_cursor_anim;
extern RECT g_world_name_entry_screen_image_rect;
extern s16 g_world_name_entry_key_codes[];
extern u8 g_world_name_entry_confirm_message_shown;
extern u16 g_world_name_entry_key_cursor_anim;
extern u8 g_world_name_entry_char_to_special_column[];
extern u8 g_world_name_entry_special_to_char_column[];
extern u8 g_world_name_entry_cursor_x_positions[];
extern RECT g_world_name_entry_text_vram_rect;
extern s16 g_world_name_entry_text_row_ids[];
extern world_menu_entry_t g_world_name_confirm_menu;
extern u8 g_world_name_confirm_initialized;
extern s8 g_world_name_keyboard_last_state; /* last drawn state */
extern s8 g_world_name_entry_last_state;    /* last drawn name-entry state */
extern u8 g_world_name_entry_input_delay;
extern u8 g_world_name_confirm_finished;
extern u8 g_world_name_used_original_name;
extern s16 g_world_name_entry_key_ids[];
extern u8 g_world_name_entry_state;
extern u8 g_world_name_entry_saved_state;
extern u8 g_world_name_entry_cursor_index;
extern u8 g_world_name_entry_list_cursor;
extern u8 g_world_name_entry_cursor;
extern u8 g_world_name_entry_text[17];
extern u8* g_world_name_entry_original_name;
extern s16 g_world_name_confirm_result;
void world_name_apply_entry_key(s16 key);
void world_name_run_confirmation_menu(void);
u8* world_name_run_entry_screen(u8* list, u32* image);
u8* world_name_run_entry_keyboard_frame(u8* list, s32 unused_1);
void world_name_update_entry_cursor(void);

/* map */
/* State of a screen request raised from the world map menu (the
 * g_world_*_request_state globals stepped by world_menu_run_frame):
 * fade out, run the screen, fade back in. States 4 to 7 only count frames. */
typedef enum world_screen_request_state {
    WORLD_SCREEN_REQUEST_IDLE = 0,
    WORLD_SCREEN_REQUEST_START_FADE_OUT = 1,
    WORLD_SCREEN_REQUEST_WAIT_FADE_OUT = 2,
    WORLD_SCREEN_REQUEST_RUN = 3,
    WORLD_SCREEN_REQUEST_START_FADE_IN = 8,
    WORLD_SCREEN_REQUEST_WAIT_FADE_IN = 9,
} world_screen_request_state_e;

extern u16 g_world_map_append_state;
extern u16 g_world_map_reload_state;
extern map_selected_tile_t g_world_map_selected_tile_data;
extern s16 g_world_map_destruction_wait_state;
extern u16 g_world_map_refresh_pending;
extern RECT g_world_tile_info_image_location;
extern world_gfx_image_load_parameters_t g_world_tile_info_image_params[2];
extern u16 g_world_map_darkness_wait_frames[];
extern s16 g_world_brave_story_request_state; /* world_screen_request_state_e */
extern s32 g_world_map_light_scale[4];        /* current light scale (VECTOR) */
extern MATRIX g_world_map_light_base_matrix;
extern s16 g_world_map_light_rotation[4]; /* current light rotation (SVECTOR) */
extern MATRIX g_world_map_light_color_matrix;
void world_map_data_load_complete_thread(void);
void world_menu_selected_tile_info_display_thread(void);
void world_map_store_selected_tile_data(void);
void world_map_wait_for_refresh(void);
void world_map_light_thread(void);

/* status */
/* Status-ID-indexed image source rectangles at 0x8018cf68. Not const: a const
 * qualifier lets GCC hoist world_item_build_status_list_polygons' loads above
 * its rect stores, which the target does not do. */
extern RECT g_world_status_display_image_rects[BATTLE_STATUS_COUNT];

/* Per-byte status removal masks applied while an event stages a unit. */
extern u8 g_world_status_removal_mask_unstaged[BATTLE_STATUS_BYTE_COUNT];
extern u8 g_world_status_removal_mask_staged[BATTLE_STATUS_BYTE_COUNT];
extern u8 g_world_status_removal_mask_staged_monster[BATTLE_STATUS_BYTE_COUNT];

/* Thread parameter blocks of the formation status panels (0x8018ba2c, eight
 * 0x14-byte world_status_thread_t records). */
extern world_status_thread_t g_world_status_display_thread_params;
extern world_status_thread_t g_world_preview_stats_thread_params;

/* gfx */
/* Separate decreasing/increasing RGB-intensity flags.
 * The getter packs them into bits 0/1 and returns 0 when disabled. */
typedef enum world_fade_state {
    WORLD_FADE_STATE_NONE = 0,
    WORLD_FADE_STATE_DECREASING = 1,
    WORLD_FADE_STATE_INCREASING = 2
} world_fade_state_e;

/* Provisional: record whose two TIM pointers at +0x1e00/+0x1e04 are filled
 * by world_script_load_tutorial_files from the 8-byte LBA/size tables at
 * 0x80193c40 and 0x80193c78. Only these fields are known. */
typedef struct world_gfx_tim_pair_record {
    u8 _unused_0000[0x1e00];
    void* tim_pointers[2];
} world_gfx_tim_pair_record_t;
typedef char world_tim_pair_record_tims_offset_must_be_0x1e00
    [((unsigned long)&((world_gfx_tim_pair_record_t*)0)->tim_pointers == 0x1e00) ? 1 : -1];

/* Provisional: the WORLD GPU packet buffer pointed to by 0x801cd528.  Only the
 * ordering table base and the DR_MODE pool used by 0x8012d340 are named; the
 * 12-byte pool at 0x5c is consumed by the sibling 0x8012d25c; the TILE pool
 * at 0x3c by 0x8012bdbc. */
typedef struct world_gfx_packet_buffer {
    u32* otag; /* 0x00 */
    u8 _unused_04[0x0c - 0x04];
    POLY_F4* poly_f4s;        /* 0x0c; pool consumed by world_gfx_append_poly_f4_to_otag */
    POLY_FT4* textured_quads; /* 0x10; pool consumed by world_gfx_enqueue_textured_quad_corners */
    u8 _unused_14[0x1c - 0x14];
    POLY_G4* gradient_quads;           /* 0x1c; world_gfx_enqueue_gouraud_quad */
    POLY_GT4* textured_gradient_quads; /* 0x20; initialised by world_gfx_init_packet_pools */
    LINE_F2* flat_lines;               /* 0x24; set up with SetLineF2 by world_gfx_init_packet_pools (0x80125e40),
                                          consumed by world_gfx_enqueue_line_f2 */
    u8 _unused_28[0x30 - 0x28];
    LINE_G2* gradient_lines; /* 0x30; world_menu_add_gradient_line_primitive */
    u8 _unused_34[0x3c - 0x34];
    TILE* tiles; /* 0x3c; pool consumed by 0x8012bdbc */
    u8 _unused_40[0x58 - 0x40];
    DR_MOVE* draw_moves; /* 0x58; pool consumed by world_gfx_enqueue_draw_move */
    DR_AREA* draw_areas; /* 0x5c; pool consumed by world_gfx_add_draw_area_primitive */
    DR_MODE* draw_modes; /* 0x60 */
    DRAWENV draw_env;    /* 0x64; installed by world_gfx_present_frame_and_swap_packet_buffer */
    DISPENV disp_env;    /* 0xc0; installed by world_gfx_present_frame_and_swap_packet_buffer */
    u8 _unused_d4[0xec - 0xd4];
} world_gfx_packet_buffer_t;

typedef char world_packet_buffer_gradient_lines_offset
    [((unsigned long)&((world_gfx_packet_buffer_t*)0)->gradient_lines == 0x30) ? 1 : -1];

/* Flags word that leads every display object: the WLDCORE window and render
 * records and the OPEN render records. The two kind bits select the draw
 * routine; an object with either BLANK or HIDDEN set is skipped. */
typedef enum wldcore_display_object_flag {
    WLDCORE_DISPLAY_OBJECT_ANIMATED = 0x001,  /* 36-byte record drawn from its animation sequence */
    WLDCORE_DISPLAY_OBJECT_IMAGE = 0x002,     /* 52- or 56-byte record drawn as one sprite */
    WLDCORE_DISPLAY_OBJECT_8BIT_CLUT = 0x004, /* image: 8-bit texture instead of 4-bit */
    WLDCORE_DISPLAY_OBJECT_BLANK = 0x008,     /* nothing to draw, whatever HIDDEN says */
    WLDCORE_DISPLAY_OBJECT_HIDDEN = 0x010,    /* the show/hide toggle */
    /* Semi-transparent at the rate in BLEND_RATE_MASK; a WLDCORE animated object otherwise takes
     * both from each cel part. */
    WLDCORE_DISPLAY_OBJECT_SEMI_TRANSPARENT = 0x020,
    WLDCORE_DISPLAY_OBJECT_BLEND_RATE_MASK = 0x0c0,
    WLDCORE_DISPLAY_OBJECT_POP_IN = 0x100,   /* image: the opening animation is running; clears itself */
    WLDCORE_DISPLAY_OBJECT_UNTINTED = 0x400, /* WLDCORE: the colour is used as is, without the fade tint */
    /* WLDCORE animated object: steps its frame counter even while system flag 0x08000000 is set. */
    WLDCORE_DISPLAY_OBJECT_ANIMATE_WHILE_PAUSED = 0x800,
} wldcore_display_object_flag_e;

/* One frame of a WLDCORE display-object or OPEN sprite-actor animation
 * sequence: the cel (part list) it draws and how long it holds. duration
 * 0xffff pins the frame. */
typedef struct wldcore_anim_frame {
    u16 cel;      /* 0x00 */
    u16 duration; /* 0x02 */
} wldcore_anim_frame_t;

/* An animation: a frame count followed by its frames. */
typedef struct wldcore_anim_sequence {
    s32 frame_count;                /* 0x00 */
    wldcore_anim_frame_t frames[1]; /* 0x04 */
} wldcore_anim_sequence_t;

/* Padded pixel size of a text image (wldcore_text_get_padded_dimensions,
 * open_text_measure_menu_image). */
typedef struct wldcore_text_dimensions {
    s32 width;
    s32 height;
} wldcore_text_dimensions_t;

/* One remembered list position (g_world_menu_selection_records, 14 entries / 0x54 bytes), the
 * WORLD twin of bunit_menu_selection_record_t. world_menu_set_selection_record stores the cursor
 * index, the scroll base and the masked id of the entry under the cursor;
 * world_menu_get_selection_record reads them back and re-finds entry_id in the live list when
 * the remembered index no longer points at the same entry. */
typedef struct world_menu_selection_record {
    s16 selected_index;    /* 0x00 */
    s16 scroll_base_index; /* 0x02 */
    s16 entry_id;          /* 0x04; list entry id, low 10 bits */
} world_menu_selection_record_t;

typedef char wldcore_text_dimensions_size_must_be_8[(sizeof(wldcore_text_dimensions_t) == 8) ? 1 : -1];

/* Provisional: the column view of the 36-byte WLDCORE window records and OPEN
 * render records, rebased on their x/y pair (+0x18) and keeping the 0x24-byte
 * stride. */
typedef struct wldcore_window_xy {
    s32 x;
    s32 y;
    u8 _unused_08[0x1c];
} wldcore_window_xy_t;

typedef char wldcore_window_xy_size_must_be_0x24[sizeof(wldcore_window_xy_t) == 0x24 ? 1 : -1];

/* Colour triple of the 36-byte WLDCORE window records and OPEN render records
 * (+0x20), rebased so the stores are `sb 0/1/2(records + 0x20 + index * 36)`. */
typedef struct wldcore_window_entry_36_rgb {
    u8 red;
    u8 green;
    u8 blue;
    u8 _unused_03[0x21];
} wldcore_window_entry_36_rgb_t;

typedef char wldcore_entry_36_rgb_size_must_be_0x24[sizeof(wldcore_window_entry_36_rgb_t) == 0x24 ? 1 : -1];

/* The index argument of world_text_bind_section_data: the resident text data
 * it publishes in g_world_text_section_pointers. */
typedef enum world_text_data {
    WORLD_TEXT_DATA_SKILLSET_NAMES = 0,
    WORLD_TEXT_DATA_JOB_NAMES = 1,
    WORLD_TEXT_DATA_ABILITY_NAMES = 2,
    WORLD_TEXT_DATA_FORMATION_MESSAGES = 3,   /* TEXT_SECTION_FORMATION_MESSAGES */
    WORLD_TEXT_DATA_DISMISS_UNIT_REPLIES = 4, /* WORLD_TEXT_SECTION_FORMATION_TEXT */
    WORLD_TEXT_DATA_JOB_DESCRIPTIONS = 5,     /* WORLD_TEXT_SECTION_FORMATION_TEXT */
    WORLD_TEXT_DATA_NAME_ENTRY_KEYBOARD = 7,
    WORLD_TEXT_DATA_SHOP_DIALOGUE = 8, /* WORLD_TEXT_SECTION_SHOP_DIALOGUE */
} world_text_data_e;

extern battle_menu_status_panel_buffer_t g_world_change_banner_panel_frames[2];

/* Scenario interpreter interfaces shared by battle_script_execute_event and
 * world_script_execute_event. Signatures are as the interpreters bind them;
 * unverified names stay provisional. */
extern s16 g_world_3d_object_use_request;
extern u16 g_world_3d_object_wait_status;
extern u32* g_world_gfx_active_otag_entries;
extern world_gfx_packet_buffer_t* g_world_gfx_active_packet_buffer;
extern u16 g_world_gfx_draw_primitives_immediately;
extern u16 g_world_gfx_draw_suppress_flag;
extern u8 g_world_gfx_fade_decreasing;
extern u8 g_world_gfx_fade_disabled;
extern u8 g_world_gfx_fade_increasing;
extern s16 g_world_gfx_fade_intensity;
extern world_glyph_blit_t g_world_gfx_image_blit_destination;
extern world_glyph_blit_t g_world_gfx_image_blit_source;
extern world_menu_selection_record_t g_world_menu_selection_records[];
extern RECT g_world_gfx_temporary_draw_area;
extern u8 g_world_gfx_texture_allocation_grid[15][16];
extern s8 g_world_gfx_vsync_mode;
extern u16 g_world_gfx_textured_quad_count;

extern world_image_location_t g_world_gfx_menu_image_source;
extern RECT g_world_gfx_texture_window;
extern u16 g_world_gfx_draw_area_scale_percent[];

/* A full-screen texture window (x=0, y=0, w=0x100, h=0x100 in the WORLD data
 * at 0x8016e44c), copied verbatim into a menu record's DR_MODE by
 * world_screen_gfx_init_scaled_draw_area_packets; the formation view reads the same
 * bytes as a zero screen point. Same 8-byte RECT shape as
 * g_world_gfx_texture_window, whose reset sibling is byte-for-byte identical. */
extern const RECT g_world_gfx_full_texture_window;

/* Frame-buffer rectangles for the four WLDFACE pages; four separate objects,
 * since as RECT[4] world_gfx_load_wldface_to_frame_buffer shares one base. */
extern RECT g_world_wldface_vram_rects;
extern RECT g_world_wldface_vram_rect_1;
extern RECT g_world_wldface_vram_rect_2;
extern RECT g_world_wldface_vram_rect_3;
extern u16 g_world_portrait_clut_by_sheet_id[]; /* CLUT word per sprite-sheet id */
extern RECT g_world_gfx_portrait_origin;
extern const u16 g_world_gfx_panel_draw_area_scale_percent[12];
extern u8 g_world_gfx_portrait_transition_offsets[];
extern u8 g_world_gfx_black_tile_color[];
extern world_status_thread_t g_world_change_banner_panel_thread_params;
extern world_gfx_textured_quad_source_t g_world_vertical_cursor_quad;
extern RECT g_world_gfx_fade_tile_rect;
/* Sector and size pairs, read as flat words ([i * 2], [i * 2 + 1]): a record
 * member access changes how the target addresses them. */
extern s32 g_world_script_tutorial_message_files[7 * 2];
extern s32 g_world_script_tutorial_script_files[7 * 2];

/* The sprites world_gfx_build_zodiac_primitives draws: texture page, frames, 12-bit fixed
 * offsets and count. */
extern world_image_location_t g_world_gfx_zodiac_texture_page;
extern main_zodiac_sprite_frame_t g_world_gfx_zodiac_sprite_frames[];
extern point16_t g_world_gfx_zodiac_sprite_offsets[];
extern u8 g_world_gfx_zodiac_sprite_count;

extern s32 g_world_gfx_free_rect_texels[4];
extern s32 g_world_gfx_free_rect_grid_cells[4];
extern RECT g_world_gfx_draw_area_template;
extern u16 g_world_gfx_draw_area_y;
extern u8 g_world_change_banner_text_image[];

/* Fade tile colour: the only byte-packed triple in WORLD bss; the fade
 * updaters pass &r as the rgb pointer and store g/b. */
extern CVECTOR g_world_fade_tile_color;
extern u16 g_world_gfx_flat_line_capacity; /* +0x24 flat line count */
extern u16 g_world_gfx_line_f3_capacity;
extern u16 g_world_gfx_gradient_line_capacity; /* gradient line count */
extern u16 g_world_gfx_line_g3_capacity;
extern u16 g_world_gfx_line_f4_capacity;
extern u16 g_world_gfx_line_g4_capacity;
extern u16 g_world_gfx_poly_ft3_capacity;
extern u16 g_world_gfx_flat_line_count;
extern u16 g_world_gfx_poly_gt3_capacity;
extern u16 g_world_gfx_textured_quad_capacity; /* textured quad count */
extern u16 g_world_gfx_line_f3_count;
extern u16 g_world_gfx_gradient_line_count;
extern u16 g_world_gfx_textured_gradient_quad_capacity; /* textured gradient quad count */
extern u16 g_world_gfx_line_g3_count;
extern u16 g_world_gfx_line_f4_count;
extern u16 g_world_gfx_line_g4_count;
extern u16 g_world_gfx_poly_ft3_count;
extern u16 g_world_gfx_poly_gt3_count;
extern u16 g_world_gfx_textured_gradient_quad_count;
extern u16 g_world_gfx_sprite_8_capacity;
extern u16 g_world_gfx_draw_buffer_clip_y;

extern u16 g_world_gfx_sprite_16_capacity;

extern u16 g_world_ot_length;

/* Volatile: world_gfx_init_double_packet_buffers must read the draw-area count
 * after the draw-move count, as the target does. */
extern volatile u16 g_world_gfx_draw_area_capacity;
extern u16 g_world_gfx_draw_area_count;
extern u16 g_world_gfx_draw_mode_capacity;
extern u16 g_world_gfx_tile_capacity; /* tile count */
extern u16 g_world_gfx_draw_mode_count;

/* Draw move count; volatile for the same read order as g_world_gfx_draw_area_capacity. */
extern volatile u16 g_world_gfx_draw_move_capacity;
extern u16 g_world_gfx_tile_count;

extern u16 g_world_gfx_draw_move_count;
extern u16 g_world_gfx_sprite_count;
extern u16 g_world_gfx_tile_1_capacity;
extern u16 g_world_gfx_tile_8_capacity;
extern u16 g_world_gfx_tile_1_count;
extern u16 g_world_gfx_tile_8_count;

extern world_gfx_packet_buffer_t* g_world_gfx_packet_buffers;
extern u16 g_world_gfx_sprite_8_count;

extern u16 g_world_gfx_tile_16_capacity;

extern u16 g_world_gfx_tile_16_count;

extern u16 g_world_gfx_sprite_16_count;

extern u16 g_world_gfx_poly_f3_capacity;
extern u16 g_world_gfx_poly_g3_capacity;
extern u16 g_world_gfx_poly_f4_capacity;
extern u16 g_world_gfx_gradient_quad_capacity; /* gradient quad count */
extern u16 g_world_gfx_poly_f3_count;
extern u16 g_world_gfx_poly_g3_count;
extern u16 g_world_gfx_poly_f4_count;
extern u16 g_world_gfx_gradient_quad_count;

extern u16 g_world_gfx_sprite_capacity;
extern s32 g_active_graphics_buffer_index;
void world_gfx_set_party_unit_portrait_poly_texture(s32 portrait_id, POLY_FT4* poly);
void world_menu_display_text_string(u8* image, world_glyph_blit_t* origin, u8* text);
void world_display_menu_number_entry(s32 value, u8 digit_count, void* image, world_glyph_blit_t* origin);

void world_panel_gfx_build_scaled_draw_area_packets(
    world_gfx_scaled_draw_area_pair_t* packets, const void* geometry, s32 scale_index, s32 frame_offset, void* thread);

void world_gfx_add_draw_area_primitive(RECT* rect, s32 otag_index);
void world_gfx_enqueue_draw_mode(s32 dfe, s32 dtd, s32 tpage, RECT* texture_window, s32 ot_index);
void world_gfx_enqueue_draw_move(RECT* src, s32 x, s32 y, s32 ot_index);

void world_gfx_enqueue_textured_quad_list(
    battle_menu_sprite_cell_t* cell, u8* rgb, s32 semi_trans, u16 tpage, u16 clut, s32 ot_index, s32 count);

void world_gfx_alloc_tpage7_vram(RECT* rect, world_texture_prim_t* prim, u32* image);
void world_gfx_enqueue_textured_quad_corners(
    world_gfx_textured_quad_source_t* source, u8* rgb, s32 semi_trans, s32 ot_index);
void world_gfx_enqueue_gouraud_quad(s16* xy, u8* rgb, s32 semi_trans, s32 ot_index);
void world_gfx_enqueue_gouraud_textured_quad(
    world_gfx_textured_rect_source_t* source, u8* rgb, s32 semi_trans, s32 ot_index);
void world_gfx_enqueue_tile(RECT* box, u8* rgb, u8 semi_trans, s32 ot_index);
u8* world_text_bind_section_data(u32 index);
void world_gfx_build_scaled_draw_area_pair_swapped(
    world_gfx_scaled_draw_area_pair_t* packets, void* geometry, s32 step, s32 flag);
void world_menu_clear_selection_record(s32 index);
void world_menu_clear_selection_records(void);
void world_gfx_clear_tpage7_vram_allocation_grid(void);

/* Portrait primitive helpers shared with the EVENT debug character editor.
 * world_gfx_clip_portrait_poly_from_right/right are named for the way they move the
 * quad: _right shifts x right and trims the trailing texture edge (debugchr's
 * clip_portrait_poly_from_left), _left is debugchr's clip_..._from_right. */
void world_gfx_clip_portrait_poly_from_right(POLY_FT4* poly, s32 delta);
void world_gfx_clip_portrait_poly_from_left(POLY_FT4* poly, s32 delta);
void world_menu_copy_billboard_data_in(const void* source_a, const void* source_b, const void* source_c);
void world_menu_copy_billboard_data_out(void* dest0, void* dest1, void* dest2, void* dest3);
void world_gfx_draw_or_append_gpu_primitive(void* primitive);
void world_gfx_submit_screen_fade_overlay(battle_gfx_fade_overlay_t* prims, s32 intensity);
void world_gfx_draw_signed_number(s32 value, s32 digits, void* image, world_glyph_blit_t* pen);

void world_gfx_enqueue_oriented_textured_quad(
    const world_oriented_quad_t* descriptor, const u8* color, s32 orientation, s32 semitrans, s32 otag_index);

void world_gfx_flip_poly_ft4_u_if_negative(POLY_FT4* poly, s32 direction);

void world_gfx_init_double_packet_buffers(world_gfx_packet_buffer_t* buffers, u32* otags, void* unused_2,
    void* unused_3, void* unused_4, POLY_FT4* textured_quads, void* unused_6, void* unused_7, POLY_G4* gradient_quads,
    POLY_GT4* textured_gradient_quads, LINE_F2* flat_lines, void* unused_11, void* unused_12, LINE_G2* gradient_lines,
    void* unused_14, void* unused_15, TILE* tiles, void* unused_17, void* unused_18, void* unused_19, void* unused_20,
    void* unused_21, void* unused_22, DR_MOVE* draw_moves, DR_AREA* draw_areas, DR_MODE* draw_modes);

void world_gfx_init_image_loading(void* prim, const void* load_origin, const void* screen_origin,
    const world_gfx_image_load_parameters_t* parameters);

void world_gfx_init_packet_pools(world_gfx_packet_buffer_t* buffer);
void world_screen_menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, RECT* from, RECT* to, s32 t);
void world_gfx_mirror_4bpp_rect(u8* pixels, world_glyph_blit_t* rect);
void world_gfx_move_image_and_wait(RECT* rect, s16 x, s16 y);
void world_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* pair);
void world_panel_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* pair);
void world_gfx_set_blit_dest_position(s32 x, s32 y);
void world_gfx_set_fade_disabled(u8 disabled);
void world_gfx_set_draw_mode_for_texture_page(DR_MODE* mode, s32 image_type);
void world_gfx_set_portrait_poly_texture(POLY_FT4* poly, s32 code);
void world_gfx_set_primitive_brightness(void* primitive, s32 brightness);
void world_menu_set_selection_record(s32 index, s32 selected_index, s32 scroll_base_index, s16* entries);
void world_menu_submit_numeric_display_frame_primitives(world_menu_palette_primitives_t* frame);

void world_gfx_enqueue_textured_quad(
    RECT* rect, s32 u, s32 v, void* color, s32 semi_trans, u16 texture_page, u16 clut, s32 priority);

void world_gfx_free_tpage7_vram(RECT* rect);
s32 world_gfx_get_fade_state(void);
void world_menu_get_selection_record(s32 index, s16* selected_index, s16* scroll_base_index, s16* entries);
s32 world_gfx_get_vsync_mode_or_one(void);
void world_menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, RECT* from, RECT* to, s32 t);
void world_gfx_load_image_and_wait(RECT* rect, void* data);
void world_script_load_tutorial_files(s32 index, world_gfx_tim_pair_record_t* record);
void world_gfx_load_wldface_to_frame_buffer(void);
void world_gfx_store_image_and_wait(RECT* rect, void* dest);
void world_gfx_present_frame_and_swap_packet_buffer(s32 first_otag, s32 draw_otag);
void world_gfx_ramp_screen_overlay_intensity_to_max(void);
void world_gfx_set_blit_source_rect(s16 x, s16 y, s16 width, s16 height);
void world_gfx_set_clut_rect_from_id(RECT* rect, u16 clut_id);

void world_gfx_set_draw_mode_from_rect(DR_MODE* mode, const u16* rect);
void world_gfx_set_image_blit_destination_origin(s16 x, s16 y);
void world_gfx_set_image_blit_source_rect(s16 x, s16 y, s16 width, s16 height);
void world_gfx_start_decreasing_fade(void);
void world_gfx_start_increasing_fade(void);

s32 world_gfx_update_fade_in_tile(void);
s32 world_gfx_update_fade_out_tile(void);

void world_script_assign_bitset_flag(u32* bits, s32 index, s32 set);
void world_gfx_load_evtchr_thread(void);
void world_script_resetpalette_event_instruction(const u8* parameters);

/* status panel */
extern s16 g_world_auto_battle_option_colors[5]; /* text colour per option */
void world_menu_run_battle_help(void);

/* ability */
/* Final selector for the WORLD ability-list builder at 0x801228f0. */
typedef enum world_ability_list_mode {
    WORLD_ABILITY_LIST_MODE_VIEW_ALL = 0,
    WORLD_ABILITY_LIST_MODE_LEARNED_ONLY = 1,
    WORLD_ABILITY_LIST_MODE_LEARN_MENU = 2,
    WORLD_ABILITY_LIST_MODE_COUNT_UNLEARNED = 3,
} world_ability_list_mode_e;

/* Provisional ability-list window layout at 0x80153c78: a
 * world_menu_text_layout_t whose three columns are ids, values and extras
 * (x[0] is at 0x04). */
typedef struct world_ability_menu_layout {
    s16 visible_rows; /* 0x00 */
    s16 hidden_rows;  /* 0x02 */
    u8 _unused_04[2];
    s16 values_x;     /* 0x06: x[1] of world_menu_text_layout_t */
    s16 extras_x;     /* 0x08: x[2]; a column x origin, not a y */
    s16 ids_mode;     /* 0x0a: mode[0] (menu_text_column_mode_e) */
    s16 values_mode;  /* 0x0c: mode[1] */
    s16 extras_mode;  /* 0x0e: mode[2] */
    u16* ids;         /* 0x10 */
    u16* values;      /* 0x14 */
    u16* extras;      /* 0x18 */
    s16* row_actions; /* 0x1c; indexed by the selected skill */
    u8 _unused_20[8];
    s16 row_offset; /* 0x28; first visible row */
} world_ability_menu_layout_t;
typedef char world_ability_menu_layout_size_must_be_0x2c[sizeof(world_ability_menu_layout_t) == 0x2c ? 1 : -1];

extern s16 g_world_ability_category; /* menu_ability_category_e */
extern s32 g_world_ability_selected_skill_ct;
extern s32 g_world_ability_selected_type;
extern s32 g_world_secondary_bit_read_index;
extern u8 g_world_secondary_bit_cursor_reset;
extern u8* g_world_secondary_bit_cursor;
extern s32 g_world_spell_quote_last_ability_id;
extern world_ability_menu_layout_t g_world_ability_menu_layout;
extern u8 g_world_spell_quote_exception_skillsets[];
extern RECT g_world_ability_panel_origin;
extern RECT g_world_ability_panel_draw_area;
extern world_status_thread_t g_world_ability_panel_thread_params;
extern s8 g_world_ability_menu_initialized;
extern s8 g_world_remove_ability_initialized;
extern s8 g_world_set_ability_initialized;
extern s8 g_world_ability_view_list_initialized;
extern u8 g_world_ability_menu_slots_changed;
extern world_menu_entry_t g_world_ability_menu;
extern u8 g_world_ability_submenu_running;
extern u8 g_world_ability_menu_unit_index;
extern s8 g_world_remove_ability_unit_index;
extern u16 g_world_remove_ability_cursor_anim;
extern point16_t g_world_remove_ability_cursor_point;
extern point16_t g_world_clear_abilities_cursor_point;
extern u8 g_world_set_ability_window_script[];
extern s8 g_world_set_ability_slot;               /* ability slot under the cursor: 0..4 */
extern s8 g_world_set_ability_list_close_pending; /* close the ability list this frame */
extern s8 g_world_set_ability_show_stat_preview;  /* show the stat preview window */
extern s8 D_8018D18A;                             /* zeroed when the Set Ability menu opens; never read */
extern u16 g_world_set_ability_cursor_anim;
extern point16_t g_world_set_ability_cursor_point;
extern u8 g_world_ability_list_script[];
extern s32 g_world_menu_selected_command;
extern world_menu_list_page_t g_world_ability_list_pages[][2];
extern u8 g_world_ability_menu_saved_browse_enabled;
extern s8 g_world_remove_ability_cursor;
extern s8 g_world_clear_abilities_prompt_active;
extern s16 g_world_remove_ability_slots[5];
extern volatile u16 g_world_clear_abilities_blink_counter; /* volatile: re-read right after the increment */
extern s8 g_world_set_ability_list_open;                   /* ability list open */
extern s8 g_world_set_ability_entry_count;                 /* ability list entry count */
extern u8 g_world_set_ability_unit_index;
extern u8 g_world_set_ability_cursor;

/* [0]: 0x0c = Learn Abilities, 0x0d = view only. */
extern s16 g_world_ability_entries[];
extern u8* g_world_selected_ability_secondary_data;
extern u8* g_world_selected_ability_data;
s32 world_ability_find_unit_abilities(s16 unit, s16 job, s32 kind, s16* out, s32 mode);
s32 world_ability_is_non_action_learned(s32 index);

void world_ability_find_stat_changes_due_to_equipped(
    world_item_stat_detail_t* stat_diff, s32 old_ability, s32 new_ability);

s32 world_ability_get_ct_display_value(s32 index);
s32 world_ability_get_jp_cost_display_value(s32 index);
void world_ability_get_move_and_jump_increase_values(s16 ability, world_item_stat_detail_t* out);
s32 world_ability_get_mp_cost_display_value(s32 index);
s32 world_ability_has_secret_hunt(s32 unit_id);
s32 world_ability_has_two_hands(s16 unit_id);
s32 world_ability_has_two_swords(s16 unit_id);
s32 world_ability_is_category_action(s32 unused_row);
s32 world_ability_is_category_movement(s32 unused_row);
s32 world_ability_is_category_reaction(s32 unused_row);
s32 world_ability_is_category_support(s32 unused_row);
s32 world_ability_is_equip_support_change(s16 unit_id, s16 ability_id);
s32 world_ability_is_non_action_unlearned(s32 index);
s32 world_ability_is_selected_action_type(s32 unused_row);
void world_ability_learn(s16 unit_id, s16 job_id, s16 ability_id);
s32 world_ability_run_view_list(void);
s32 world_ability_build_equippable_rsm_list(s16 unit_index, s32 slot, s16* out);
s32 world_ability_run_learn_list(void);

/* job */
/* The formation's job screens: the job wheel and the job change, the Learn Abilities job list and its
 * cached numbers, and the job lookups the ability lists use. */

/* Result of world_formation_step_job_change_animation, kept in
 * g_world_job_change_anim_state: the phase of the job-change dissolve. */
typedef enum world_job_change_anim {
    WORLD_JOB_CHANGE_ANIM_DONE = 0,
    WORLD_JOB_CHANGE_ANIM_DISSOLVE = 1,
    WORLD_JOB_CHANGE_ANIM_CLOSING = 2, /* the last 20 frames */
} world_job_change_anim_e;

/* An entry of the job list world_job_build_unit_list writes. */
enum {
    WORLD_JOB_LIST_ENTRY_ID_MASK = 0x3ff,
    /* Set in mode INCLUDE_OTHER_UNIT_UNLOCKS on the unit's current job and on
     * a job only another unit has unlocked: the job wheel shows the entry
     * but does not change to it. */
    WORLD_JOB_LIST_ENTRY_UNAVAILABLE = 0x4000,
};

/* The one mode world_job_build_unit_list tests; its callers also pass 0 and 1. */
enum { WORLD_JOB_LIST_MODE_INCLUDE_OTHER_UNIT_UNLOCKS = 2 };

extern s32 g_world_job_cached_level;
extern s32 g_world_job_cached_level_jp_requirement;
extern s32 g_world_job_cached_skillset_empty;
extern s32 g_world_job_cached_total_jp;
extern s16 g_world_job_wheel_job_count;
extern s16 g_world_job_wheel_name_text;
extern s8 g_world_learn_menu_initialized;
extern s8 g_world_job_wheel_initialized;
extern s8 g_world_learn_job_list_initialized;
extern s8 g_world_learn_ability_list_open;
extern s8 g_world_view_abilities_initialized;
extern s16 g_world_job_wheel_unit_index;

/* The current and the new job's portrait; separate objects, since
 * world_formation_run_job_wheel_menu addresses each with its own lui/addiu. */
extern world_gfx_textured_rect_source_t g_world_job_wheel_portrait_quad;
extern world_gfx_textured_rect_source_t g_world_job_wheel_new_portrait_quad;
extern u8 g_world_job_wheel_script[];
extern u8 g_world_job_wheel_job_selected;
extern point16_t g_world_job_wheel_unit_sprite_position;
extern u8 g_world_job_wheel_center_x;
extern u8 g_world_job_wheel_center_y;
extern u8 g_world_job_wheel_animation_counter;
extern RECT g_world_job_change_old_sprite_rect;
extern RECT g_world_job_change_new_sprite_rect;
extern u8 g_world_job_change_active;
extern s16 g_world_job_change_frame;
extern u8 g_world_job_change_wave_colors[];
extern u8 g_world_job_wheel_close_frame;
extern u8 g_world_job_wheel_open_frame;
extern u8 g_world_job_wheel_closing;
extern u8 g_world_job_wheel_opening;
extern world_oriented_quad_t g_world_job_wheel_mastered_icon_sprite;
extern u8 g_world_learn_job_list_script[];
extern u8 g_world_learn_confirm_menu[];

/* The prompt chained after g_world_learn_confirm_menu. */
extern world_menu_entry_t g_world_learn_confirm_submenu;
extern s16 g_world_learn_job_name_list[];
extern s16 g_world_view_job_name_list[];
extern s8 g_world_job_wheel_spin_progress;
extern s8 g_world_job_wheel_spin_direction;
extern s8 g_world_job_change_anim_state; /* world_job_change_anim_e */
extern u8 g_world_job_change_confirmed;
extern u8 g_world_job_wheel_saved_browse_enabled;
extern u8 g_world_job_change_flash_colors[4][3];
extern s16* g_world_job_change_dissolve_order;
extern u8* g_world_job_change_old_sprite_image;
extern u8* g_world_job_change_new_sprite_image;
extern u8* g_world_job_change_new_sprite_source;
extern s16 g_world_job_change_skip_requested;
extern s16 g_world_job_wheel_cursor_index;
extern s16 g_world_job_wheel_jobs[];
extern s16 g_world_job_wheel_mastered_jobs[];
extern RECT g_world_job_wheel_name_rect;
extern s8 g_world_learn_menu_step; /* menu_job_list_step_e */

/* Nonzero while the learn-confirmation menu thread (thread 6) is running. */
extern u8 g_world_learn_confirm_prompt_state;
extern s8 g_world_view_abilities_step; /* menu_job_list_step_e */
extern u8 g_world_view_abilities_confirm_prompt_state;
extern s16 g_world_learn_selected_job;
extern s16 g_world_learn_selected_job_index;
s32 world_job_build_unit_list(u8 unit_index, s16* jobs, s32 mode);
s32 world_job_find_first_for_skillset(s32 skillset);
s32 world_job_get_base(s16 formation_idx);

/* Job id -> generic job index 0..0x13, else 0. */
s32 world_job_get_generic_index(s32 job_id);
s32 world_job_get_skillset(s32 job_id);
s32 world_job_get_text_index(s16 job_id);
s32 world_job_is_locked_for_all_units(s16 job_id);
s32 world_job_is_special_monster(s32 job_id);

/* Menu script callbacks (g_world_menu_script_callbacks): the Learn Abilities job list sets the
 * calculate_* ones, which cache the cursor job's numbers, and the ability lists the get_* ones,
 * which return that cache or the chosen job's JP. */
s32 world_job_calculate_current_level(s32 index);
s32 world_job_calculate_current_mastered(s32 index);
s32 world_job_calculate_current_total_jp(s32 index);
s32 world_job_calculate_next_level_jp_requirement(s32 index);
s32 world_job_get_cached_level(s32 unused_row);
s32 world_job_get_cached_level_jp_requirement(s32 unused_row);
s32 world_job_get_cached_skillset_empty_flag(s32 unused_row);
s32 world_job_get_cached_total_jp(s32 unused_row);
s32 world_job_get_selected_unit_job_points_entry(s32 unused_row);

/* equipment */
typedef enum world_equipment_slot {
    WORLD_EQUIPMENT_SLOT_RIGHT_HAND = 0,
    WORLD_EQUIPMENT_SLOT_LEFT_HAND = 1,
    WORLD_EQUIPMENT_SLOT_HEAD = 2,
    WORLD_EQUIPMENT_SLOT_BODY = 3,
    WORLD_EQUIPMENT_SLOT_ACCESSORY = 4,
    WORLD_EQUIPMENT_SLOT_COUNT = 5,
} world_unit_equipment_slot_e;

extern RECT g_world_equipment_panel_origin;
extern RECT g_world_equipment_ability_panel_origin;
extern RECT g_world_equipment_panel_draw_area;
extern RECT g_world_equipment_ability_panel_draw_area;
extern RECT g_world_equipment_panel_item_text_rect;
extern RECT g_world_equipment_panel_ability_text_rect;
extern world_gfx_image_load_parameters_t g_world_equipment_panel_sprite_params_0[19];
extern world_gfx_image_load_parameters_t g_world_equipment_panel_sprite_params_1[19];
extern world_gfx_image_load_parameters_t g_world_equipment_panel_sprite_params_2[19];
extern world_gfx_image_load_parameters_t g_world_equipment_panel_sprite_params_3[19];
extern world_gfx_image_load_parameters_t g_world_equipment_panel_icon_params[5];
extern world_status_thread_t g_world_equipment_ability_panel_thread_params;
extern s8 g_world_equip_menu_initialized;
extern u8 g_world_equip_item_menu_script[];
extern s8 g_world_equip_item_slot;
extern s8 g_world_equip_item_list_refresh;
extern s8 g_world_equip_item_preview_active;
extern u16 g_world_equip_item_cursor_anim;
extern point16_t g_world_equip_item_cursor_point;
extern u16 g_world_remove_all_equipment_ids[5];
extern point16_t g_world_remove_all_equipment_cursor_point;
extern s8 g_world_equip_item_list_open;
extern s16 g_world_equip_item_previewed_cursor;
extern s8 g_world_remove_all_equipment_prompt_active;
extern volatile u16 g_world_remove_all_equipment_blink_counter;

/* editor */
extern world_image_location_t g_world_editor_numeric_geometry[];
extern RECT g_world_editor_numeric_table[2];
extern RECT g_world_editor_numeric_entries_a[];
extern RECT g_world_editor_numeric_entries_b[];
extern RECT g_world_editor_numeric_entries_c[];
extern RECT g_world_editor_numeric_entries_d[];
extern RECT g_world_editor_numeric_entries_e[];
extern RECT g_world_editor_numeric_entries_f[];
extern world_gfx_image_load_parameters_t g_world_editor_numeric_texture[];
extern menu_number_entry_t g_world_editor_numeric_descriptor_a[];
extern menu_number_entry_t g_world_editor_numeric_descriptor_b[];
extern battle_menu_status_panel_numeric_buffer_t g_world_editor_numeric_state_a[2];
extern battle_menu_status_panel_numeric_buffer_t g_world_editor_numeric_state_b[2];
extern u8 g_world_editor_numeric_text_a[];
extern u8 g_world_editor_numeric_text_b[];

/* comparison */
/* Display identity of a status-panel unit (0xe bytes; g_world_selected_unit_identity
 * and g_world_comparison_unit_identity). Formation code copies it from
 * world_formation_unit_t +0x22; wldcore_unit_build_status_panel_data fills it
 * from a party member. */
typedef struct world_unit_status_identity {
    s16 unit_index;  /* 0x00: name text 0x4000 + value (world_menu_unit_status_banner_thread) */
    s16 job_id;      /* 0x02: job name text 0x3000 + value */
    s16 brave;       /* 0x04 */
    s16 faith;       /* 0x06 */
    s16 zodiac;      /* 0x08 */
    s16 roster_slot; /* 0x0a: party slot (world_menu_get_party_unit_name_id); shown as value + 1 */
    u8 _unused_0c[2];
} world_unit_status_identity_t;
typedef char world_unit_status_identity_size_must_be_0xe[sizeof(world_unit_status_identity_t) == 0xe ? 1 : -1];

extern u8 g_world_comparison_unit_stat_summary[];
extern world_unit_status_identity_t g_world_comparison_unit_identity;
extern menu_number_entry_t g_world_comparison_unit_number_entries[];
extern RECT g_world_comparison_unit_portrait_rect;
extern world_status_thread_t g_world_comparison_display_thread_params;
extern u8 g_world_comparison_unit_summary_packets[];
extern u8 g_world_comparison_unit_status_packets[];
extern u8 g_world_comparison_unit_stat_text_image[];
extern u8 g_world_comparison_unit_name_text_image[];
extern u8 g_world_comparison_unit_number_text_image[];
extern u8 g_world_comparison_unit_portrait_image[];

/* formation */
/* The extra formation record after the roster records. The fitting room and
 * the equip/remove menus copy the selected unit into it
 * (world_formation_copy_unit_to_temp) to preview equipment, so changes to it
 * skip the inventory and the max-equip-at-job-change option. */
enum {
    WORLD_FORMATION_PREVIEW_RECORD = 20,
};

/* Provisional 12-byte formation portrait source rectangle.
 *
 * The builder replaces clut after copying a table entry. tpage selects the
 * VRAM x base: 0x100 for 0x64, else 0x140. */
typedef struct world_formation_graphic_entry {
    s16 x;     /* 0x00 */
    u16 y;     /* 0x02 */
    s16 w;     /* 0x04 */
    s16 h;     /* 0x06 */
    u16 clut;  /* 0x08 */
    s16 tpage; /* 0x0a: GetTPage(0, 3, 0x100 or 0x140, 0), 0x64 or 0x65 */
} world_formation_graphic_entry_t;
typedef char world_formation_graphic_entry_size_must_be_0xc[(sizeof(world_formation_graphic_entry_t) == 0xc) ? 1 : -1];

typedef enum world_monster_breed_rarity {
    WORLD_MONSTER_BREED_RARITY_NONE = 0,
    WORLD_MONSTER_BREED_RARITY_COMMON = 1,
    WORLD_MONSTER_BREED_RARITY_UNCOMMON = 2,
    WORLD_MONSTER_BREED_RARITY_RARE = 3,
} world_monster_breed_rarity_e;

enum { WORLD_MONSTER_BREED_RARITY_COUNT = 3 };

/* One inclusive date limit and its zodiac ID. The 13-entry WORLD table has
 * Capricorn at both calendar ends and does not contain Serpentarius. */
typedef struct world_zodiac_date_limit {
    u8 month;
    u8 day;
    u8 zodiac;
} world_zodiac_date_limit_t;
typedef char world_zodiac_date_limit_size_must_be_3[(sizeof(world_zodiac_date_limit_t) == 3) ? 1 : -1];

enum {
    WORLD_ZODIAC_DATE_LIMIT_COUNT = 13,
    WORLD_MONTH_LENGTH_TABLE_COUNT = 13,
    WORLD_MONSTER_BREED_ENTRY_COUNT = JOB_ID_TIAMAT - JOB_ID_CHOCOBO + 1,
};

/* world_formation_unit_t::proposition_status, copied from the party record:
 * the label the formation grid draws below a unit that is away. */
typedef enum world_formation_unit_proposition {
    WORLD_FORMATION_UNIT_PROPOSITION_LEAVING = 1,
    WORLD_FORMATION_UNIT_PROPOSITION_MISSING = 2,
} world_formation_unit_proposition_e;

/*
 * WORLD menu unit record, compacted after roster and menu-specific filtering.
 * Records at 0x801c8638 have stride 0x128; roster_slot preserves their roster
 * identity. The builder at 0x801210e8 and sorter at 0x80121c60 produce the
 * ordered g_world_formation_unit_pointers list, not a roster-indexed array.
 * The byte fields from 0x70 on mirror the party_data_t layout above. Magical
 * shield/accessory evade are provisionally placed at 0x50/0x52, after the
 * physical fields at 0x40/0x42, to follow the halfword sequence. Other bytes
 * stay as padding.
 */
typedef struct world_formation_unit {
    s16 level;         /* 0x000 */
    s16 team_kind;     /* 0x002; 2 for guest slots */
    u8 _unused_004[4]; /* 0x004; status record list_index and unit_count, left zero */
    s16 experience;    /* 0x008 */
    s16 party_index;   /* 0x00a; portrait */
    s16 hp;            /* 0x00c */
    u8 _unused_00e[2]; /* 0x00e; HP gauge bonus slot */
    s16 max_hp;        /* 0x010 */
    s16 mp;            /* 0x012 */
    u8 _unused_014[2];
    s16 max_mp; /* 0x016 */
    s16 ct;     /* 0x018 */
    u8 _unused_01a[2];
    s16 max_ct;                /* 0x01c; always 100 (world_formation_build_unit_record) */
    u8 _unused_01e[4];         /* 0x01e; tail of the 0x22-byte status record */
    u8 name_index[2];          /* 0x022; s16 formation index; starts the 14-byte display snapshot at WORLD 0x80114bc8 */
    s16 job_id;                /* 0x024; "Current Job" */
    s16 brave;                 /* 0x026 */
    s16 faith;                 /* 0x028 */
    s16 zodiac;                /* 0x02a; battle_stats_t birthday >> 12 (world_formation_build_unit_record) */
    s16 roster_slot;           /* 0x02c; "Roster Slot Index" */
    u8 _unused_02e[2];         /* 0x02e; end of the 0xe-byte identity record */
    s16 move;                  /* 0x030 */
    s16 speed;                 /* 0x032 */
    s16 jump;                  /* 0x034 */
    s16 right_weapon_power;    /* 0x036 */
    s16 left_weapon_power;     /* 0x038 */
    s16 right_weapon_evade;    /* 0x03a */
    s16 left_weapon_evade;     /* 0x03c */
    s16 uses_monster_skillset; /* 0x03e; primary skillset is in the monster range 0xb0..0xdf */
    s16 two_handed;            /* 0x040; world_unit_is_two_handing_weapon result (WORLD 0x80115198) */
    s16 physical_attack;       /* 0x042 */
    s16 physical_class_evade;  /* 0x044 */
    s16 physical_shield_evade; /* 0x046 */
    s16 physical_accessory_evade; /* 0x048 */
    u8 _unused_04a[2];
    s16 magical_attack;          /* 0x04c */
    s16 magical_class_evade;     /* 0x04e */
    s16 magical_shield_evade;    /* 0x050 */
    s16 magical_accessory_evade; /* 0x052 */
    u16 equipment[5];            /* 0x054; right hand, left hand, head, body, accessory */
    u16 primary_skillset;        /* 0x05e */
    u16 secondary_skillset;      /* 0x060 */
    s16 reaction_ability;        /* 0x062 */
    u16 support_ability;         /* 0x064 */
    s16 movement_ability;        /* 0x066 */
    u8 _unused_068[8];           /* 0x068; spare tail of the stat-detail block */
    u8 gender_flags;             /* 0x070; unit_flags_e (fft/unit.h) */
    u8 _unused_071;
    u8 sprite_set;               /* 0x072; "Sprite Set ID": party_data_t.sprite_set / character identity */
    u8 equippable_item_types[4]; /* 0x073; bit per item type, read by can_unit_equip_item_id */
    u8 unlocked_jobs[3];         /* 0x077 */
    u8 learned_abilities[0x39];  /* 0x07a through 0x0b2 */
    u8 job_levels[0x0a];         /* 0x0b3 */
    u8 _padding_0bd;             /* aligns job_points */
    u16 job_points[0x14];        /* 0x0be; per-job JP */
    u16 total_job_points[0x14];  /* 0x0e6; per-job total JP */
    u8 name[0x10];               /* 0x10e; "unit_t Nickname" */
    u8 monster_graphic;          /* 0x11e; added to formation graphic entry 0x3d when sprite_set == 0x82
                                    (world_formation_build_unit_graphic_entry) */
    u8 proposition_status;       /* 0x11f; "Proposition Byte": world_formation_unit_proposition_e, 0 when present */
    u16 birthday;                /* 0x120; battle_stats_t birthday & 0x1ff; egg readers split
                                    it into nibbles */
    u16 egg_color; /* 0x122; party_data_t 0xd2; the formation sprite drawer (0x8011751c) loads it as a halfword palette
                      index */
    u8 support_abilities[4]; /* 0x124; copy of battle_stats_t.support_abilities, BATTLE_SUPPORT_SET_1..4_* */
} world_formation_unit_t;
typedef char world_formation_unit_t_size_must_be_0x128[(sizeof(world_formation_unit_t) == 0x128) ? 1 : -1];

/* Slot-indexed view of the five ability halfwords at 0x5e (primary skillset,
 * secondary skillset, reaction, support, movement).  The WORLD ability-slot
 * menu walks them by slot number; the bytes are the named fields above. */
typedef struct world_formation_unit_ability_slots {
    u8 _unused_00[0x5e];
    u16 ability_slots[5]; /* 0x05e */
} world_formation_unit_ability_slots_t;
typedef char world_formation_unit_ability_slots_offset_must_be_0x5e
    [((unsigned long)&((world_formation_unit_ability_slots_t*)0)->ability_slots == 0x5e) ? 1 : -1];

/* The formation screen's state: the index of its handler in
 * g_world_formation_menu_handlers. */
typedef enum world_formation_menu {
    WORLD_FORMATION_MENU_UNIT_GRID = 0, /* unit grid and its triangle menu */
    WORLD_FORMATION_MENU_ITEM = 1,      /* item icon list */
    WORLD_FORMATION_MENU_ABILITY = 2,
    WORLD_FORMATION_MENU_JOB_WHEEL = 3,
    WORLD_FORMATION_MENU_DISMISS_UNIT = 4,
    WORLD_FORMATION_MENU_ORDER_UNIT = 5,
    WORLD_FORMATION_MENU_ITEM_EQUIP = 6, /* the item list's rows follow in order */
    WORLD_FORMATION_MENU_ITEM_BEST_FIT = 7,
    WORLD_FORMATION_MENU_ITEM_REMOVE = 8,
    WORLD_FORMATION_MENU_ITEM_LIST = 9,
    WORLD_FORMATION_MENU_ABILITY_SET = 10, /* the ability menu's rows follow in order */
    WORLD_FORMATION_MENU_ABILITY_REMOVE = 11,
    WORLD_FORMATION_MENU_ABILITY_LEARN = 12,
    WORLD_FORMATION_MENU_VIEW_ABILITIES = 13,
    WORLD_FORMATION_MENU_JOB_WHEEL_ENTER = 14,              /* the grid slides out, then the wheel opens */
    WORLD_FORMATION_MENU_JOB_WHEEL_LEAVE = 15,              /* the wheel closes, then the grid returns */
    WORLD_FORMATION_MENU_ITEM_ENTER = 16,                   /* the grid slides out */
    WORLD_FORMATION_MENU_ITEM_LEAVE = 17,                   /* the grid returns */
    WORLD_FORMATION_MENU_ABILITY_ENTER = 18,                /* the grid slides out */
    WORLD_FORMATION_MENU_ABILITY_LEAVE = 19,                /* the grid returns */
    WORLD_FORMATION_MENU_JOB_WHEEL_CLOSING = 20,            /* wheel close animation */
    WORLD_FORMATION_MENU_JOB_WHEEL_OPENING = 21,            /* wheel open animation */
    WORLD_FORMATION_MENU_ABILITY_ENTER_PORTRAIT_SLIDE = 22, /* only the portrait slides; nothing stores it */
    WORLD_FORMATION_MENU_ITEM_ENTER_NO_SLIDE = 23,          /* nothing stores it */
    WORLD_FORMATION_MENU_JOB_CHANGE = 24,                   /* job change flash on the wheel */
} world_formation_menu_e;

/* A formation menu handler (g_world_formation_menu_handlers). */
typedef void (*world_formation_menu_handler_t)(void);

/* Screen x and texture u of one formation background tile; two alternating rows of four
 * (g_world_formation_background_tiles, drawn by world_formation_draw_background_tiles). */
typedef struct world_formation_background_tile {
    u16 x;
    u16 u;
} world_formation_background_tile_t;

/*
 * The sprite-set -> script-variable table walked by the
 * dismissal tail, terminated by sprite_set == -1. The four-byte stride is
 * proved by the target advancing both its record pointer and its byte-offset
 * index by 4 per iteration.
 */
typedef struct world_formation_dismiss_entry {
    s16 sprite_set;  /* 0x00 */
    s16 variable_id; /* 0x02 */
} world_formation_dismiss_entry_t;

/* The mode argument of world_formation_build_record_list: which roster units
 * it lists. */
typedef enum world_formation_record_list_mode {
    WORLD_FORMATION_RECORD_LIST_ALL = 0,
    WORLD_FORMATION_RECORD_LIST_AVAILABLE_HUMANS = 1, /* no unit on a proposition, egg or monster */
    WORLD_FORMATION_RECORD_LIST_MONSTERS = 2,         /* no egg, male or female unit */
} world_formation_record_list_mode_e;

/* Result of world_formation_get_unit_lock: whether the unit's
 * equipment and abilities can be changed, and why not. */
typedef enum world_formation_unit_lock {
    WORLD_FORMATION_UNIT_LOCK_MIME = -6, /* a Mime can still learn abilities */
    WORLD_FORMATION_UNIT_LOCK_EGG = -4,
    WORLD_FORMATION_UNIT_LOCK_MONSTER = -3,
    WORLD_FORMATION_UNIT_LOCK_PROPOSITION = -2, /* away on a proposition */
    WORLD_FORMATION_UNIT_UNLOCKED = 1,
} world_formation_unit_lock_e;

extern world_oriented_quad_t g_world_formation_cursor_shadow_sprite;
extern world_oriented_quad_t g_world_formation_cursor_sprite;
extern s32 g_world_formation_current_menu;                               /* world_formation_menu_e */
extern world_formation_menu_handler_t g_world_formation_menu_handlers[]; /* one per world_formation_menu_e */
extern world_formation_background_tile_t g_world_formation_background_tiles[2][4];
extern world_formation_dismiss_entry_t g_world_formation_dismiss_entries[];
extern point16_t g_world_formation_cursor_position;
extern u8 g_world_formation_generic_human_graphic_index_table[];
extern world_formation_graphic_entry_t g_world_formation_graphic_entries[];

/* Set while the selected unit is in the upper rows of the grid: the unit banner
 * then sits at the bottom of the screen and the stat bar at the top (BUNIT's
 * g_bunit_status_banner_at_bottom). The latch is the value the banner was last
 * placed for. */
extern u8 g_world_formation_unit_banner_at_bottom;
extern u8 g_world_formation_unit_banner_at_bottom_latch;
extern s32 g_world_menu_resolved_command;
extern s16 g_world_formation_selected_unit_index;
extern s16 g_world_formation_selected_unit_index_latch;
extern struct world_formation_unit* g_world_formation_temp_unit;
extern u8 g_world_formation_unit_banner_enabled;
extern u8 g_world_formation_unit_browse_enabled;

/* Provisional: month lengths indexed by 1-based month (0x8018deac; follows
 * the zodiac date limits at 0x8018de84). Read by world_unit_update_monster_breeding. */
extern const u8 g_world_month_lengths[WORLD_MONTH_LENGTH_TABLE_COUNT];
extern const world_zodiac_date_limit_t g_world_zodiac_date_limits[WORLD_ZODIAC_DATE_LIMIT_COUNT];
extern s16 g_world_formation_open_request_state; /* world_screen_request_state_e */
extern s32 g_world_formation_name_string_revision;

/* Portrait code per sprite set: low byte is the portrait index. */
extern u16 g_world_formation_sprite_by_portrait[];

/* Remaps the monster portrait indices, 0x3f and above; indexed from 0x3f. */
extern u8 g_world_monster_formation_sprite_table[20];
extern u8 g_world_formation_menu_text_table[];
extern u32 g_world_formation_menu_palettes[];
extern u8 g_world_formation_saved_sprite_color[4];
extern u8 g_world_formation_dimmed_sprite_color[];
extern u8 g_world_formation_normal_sprite_color[];
extern u8 g_world_formation_unit_cycle_mode;
extern u8 g_world_formation_cursor_ot_override;
extern s8 g_world_formation_main_menu_initialized;
extern u8 g_world_formation_saved_banner_enabled;
extern u8 g_world_formation_screen_running;
extern u32 g_world_formation_status_label_image[];
extern RECT g_world_formation_status_label_image_rect;
extern world_menu_entry_t g_world_formation_egg_triangle_menu;

/* Chained after the egg triangle menu; world_formation_run_unit_grid_menu only resets it. */
extern world_menu_entry_t g_world_formation_egg_submenu;
extern u8 g_world_formation_unit_slide_pending;
extern s8 g_world_formation_scroll_position;
extern s8 g_world_formation_scroll_velocity;
extern s8 g_world_formation_selection_cursor_ready;
extern u8 g_world_formation_triangle_menu_open;
extern u8 g_world_formation_triangle_menu_running;
extern volatile u8 g_world_formation_unit_slide_frame;
extern s8 g_world_formation_banner_slide_step;
extern u8 g_world_formation_stat_display_mode; /* menu_unit_readout_e, HP to BRAVE_FAITH */
extern world_draw_number_command_t g_world_formation_gauge_value_number;
extern world_draw_number_command_t g_world_formation_level_number;
extern world_draw_number_command_t g_world_formation_experience_number;
extern world_draw_number_command_t g_world_formation_brave_number;
extern world_draw_number_command_t g_world_formation_faith_number;
extern world_draw_number_command_t g_world_formation_brave_number_wide;
extern world_draw_number_command_t g_world_formation_faith_number_wide;
extern world_draw_number_command_t g_world_formation_gauge_max_number;
extern world_menu_window_command_t g_world_formation_stat_bar_window_top;
extern world_menu_window_command_t g_world_formation_stat_bar_window_bottom;

/* Three consecutive sprite records at 0x8018c704 / 0x8018c718 / 0x8018c72c. */
extern world_oriented_quad_t g_world_formation_shadow_egg_sprites[3];
extern s8 g_world_formation_gauge_bar_offsets[4];
extern u8 g_world_formation_hp_gauge_colors[12];
extern u8 g_world_formation_mp_gauge_colors[12];
extern u8 g_world_formation_ct_gauge_colors[12];
extern world_oriented_quad_t g_world_formation_label_cant_equip;
extern world_oriented_quad_t g_world_formation_label_equipping;
extern world_oriented_quad_t g_world_formation_label_leaving;
extern world_oriented_quad_t g_world_formation_unit_dot_sprite;
extern world_oriented_quad_t g_world_formation_label_missing;

extern battle_menu_sprite_cell_t g_world_formation_label_tiles[];
extern battle_menu_sprite_cell_t g_world_formation_label_tiles_bottom[];
extern s8 g_world_formation_dot_glow_timer;
extern s8 g_world_formation_dot_glow_direction;
extern u8 g_world_formation_cursor_trail_brightness[8];
extern u8 g_world_formation_grid_transition_frame;
extern u8 g_world_formation_grid_return_frame;
extern u8 g_world_formation_grid_return_done;
extern u8 g_world_formation_portrait_slide_frame;
extern u8 g_world_formation_away_unit_color[];
extern world_menu_entry_t g_world_formation_triangle_menu;
extern u8 g_world_formation_dismiss_prompt[];
extern s16 g_world_formation_dismiss_prompt_text_id;
extern s16 g_world_formation_dismiss_prompt_kind;
extern s16 g_world_formation_dismiss_anim_timer;
extern s8 g_world_formation_sort_field_indices[];
extern point16_t g_world_formation_cursor_target;
extern s8 D_801C833C; /* zeroed with g_world_formation_scroll_velocity when the formation cursor starts; never read */
/* Cursor trail position history at 0x801c8344, reset by
 * world_formation_reset_cursor_trail; entry 7 is the newest and x is
 * -1 in an unused entry. */
extern point16_t g_world_formation_cursor_trail[8];
extern u8 g_world_formation_dismiss_step;
extern u8* g_world_formation_dismiss_menu_data;
extern s16 g_world_formation_record_order[];
extern struct world_formation_unit g_world_formation_unit_records[];
extern u8 g_world_formation_menu_text_buffer[];

/* Signed count of accepted records; pointer entry 20 is a separate extra
 * record at 0x801c9d58, installed by 0x80121fac–0x80121fb8. */
extern s16 g_world_formation_record_count; /* 0x801c8634 */

/* Menu callers cache the builder's return here; rebuilding the record list
 * does not itself refresh this byte. */
extern u8 g_world_formation_unit_count;
extern world_formation_unit_t* g_world_formation_unit_pointers[];
void world_formation_run_screen(s32 tutorial, s32 unused);
s32 world_formation_is_unit_list_scroll_needed(void);
void world_formation_draw_unit_slots(point16_t* positions, s32 shade);

void world_formation_draw_unit_readout(
    s16 unit_index, s32 mode, s32 x, s32 y, u8* color, s32 (*get_value)(s32), s32 enabled, s32 priority);

s32 world_formation_run_equip_item_menu(void);
s32 world_formation_run_item_list_menu(void);
s32 world_formation_run_set_ability_menu(void);
void world_formation_build_job_wheel_availability(void);
void world_formation_build_job_wheel_job_name(void);
void world_gfx_set_scaled_poly_ft4_geometry_and_uv(POLY_FT4* poly, world_image_location_t* page, upoint16_t* position,
    main_zodiac_sprite_frame_t* rect, point16_t* scale, main_zodiac_draw_offset_t* origin);
s16 world_formation_build_record_list(s32 item_id, world_formation_unit_t** list, s32 mode);

void world_formation_build_unit_graphic_entry(
    s16 formation_index, world_formation_graphic_entry_t* destination, s32 unused);

void world_formation_build_unit_record(
    struct battle_stats* unit, struct world_formation_unit* record, struct party_data* party);

void world_gfx_build_zodiac_primitives(zodiac_draw_context_t* view, POLY_FT4* poly);
s32 world_formation_can_dismiss_unit(s16 formation_idx);
s32 world_formation_can_equip_item_in_slot(s16 unit_index, s16 slot, s16 item_id);
s32 world_formation_is_selected_unit_in_upper_rows(s32 scroll_position);
s32 world_formation_change_unit_job(s16 unit_id, s16 job_id, s32 update_equipment);
void world_formation_clear_stat_preview(world_item_stat_summary_t* summary, world_item_stat_detail_t* detail);
void world_formation_copy_unit_to_temp(s16 index, s32 unused_slot);
s32 world_formation_cursor_distance_falloff(s32 x, s32 y, s32 base, s32 floor);
void world_formation_cycle_selected_unit(s16 pad_buttons);
void world_formation_dismiss_unit(s16 formation_idx);
void world_formation_draw_background_tiles(s32 y, s32 unused_1);
void world_formation_draw_cursor_trail(s16* cursor);
void world_formation_draw_job_wheel_sprites(s32 angle, s32 radius_x, s32 radius_y);
void world_formation_draw_job_wheel_mastered_star(s32 x, s32 y, s32 otag_index, s32 value);
void world_formation_draw_sprite_with_shadow(world_oriented_quad_t* pos, const u8* rgb, s32 ot_index);

void world_formation_draw_egg_sprite(
    world_oriented_quad_t* pos, s32 palette, const u8* rgb, s32 hatch_countdown, s32 ot_index);

void world_formation_draw_unit_sprite(s16 unit_index, point16_t* pos, u8* rgb, s32 ot_index);
s32 world_formation_equip_item_to_unit_slot(s16 unit, s16 slot, s16 item_id);
s32 world_formation_find_index_by_roster_slot(s16 roster_slot);
s32 world_formation_generate_unit(s16 unit_type);

void world_formation_get_generic_human_graphic_entry(
    s32 job_id, s16 is_female, world_formation_graphic_entry_t* destination);

s32 world_formation_get_job_wheel_cursor_job_level(s32 unused_row);
s32 world_formation_get_unit_brave(s32 unit_id);
s32 world_formation_get_unit_experience(s32 unit_id);
s32 world_formation_get_unit_faith(s32 unit_id);
s32 world_formation_get_unit_egg_hatch_total(s32 unit_id);
s32 world_formation_get_unit_egg_hatch_progress(s32 unit_id);
s32 world_formation_get_unit_level(s32 unit_id);
void world_formation_init_cursor_and_scroll_state(s32 formation_index);
void world_formation_init_menu_display(void);
void world_formation_init_menu_state(void);
void world_formation_init_selection_cursor(s16 formation_index);
s32 world_formation_is_job_wheel_selection_available(s32 unused_row);
void world_formation_run_item_menu(void);
void world_formation_layout_job_wheel_portrait(void);
s32 world_formation_get_unit_lock(s32 unit_id);
void world_formation_place_unit_portrait_at_rest(s16 unit_index);
s32 world_formation_step_job_change_animation(s32 job);
void world_formation_rebuild_unit_list(void);
void world_formation_recalculate_selected_unit_stats(void);
void world_formation_remove_invalid_unit_loadout(s16 unit_id);
void world_formation_reset_cursor_trail(void);
void world_formation_run_before_event(void);
void world_formation_run_dismiss_unit_menu(void);
void world_formation_run_job_wheel_menu(void);
void world_formation_run_tutorial_screen(s32 menu_index);
void world_formation_save_records_to_party_data(void);
s32 world_formation_set_unit_ability_slot(s16 unit_id, s16 slot, s16 ability, s32 update_equipment);
void world_formation_set_unit_name(s16 formation_index, const u8* name);
void world_formation_stage_selected_unit(void);
void world_formation_start_job_wheel_close(void);
s32 world_formation_step_grid_return_transition(s32 mode);
s32 world_formation_step_grid_transition(s32 mode);
void world_formation_step_job_wheel_close_animation(void);
void world_formation_step_job_wheel_open_animation(void);
void world_formation_step_scroll_velocity(s8* velocity, s8* position);
u8 world_formation_step_unit_portrait_slide(void);
void world_formation_step_unit_selection_slide(void);
void world_formation_stop_menu_threads(void);
/* get_mode and get_shade take the unit slot index; NULL selects the default. */
s32 world_formation_update_and_draw_unit_grid(s16 background_y, s16 toggle_banner, s16 allow_browse, s16 input,
    s16 locked, s32 (*get_mode)(s32), s32 (*get_shade)(s32));
void world_formation_switch_job_wheel_unit(void);
void world_formation_update_open_request(void);
void world_formation_update_unit_selection(void);
void world_formation_sort_roster(s32 index);
s32 world_formation_get_unit_stat_by_mode(s32 unit_index);
s32 world_formation_run_ability_job_list(void);
void world_formation_run_ability_menu(void);
s32 world_formation_run_best_fit_menu(void);
s32 world_formation_run_learn_menu(void);
void world_formation_run_order_unit_menu(void);
s32 world_formation_run_remove_ability_menu(void);
s32 world_formation_run_remove_all_abilities_prompt(void);
s32 world_formation_run_remove_all_equipment_prompt(void);
s32 world_formation_run_remove_item_menu(void);

/* unit */
/* WORLD's unit helpers: the event-script unit operations it shares with BATTLE (status snapshots,
 * facing and animations, screen positions), the unit view panels and editor, Best Fit and the
 * equipment checks, monster breeding and the zodiac sign of a date. */

/* Provisional: one 4-byte breeding entry per monster job from 0x5e
 * (0x8018debc): the hatch-day range and the three offspring jobs by rarity
 * class (world_unit_update_monster_breeding). */
typedef struct world_unit_monster_breed_entry {
    u8 hatch_range; /* 0x00: rand() % hatch_range + 1 (at least 2) egg days */
    u8 offspring_job_by_rarity[WORLD_MONSTER_BREED_RARITY_COUNT]; /* 0x01; common through rare */
} world_unit_monster_breed_entry_t;
typedef char world_unit_monster_breed_entry_size_must_be_4[(sizeof(world_unit_monster_breed_entry_t) == 4) ? 1 : -1];

/* Unpadded unsigned 16-bit x/y/z triple (6 bytes). */
typedef struct vec3u16 {
    u16 x;
    u16 y;
    u16 z;
} vec3u16_t;
typedef char assert_vec3u16_size[sizeof(vec3u16_t) == 6 ? 1 : -1];

/* WORLD text files materialize 32 section pointers after their 0x80-byte
 * offset/header area. The same table is saved and restored around overlays. */
enum {
    WORLD_TEXT_SECTION_TABLE_BYTES = 0x80,
    WORLD_TEXT_FILE_HEADER_BYTES = 0x80,
};

/* The text sections WORLD's screens bind while they are up (text_section_e
 * has the resident ones). */
typedef enum world_text_section {
    WORLD_TEXT_SECTION_TUTORIAL_MESSAGES = 23, /* the running tutorial's message file */
    WORLD_TEXT_SECTION_NAME_ENTRY_KEYS = 28,   /* name entry: the key rows */
    WORLD_TEXT_SECTION_FORMATION_TEXT = 29,    /* formation: what a dismissed unit says, or the job wheel's
                                                * descriptions, whichever was bound last */
    WORLD_TEXT_SECTION_SHOP_MENU_LABELS = 30,
    WORLD_TEXT_SECTION_SHOP_DIALOGUE = 31, /* the shopkeeper's lines */
} world_text_section_e;

extern world_item_stat_summary_t g_world_selected_unit_stat_summary;
extern battle_unit_status_record_t g_world_unit_selected_status_billboard;
extern battle_unit_status_record_t g_world_unit_comparison_status_billboard;
extern battle_menu_status_panel_buffer_t g_world_stat_preview_panel_frames[2];
/* Unit editor state; same layout as the EVENT debug character editor's
 * (debugchr_editor_load_selected_unit). */
extern battle_menu_status_panel_slot_storage_t g_world_unit_editor_fields;

/* The comparison unit's flag and label_text_ids[2], copied by
 * copies the comparison unit's generic_monster flag and ability_ids[2] into it. */
extern battle_menu_status_panel_slot_storage_t g_world_unit_comparison_editor_fields;
extern u32 g_world_companion_overlay_state;
extern struct world_item_stat_detail g_world_selected_unit_stat_detail;
extern world_unit_animation_state_t g_world_unit_animation_states[];
extern s16 g_world_unit_comparison_battle_id;
extern u16 g_world_unit_graphics_load_pending;
extern const world_unit_monster_breed_entry_t g_world_unit_monster_breed_table[WORLD_MONSTER_BREED_ENTRY_COUNT];
extern unit_status_staging_t* g_world_unit_status_staging_data;

extern battle_active_unit_t g_world_active_turn_unit;

extern u8 g_world_unit_editor_primary_skillset_name[];   /* primary skillset name text window */
extern u8 g_world_unit_editor_secondary_skillset_name[]; /* secondary skillset name text window */
extern u8 g_world_unit_editor_unit_name[];               /* name text window */
extern u8 g_world_unit_editor_job_name[];                /* job name text window */
extern u16 g_world_unit_view_started_battle_id;
extern u16 g_world_unit_view_started_comparison_id;
extern s32 g_world_unit_view_mode;
extern s32 g_world_unit_join_screen_active;
extern u16 g_world_unit_status_bounce_offsets[];
extern u16 g_world_unit_status_bounce_step;
extern u16 g_world_unit_status_quad_clut_pairs[][2];
extern u16 g_world_unit_status_sprite_clut_pairs[][2];
extern world_gfx_image_load_parameters_t g_world_unit_status_icon_image_params[];
extern CVECTOR g_world_unit_status_gauge_colors[];
extern world_gfx_image_load_parameters_t* g_world_unit_status_quad_image_params_by_mode[];
extern world_gfx_image_load_parameters_t* g_world_unit_status_sprite_image_params_by_mode[];
extern u8 g_world_unit_status_first_page_by_mode[];
extern RECT g_world_unit_summary_panel_rect;
extern world_gfx_image_load_parameters_t g_world_unit_summary_sprite_image_params[];
extern RECT g_world_unit_editor_panel_rect;  /* panel box */
extern RECT g_world_unit_editor_cursor_rect; /* cursor source */
extern RECT g_world_unit_editor_text_rect_a; /* first text image */
extern RECT g_world_unit_editor_text_rect_b; /* second text image */
extern world_gfx_image_load_parameters_t g_world_unit_editor_sprite_image_params_a[19];
extern world_gfx_image_load_parameters_t g_world_unit_editor_sprite_image_params_b[19];
extern world_gfx_image_load_parameters_t g_world_unit_editor_row_image_params_a[4];
extern world_gfx_image_load_parameters_t g_world_unit_editor_row_image_params_b[4];
extern world_gfx_image_load_parameters_t g_world_unit_editor_item_icon_image_params[5];
extern u8 g_world_unit_editor_line_colors[8 * WORLD_UNIT_EDITOR_LINE_SIZE];
extern u16 g_world_unit_editor_sprite_clut_pairs[19][2];
extern world_unit_view_thread_set_t g_world_unit_view_thread_sets[];
extern u8 g_world_unit_view_panel_fade_modes[];
extern s16 g_world_unit_face_slot_order[];
extern world_unit_status_identity_t g_world_selected_unit_identity;
extern CVECTOR g_world_unit_panel_bar_colors[];
extern menu_number_entry_t g_world_selected_unit_number_entries[];
extern world_gfx_image_load_parameters_t g_world_unit_summary_label_sprite_params[];
extern world_gfx_image_load_parameters_t g_world_unit_summary_value_sprite_params[];
extern RECT g_world_unit_status_panel_origin;
extern world_gfx_image_load_parameters_t g_world_unit_status_sprite_params[];
extern u8 g_world_unit_status_portrait_params[];
extern u16 g_world_unit_summary_value_cluts[4][2]; /* [row][dimmed] */
extern u16 g_world_unit_summary_label_cluts[7][2]; /* [row][dimmed] */
extern u8 g_world_unit_summary_mode_row_params[];
extern RECT g_world_selected_unit_portrait_rect;
extern world_status_thread_t g_world_stat_preview_panel_thread_params;
extern world_menu_text_binding_t g_world_selected_unit_text_binding;
extern u8 g_world_unit_status_banner_active;
extern world_menu_entry_t g_world_unit_status_banner_menu;
extern s8 g_world_unit_status_banner_state; /* banner state: 0 idle, 1 finishing, else countdown */

/* Panel scratch areas inside the stack bytes of g_world_thread_contexts[0]
 * (+0x100, +0x200) and [14] (+0x100, +0x200). native_thread_t leaves those
 * bytes unnamed, so a member spelling would be raw offset arithmetic; they
 * stay separate symbols. The palette areas hold one slot per page. */
extern u8 g_world_unit_summary_digit_images[];
extern world_menu_palette_primitives_t g_world_unit_editor_palette;
extern world_menu_palette_slot_t g_world_unit_summary_palettes[];
extern s32 g_world_companion_overlay_prims;
/* WalkToAnim: the animation each unit takes once it has moved and stopped, and
 * whether it has been seen moving since. Twins of g_battle_unit_pending_animation_ids
 * and g_battle_unit_was_moving_latch. */
extern u16 g_world_unit_pending_animations[];
extern u16 g_world_unit_was_moving_latch[];
extern vec3u16_t g_world_misc_unit_screen_coords;
extern world_unit_status_page_t g_world_unit_status_panel_pages[];
extern world_unit_editor_frame_t g_world_unit_editor_frames[2];
extern u8 g_world_selected_unit_summary_packets[];
extern u8 g_world_selected_unit_status_packets[];
extern u8 g_world_selected_unit_stat_text_image[];
extern u8 g_world_selected_unit_name_text_image[];
extern u8 g_world_selected_unit_number_text_image[];
extern u8 g_world_selected_unit_portrait_image[];
extern u8 g_world_stat_preview_text_image[];
void set_unit_cannot_exist(s32 unit_index);
struct battle_stats* find_unit_by_id(s32 unit_id, s32* out_index);
s32 world_get_party_unit_formation_sprite(s32 party_index);
void resolve_unit_status_changes(void);
s32 unit_has_adjacent_monster_skill_support(struct battle_stats* unit);
s32 world_formation_get_unit_palette_index(s32 roster_slot);
void world_script_join_units_silently(void);
void world_menu_load_selected_unit(void);
s32 world_script_get_misc_id(s32 unit_id);

void world_unit_apply_staged_status_data(s32 unit_id, s32 misc_id);
void world_unit_build_best_fit_equipment(s16 unit_index, s16* equipment);
s32 world_unit_can_equip_item_id(s16 unit_index, s16 item_id);
s32 world_unit_check_facing_relative_to_camera(s32 unit_id);
void world_unit_clear_status_staging_data(void);
void world_unit_copy_misc_screen_coords(s32 misc_id, s16* out);
void world_unit_copy_misc_screen_location(u32 misc_id, VECTOR* destination);
struct battle_stats* world_unit_get_active_turn_unit_stats(void);
s32 world_unit_get_battle_index_by_entd_unit_id(s32 entd_unit_id);
struct battle_stats* world_unit_get_by_unit_id(s32 unit_id);
s32 world_unit_get_map_coords_from_misc_id(s32 misc_id, s16* out);
s32 world_unit_get_zodiac_for_date(s32 month, s32 day);
s32 world_unit_has_any_current_status(s32 unit_id);
void world_unit_increase_blue_team_brave_by_10(void);
void world_unit_inflict_status_by_entd_unit_id(
    s32 entd_unit_id, s32 status_index, s32 is_infliction, s32 less_strict_removal);
s32 world_unit_is_moving(s32 id);
s32 world_unit_is_two_handing_weapon(s16* equipment, s32 two_hands_support);
void world_unit_project_misc_to_screen(s32 misc_id, s16* screen_coords);
void world_unit_queue_current_status_graphics(s32 battle_unit_index, s32 enabled);
void world_unit_reset_animation_states(void);
s32 world_unit_save_staged_status_data(s32 unit_id);
void world_unit_set_animation(s32 misc_id, u32 animation);
void world_unit_set_facing(s32 misc_id, s32 facing);
void world_unit_start_view_thread(s32 mode, s32 unit_a, s32 unit_b);
s32 world_unit_try_get_misc_data_by_unit_id(u16* unit_id, s32* misc_index);
void world_unit_update_monster_breeding(s32 unused);
void world_unit_update_staged_status_data(u16 unit_id, u16 restore);
void world_unit_view_supervisor_thread(void);

/* tutorial */
/*
 * WORLD.BIN tutorial command stream. Commands below 0x10 are one-byte shifts;
 * the rest are ID (1) followed by the listed parameters: 0x10 message id (2) +
 * y (1), 0x11 message id (2), 0x13 x, y, size, width mod, height mod (1 each),
 * 0x15 time (1).
 */
enum {
    WORLD_TUTORIAL_CMD_SHIFT_LIMIT = 0x10,
    WORLD_TUTORIAL_CMD_DISPLAY_MESSAGE = 0x10,
    WORLD_TUTORIAL_CMD_CHANGE_DIALOG = 0x11,
    WORLD_TUTORIAL_CMD_WAIT_FOR_MESSAGE = 0x12,
    WORLD_TUTORIAL_CMD_DRAW_CIRCLE = 0x13,
    WORLD_TUTORIAL_CMD_WAIT = 0x14,
    WORLD_TUTORIAL_CMD_WAIT_TIME = 0x15,
    WORLD_TUTORIAL_CMD_WAIT_FOR_MESSAGE_2 = 0x16,
    WORLD_TUTORIAL_CMD_END = 0x17,
};

extern s16 g_world_tutorial_categories_request_state; /* world_screen_request_state_e */
extern world_gfx_tim_pair_record_t g_world_tutorial_saved_game_image;

extern s16 g_world_tutorial_highlight_corners_a[][4]; /* first tutorial highlight quad corners */
extern s16 g_world_tutorial_highlight_corners_b[][4]; /* second tutorial highlight quad corners */
extern u8 g_world_tutorial_highlight_colors_a[4][3];  /* first quad corner colours */
extern u8 g_world_tutorial_highlight_colors_b[4][3];  /* second quad corner colours */
extern u8 g_world_tutorial_highlight_quad_count;

/* help */
extern u16 g_world_help_portrait_clut_by_sheet_id[];
/* Saved copies of the WORLD records world_menu_copy_billboard_data_out reads. */
extern map_selected_tile_t g_world_help_menu_cursor_tile;
extern battle_unit_status_record_t g_world_help_active_banner;
extern battle_active_unit_t g_world_help_active_unit_data;

/* Copy of the 0x40-byte unit editor fields (world_menu_copy_billboard_data_out);
 * the layout is battle_menu_status_panel_slot_state_t, as in the HELPMENU twin. */
extern battle_menu_status_panel_slot_state_t g_world_help_billboard;
extern help_navigation_record_t g_world_help_terrain_navigation[];
extern help_navigation_record_t g_world_help_attack_preview_navigation[];
extern help_navigation_record_t g_world_help_unit_box_navigation[];
extern help_navigation_record_t g_world_help_require_navigation[];
extern u8 g_world_help_menu_text_id_tables[];

/* Help-file offsets of the text sections the help menus rebind, added to
 * g_world_help_data_base (HELPMENU twin at 0x801e0b30). */
extern s32 g_world_help_text_section_offsets[32];
extern u8 g_world_help_data_base[];
extern u32* g_world_help_controller_state_ptr;
extern u16* g_world_help_topic_text_ids;
extern s32 g_world_help_text_id_tables_ptr;

/* options */
extern world_input_timing_profile_t g_world_option_input_timing_profiles[];
extern world_menu_text_binding_t g_world_option_menu_entry_table;
extern s32 g_world_option_menu_text_widths[];    /* option menu text-image width, per option */
extern s32 g_world_option_menu_entry_counts[];   /* option menu entry count, per option */
extern s32 g_world_option_menu_first_text_ids[]; /* first text id of the option's entries */
extern world_menu_entry_t g_world_option_menu_icon_thread_param;
extern RECT g_world_options_menu_window_rect;
extern RECT g_world_options_menu_title_rect;
extern RECT g_world_options_menu_value_rect;
extern world_menu_icon_record_t g_world_option_picker_icon_records[2];
void world_build_options_menu(void);
void world_run_option_choice_menu(void);

/* panel */
extern s16 g_world_panel_slide_up_y[];
extern world_image_location_t g_world_panel_item_icon_texture[];
extern s16 g_world_panel_slide_down_y[];

/* text */
/* Provisional: destination placement for the packed-font glyph decoder at
 * 0x80129e4c.  x is a 4bpp pixel column (bit 0 selects the odd nibble),
 * y a row, and stride the destination row pitch in halfwords. */
typedef struct world_text_glyph_target {
    s16 x;      /* 0x00 */
    s16 y;      /* 0x02 */
    s16 stride; /* 0x04: halfwords per row */
} world_text_glyph_target_t;

/* Provisional: menu text origin/stride words at 0x80153280 (the WORLD
 * script variable region; world_menu_set_text_origin writes +0x04/+0x06).
 * The text-image builders address these through one base register, so they
 * are modelled as one aggregate; the two parameter words at 0x80153304 and
 * 0x80153308 schedule as plain scalars and stay separate globals. */

extern battle_text_glyph_state_t g_world_text_message_box_glyph;

extern u8 g_world_numeric_display_palette_indices[12];
extern u8 g_world_text_glyph_widths[];
extern s32 g_world_text_line_widths[];
extern s32 g_world_text_overlay_active;
extern u8 g_world_text_player_name_buffer[20];
extern u8* g_world_text_saved_section_pointers[32];
extern s32 g_world_text_section_offsets[32];
extern u8* g_world_text_section_pointer_backup[32];
extern s32 g_world_text_section_pointers_saved;
extern s32 g_world_text_substitution_values[32];
extern s16 g_world_script_opcode_dc_value; /* EVENT_OPCODE_UNKNOWN_DC stores 13; nothing reads it */

/* Text substitutions $7E/$7F (0x801533d0, table index 0x7e from 0x801531d8)
 * as the four halfwords world_text_set_substitution_values_7e_7f stores. */
extern RECT g_world_text_substitution_rect;
extern char g_world_text_wait_for_allocation_message[];
extern char g_world_text_decimal_format[];
extern char g_world_text_wait_for_file_allocation_message[]; /* "WAIT FOR FILEALLOCATION\n" */
extern char g_world_text_reading_character_message[];        /* "reading charactor%d \n" */
extern u8 g_world_text_glyph_bitmaps[];
extern s32 g_world_text_glyph_first_row;
extern s32 g_world_text_glyph_row_limit;
extern u16 g_world_text_speaker_swap_unit_a;
extern u16 g_world_text_speaker_swap_unit_b;
extern s32 g_world_text_glyphs_per_wait;      /* frames a glyph stays up before the typewriter advances */
extern u8 g_world_text_slot_source_columns[]; /* per-slot source column, stride 1 */
extern u8 g_world_text_slot_row_pitches[];    /* per-slot source row pitch in bytes, stride 1 */
extern u32 g_world_text_decimal_divisors[];
extern s32 g_world_text_current_line;
extern RECT g_world_numeric_display_frame_rect;
extern RECT g_world_numeric_display_texture_window;
extern RECT g_world_numeric_display_text_rect_a;
extern RECT g_world_numeric_display_text_rect_b;
extern RECT g_world_numeric_display_text_rect_c;
extern world_gfx_image_load_parameters_t g_world_numeric_display_image_params[18];
extern menu_number_entry_t g_world_numeric_display_entries[];
extern u16 g_world_text_message_duration_frames;

/* Same layout as g_world_decimal_glyph_source. */
extern world_glyph_blit_t g_world_text_large_number_glyph_rect;
extern world_glyph_blit_t g_world_text_shadow_bottom_glyph_source[];
extern world_glyph_blit_t g_world_text_shadow_top_glyph_source[];
extern s16 g_world_fixed_number_panel_request_state;
extern s32 g_world_text_message_box_digit_divisors[];
extern s32 g_world_text_message_box_slot_threads[3];
extern s32 g_world_text_message_box_slot_glyph_counters[3];
extern s32 g_world_text_message_box_glyph_delay;
extern u8 g_world_text_message_box_slot_carry_offset[4];
extern u8 g_world_text_message_box_slot_prev_width[4];

/* Same layout as world_text_render_decimal_entry_list's output record. */
extern world_glyph_blit_t g_world_decimal_glyph_source;
extern world_status_thread_t g_world_numeric_editor_thread_params;
extern u8 g_world_text_task_phase;
extern u8 g_world_text_skillset_names_data[];
extern u8 g_world_text_job_names_data[];
extern u8 g_world_text_ability_names_data[];
extern u8 g_world_text_formation_messages_data[];
extern u8 g_world_text_dismiss_unit_replies_data[];
extern u8 g_world_text_job_descriptions_data[];
extern u8 g_world_text_name_entry_keyboard_data[];
extern u8 g_world_text_shop_dialogue_data[];
extern u8 g_world_text_shop_menu_labels_data[];
extern u8 g_world_text_name_entry_keys_data[];
extern world_menu_palette_slot_t g_world_numeric_display_palettes[];
extern world_glyph_blit_t g_world_text_draw_origin;
extern battle_text_glyph_state_t g_world_text_typewriter_glyph;
extern menu_text_state_t g_world_text_typewriter_state;     /* origin/stride passed to 0x800ff284 */
extern RECT g_world_text_typewriter_vram_rect;              /* VRAM destination of the finished glyph */
extern u8 g_world_text_typewriter_glyph_images[3][8][0x54]; /* eight glyph images per slot */
extern u8 g_world_text_typewriter_previous_images[3][0x54]; /* previous image per slot */
extern u8 g_world_text_typewriter_column_scratch[14];       /* one-column scratch used by the phase shift */
extern s16 g_world_text_partner_screen_coords[];
extern s16 g_world_text_speaker_screen_coords[3];
extern world_panel_record_t g_world_numeric_display_panel_records[2];
extern s32 g_world_text_substitution_values_backup[32]; /* substitution words saved around the text calls */
extern world_menu_list_page_t g_world_text_list_pages[2];

/* The two glyph blit descriptors, filled by world_gfx_set_blit_source_rect
 * and world_gfx_set_blit_dest_position. Each is 0x10 bytes and the second
 * starts 0x10 past the first. The stride is the surface width in pixels:
 * 0x100 for the glyph sheet, 0x80 for a window frame's scratch image. */
extern world_glyph_blit_t g_world_text_glyph_source;
extern world_glyph_blit_t g_world_text_glyph_dest;
extern RECT g_world_text_message_box_vram_rect;
extern u8 g_world_text_message_box_glyph_images[3][8][84];
extern u8 g_world_text_message_box_previous_images[3][84];
extern u8 g_world_text_message_box_column_scratch[14];
extern s32 g_world_text_thread_active_text;
extern u8 g_world_text_row_image_buffer[];
extern u8* g_world_text_glyph_bitmap_data;

/* Text section base per text-id section (id >> 11 & 0x1f, text_section_e);
 * several elements also carry their own names below. */
extern u8* g_world_text_section_pointers[TEXT_SECTION_COUNT];

void world_text_message_box_thread(void);
void world_text_blit_font_glyph_to_4bpp(u8* glyph, void* arg1, world_glyph_blit_t* origin, s32 color);
s32 world_text_decode_font_glyph_to_4bpp(s32 glyph, u8* image, world_text_glyph_target_t* target, s32 palette);
void world_text_build_display_message(s32 offset, s32 flags, s32* out_width);
u8* world_menu_text_advance_cursor_with_backreferences(world_text_backreference_state_t* state, u8* cursor);
void world_text_blit_glyph(void* otag, void* text_id, world_glyph_blit_t* resource, world_glyph_blit_t* pos);
s32 world_text_count_decimal_digits(s32 value);
void world_text_draw_glyph_with_typewriter_delay(s32 pen_x, s32 pen_y, s32 delay);
void world_text_message_box_draw_glyph_with_typewriter_delay(s32 x, s32 y, s32 wait);
void world_menu_draw_stacked_glyph_pair(void* buffer, world_glyph_blit_t* dest);
u8* world_text_find_entry_by_index(u8* p, s16 count, s32 unused);

void world_text_render_decimal_entry_list(
    void* pixels, menu_number_entry_t* entries, world_glyph_blit_t* output, s32 count);

void world_text_render_decimal_value(s32 value, s32 flags, void* pixels, world_glyph_blit_t* position);
void world_text_render_decimal_value_or_dashes(s32 value, s32 flags, void* pixels, world_glyph_blit_t* position);
void world_text_render_id_list_to_image_rows(u8* font, s16* text_ids, RECT* destination, s32 palette);

void world_text_render_encoded_ids_to_image(
    u8* image, RECT* rect, s32 unused, s32 max_glyphs, u8* font, s16* ids, s32 count, s16 glyph, s32 unused_palette);

void world_text_render_signed_decimal_entries(
    void* pixels, menu_number_entry_t* entries, world_glyph_blit_t* output, s32 count);

s32 world_text_resolve_format_string_id(s32 event_code, s32 value);
u8* world_text_advance_cursor_with_backreferences(world_text_backreference_state_t* state, u8* cursor);
u8* world_text_advance_entry_by_char_count(u8* text, s32 count);
void world_text_concatenate_entries(u8* text_table, u8* destination, const s16* text_ids, s32 separate_entries);
u8* world_text_find_entry(s32 combined_text_index);
void world_text_determine_spell_quote(struct battle_ai_command_action* action, s32 unit_id, s32 enabled);
void world_text_build_unit_name_list(void);
void world_text_generate_formation_unit_name_string(void);
const u8* world_text_skip_to_entry(const u8* text, s32 entry_index);
s16 world_text_get_encoded_char_at_position(u8* text, s16 position, s32 mode);
void world_text_relocate_pointer_table(s32* offset_table);
void world_text_init_section_pointers(void);
s32 world_text_is_printing(s32 thread_id);
void world_text_kern_into_command_buffer(u8* text, s16* name, RECT* rect, s32 palette);
void world_text_measure(s16* columns, s16* rows, const u8* text);
s32 world_text_measure_entry_pixel_width(u8* entry);
void world_text_measure_pixels(s16* width, s16* rows, const u8* text);
void world_text_restore_section_pointers(void);
void world_menu_close_help_menu(void);
void world_text_resume_printing(s32 thread_id);
void world_menu_run_helpmenu_thread(void);
void world_text_save_section_pointers(void);
void world_text_set_message_duration_frames(s32 frames);
void world_text_show_message_and_play_sound(s32 message_id, s32 sound_id);
void world_text_start_character_thread_if_idle(s32 thread_id, s32 first, s32 second, s32 third);
void world_menu_start_building_thread(void);
void world_text_character_handling_thread(void);
void world_text_release_current_message_box_slot(void);

/* turn */
extern world_menu_list_page_t g_world_at_list_pages[2];
void world_build_at_list(void);
void world_screen_build_at_list(void);

/* dead unit */
extern s32 g_world_dead_unit_action; /* committed action */
extern s32 g_world_dead_unit_result;
extern dead_unit_entry_counts_t g_world_dead_unit_menu_entries;
extern dead_unit_menu_t g_world_dead_unit_menu;
extern RECT g_world_screen_menu_thin_panel_texture_windows[7];

/* One libgpu texture window per DR_MODE packet (the seven RECTs after
 * g_world_screen_menu_thin_panel_texture_windows). The static table holds each
 * window's w/h; world_screen_menu_window_frame_thread fills in the runtime x/y.
 * Indexing the array reproduces the target's symbol-plus-byte-offset addressing. */
extern RECT g_world_screen_menu_panel_texture_windows[7];
extern u32* g_world_dead_unit_controller_input;
extern s16 g_world_dead_unit_menu_flags[256];
extern s16 g_world_dead_unit_menu_result; /* primary menu result */
extern s16 g_world_dead_unit_move_result; /* secondary (move) menu result */

/* menu */
/* The builder walks the table with a byte offset stepping by 8 (the
 * target's induction variable), so it addresses these fields by offset. */
enum world_menu_line_layout_offset {
    WORLD_MENU_LINE_LAYOUT_X0 = 0,
    WORLD_MENU_LINE_LAYOUT_Y0 = 1,
    WORLD_MENU_LINE_LAYOUT_X1 = 2,
    WORLD_MENU_LINE_LAYOUT_Y1 = 3,
    WORLD_MENU_LINE_LAYOUT_ADD_W0 = 4,
    WORLD_MENU_LINE_LAYOUT_ADD_H0 = 5,
    WORLD_MENU_LINE_LAYOUT_ADD_W1 = 6,
    WORLD_MENU_LINE_LAYOUT_ADD_H1 = 7,
    WORLD_MENU_LINE_LAYOUT_SIZE = 8
};

/* Provisional: menu-thread data record reached through
 * g_world_menu_thread_menu_data; the thread entry at 0x28 and the
 * selection at 0x74 are the only known fields. */
typedef struct world_menu_thread_data {
    u8 _unused_00[0x28];
    void (*thread_entry)(void); /* 0x28 */
    u8 _unused_2c[0x74 - 0x2c];
    s16 selection; /* 0x74 */
} world_menu_thread_data_t;

/* Variable-length list command (0x10) in a WORLD menu script: the layout
 * world_menu_load_scrollable_list_layout reads and the placement
 * world_menu_script_draw_scrollable_list draws with. */
typedef struct world_menu_list_record {
    u8 type;            /* 0x00: 0x10 = list layout, 0x1c = end of stream */
    u8 length;          /* 0x01: byte length of this record */
    u8 show_cursor;     /* 0x02 */
    u8 _unused_03;      /* 0x03 */
    u8 skip_count;      /* 0x04: row records that follow (g_world_menu_list_row_record_count) */
    u8 row_height;      /* 0x05 */
    u8 visible_rows;    /* 0x06 */
    u8 x;               /* 0x07 */
    u8 y;               /* 0x08 */
    u8 width_a;         /* 0x09 */
    u8 width_b;         /* 0x0a */
    u8 cursor_x_offset; /* 0x0b */
    u8 arrow_up_y;      /* 0x0c */
    u8 arrow_down_y;    /* 0x0d */
} world_menu_list_record_t;

typedef char world_menu_line_f2_size_must_be_16[(sizeof(LINE_F2) == 16) ? 1 : -1];

/* Provisional record handed to the world text character thread by
 * world_menu_start_description_text_thread; only the description text buffer at +0x38 is known. */
typedef struct world_menu_description_record {
    u8 _unused_00[0x38];
    u8 description_text[1]; /* 0x38: rendered menu description text */
} world_menu_description_record_t;

/* Provisional record read by world_menu_build_layout_sprites: an x/y origin at +0x08
 * and a layout mode halfword at +0x2c (1 or 2). Other fields are unknown. */
typedef struct world_menu_sprite_layout {
    u8 _unused_00[8];
    u16 x; /* 0x08 */
    u16 y; /* 0x0a */
    u8 _unused_0c[0x2c - 0x0c];
    s16 mode; /* 0x2c */
} world_menu_sprite_layout_t;

/* Partial image view used by 0x801061e0. Upload rectangles determine the
 * second page's size; it begins at byte +0x8000 in both resource layouts. */
typedef struct world_menu_image_pages {
    u32 first_page[8192];
    u32 second_page[1];
} world_menu_image_pages_t;

/* Persistent palette source, separate from the temporary FRAME.BIN buffer.
 * Only the 64-byte color block at +0x1000 is established. */
typedef struct world_menu_palette_source {
    u8 prefix[4096];
    u8 colors[64];
} world_menu_palette_source_t;

/* Selection caches read the low byte of a little-endian halfword. */
typedef union world_menu_halfword_view {
    s16 value;
    u8 bytes[2];
} world_menu_halfword_view_t;

/* Value range of a numeric menu entry (world_menu_number_entry_thread and
 * world_menu_labeled_number_entry_thread). */
typedef struct world_menu_number_range {
    s16 min;                 /* 0x00 */
    s16 max;                 /* 0x02 */
    s16 _unknown_04[2];      /* 0x04: -1 from wldcore_window_init_number_panel_render_thread */
    void (*on_change)(void); /* 0x08 */
} world_menu_number_range_t;

/* The menu thread's first parameter as the window frame threads read it. Only the
 * leading window rectangle is used and the record is certainly longer, so no
 * size is asserted. The fields are unsigned: every read in the target is an
 * lhu, and making them signed turns each one into an lh. */
typedef struct world_menu_window {
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} world_menu_window_t;

/* The primitive block at 0x801cc074: the dead-unit and window-frame panels
 * lay it out as two panel frames, world_menu_build_sprite_page as two sprite
 * pages. */
typedef union world_menu_panel_buffer {
    world_panel_frame_t panel_frames[2];
    menu_sprite_page_t sprite_pages[2];
} world_menu_panel_buffer_t;

/* Result slots the shop menus share. */
enum {
    WORLD_MENU_SELECTION_SHOP_QUANTITY = 3, /* chosen purchase/sale quantity */
    WORLD_MENU_SELECTION_PROMPT_RESULT = 4, /* yes/no prompt result (0 confirms) */
};

/* Index of a saved cursor in g_world_menu_cursor_positions, as the formation
 * and shop menus use them. */
typedef enum world_menu_cursor {
    WORLD_MENU_CURSOR_SHOP_ITEM_TAB = 0,
    WORLD_MENU_CURSOR_EQUIP_SLOT = 2,
    WORLD_MENU_CURSOR_REMOVE_SLOT = 3, /* equipment slot of the remove and sell menus */
    WORLD_MENU_CURSOR_SET_ABILITY_SLOT = 4,
    WORLD_MENU_CURSOR_REMOVE_ABILITY_SLOT = 5,
    WORLD_MENU_CURSOR_ITEM_TAB = 7,
    WORLD_MENU_CURSOR_ABILITY_TAB = 8,
} world_menu_cursor_e;

extern u8 g_world_menu_buffer_allocation_map[0x70];
extern u16 g_world_menu_clut;
extern u16 g_world_menu_clut_back;
extern u16 g_world_menu_clut_back_alt;
extern u16 g_world_menu_clut_front;
extern u16 g_world_menu_clut_front_alt;
extern s32 g_world_menu_color_blue;
extern s32 g_world_menu_color_green;
extern s32 g_world_menu_color_red;
extern u32 g_world_menu_current_button_input;
extern s32 g_world_menu_current_id; /* battle_menu_id_e */
extern s16 g_world_menu_cursor_position;
extern s16 g_world_menu_cursor_positions[16];
extern s32 g_world_menu_description_text_id;
extern s16 g_world_menu_draw_priority;
extern s32 g_world_menu_event_state_flag;
extern u16 g_world_menu_icon_tpage;
extern u16 g_world_menu_input_mode; /* menu_input_mode_e */
extern u32 g_world_menu_input_initial_repeat_counter;
extern u32 g_world_menu_input_repeat_counter;
extern u32 g_world_menu_input_secondary_repeat_counter;
extern s16 g_world_menu_learn_unlocked_jobs[];
extern s32 g_world_menu_main_scroll_direction;
extern s32 g_world_menu_main_scroll_threshold;
extern s16 g_world_menu_option_count;
extern s16 g_world_menu_order_unit_cursor;
extern s16 g_world_menu_order_unit_entries[];
extern s8 g_world_menu_order_unit_open;
extern s32 g_world_menu_packet_buffer_index;
extern u16 g_world_menu_palette_clut;
extern u8 g_world_menu_palette_colors[];
extern s32 g_world_menu_panel_fade_intensity;
extern s32 g_world_menu_panel_fade_mode; /* menu_panel_fade_mode_e */
extern u32 g_world_menu_previous_button_input;
extern u8 g_world_menu_prompt_active;
extern const u8 g_world_menu_record_template[8];
extern u8 g_world_menu_restore_pending;
extern s16 g_world_menu_row_height;
extern s16 g_world_menu_screen_open_request;
extern u8* g_world_menu_scripts[];
extern s16 g_world_menu_scroll_offset;
extern s16 g_world_menu_scroll_pixel_offset;
extern s32 g_world_menu_scroll_row_offset;
extern u8 g_world_menu_sound_effect_id; /* main_sfx_e */
extern s32 g_world_menu_sound_muted;
extern CVECTOR g_world_menu_sprite_color;
extern world_menu_entry_t g_world_menu_system_entries[];
extern s8 g_world_menu_temporary_draw_area_active;
extern s32 g_world_menu_text_color;
extern s32 g_world_menu_text_file_buffer[];
extern s32 g_world_menu_text_param_22;            /* 0x80153304: record +0x22 */
extern s32 g_world_menu_text_param_24;            /* 0x80153308: record +0x24 */
extern menu_text_state_t g_world_menu_text_state; /* 0x80153280 */
extern u16 g_world_menu_texture_page;
extern s32 g_world_menu_thread_result;
extern s32 g_world_menu_thread_running_status;
extern u16 g_world_menu_transition_active;
extern u16 g_world_menu_transition_timeout;
extern battle_menu_record_t g_world_menu_unit_selection_rows[];
extern s32 g_world_menu_use_alternate_main_palette;
extern u8 g_world_menu_use_alternate_palette;
extern s16 g_world_menu_use_scroll_position;
extern menu_window_buffer_t* g_world_menu_window_buffer_pointers[6];
extern s8 g_world_menu_window_scale_step;
extern u16 g_world_menu_ability_display_flags[20];
extern s32 g_world_menu_resolved_skillset;
extern s32 g_world_menu_resolved_ability;

extern s32 g_world_menu_last_dispatched_command;
extern s32 g_world_menu_last_dispatched_option;
extern u8 g_world_menu_restore_depth;
extern u8 g_world_menu_restore_unit_id;
extern battle_menu_id_record_t g_world_menu_command_maps[31];
extern world_menu_palette_source_t g_world_menu_glyph_image[];
extern u8 g_world_menu_default_palette_colors[];
extern RECT g_world_menu_portrait_pixel_rect;
extern RECT g_world_menu_portrait_palette_rect;
extern s32 g_world_menu_scrolling_list_depth;
extern s32 g_world_menu_hide_numeric_values;
extern u16 g_world_menu_overlay_state;
extern u16 g_world_menu_transition_finished;
extern u16 g_world_menu_input_block_frames;
extern s32 g_world_menu_slot_owner_thread_ids[];
extern s32 g_world_menu_slot_states[];

/* Twelve crop percentages, indexed by step. */
extern u16 g_world_menu_cursor_zoom_percentages[];
extern world_menu_scroll_text_layout_t g_world_menu_status_list_layout;
extern world_menu_scroll_text_layout_t g_world_menu_at_list_layout;
extern s16 g_world_menu_auto_battle_row_actions[8];
extern s16 g_world_menu_action_slot_row_actions;
extern s16 g_world_menu_skillset_row_actions[]; /* per-row message id */
extern s16 g_world_menu_wait_row_actions[4];
extern u16 g_world_menu_status_list_row_actions[8];
extern u16 g_world_menu_system_function_row_actions[];
extern s16 g_world_menu_confirm_stored_unit_row_actions;
extern s16 g_world_menu_result; /* menu result: -4 pending, -1 cancelled */
extern s16 g_world_menu_set_auto_battle_row_actions;
extern RECT g_world_menu_numeric_display_texture_window; /* texture window for the menu page */

extern s8
    g_world_menu_numeric_display_frame_offsets[]; /* twelve world_menu_line_layout_t records, walked by byte offset */

extern u16* g_world_menu_window_tween_easing_tables[];
extern u16 g_world_menu_hovered_stats_frame_count;
extern world_menu_quad_piece_t g_world_menu_quad_pieces[]; /* quad piece table */
extern u8 g_world_menu_quad_group_starts[];                /* first piece index per group; [n + 1] ends group n */
extern world_menu_quad_colors_t g_world_menu_quad_colors;  /* vertex colours */
extern point16_t g_world_menu_quad_origin;                 /* screen origin */
extern RECT g_world_menu_sprite_page_texture_windows[5];
extern world_gfx_image_load_parameters_t g_world_menu_sprite_page_image_params[30];
extern RECT g_world_menu_hovered_unit_stats_display;
extern RECT g_world_menu_attack_caster_stats_display;
extern RECT g_world_menu_attack_target_stats_display;
extern RECT g_world_menu_right_unit_stats_display;
extern menu_number_entry_t g_world_menu_hovered_unit_stats_setup[];
extern menu_number_entry_t g_world_menu_right_unit_stats_setup[];
extern menu_number_entry_t g_world_menu_attack_caster_stats_setup[];
extern menu_number_entry_t g_world_menu_attack_target_stats_setup[];

/* Four image-load parameter blocks per icon kind, for kinds 5..15. */
extern world_gfx_image_load_parameters_t g_world_menu_icon_strip_image_params[];

/* One image-load parameter block per single-icon kind 0..4; index 1 doubles as
 * the arrow placement used by kind 0x10. */
extern world_gfx_image_load_parameters_t g_world_menu_single_icon_image_params[];
extern s32 g_world_menu_building_thread_started;
extern u8 g_world_menu_cleared_entry_indices[11];
extern s16 g_world_menu_row_type_confirm_actions[][2]; /* per row type: [1] == 1 lists by id */
extern u8 g_world_menu_action_slot_default_messages[];
extern world_glyph_blit_t g_world_menu_number_glyph_rect; /* glyph source rectangle in the number/sign font */

/* Tail glyph source and destination records for world_menu_build_window_image. */
extern world_glyph_blit_t g_world_menu_window_sprite_source;
extern world_glyph_blit_t g_world_menu_window_sprite_position;
extern RECT g_world_menu_thin_panel_texture_windows[7];
extern RECT g_world_menu_panel_texture_windows[7];
extern u8 g_world_menu_at_list_entry_table[];
extern world_menu_entry_t g_world_menu_at_list_state;

extern s16 g_world_menu_open_entry_index;
extern s32 g_world_menu_thread_group_parameter;
extern s32 g_world_menu_location_id;
extern s32 g_world_menu_location_window_x;
extern s32 g_world_menu_location_window_y;
extern u32 g_world_menu_default_entry_0_text_values[2]; /* text index + 0xa000, value */
extern s32 g_world_menu_default_entry_1_text_values[2]; /* value, 0 */
extern s32 g_world_menu_location_text_id;
extern u16 g_world_menu_location_list_text_ids[];
extern s16 g_world_menu_location_list_row_colors[];
extern world_menu_scroll_text_layout_t g_world_menu_location_list_layout;
extern u8 g_world_menu_submenu_page_counts[];
extern u8* g_world_menu_submenu_page_text_ids[];
extern world_menu_scroll_text_layout_t g_world_menu_submenu_page_layout;
extern s32 g_world_menu_number_entry_value;
extern world_menu_entry_t g_world_menu_default_entries[];

/* The menu frame of world_menu_run_frame: its input words, the buttons it lets through, the
 * thread states it compares across its yield, and the system flags world_menu_hide_world_map saves. */
extern world_menu_input_state_t g_world_menu_frame_input_state;

/* First-word alias retained for the target's separate absolute store. */
extern s32 g_world_menu_frame_new_buttons;
extern u32 g_world_menu_allowed_buttons; /* pad mask applied to the new presses of each menu frame */
extern u8 g_world_menu_thread_status_snapshot[16];
extern u32 g_world_menu_saved_system_flags; /* main_system_flags_e */

/* 1 while the menu frame runs inside WLDCORE's or OPEN's frame (world_menu_run_frame); WORLD's own
 * screens clear it (world_input_update_event_state). Message boxes and the zodiac backdrop place
 * themselves by it. */
extern s32 g_world_menu_hosted_frame;

/* 1 holds the menu cursors in their confirm bob (world_menu_set_cursor_confirm_hold). */
extern s32 g_world_menu_cursor_confirm_hold;

/* The zodiac backdrop thread's view record, its two frames of quads and its ordering table. */
extern zodiac_draw_context_t g_world_menu_zodiac_backdrop;
extern u8 g_world_menu_zodiac_backdrop_quads[2][0x4b0];
extern void* g_world_menu_zodiac_backdrop_otag;

extern RECT g_world_menu_frame_palette_rect;
extern RECT g_world_menu_frame_palette_tail_rect;
extern RECT g_world_menu_frame_image_rect;
extern RECT g_world_menu_item_image_rect;
extern RECT g_world_menu_common_clut_rect;
extern RECT g_world_menu_item_palette_rect;
extern RECT g_world_menu_unit_image_rect_a;
extern RECT g_world_menu_unit_image_rect_b;
extern u32 g_world_menu_common_clut_data[];
extern const u16 g_world_menu_zoom_percentages[12]; /* twelve crop percentages, indexed by step */
extern s16* g_world_menu_entry_text_id_tables[];    /* halfword tables selected by entry select_text_table */
extern s32 g_world_menu_number_entry_digit_steps[];
extern s32 g_world_menu_indicator_packet_index[2];
extern s32 g_world_menu_indicator_brightness[2];
extern s32 g_world_menu_indicator_fade_work[2];
extern s32 g_world_menu_indicator_state[2];

/* The sprite pieces of the on-screen buttons (world_menu_draw_pressable_button): button i draws
 * pieces g_world_menu_button_piece_starts[i] to g_world_menu_button_piece_starts[i + 1] - 1. */
extern world_menu_button_piece_t g_world_menu_button_pieces[];

/* Per-piece 4.12 fixed-point x/y scale; a negative factor flips that axis. */
extern point16_t g_world_menu_button_piece_scales[];
extern u8 g_world_menu_button_piece_starts[];
extern s8 g_world_menu_button_press_timers[];
extern u16 g_world_menu_window_tpage;
extern u16 g_world_menu_palette_clut_normal;
extern u16 g_world_menu_palette_clut_alt;
extern u16 g_world_menu_alternate_number_clut_alt;
extern u16 g_world_menu_alternate_number_clut_normal;
extern u16 g_world_menu_disabled_number_clut_normal;
extern u16 g_world_menu_disabled_number_clut_alt;
extern u16 g_world_menu_button_clut;
extern u16 g_world_menu_hp_gauge_clut;
extern u16 g_world_menu_mp_gauge_clut;
extern u16 g_world_menu_ct_gauge_clut;
extern u16 g_world_menu_background_clut;
extern u16 g_world_menu_window_clut_alt;
extern u16 g_world_menu_window_clut_normal;
extern u8* (*g_world_menu_script_handlers[])(u8* script);
extern u8 g_world_menu_script_sprite_command[];
extern world_menu_window_command_t g_world_menu_window_interior_command;
extern s16 g_world_menu_window_scale_table[];
extern world_oriented_quad_t g_world_menu_number_blank_sprite;
extern s8 g_world_menu_cursor_bob_thresholds[];
extern s8 g_world_menu_cursor_bob_active_thresholds[];
extern s16 g_world_menu_list_row_text_ids[];
extern u16 g_world_menu_list_cursor_anim;
extern u16 g_world_menu_scroll_hold_counter;
extern s16 g_world_menu_text_redraw_request;
extern s16 g_world_menu_pending_selection[4];
extern u16 g_world_grid_menu_cursor_anim; /* grid cursor animation state */
extern u8 g_world_grid_menu_initialized;  /* grid menu initialised */
extern u32 g_world_menu_new_button_input;
extern s16 g_world_menu_status_left_text_ids[20];
extern s16 g_world_menu_status_right_text_ids[20];
extern struct battle_ai_command_action g_world_menu_preview_action;
extern s32 g_world_menu_monster_skillset_flag;
extern u16 g_world_menu_anything_ability_id;
extern s16 g_world_menu_selected_option;
extern s16 g_world_menu_action_slot_selected_option;
extern battle_menu_allocation_t g_world_menu_buffer_allocations[16];
extern u8 g_world_menu_buffer_arena[0x70][0x100];
extern s32 g_world_menu_preview_target_action;
extern world_fade_tile_frame_t g_world_menu_panel_fade_frames[];
extern s32 g_world_active_menu;                    /* active menu */
extern s32 g_world_menu_dead_unit_panel_parameter; /* WORLD twin of g_battle_menu_dead_unit_panel_parameter */
extern u8 g_world_menu_text_page_image[];
extern u8 g_world_menu_window_frame_image[];
extern s16 g_world_menu_at_list_primary_values[];
extern s16 g_world_menu_at_list_secondary_values[];
extern s16 g_world_menu_at_list_indices[];
extern s16 g_world_menu_at_list_flags[256];
extern u8 g_world_menu_at_list_text[];
extern s16 g_world_menu_submenu_page_row_text_ids[];
extern u32* g_world_menu_list_controller_input;
extern world_item_icon_source_t g_world_menu_entry_item_icon_source;
extern u16 g_world_menu_scroll_repeat_timer;
extern s32 g_world_menu_display_script_input;
extern s16 g_world_menu_list_visible_rows;
extern s16 g_world_menu_list_row_record_count;
extern s16 g_world_menu_semi_trans;
extern s16 g_world_menu_window_right_x;
extern s8 g_world_menu_list_layout_pending;
extern const s16* g_world_menu_list_entry_ids;
extern u8* g_world_menu_list_text_table;
extern u16 g_world_menu_list_image_width;
extern u8 g_world_menu_list_glyph_width;
extern u8 g_world_menu_list_text_columns;
extern s8 g_world_menu_list_scroll_direction;
extern RECT g_world_menu_window_rect;
extern u8 g_world_grid_menu_selected_cell; /* selected cell */
extern world_menu_panel_buffer_t g_world_menu_panel_buffer;
extern u16 g_world_menu_alternate_number_clut;
extern s16 g_world_menu_entry_ids[];
extern s8 g_world_menu_music_slot; /* formation music slot */
extern u16 g_world_menu_window_clut;
extern u16 g_world_menu_active_clut_front;
extern u16 g_world_menu_active_clut_back;
extern world_menu_window_rect_source_t* g_world_menu_window_rect_source;

/* Result slot per menu (-1 while none); world_menu_reset_selection_results clears all 16. */
extern s16 g_world_menu_selection_results[16];
extern s32 g_world_grid_menu_id; /* menu_grid_help_id_e */

extern s16 g_world_menu_scenario_event_started;
extern world_menu_entry_t* g_world_menu_thread_menu_data;

/* Menu-script callback slots, called as s32 (*)(s32) by the script commands. */
extern s32 (*g_world_menu_script_callbacks[16])(s32);

extern world_menu_palette_source_t* g_world_menu_glyph_sheet; /* decoded glyph page, 0x80 bytes per row */
extern u16 g_world_menu_disabled_number_clut;
extern menu_window_buffer_t g_world_menu_window_buffers[6]; /* 0x8019aca0 */

/*
 * WORLD.BIN routines shared by the world, wldcore, and menu-overlay
 * reconstructions.  Each module's linker table maps these names to its own
 * copy of the routine.
 */
void world_menu_unit_status_banner_thread(void);
void world_menu_equipment_panel_thread(void);
void world_menu_ability_panel_thread(void);

void world_menu_display_hovered_unit_stats(
    RECT* frame_rect, s32 mode, menu_number_entry_t* entries, battle_unit_status_record_t* status);

void world_menu_build_ability_preview_at_list(void);
void world_menu_scrolling_list_thread(void);
void world_menu_select_unit_action_slots_thread(void);
s32 world_menu_resolve_selection(void);
void world_menu_number_entry_thread(void);
void world_menu_equipment_and_ability_panel_thread(void);
s16 world_menu_update_wrapped_vertical_selection(u16, u8, s32);
void world_menu_run_script_with_callback_suppressed(void);
void world_menu_open_name_entry_key_list(void);
void world_clear_menu_render_buffer(u8* dst, u32 count);
void world_menu_zoom_cursor_frame(const void* source, SPRT* sprite, s32 step);
void world_gfx_copy_sprite_position_uv_and_size(const world_menu_entry_t* param, SPRT* sprite, s32 unused_frame);
void world_draw_menu_text_columns_narrow(world_menu_entry_t* entry, s32* row_offset, void* buffer);
void world_menu_init_panel_fade_tiles(void);
void world_draw_menu_number_glyphs(s32 value, s32 digits, void* resource, world_glyph_blit_t* pos);
void world_menu_labeled_number_entry_thread(void);
world_menu_window_command_t* world_menu_parse_draw_window_frame(world_menu_window_command_t* command);
void* world_menu_alloc_window_buffer_pair(void);
void world_menu_animate_window_quad_crop(menu_window_buffer_t* buffers, RECT* rect, RECT* unused_to);
void world_menu_animate_window_quad_crop_full(menu_window_buffer_t* buffers, RECT* rect, RECT* unused_to);
void world_menu_animate_window_quads_to_rects(menu_window_buffer_t* buffers, RECT* to_main, RECT* to_icon);
void world_menu_build_ability_list(s32 mode);
void world_menu_build_and_upload_window_image(s32 width, s32 height, RECT* rect, s32 mode, s32 tail_offset);
void world_menu_build_idle_action_stub(s32 unused_menu_id);
void world_menu_build_sprite_page(s32 page_index, RECT* base_screen);
void world_menu_check_action_slot_restrictions(s32 menu, s32 value, s32 check_unit);
void world_menu_release_current_thread_slot(void);

s32 world_menu_dispatch_system_function(
    s32 command, s32 option, s32 unit_id, s32 parameter, s32 enabled, struct battle_ai_command_action* action);

void world_menu_draw_active_window_frames(void);

void world_menu_draw_numeric_display_entries(
    void* pixels, menu_number_entry_t* entries, world_glyph_blit_t* position, s32 count);

void world_menu_draw_pressable_button(s16 index, s32 x, s16 y, s16 pressed, s16 disabled, u16 sound_effect_id);
void world_screen_menu_draw_text_columns(world_menu_entry_t* entry, s32* row_offset, void* buffer);
s32 world_menu_get_selected_unit_hp_bonus_or_dashes(s32 unused_row);
s32 world_menu_get_selected_unit_mp_bonus_or_dashes(s32 unused_row);
s32 world_menu_get_party_unit_name_id(void);
u8* world_menu_get_script(s32 index);
s32 world_menu_get_selected_unit_hp_bonus(s32 unused_row);
s32 world_menu_get_selected_unit_mp_bonus(s32 unused_row);
void world_text_configure_sprite_vram(RECT* rect, s32 width, s32 height, void* prim, s32 clut_offset);
s32 world_menu_is_busy(s32 unused);
s32 world_menu_is_screen_busy(void);
void world_menu_open_entry_window(s32 index, s32 unused_x, s32 unused_y);
u8* world_menu_redraw_text_page_on_scroll(world_menu_entry_t* entry, s32* row_offset, s32* redraw, s32* unused_cursor);
u8* world_screen_menu_redraw_text_page_on_scroll(
    world_menu_entry_t* entry, s32* row_offset, s32* redraw, s32* unused_cursor);

u8* world_menu_redraw_text_page_on_scroll_in_rect(
    world_menu_entry_t* entry, s32* row_offset, s32* redraw, s32* unused, RECT* area);

void world_script_apply_input_overrides(u32* state);
void world_menu_render_text_image_at_fixed_origin(world_text_dialog_record_t* record);
void world_menu_render_text_image_at_record_origin(world_text_dialog_record_t* record);
void world_menu_run_equip_thread(void);
void world_screen_menu_window_frame_thread(void);
s32 world_menu_run_unit_status_banner_countdown(s32 message_id, s32 frames);
u8* world_menu_script_draw_sprite(u8* cmd);
world_draw_number_command_t* world_menu_script_draw_formatted_number(world_draw_number_command_t* command);
void world_menu_scroll_list_page(s32 dir, world_menu_list_record_t* list);
void world_menu_select_icon_cluts(world_menu_icon_sprites_t* menu);
void world_menu_select_sprite_clut(void* prim);
void world_menu_add_gradient_line_primitive(s16* endpoints, u8* rgb, s32 semi_trans, s32 ot_index);
void* world_menu_alloc_buffer(s32 size);
void* world_menu_alloc_ui_buffer(s32 size);
void world_menu_announce_entry_value_thread(void);
void* world_menu_build_and_upload_window_frame_image(s32 width, s32 height, RECT* rect, s32 mode);
void world_gfx_enqueue_line_f2(RECT* rect, u8* data, u8 semi_trans, s32 priority);
void world_menu_build_icon_record(RECT* rect, world_menu_entry_t* param, world_menu_icon_record_t* buffer);
void world_menu_submit_icon_primitives(world_menu_icon_sprites_t* prims);
void world_menu_build_layout_sprites(world_menu_sprite_layout_t* layout, SPRT* sprite, s32 unused_2);
void world_menu_init_numeric_display_frame_primitives(RECT* rect, world_menu_palette_primitives_t* menu);
s32 world_menu_build_quad_pieces(s32 group, POLY_GT4* poly);
void world_menu_build_skillset_entries(void);
void world_menu_build_unit_status_list(void);
s32 world_menu_build_window_frame_image(s32 width, s32 height, u16* destination);
void* world_menu_build_window_image(s32 width, s32 height, RECT* rect, s32 mode, s32 tail_offset, s32 keep);
void world_menu_cancel_thread_group(world_menu_entry_t* context);
void world_menu_step_wrapping_cursor_on_scroll_buttons(world_menu_entry_t* bounds, s32* cursor);
void world_menu_check_action_restrictions(void);
s32 world_menu_check_thread_completion(u32* input);
void world_menu_confirm_action_silently(world_menu_entry_t* entry);
void world_menu_handle_entry_confirm(world_menu_entry_t* entry, s32 row_index);
void world_menu_clear_cursor_positions(void);
void world_menu_clear_entry_selections(void);
void world_menu_clear_transition_active_flag(void);
void world_menu_clear_window_buffer_pointers(void);

void world_menu_configure_status_panel_primitive_colors(
    battle_menu_status_panel_buffer_t* primitives, const world_status_thread_t* state);

void world_menu_display_text(s32 text_id, void* pixels, world_glyph_blit_t* origin, u8* text);
void world_menu_display_triangle_selection(void);
void world_menu_draw_animated_cursor(point16_t* position, u16* state, s32 mode);
void world_menu_draw_animated_vertical_cursor(point16_t* position, u16* state, s32 mode);
void world_menu_draw_item_category_tabs(s32 wide, s32 row, s32 mode);
void world_menu_draw_text_columns(world_menu_entry_t* entry, s32* row_offset, void* buffer);
void world_menu_draw_thread_status_indicators(void);
void world_menu_request_unit_panel_redraw(void);
void world_menu_fade_out_thread(void);
void world_menu_build_window_image_row(u16* source, u16* dest, s32 count, s32 offset);
void world_screen_menu_build_window_image_row(u16* source, u16* dest, s32 count, s32 offset);
void world_menu_free_high_overlay(void);
void world_menu_free_memory(void* buffer);
s32 world_menu_get_entry_item_count(s32 entry_index);
s32 world_menu_get_entry_item_equipped_count(s32 entry_index);
world_item_icon_source_t* world_menu_get_entry_item_icon_source(s32 index);
s32 world_menu_get_event_state_flag(void);
s32 world_menu_get_item_half_price_display_value(s32 entry_index);
s32 world_menu_get_item_price_display_value(s32 entry_index);
s32 world_menu_get_learn_job_jp(s32 index);
s32 world_menu_get_window_scale_step(void);
void world_menu_copy_unit_data_to_status_billboard(struct battle_stats* unit, battle_unit_status_record_t* output);
void world_menu_init_quad_from_record(u16* source, s16* record, POLY_FT4* poly);
void world_menu_reserve_high_overlay(u32 size);
void world_menu_run_display_script(u8* script, s32 value);
u8* world_menu_get_sprite_color(void);
void world_menu_handle_window_command_with_scaled_clip(world_menu_window_command_t* command);
s32 world_menu_has_items(s32 mode);
void world_menu_icon_linked_entry_thread(void);
void world_menu_icon_list_thread(void);
void world_menu_init_and_load_scrollable_list(const s16* values, s32 selected_index, u8* text_table, void* menu);
void world_menu_init_at_list(world_menu_entry_t* entry);
void world_menu_init_palette_and_text(s32 unused);
void world_menu_init_primitive_colors_normal(world_menu_palette_primitives_t*);
void world_menu_init_primitive_colors_dimmed(world_menu_palette_primitives_t*);
void world_gfx_init_default_poly_ft4(POLY_FT4* poly);
void world_option_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* pair);
void world_menu_init_scrollable_list(const s16* values, s32 value, s32 selected, u8* text_table);
void world_menu_init_scrollable_list_core(const s16* values, s32 selected_index, u8* text_table);
void world_menu_init_sprite_array(SPRT* sprites, s32 count, s32 clut);
void world_menu_init_subsystems(void);
s32 world_menu_is_input_allowed(void);
s32 world_menu_is_learn_ability_learned(s32 index);
s32 world_menu_lerp_fixed12(s32 from, s32 to, s32 t);
s32 world_screen_menu_lerp_fixed12(s32 from, s32 to, s32 t);
void world_menu_load_common_graphics(s32 load_extra_pages);
void world_menu_load_scrollable_list_layout(world_menu_list_record_t* record);
void world_menu_load_text_1bd8(void);
void world_menu_load_text_1bd8_with_face(void);
void world_menu_load_text_1c18(void);
void world_menu_store_sort_key_list(s32 index, s16* source);
void world_menu_open_scrollable_list(u8* list);
world_menu_window_command_t* world_menu_parse_draw_tiled_rectangle(world_menu_window_command_t* command);
void world_menu_release_window_buffer_pair(menu_window_buffer_t* value);
void world_menu_reset_subsystems(void);
void world_menu_reset_selection_results(void);
void world_menu_init_buffer_allocator(void);
void world_menu_reset_unit_records(void);
void world_menu_run_bunit_transition_thread(void);
void world_menu_run_jobstts_thread(void);
void world_menu_run_icon_selection_loop(void);
void world_menu_run_main_mode(s32 mode);
void world_menu_run_main_mode_0_thread(void);
void world_menu_run_numeric_display_panel_thread(void);
void world_menu_run_numeric_editor_thread(void);
void world_menu_run_script_with_palette_mode(void* output, s32 input, s32 mode);
void world_menu_run_skillset_thread(void);
void world_menu_run_system_function_thread(void);
s32 world_menu_run_thread(s32 thread_id, void* menu);
void world_menu_open_debugchr_unit_editor(void);
u8* world_menu_script_add_blend_draw_mode(u8* script);
u8* world_menu_script_clear_temporary_draw_area(u8* script);
u8* world_menu_script_set_draw_area(u8* script);
u8* world_menu_script_set_sprite_color(u8* script);
void world_menu_select_primitive_color_palette(world_menu_palette_primitives_t* menu, world_status_thread_t* input);
void world_menu_select_icon_sprite_clut(SPRT* slot);
void world_menu_set_brightness(s32 red, s32 green, s32 blue);
s16 world_menu_set_cursor_position(u8 index, s16 value);
s16 world_menu_init_cursor_position(u8 index, s16 value);
void world_menu_set_default_entry_0_text_values(u32 text_index, u32 value);
void world_menu_set_default_entry_1_text_values(s32 value);
void world_menu_set_draw_priority(s32 priority);
void world_menu_set_palette_colors(const u8* source);
void world_menu_set_palette_mode(s32 mode);
void world_menu_set_sprite_color(u8* src);
void world_menu_set_transition_active_flag(void);
void world_menu_set_window_scale_step(s8 step);
void world_menu_start_at_list_thread(void);
void world_menu_start_dead_unit_threads(void);
void world_menu_start_description_text_thread(world_menu_description_record_t* record);
void world_menu_open_option_dead_unit_panel(void);
void world_menu_start_or_poll_thread(s32 thread_id, world_menu_thread_data_t* data);
void world_menu_start_system_function_thread(s32 parameter);
void world_menu_start_thread_group(s32 parameter, s32 location_id, s32 window_x, s32 window_y);
s32 world_menu_update_vertical_selection_and_mark_change(u16 count, u8 cursor_id, s32 input, u8 sound);
s32 world_menu_update_horizontal_selection_and_mark_change(u16 count, u8 index, s32 input, u8 value);
s32 world_menu_update_wrapped_grid_selection(s32 columns, s32 rows, s32 last_index, s32 old_index, s32 input);
void world_menu_stop_thread(s32 thread_id);
void world_menu_stop_thread_and_wait(s32 thread_id);
void world_menu_stop_unit_status_banner_thread(s32 thread_id);
void world_menu_record_selection_for_selected_unit(void);
void world_menu_switch_item_category(s32 input);
void world_menu_text_entry_wait_thread(void);
void world_menu_thread_group_supervisor_thread(void);
void world_menu_toggle_preview_stats_window(s32 flag);
void world_menu_toggle_change_banner_panel_thread(s32 enable);
void world_menu_toggle_ability_panel_thread(s32 enable);
void world_menu_toggle_numeric_editor_thread(s32 enable);
void world_menu_toggle_comparison_status_thread(s32 flag);
void world_menu_toggle_stat_preview_panel_thread(s32 enable);
void world_menu_start_equipment_and_ability_panel_thread(s32 enabled);
void world_menu_toggle_unit_status_thread(s32 enable);
void world_menu_tween_window_quads(menu_window_buffer_t* buffers, RECT* to, RECT* to2, s32 table);

void world_menu_update_entry_cursor_quads(
    world_menu_entry_t* entry, POLY_FT4* quad, POLY_FT4* shadow, s32 unused, s32 cursor);

void world_menu_update_grid_cursor(s16 alternate);

void world_menu_update_icon_cursor_sprites(
    world_menu_entry_t* param, world_menu_icon_sprites_t* record, s32 frame, s32 cursor);

void world_menu_update_panel_fade(void);
void world_menu_load_sort_key_list(s32 index, s16* out);
s32 world_menu_copy_ff_terminated_bytes_to_halfwords(u16* destination, const u8* source);
void world_menu_window_frame_thread(void);
void world_menu_set_slot_4_cursor_index(s32 index);
s32 world_menu_get_thread_activity(void);
void world_menu_display_text_entry_past_choice_marks(s32 text_id, void* image, world_glyph_blit_t* origin);
void world_menu_display_text_entry(s32 text_id, void* image, world_glyph_blit_t* origin);
void world_menu_init_sprite(SPRT* sprite);
void world_menu_set_text_origin(s32 x, s32 y);
void world_menu_hide_world_map(void);
void world_menu_reset_context(void);
void world_menu_restore_world_map(void);
s16 world_menu_run_frame(u32* otag, u32 input);
void world_menu_set_allowed_buttons(u32 mask);
void world_menu_set_cursor_confirm_hold(s32 hold);
void world_menu_set_zodiac_backdrop_otag(void* ordering_table);
void world_menu_update_brave_story_request(void);
void world_menu_update_debug_variable_list_request(void);
void world_menu_update_fixed_number_panel_request(void);
void world_menu_update_tutorial_categories_request(void);
void world_menu_zodiac_backdrop_thread(void);

/* item */
/* WORLD's item screens: the formation item menu (categories, sort orders, rearranging, the
 * equipped-by view), the item lists and sorts the shops and equipment screens share, and the stat
 * previews of an equipment change. */

/* Halfword list entries retain their high byte when filtered at 0x801220d8;
 * only the low byte selects an item. The complete halfword sentinel is -1. */
typedef union world_item_list_entry {
    s16 value;
    struct {
        u8 item_id;
        u8 _unused_01;
    } bytes;
} world_item_list_entry_t;
typedef char world_item_list_entry_size_must_be_2[(sizeof(world_item_list_entry_t) == 2) ? 1 : -1];

/* World-map location ids (EVENT_SCRIPT_VAR_LOCATION), named as the location
 * name text (TEXT_ID_LOCATION_NAME_BASE + id) spells them. The first fifteen
 * are the towns; a town's id is also its outfitter's shop id and its bit,
 * counted from the top, in g_world_shop_item_availability. */
typedef enum world_location_id {
    WORLD_LOCATION_LESALIA = 0,
    WORLD_LOCATION_RIOVANES = 1,
    WORLD_LOCATION_IGROS = 2,
    WORLD_LOCATION_LIONEL = 3,
    WORLD_LOCATION_LIMBERRY = 4,
    WORLD_LOCATION_ZELTENNIA = 5,
    WORLD_LOCATION_GARILAND = 6,
    WORLD_LOCATION_YARDOW = 7,
    WORLD_LOCATION_GOLAND = 8,
    WORLD_LOCATION_DORTER = 9,
    WORLD_LOCATION_ZALAND = 10,
    WORLD_LOCATION_GOUG = 11,
    WORLD_LOCATION_WARJILIS = 12,
    WORLD_LOCATION_BERVENIA = 13,
    WORLD_LOCATION_ZARGHIDAS = 14,
    WORLD_LOCATION_TOWN_COUNT = 15,
    WORLD_LOCATION_FORT_ZEAKDEN = 15,
    WORLD_LOCATION_MUROND_HOLY_PLACE = 16,
    WORLD_LOCATION_THIEVES_FORT = 17,
    WORLD_LOCATION_ORBONNE_MONASTERY = 18,
    WORLD_LOCATION_GOLGORAND_EXECUTION_SITE = 19,
    WORLD_LOCATION_MUROND_DEATH_CITY = 20,
    WORLD_LOCATION_BETHLA_GARRISON = 21,
    WORLD_LOCATION_DEEP_DUNGEON = 22,
    WORLD_LOCATION_NELVESKA_TEMPLE = 23,
    WORLD_LOCATION_MANDALIA_PLAINS = 24,
    WORLD_LOCATION_FOVOHAM_PLAINS = 25,
    WORLD_LOCATION_SWEEGY_WOODS = 26,
    WORLD_LOCATION_BERVENIA_VOLCANO = 27,
    WORLD_LOCATION_ZEKLAUS_DESERT = 28,
    WORLD_LOCATION_LENALIA_PLATEAU = 29,
    WORLD_LOCATION_ZIGOLIS_SWAMP = 30,
    WORLD_LOCATION_YUGUO_WOODS = 31,
    WORLD_LOCATION_ARAGUAY_WOODS = 32,
    WORLD_LOCATION_GROG_HILL = 33,
    WORLD_LOCATION_BED_DESERT = 34,
    WORLD_LOCATION_ZIREKILE_FALLS = 35,
    WORLD_LOCATION_DOLBODAR_SWAMP = 36,
    WORLD_LOCATION_BARIAUS_HILL = 37,
    WORLD_LOCATION_DOGUOLA_PASS = 38,
    WORLD_LOCATION_BARIAUS_VALLEY = 39,
    WORLD_LOCATION_FINATH_RIVER = 40,
    WORLD_LOCATION_POESKAS_LAKE = 41,
    WORLD_LOCATION_GERMINAS_PEAK = 42,
    /* The fields, where random battles happen: Mandalia Plains to Germinas Peak. */
    WORLD_LOCATION_FIELD_FIRST = WORLD_LOCATION_MANDALIA_PLAINS,
    WORLD_LOCATION_FIELD_COUNT = 19,
    WORLD_LOCATION_COUNT = 43,
} world_location_id_e;

/* g_world_shop_id: a town's location id while its outfitter is open, or one of
 * these. */
enum {
    WORLD_SHOP_ID_NAME_ENTRY = -2,
    WORLD_SHOP_ID_FORMATION = -1,
    WORLD_SHOP_ID_FUR_SHOP = 0x64,
    WORLD_SHOP_ID_SOLDIER_OFFICE = 0x65,
};

extern world_oriented_quad_t g_world_item_category_tab_cursor_sprite;
extern struct world_item_stat_detail g_world_item_preview_stat_detail;
extern u8* g_world_item_sorted_lists[];
extern world_image_location_t g_world_item_icon_image_location;
extern s16 g_world_item_icon_clut_base_x; /* item icon CLUT base x */
extern s16 g_world_item_icon_clut_base_y; /* item icon CLUT base y */

/* Menu-subflow "running" flags. The readers sign-extend (lb), so these are
 * signed; the initialiser only stores 0/1/2 and cannot distinguish. */
extern s8 g_world_item_menu_initialized;
extern s8 g_world_item_best_remove_initialized;
extern s8 g_world_item_submenu_initialized;
extern s8 g_world_item_category_menu_initialized;
extern s8 g_world_item_rearrange_initialized;
extern s8 g_world_item_action_menu_initialized;
extern s8 g_world_item_sort_order_initialized;
extern s8 g_world_item_equipping_units_initialized;
extern world_menu_entry_t g_world_item_icon_menu;
extern u8 g_world_item_menu_unit_index;
extern u8 g_world_item_submenu_exit_pending;
extern s8 g_world_remove_item_unit_index;
extern s16 g_world_remove_item_saved_primary_input;
extern s16 g_world_remove_item_saved_secondary_input;
extern u16 g_world_remove_item_saved_newly_pressed;
extern world_menu_number_range_t g_world_item_action_quantity_range;
extern s16 g_world_item_action_option_states[3];
extern world_menu_entry_t g_world_item_action_menu;
extern u16 g_world_remove_item_cursor_anim;
extern point16_t g_world_remove_item_cursor_point;
extern world_menu_entry_t g_world_item_quantity_change_menu;
extern world_menu_entry_t g_world_item_unequip_all_menu;
extern u8 g_world_item_list_window_script[];
extern u8 g_world_item_hp_mp_bonus_window_script[];
extern s8 g_world_item_menu_mode; /* menu_item_list_state_e */

/* The two item-category icon strips; the active one is kept in g_world_item_category_icon_strip. */
extern world_menu_entry_t g_world_item_category_icon_strip_a;
extern world_menu_entry_t g_world_item_category_icon_strip_b;
extern s16 g_world_item_category_tabs_script;       /* window descriptor (narrow variant) */
extern s16 g_world_item_category_tabs_wide_script;  /* window descriptor (wide variant) */
extern s16 g_world_item_category_tab_sprites[][10]; /* 20-byte rects, one per row */
extern u8 g_world_item_category_tab_x[];            /* per-row base x */
extern u8 g_world_item_category_input_lock;
extern world_item_icon_source_t g_world_item_type_icon_source;
extern u8 g_world_item_menu_saved_browse_enabled;
extern s8 g_world_remove_item_previewed_slot;
extern world_menu_halfword_view_t g_world_remove_item_equipment[5];
extern s8 g_world_item_category_previewed_cursor;
extern s16 g_world_item_rearrange_picked_cursor;   /* cursor position of the picked-up item */
extern u16 g_world_item_rearrange_picked_scroll;   /* scroll offset when picked up */
extern s16 g_world_item_rearrange_preview_cursor;  /* last previewed cursor position */
extern u8 g_world_item_rearrange_anim_frame;       /* animation frame counter */
extern s16 g_world_item_category_strip_text_ids[]; /* option text ids of the active strip, -1 terminated */
extern world_menu_entry_t* g_world_item_category_icon_strip;
extern u16 g_world_item_saved_selected_unit_index;
extern u8 g_world_item_menu_category;

void world_gfx_set_item_icon_sprite(SPRT* sprite, s32 item_id);
s32 world_count_item_equipped_by_party(s32 item_id);
s32 world_item_build_category_list(s16 unit_id, s16 sort_mode, s8 category, s16* entries, u8 mode);
void world_gfx_build_item_graphic_descriptor(world_item_icon_source_t* source, s32 item_id);
void world_item_build_status_list_polygons(s16 item_id);
world_item_icon_source_t* world_item_build_type_icon_source(s32 entry_index);

void world_item_calculate_equipment_swap_stat_delta(
    world_item_stat_detail_t* detail_total, world_item_stat_summary_t* total, u16* items_before, u16* items_after);

void world_item_calculate_swap_stat_delta(
    world_item_stat_detail_t* output, world_item_stat_summary_t* delta, s16 item_before, s16 item_after, s32 slot);

void world_item_cash_out_excess_inventory(void);
s32 world_item_adjust_inventory_count(s16 item_id, s32 delta);

void world_item_combine_stat_details(world_item_stat_detail_t* output, world_item_stat_detail_t* before,
    world_item_stat_detail_t* after, s32 multiplier);

s32 world_item_count_owned_and_equipped(s16 item_id);
s32 world_item_count_selected_equipped_by_party(s32 unused_row);
s32 world_item_count_selected_owned_and_equipped(s32 unused_row);
void world_item_draw_weapon_hand_icons(s16 weapon_id);
s32 world_item_sort_list_by_category_order(s32 list_index, s16* entries);
void world_item_store_category_list(s32 list_index, s16* sorted_ids);
s32 world_item_get_equip_candidate_brightness(s32 unit_index);
s32 world_item_get_equip_candidate_marker(s32 unit_index);

/* WORLD menu helpers. Half-price is clamped to one; stock to a byte. */
s32 world_item_get_half_price(s16 item_id);
s32 world_item_get_price(s16 item_id);
s32 world_item_get_ranking_value(s32 item_id);
s32 world_item_get_selected_half_price(s32 unused_row);
s32 world_item_get_type(s32 item_id);
void world_item_get_type_icon_rect(s32 type, world_item_icon_source_t* out);
item_menu_category_e world_item_get_menu_category(s16 item_id);
s32 world_item_is_in_category_list(s32 item_id, s32 list_index);

void world_item_populate_stat_preview(
    s16 item_id, world_item_stat_summary_t* summary, world_item_stat_detail_t* detail, s32 slot);

void world_item_prepend_to_category_list(s32 item_id, s32 list_index);
void world_item_reconcile_sorted_list(s32 list_index);
void world_item_remove_from_category_list(s32 item_id, s32 list_index);
s8 world_item_run_action_menu(void);
s32 world_item_run_category_menu(void);
s32 world_item_run_rearrange_mode(void);
s8 world_item_run_sort_order_menu(void);
world_unit_equipment_slot_e world_item_select_equipment_slot(s16 unit_index, s32 item_id);
s32 world_item_show_units_equipping_selected(void);
void world_item_sort_id_list(s32 mode, s16* list);
void world_item_sum_equipment_stat_details(struct world_item_stat_detail* total, u16* equipment);

/* shop */
/* The shop screen (world_shop_run_screen) and its steps: the town outfitters (buy, sell and the
 * fitting room, where units try on shop items before paying), the fur shop (poached items) and the
 * soldier office (hiring and renaming monsters). */

/* Two-byte shop availability mask, stored most significant byte first: bit
 * 0x8000 >> location id is set when that town's outfitter sells the item. */
typedef struct world_shop_item_availability {
    u8 high;
    u8 low;
} world_shop_item_availability_t;

/* The shop screen's step: the index of its handler in
 * g_world_shop_menu_step_handlers. The ordinary shop starts at MAIN_MENU, the
 * fur shop at FUR_MENU and the soldier office at SOLDIER_OFFICE_MENU. */
typedef enum world_shop_step {
    WORLD_SHOP_STEP_EXIT = -1, /* world_shop_run_screen leaves its loop */
    WORLD_SHOP_STEP_MAIN_MENU = 0,
    WORLD_SHOP_STEP_BUY_LIST = 1,
    WORLD_SHOP_STEP_BUY_CONFIRM = 2,
    WORLD_SHOP_STEP_BUY_EQUIP = 3, /* who can equip the item under the cursor */
    WORLD_SHOP_STEP_SELL_LIST = 4,
    WORLD_SHOP_STEP_SELL_CONFIRM = 5,
    WORLD_SHOP_STEP_FITTING_ROOM = 6, /* unit grid, then the fitting menu */
    WORLD_SHOP_STEP_FITTING_ROOM_EQUIP = 7,
    WORLD_SHOP_STEP_FITTING_ROOM_BEST_FIT = 8,
    WORLD_SHOP_STEP_FITTING_ROOM_REMOVE = 9,
    WORLD_SHOP_STEP_FITTING_ROOM_SELL_EQUIPPED = 10,
    WORLD_SHOP_STEP_FITTING_ROOM_SELL = 11, /* runs the SELL_LIST and SELL_CONFIRM handlers */
    WORLD_SHOP_STEP_FITTING_ROOM_CHECKOUT = 12,
    WORLD_SHOP_STEP_EXIT_FADE = 13,
    WORLD_SHOP_STEP_FUR_MENU = 14,
    WORLD_SHOP_STEP_FUR_BUY_LIST = 15, /* the shop's poached stock */
    WORLD_SHOP_STEP_FUR_BUY_CONFIRM = 16,
    WORLD_SHOP_STEP_FUR_BUY_EQUIP = 17,
    WORLD_SHOP_STEP_FUR_SELL_LIST = 18, /* the party's inventory */
    WORLD_SHOP_STEP_FUR_SELL_CONFIRM = 19,
    WORLD_SHOP_STEP_SOLDIER_OFFICE_MENU = 20,
    WORLD_SHOP_STEP_HIRE_PREVIEW = 21,
    WORLD_SHOP_STEP_HIRE_CONFIRM = 22, /* declining removes the recruit from the roster */
    WORLD_SHOP_STEP_HIRE_NAME_ENTRY = 23,
    WORLD_SHOP_STEP_HIRE_FEE_CONFIRM = 24,
    WORLD_SHOP_STEP_SOLDIER_OFFICE_REOPEN = 25,
    WORLD_SHOP_STEP_RENAME_SELECT_UNIT = 26,
    WORLD_SHOP_STEP_RENAME_NAME_ENTRY = 27,
} world_shop_step_e;

/* A shop step handler (g_world_shop_menu_step_handlers). */
typedef void (*world_shop_menu_handler_t)(void);

/* Result of world_shop_run_fitting_room_checkout_prompt. */
typedef enum world_shop_checkout_result {
    WORLD_SHOP_CHECKOUT_DONE = -1,     /* bought, or left without buying: back to the shop's main menu */
    WORLD_SHOP_CHECKOUT_CANCELLED = 0, /* back to the fitting room */
    WORLD_SHOP_CHECKOUT_PROMPT_OPEN = 1,
} world_shop_checkout_result_e;

/* Refusals of world_shop_run_buy_quantity_menu and
 * world_shop_run_fur_buy_quantity_menu; otherwise they return whether the
 * quantity prompt is still open. */
enum {
    WORLD_SHOP_QUANTITY_HOLDING_MAX = -2, /* the party already holds 99 */
    WORLD_SHOP_QUANTITY_CANNOT_AFFORD = -1,
};

/* Sector and size of the nine background images (three each for the outfitter, the fur shop and
 * the soldier office), read as flat words ([i * 2], [i * 2 + 1]): a record member access changes
 * how the target addresses them. */
extern s32 g_world_shop_background_tim_files[9 * 2];

/* The fitting room's copy of g_world_best_fit_slot_categories (the same five bytes);
 * world_shop_build_fitting_room_best_fit_equipment sets its slot 1. */
extern u8 g_world_shop_best_fit_slot_categories[5];

/* Per formation unit and slot: the item the unit owns, set aside while it tries on a shop item. */
extern s16 g_world_shop_fitting_room_items[][5];
extern u8 g_world_shop_hire_menu_initialized;
extern u8 g_world_shop_item_category;
extern world_shop_item_availability_t g_world_shop_item_availability[];
extern world_shop_menu_handler_t g_world_shop_menu_step_handlers[]; /* one per world_shop_step_e */
extern s16 g_world_shop_menu_step;                                  /* world_shop_step_e */
extern s16 g_world_shop_quantity_limit;
extern world_menu_thread_data_t g_world_shop_quantity_menu_data;
extern u8 g_world_shop_background_visible;
extern world_menu_entry_t g_world_shop_main_menu_thread_data;
extern world_menu_entry_t g_world_shop_fur_menu_thread_data;

/* The cursor doubles as the soldier-office mode (world_shop_get_soldier_office_entry_fee). */
extern world_menu_entry_t g_world_shop_hire_menu_thread_data;
extern RECT g_world_shop_background_rect;
extern RECT g_world_shop_backdrop_tile_rect;
extern RECT g_world_shop_edge_tile_rect;
extern RECT g_world_shop_background_vram_rect;
extern RECT g_world_shop_vram_backup_rect;
extern u8 g_world_shop_main_menu_running;     /* menu open flag */
extern u8 g_world_shop_exit_fade_started;     /* fade-out started */
extern u8 g_world_shop_exit_step_initialized; /* state initialised */
extern u8 g_world_shop_gil_menu_script[];
extern s16 g_world_shop_purchase_menu_script[];
extern s8 g_world_shop_purchase_list_initialized;
extern s8 g_world_shop_purchase_equip_check_pending;
extern s16 g_world_shop_purchase_message_wait;
extern u8 g_world_shop_quantity_menu_running;
extern u8 g_world_shop_equip_candidate_initialized;
extern u8 g_world_shop_equip_candidate_preview_visible;
extern u8 g_world_shop_sell_list_menu_script[];
extern s8 g_world_shop_sell_list_initialized;
extern s16 g_world_shop_sell_message_wait;
extern u8 g_world_shop_sell_result; /* sell-menu thread result */
extern u8 g_world_shop_fitting_room_initialized;

/* Two-entry menu chain: row 3 of [0] opens [1] through [0]'s parent-index
 * table at 0x80195518. */
extern world_menu_entry_t g_world_shop_fitting_room_menu_thread_data[2];
extern u8 g_world_shop_fitting_item_list_menu_script[];
extern s8 g_world_shop_fitting_equip_menu_initialized;
extern s8 g_world_shop_fitting_equip_slot;
extern s8 g_world_shop_fitting_item_list_close;
extern u16 g_world_shop_fitting_slot_cursor_anim;
extern point16_t g_world_shop_fitting_slot_cursor_point;
extern u8 g_world_shop_fitting_cost_menu_script[];
extern s8 g_world_shop_equip_slot_preview_initialized;
extern u16 g_world_shop_equip_slot_cursor_anim;
extern point16_t g_world_shop_equip_slot_cursor_point;
extern world_menu_entry_t g_world_shop_checkout_confirm_menu;
extern world_menu_entry_t g_world_shop_checkout_quit_menu;
extern u8 g_world_shop_checkout_prompt_initialized;

/* The best-fit confirmation menu entry, followed directly by its open/closed
 * gate byte, as g_world_shop_sell_equipped_menu / _open are paired. */
extern world_menu_entry_t g_world_shop_best_fit_menu;
extern s8 g_world_shop_best_fit_menu_open;
extern world_menu_entry_t g_world_shop_sell_equipped_menu;
extern u8 g_world_shop_sell_equipped_menu_open;
extern u16 g_world_shop_sell_equipped_cursor_anim;
extern point16_t g_world_shop_sell_equipped_cursor_point;
extern u8 g_world_shop_sell_subflow_initialized;
extern u8 g_world_shop_fur_opened; /* fur shop opened */
extern s16 g_world_shop_sale_confirm_menu_script[];
extern s8 g_world_shop_fur_sell_list_initialized;
extern s16 g_world_shop_fur_sale_message_wait;
extern u8 g_world_shop_fur_quantity_menu_running; /* sell-menu thread result */
extern s16 g_world_shop_fur_purchase_menu_script[];
extern s8 g_world_shop_fur_purchase_list_initialized;
extern s8 g_world_shop_fur_equip_check_pending;
extern s16 g_world_shop_fur_purchase_message_wait;
extern u8 g_world_shop_fur_purchase_result; /* quantity-menu thread result */
extern world_menu_entry_t g_world_shop_hire_confirm_menu;
extern u8 g_world_shop_hire_confirm_message_wait;
extern world_menu_entry_t g_world_shop_hire_fee_confirm_menu;
extern u8 g_world_shop_recruit_preview_initialized;
extern u8 g_world_shop_hire_name_entry_initialized;
extern u8 g_world_shop_hire_fee_message_wait;
extern u8 g_world_shop_rename_prompt_initialized;
extern u8 g_world_shop_rename_name_entry_initialized;
extern u8 g_world_shop_soldier_office_fee_menu_script[];
extern u8 g_world_shop_roster_count_menu_script[];
extern s32 g_world_shop_purchase_unit_price;
extern s16 g_world_shop_purchase_item;
extern u8 g_world_shop_purchase_total_shown;
extern s16 g_world_shop_equip_candidate_last_unit;
extern u8 g_world_shop_equip_candidate_saved_browse;
extern s16 g_world_shop_equip_candidate_item;
extern u16 g_world_shop_sell_item;         /* item being sold */
extern u8 g_world_shop_sell_confirm_shown; /* confirmation prompt shown */
extern s32 g_world_shop_fitting_room_cost;
extern u8 g_world_shop_fitting_room_unit_chosen;
extern s8 g_world_shop_fitting_room_item_list_open;
extern s16 g_world_shop_fitting_room_previewed_cursor;

/* Last equipment slot whose stat preview was built, -1 when none. The
 * ATTACK overlay has an unrelated halfword at this same address. */
extern s8 g_world_shop_previewed_equip_slot;
extern s16 g_world_shop_previewed_unit_index;
extern u8 g_world_shop_checkout_complete;
extern u8 g_world_shop_checkout_insufficient_gil;
extern u8 g_world_shop_checkout_prompt_active;
extern u8 g_world_shop_checkout_prompt_result;
extern world_menu_entry_t* g_world_shop_checkout_prompt_menu;
extern u8 g_world_shop_sell_equipped_prompt_active;
extern u8 g_world_shop_sell_equipped_slot;
extern u8 g_world_shop_sell_equipped_result_shown;
extern s16 g_world_shop_sell_subflow_step;         /* world_shop_step_e: SELL_LIST or SELL_CONFIRM */
extern u8 g_world_shop_fur_waiting_message;        /* waiting for message dismissal */
extern u8 g_world_shop_fur_has_secret_hunt;        /* any unit has Secret Hunt */
extern u8 g_world_shop_fur_has_poached_stock;      /* any poached item in stock */
extern u16 g_world_shop_sell_quantity_item;        /* item being sold */
extern u8 g_world_shop_sell_total_shown;           /* confirmation prompt shown */
extern s32 g_world_shop_fur_purchase_price;        /* half the item price */
extern u16 g_world_shop_fur_purchase_item;         /* item being bought */
extern u8 g_world_shop_fur_purchase_confirm_shown; /* confirmation prompt shown */
extern s8 g_world_shop_fur_access_granted;
extern u8 D_801CD0EC; /* zeroed when the Soldier Office hire menu opens; never read */
extern u8 g_world_shop_hire_result_shown;
extern u8 D_801CD0F4; /* zeroed when the Soldier Office hire menu opens; never read */
extern u8 g_world_shop_rename_error_shown;
extern u8 g_world_shop_party_unit_count;
extern const u8* g_world_shop_entered_unit_name;
extern u32 g_world_shop_service_fee;
extern s16 g_world_shop_id;
extern u8 g_world_shop_cost_window_visible;
s32 world_shop_add_fitting_room_cost(s32 delta);
s32 world_shop_add_gil(s32 delta);
s32 world_shop_adjust_poached_item_quantity(s16 item_id, s32 delta);
void world_shop_build_fitting_room_best_fit_equipment(s16 unit_index, s16 shop_id, s16* out);
s32 world_shop_build_item_list(s16 unit_id, s16 shop_id, s16 category, s16* entries, s32 mode);
void world_shop_buy_from_fitting_room(void);
s32 world_shop_fitting_room_get_unit_marker(s32 unused_unit_index);
s32 world_shop_get_gil_minus_fitting_room_cost(s32 unused_row);
s32 world_shop_get_selected_poached_item_quantity(s32 unused_row);
s32 world_shop_get_soldier_office_entry_fee(void);
u32* world_shop_load_background_tim(s32 index);
s32 world_shop_rename_unit_get_unit_marker(s32 unused_unit_index);
void world_shop_return_unit_equip_to_fitting_room(s16 unit_index, s16 equipment_index);
s32 world_shop_run_buy_quantity_menu(void);
void world_shop_run_equip_candidate_step(void);
void world_shop_run_fitting_cost_menu_script(void);
s32 world_shop_run_fitting_room_checkout_prompt(void);
s32 world_shop_run_fur_buy_quantity_menu(void);
s32 world_shop_run_fur_sell_quantity_menu(void);
void world_shop_run_gil_menu_script(void);
void world_shop_run_screen(s32 shop_type);
void world_shop_run_sell_confirm_step(void);
void world_shop_run_sell_list_step(void);
s32 world_shop_run_sell_quantity_menu(void);
void world_shop_run_soldier_office_fee_menu_script(void);
void world_shop_take_off_tried_on_item(s16 formation_index, s16 equipment_index);
void world_shop_undo_fitting_room(void);
void world_shop_update_unit_selection(void);

/* card */
/* Partial card-file header: WORLD 0x8013300c puts byte +3 into the upper
 * half of open's creation mode. The remaining header is not modeled. */
typedef struct world_card_file_header {
    u8 _unused_00[3];
    u8 allocation_blocks;
} world_card_file_header_t;

/* Step of the WORLD memory card screen (world_card_run_menu_screen). */
typedef enum world_card_menu_step {
    WORLD_CARD_MENU_STEP_SELECT_CARD = 0, /* memory card slot 1 or 2 */
    WORLD_CARD_MENU_STEP_SELECT_MODE = 1, /* save or load */
    WORLD_CARD_MENU_STEP_SCANNING = 2,
    WORLD_CARD_MENU_STEP_SELECT_SAVE = 3, /* the save slot list takes input */
    WORLD_CARD_MENU_STEP_LOADING = 4,
    WORLD_CARD_MENU_STEP_SAVING = 5,
    WORLD_CARD_MENU_STEP_LEAVE = 9,   /* starts the fade out */
    WORLD_CARD_MENU_STEP_LEFT = 0xff, /* no step runs while the screen fades */
} world_card_menu_step_e;

extern card_save_buffer_t* g_world_load_work_buffer;
extern u8 g_world_card_info_pending;
extern s16 g_world_card_list_scroll_velocity;
extern u8 g_world_card_menu_step; /* world_card_menu_step_e */
extern s32 g_world_card_open_descriptor;
extern card_save_icon_t g_world_card_save_icon_records[15];
extern u8 g_world_card_save_title_template[0x16];
extern u8 g_world_card_selected_slot;
extern u8 g_world_card_slot_cursor_row;
extern s16 g_world_card_menu_request_state; /* world_screen_request_state_e */
extern void* g_world_card_save_file_names[];

/* Encoded captions addressed directly by world_card_build_save_slot_description,
 * immediately after the 15 save-file pointers at g_world_card_save_file_names. */
extern u8 g_world_card_slot_text_table[];
extern s16 g_world_card_slot_select_timer;
extern RECT g_world_card_screen_background_tile;
extern world_menu_thread_data_t g_world_card_slot_select_menu;
extern u8 g_world_card_slot_select_running;

/* Menu-thread descriptors of the same 0x3c-spaced family as g_world_card_saving_message
 * below; world_card_run_mode_select_step assigns each into g_world_card_pending_message. */
extern world_menu_thread_data_t g_world_card_save_unavailable_message[];
extern world_menu_thread_data_t g_world_card_no_save_data_message[];
extern world_menu_entry_t g_world_card_mode_select_menu;
extern u8 g_world_card_mode_select_running;

/* A pointer cell, not a record: cleared to 0, tested, and assigned
 * g_world_card_save_unavailable_message / g_world_card_no_save_data_message by world_card_run_mode_select_step. */
extern world_menu_thread_data_t* g_world_card_pending_message;
extern world_menu_thread_data_t g_world_card_saving_message[];
extern world_menu_thread_data_t g_world_card_loading_message[];
extern world_menu_thread_data_t g_world_card_checking_message[];
extern world_menu_thread_data_t g_world_card_load_error_message[];
extern world_menu_thread_data_t g_world_card_save_error_message[];
extern u8 g_world_card_load_step_state;
extern u8 g_world_card_save_step_state;
extern world_menu_thread_data_t g_world_card_format_prompt[];
extern world_menu_thread_data_t g_world_card_formatting_message[];
extern world_menu_thread_data_t g_world_card_access_error_message[];
extern s8 g_world_card_slot_scan_active;
extern s8 g_world_card_format_delay_counter;
extern s8 g_world_card_error_message_closing;
extern world_menu_window_command_t g_world_card_slot_window_command;
extern world_menu_window_rect_source_t g_world_card_slot_window;
extern s16 g_world_card_slot_description_row_ids[];
extern world_draw_number_command_t g_world_card_slot_number_command;
extern world_draw_number_command_t g_world_card_playtime_hours_command;
extern world_draw_number_command_t g_world_card_playtime_minutes_command;
extern world_draw_number_command_t g_world_card_playtime_seconds_command;
extern point16_t g_world_card_slot_cursor_point;
extern u16 g_world_card_slot_cursor_anim;
extern u8 g_world_card_slot_list_input_ready;
extern u8 g_world_card_overwrite_prompt_running;
extern u8 g_world_card_load_prompt_running;
extern world_oriented_quad_t g_world_card_slot_quad_0;
extern world_oriented_quad_t g_world_card_slot_quad_1;
extern world_oriented_quad_t g_world_card_slot_quad_2;
extern world_oriented_quad_t g_world_card_slot_quad_3;
extern world_oriented_quad_t g_world_card_slot_quad_4;
extern u8 g_world_card_load_failed;
extern u8 D_801CC7D0; /* zeroed with g_world_card_load_failed when a load starts; never read */
extern u8 g_world_card_save_failed;
extern u8 D_801CC7D8; /* zeroed with g_world_card_save_failed when a save starts; never read */
extern s8 g_world_card_probe_result;
extern s8 g_world_card_scan_prompt_active;
extern s8 g_world_card_slot_scan_index;
extern s8 g_world_card_free_block_count;
extern s8 g_world_card_save_file_count;
extern s8 g_world_card_scan_start_frame;
extern u8 g_world_card_save_mode;
extern u8 g_world_card_menu_result;
extern s16 g_world_card_list_scroll_y;
extern s16 g_world_card_mode_select_enabled;
extern s16 g_world_card_active_slot;
extern s16 g_world_card_list_first_visible_row;
extern u8* g_world_card_directory_buffer;
extern s8 g_world_card_selectable_slot_count;
extern u8 g_world_card_save_slot_descriptions[][0x8C];
s32 world_card_build_save_slot_description(s32 mode, u8* slot_graphic);
s32 world_card_read_file_chunked(const char* filename, u8* destination, u32 size);
void world_card_build_save_file_header(s32 slot, s32 level, struct card_save_header* header);
s32 world_card_check_selected(void);
void world_card_clear_events(void);
s32 world_card_close_file_with_retries(s32 descriptor);
void world_card_consume_bios_events(void);
void world_card_consume_hardware_events(void);
s32 world_card_count_free_blocks(struct DIRENTRY* entries, s32 count);
s32 world_card_count_save_files(struct DIRENTRY* dir, s32 all);
s32 world_card_count_selectable_save_slots(void);
s32 world_card_create_new_with_retries(s32 port, s32 retry_limit);
s32 world_card_delete_file(char* name);
void world_card_draw_slot_text(s32 slot);
s32 world_card_format_selected_slot(void);
s32 world_card_get_selected_slot(void);
void world_card_init_menu_state(void);
s32 world_card_load_globals_from_save_image(s32 partial_load);
s32 world_card_load_with_retries(s32 channel, s32 attempts);
s32 world_card_open_file_with_retries(const char* filename, s32 mode);
s32 world_card_poll_bios_event(void);
s32 world_card_poll_hardware_event(void);
s32 world_card_poll_info_with_retries(s32 channel, s32 count);
s32 world_card_probe_selected(void);
s32 world_card_read_file_with_retries(s32 descriptor, void* destination, s32 size);
void world_card_run_load_step(s32 slot);
s32 world_card_run_menu_screen(s32 mode_select_enabled);
void world_card_run_mode_select_step(void);
void world_card_run_save_step(s32 slot);
void world_card_run_slot_select_step(void);
void world_card_scroll_menu_list(s32 row);
void world_card_set_selected_slot(s32 slot);
void world_card_update_and_draw_slot_list(s32 input);
void world_card_update_menu_request(void);
void world_card_update_save_slot_playtime(s32 slot);
void world_card_update_slot_scan(void);
s32 world_card_wait_for_bios_event(void);
s32 world_card_wait_for_hardware_event(void);
s32 world_card_wait_for_selected_status(s32 attempts);
s32 world_card_write_file_with_retries(s32 descriptor, const void* source, s32 size);
void world_card_build_save_image(s32 slot);
s32 world_card_seek_file_with_retries(s32 descriptor, s32 offset, s32 origin);
s32 world_card_write_buffer_to_file(const char* filename, const u8* source, u32 size, s32 create_file);

/* script */
extern s32 g_world_script_acting_unit_id;
extern s32 g_world_script_loaded_event_id;
extern u32 g_world_script_tutorial_button_input;
extern s32 g_world_script_tutorial_command_active;
extern u8 g_world_script_tutorial_id;
extern u16 g_world_script_tutorial_shift_command_bit;

extern s16 g_world_script_tutorial_wait_time_remaining;
extern s16 g_world_script_tutorial_wait_timer;
/* g_main_script_variables: a variable id below EVENT_SCRIPT_VAR_PACKED_BIT_FIRST
 * (event_script_variable_id_e) is its word index. */
extern s32* g_world_script_variables;
extern char g_world_script_pause_message[];

extern u16 g_world_script_attack_entry_mode; /* script_attack_entry_mode_e */
extern s16 g_world_script_debug_paused;

extern u8 g_world_scenario_condition_arg_counts[]; /* instruction length by opcode */
extern u16 g_world_script_tutorial_latched_button;
extern u8 g_world_script_tutorial_activation_timer;
extern s16 g_world_script_tutorial_buttons;
extern s32 g_world_script_saved_event_input;

extern world_unit_coordinates_t g_world_script_warp_unit_coords;

extern u32 g_world_script_tutorial_repeat_counters[16];
extern u16 g_world_script_tutorial_previous_input;
extern u8* g_world_script_tutorial_command_ptr;

s32 world_script_check_scenario_condition(s32 type, s32 id, s32 value, s32 tile_y, s32 elevation);
void world_script_handle_tutorial_command_highlight_ring(void);
s32 world_script_run_scenario_conditions(void);
void world_script_run_sprite_move(u8*, s32);
void world_script_set_specialized_map_destroyed(void);
void world_menu_init_subsystem_pointers(void);
s32 world_script_filter_unit_id_by_mode(u16* out_id, void* in_id, s32* mode);
u8* world_text_advance_cursor_with_separate_backreference_state(s32* count, u8* cursor, u8** saved);
void world_script_advance_date_by_one_year(void);
void world_script_advance_tutorial_highlight_brightness(void);
void world_script_blue_remove_all_enemy_units(void);
void world_script_blue_remove_unit(s32 unit_id);
void world_script_change_stats(const u8* parameters);
void world_camera_clear_zoom_variable_low_bit(void);
void world_script_color_unit_event_instruction(const u8* parameters);
void world_script_copy_32_bytes(void* destination, const void* source);
void world_script_copy_bytes(void* destination, const void* source, s32 count);
void world_script_earthquake_start(void);
void world_script_toggle_message_portrait_flip(const u8* parameters);
void world_script_apply_relative_camera(u8* source, const s32* deltas);
void world_script_face_tile(const u8* parameters);
void world_script_face_unit(const u8* parameters, s32 second_only);
void world_script_focus(u8* parameters);
void world_script_focus_speed(u8* parameters, s32* position, s32* rotation);
s32 world_script_get_current_scenario_finish_operation(void);

/* Event variable read via the 0xb1 instruction; 0x22 is rebuilt from 0x23/0x24. */
s32 world_script_get_variable(s32 variable_id);
s32 world_script_handle_tutorial_command(void);
void world_script_handle_tutorial_command_change_dialog(void);
void world_script_handle_tutorial_command_display_message(void);
s32 world_script_handle_tutorial_command_end(void);
void world_script_handle_tutorial_command_shift(void);
void world_script_handle_tutorial_command_wait(void);
void world_script_handle_tutorial_command_wait_for_button(void);
void world_script_handle_tutorial_command_wait_for_message(void);
void world_script_handle_tutorial_command_wait_time(void);
s32 world_script_interpolate_range_fixed12(s32 start, s32 end, s32 fraction, s32 value);
void world_script_load_attack_graphics_event_instruction(void);
void world_script_load_event(s32 id);
s32 world_script_load_next_event(void);
void world_script_map_darkness(void);
void world_script_march_units(const u8* parameters);
void world_script_play_effect_and_wait(void);
void world_script_print_debug_message(void);
void world_script_pulse_tutorial_wait_value(s32 value);
void world_script_unit_animation_rotate_event_instruction(const u8* parameters);
s32 world_script_run_frame(u32* ot, u32 buttons);
void world_script_run_next_event(void);
void world_script_seed_event_rng_from_vsync(void);
void world_script_set_event_speed(s32 speed);
void world_script_set_variable(s32 variable_id, s32 value);
void world_script_set_vsync_mode_and_event_speed(s32 value);
void world_script_show_graphic(void);
void world_script_sprite_move(void);
void world_script_sprite_move_beta(void);
void world_script_start_current_event_thread(void);
void world_script_start_tutorial(s32 index);
void world_script_store_halfword(u8* destination, s16 value);
void world_input_update_event_state(u32* otag, u32 input, s32 frame_arg);
void world_gfx_ramp_screen_overlay_intensity_to_zero(void);
void world_script_teleportout_event_instruction(s32 unit_id, s32 remove_unit);
void world_script_unit_anim(const u8* parameters);
void world_script_update_tutorial_controller_input(void);
void world_script_wait_for_unit_ready(s32 misc_id);
void world_script_wait_value_event_instruction(const u8* parameters);
void world_script_walkto_event_instruction(const struct event_walk_to_parameters* parameters);
s32 world_script_warp_unit(void* instruction);
s32 world_script_warp_unit_display_to_paired_unit(s32);
void world_script_set_units_movement_effect_suppression(const u8* parameters);
void world_script_set_units_palette_update_suppression(u16 id, s32 enable);
s32 world_script_get_variable_bit_position(s32 variable_id);
void world_script_add_unit_start_thread(void);
s32 world_script_check_tutorial_event_slot(void);
void world_script_color_screen_thread(void);
void world_script_dismiss_unit_event_instruction(s32);
void world_script_execute_display_conditions_instruction(void);
void world_script_execute_event(void);
s32 world_script_find_instruction_byte_offset(s32 offset, s32 instruction);
s32 world_script_find_jump_target(s32, s32, s32, s32);
u32 world_script_get_rand16(void);
s32* world_script_get_variable_word_pointer_from_id(s32 variable_id);
void world_script_inflict_status_thread(void);
s16 world_script_load_halfword(const u8* source);
void world_script_load_portrait_colors_event_instruction(s32);
void world_script_mirrorsprite_event_instruction(const u8* parameters);
void world_script_pause_event_instruction(void);
void world_script_play_effect_thread(void);
void world_script_run_condition(event_opcode_e opcode);
void world_script_run_variable_command(s32 opcode, s32 destination_id, s32 source, s32 unused);
void world_script_teleportin_event_instruction(s32 unit_id, s32 unused);
void world_script_unlockdate_event_instruction(u32* date_bits, s32 index, s32 month, s32 day);
void world_script_waitrotateunit_and_waitrotateall_event_instruction(s32);
void world_script_waitspritemove_event_instruction(s32);
void world_script_waitwalk_event_instruction(s32 unit_id);

/* event */
/* Operand pairs that one world_script_execute_event opcode stores together;
 * each second halfword sits off a word boundary in WORLD bss, so each pair is
 * one object. */
typedef struct world_event_command_args {
    s16 arg1;
    s16 arg2;
} world_event_command_args_t;

typedef struct world_event_music_switch {
    s16 volume;
    s16 fade;
} world_event_music_switch_t;

extern u32 g_world_event_current_input;
extern u32 g_world_event_initial_repeat_counter;
extern u32 g_world_event_previous_input;
extern u32 g_world_event_repeat_counter;
extern u32 g_world_event_secondary_repeat_counter;
extern s32 g_world_event_speed;

extern s16 g_world_event_effect_target_x;
extern s16 g_world_event_effect_target_y;

extern s16 g_world_event_map_jump_in_request;
extern s16 g_world_event_finish_check_pending;
extern u8 g_world_event_thread_status_snapshot[]; /* per-thread running flags sampled before the yield, indexed 1..15 */

extern u32 g_world_event_random_seed;

extern void* g_world_event_pending_loaded_evtchr_slot;
extern void* g_world_event_loaded_evtchr_buffer;
extern s16 g_world_event_effect_target_misc_id;
extern s16 g_world_event_effect_target_mode;
extern u8 g_world_event_instruction_sizes[0xf4];
extern world_event_command_args_t g_world_event_map_command_80_args;
extern world_event_command_args_t g_world_event_map_command_83_args;
extern world_event_music_switch_t g_world_event_music_switch;
extern s16 g_world_event_pending_effect_id;
extern s32 g_world_event_pending_evtchr_save_slot;
extern s32 g_world_event_pending_loaded_evtchr_clear_slot;
extern s16 g_world_event_pending_map_jump_out_2_id;
extern s16 g_world_event_pending_map_jump_out_id;
extern s16 g_world_event_pending_map_state;
extern s32 g_world_event_pending_reserved_vram_release_slot;
extern s32 g_world_event_pending_saved_evtchr_clear_slot;
extern s32 g_world_event_pending_unit_vram_copy;
extern const u8* g_world_event_script;
extern s16 g_world_event_unit_slots[4];
extern u8 g_world_event_variable_write_guard; /* WORLD counterpart: 0x80153387 */
extern s16 g_world_event_weather_request;
extern map_background_gradient_colors_t g_world_event_background_colors;

/* other */
extern s16 g_world_field_object_use_request;
extern u16 g_world_field_object_wait_status;
extern u16 g_world_frame_arg;
extern s32 g_world_frame_counter;
extern s8 g_world_cursor_bob_thresholds[];        /* (threshold, value) pairs, 0-terminated; entry 0 is the modulus */
extern s8 g_world_cursor_bob_active_thresholds[]; /* (threshold, value) pairs, 0-terminated */
extern u16 g_world_saved_animation_speed;         /* saved animation speed */
extern u16 g_world_animation_speed_forced;        /* animation speed override active */
extern u8 g_world_rotation_speed_frames[];
extern u8 g_world_location_names_resident[];
extern s16 g_world_debug_variable_list_request_state;
extern world_menu_quad_page_t world_D_80173CBC[2];
extern s32 g_world_preview_stats_window_active;
extern s8 g_world_dismiss_unit_prompt_running;
extern world_menu_entry_t g_world_best_fit_confirm_menu;
extern u8 g_world_order_unit_menu[];
extern s8* g_world_sort_key_lists[];
/* Item-list filter per equipment slot (right hand, left hand, head, body, accessory); Best Fit sets
 * slot 1 to ITEM_LIST_FILTER_WEAPON for a Two Swords user, else ITEM_LIST_FILTER_SHIELD. */
extern u8 g_world_best_fit_slot_categories[5];
extern game_options_t g_world_saved_game_options;
extern s32 g_world_finish_operation_event_id;
extern u8 g_world_order_unit_saved_browse_enabled;
extern u8 g_world_action_menu_skillsets[];
extern u16 g_world_frame_result_override;
extern s32 g_world_fixed_math_overflow; /* set by world_cross_product_q12 on Q12 overflow */
s32 world_cross_product_q12(s32 a, s32 b, s32 c, s32 d);
s32 get_current_facing_byte_from_misc_id(s32 misc_id);
s32 world_coords_fit_in_byte(s32 x, s32 y);
void world_fill_16_bytes_fe(u8* buf);
void world_free_work_buffer(void);

s32 world_ability_list_secondary_skillsets(s16 unit_id, s16* skillsets);
s32 world_is_skillset_in_spell_quote_exception_list(s32 value);
s16 world_menu_update_wrapped_horizontal_selection(u16 count, u8 index, s32 buttons);
void world_script_process_inflict_status_commands(void);
void world_script_rotate_unit(const u8*);
void world_script_block_start_thread(void);

#endif
