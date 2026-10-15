#ifndef EVENT_REQUIRE_H
#define EVENT_REQUIRE_H

/* EVENT/REQUIRE.OUT: the displays of event opcode DisplayConditions (require_condition_dispatch): the
 * BONUS.BIN and special banners, the battle's Congratulations, the gil reward and the war trophies, the
 * permanent Brave and Faith changes, the units that desert, and the units that join or leave the party
 * after a battle; and the burst of event opcode DarkScreen (require_bonus_run_dark_screen_burst). */

#include "fft/battle.h"

/* thread */
/* The thread helpers, the thread slots of the post-battle screens, the saved thread block and the
 * status indicator packets. */

extern battle_menu_status_panel_indicator_prims_t g_require_thread_indicator_packets[2][2];
extern s32 g_require_thread_suspended_id;
void require_thread_exit_current(void);
s32 require_thread_is_running(s32 thread_id);

/* Slots of the post-battle screens' own threads; native_screen_thread_slot_e
 * has the shared ones. */
typedef enum require_thread_slot {
    REQUIRE_THREAD_SLOT_MENU = 5,        /* selection menus, prompts and the special-case display */
    REQUIRE_THREAD_SLOT_MESSAGE = 7,     /* the icon strip, message text or menu shown with them */
    REQUIRE_THREAD_SLOT_TRANSITION = 10, /* require_gfx_run_suspended_thread_transition */
} require_thread_slot_e;

extern u8 g_require_saved_thread_block[];

/* input */
/* The pad, and the waits that Circle cuts short. */

extern u32* g_require_input_controller;

void require_input_wait_frames_or_skip(s32 frames);

/* overlay */
/* Running JOBSTTS.OUT and HELPMENU.OUT over the post-battle screens, at the high overlay address. */

void require_overlay_open_jobstts(void);

void require_overlay_run_helpmenu(void);

/* condition */
/* The DisplayConditions dispatcher and its displays: the special banners, the BONUS.BIN banners and
 * the hovered-unit stats. */

/* One display-condition sprite: texture rectangle and screen offset, shared by
 * the general- and special-case display-condition renderers. */
typedef struct require_display_condition_quad {
    s16 u;      /* 0x00 */
    s16 v;      /* 0x02 */
    s16 width;  /* 0x04 */
    s16 height; /* 0x06 */
    s16 x;      /* 0x08 */
    s16 y;      /* 0x0a */
} require_display_condition_quad_t;

void require_condition_show_bonus_banner(s32 value);
void require_condition_show_special_banner_thread(void);

extern u8 g_require_condition_general_quad_ranges[];
extern require_display_condition_quad_t g_require_condition_general_quads[];
extern u8 g_require_condition_general_vertex_colors[];

/* Per-condition first-quad indices; entry n+1 bounds entry n. */
extern u8 g_require_condition_special_quad_ranges[];
extern require_display_condition_quad_t g_require_condition_special_quads[];

/* 0x14-byte vertex-colour records, one per quad. */
extern u8 g_require_condition_special_vertex_colors[];
void require_condition_show_hovered_unit_stats(void);

/* REQUIRE 0x801cafd4, invoked by both BATTLE and WORLD event dispatch. */
void require_condition_dispatch(void);

/* bonus */
/* BONUS.BIN's images (200 rows of 64 16-bit pixels, then the palette): loading one, and the DarkScreen
 * burst that breaks one into tiles. */

/* BONUS image file loaded by require_bonus_load_image: 200 rows of
 * 64 16-bit pixels, then the palette. */
typedef struct require_bonus_image_file {
    u16 pixels[200][64]; /* 0x0000 */
    u16 palette[1];      /* 0x6400 */
} require_bonus_image_file_t;

/* The mode byte of the bonus-image burst parameters: the distance that sets
 * each tile's start delay. Any other value measures along a rotated x axis. */
typedef enum require_bonus_burst_mode {
    REQUIRE_BONUS_BURST_FROM_CENTRE = 0,
    REQUIRE_BONUS_BURST_FROM_ORIGIN = 1, /* the top-left corner */
    REQUIRE_BONUS_BURST_HORIZONTAL = 2,  /* x only */
} require_bonus_burst_mode_e;

extern RECT g_require_bonus_image_rect;
extern RECT g_require_bonus_palette_rect;

extern DR_MODE g_require_bonus_burst_draw_modes[2];
extern POLY_F4 g_require_bonus_burst_flash_polys[2];
extern MATRIX g_require_bonus_burst_matrix;
extern VECTOR g_require_bonus_burst_offset;
extern VECTOR g_require_bonus_burst_rotated_offset;
extern SVECTOR g_require_bonus_burst_rotation;

void require_bonus_load_image(s32 image_id);
void require_bonus_run_dark_screen_burst(void);

/* reward */
/* The battle's rewards: the war trophies (the items and the bonus money) and their list, and the gil
 * ticker with its rolling digits. */

extern s32 g_require_reward_war_trophy_entry_count;
extern s32 g_require_reward_war_trophy_entry_types[];
extern s32 g_require_reward_war_trophy_entry_values[];
extern u8 g_require_reward_war_trophy_item_count;
extern battle_war_result_t g_require_reward_war_trophy_search_result;

extern RECT g_require_reward_money_clip_rect_0;
extern RECT g_require_reward_money_clip_rect_1;
extern POLY_FT4 g_require_reward_money_comma_polys[2];
extern u32 g_require_reward_money_digit_initial_speeds[6];
extern POLY_FT4 g_require_reward_money_digit_polys[2][6];
extern u32 g_require_reward_money_digit_positions[6];
extern s32 g_require_reward_money_digit_settling[6];
extern u32 g_require_reward_money_digit_speeds[6];
extern s32 g_require_reward_money_digit_targets[6];
extern POLY_FT4 g_require_reward_money_next_digit_polys[2][6];
extern POLY_FT4 g_require_reward_money_sign_polys[2];
extern RECT g_require_reward_screen_clip_rect_0;
extern RECT g_require_reward_screen_clip_rect_1;
extern RECT g_require_reward_war_trophy_clear_rect;

/* Upload rectangle for the current entry's text image. */
extern RECT g_require_reward_war_trophy_image_rect;
void require_reward_collect_war_trophies_and_bonus_money(void);
s32 require_reward_get_war_trophy_column_x(s32 value, s32 index);
s32 require_reward_get_war_trophy_entry_offset(s32 value, s32 index);

/* g_require_reward_war_trophy_entry_types[]: what a war trophy entry's
 * value in g_require_reward_war_trophy_entry_values is. */
typedef enum require_war_trophy_entry {
    REQUIRE_WAR_TROPHY_ENTRY_ITEM = 0,        /* an item id */
    REQUIRE_WAR_TROPHY_ENTRY_BONUS_MONEY = 1, /* the gil amount */
} require_war_trophy_entry_e;

void require_reward_render_gil_ticker(void);
void require_reward_render_war_trophies(void);

/* party */
/* The party after a battle: the permanent Brave and Faith changes and their warnings, the units that
 * desert, the units that join (with the removal menus when the roster is full), their equipment stored or
 * sold, and the bio variables. */

extern battle_active_unit_t g_require_active_unit_data;
extern s32 g_require_party_affected_battle_unit_indices[];
extern s32 g_require_party_affected_unit_count;
extern s32 g_require_party_affected_unit_message_args[];
extern s32 g_require_party_affected_unit_message_ids[];

/* x and y of the palette MoveImage destination (a RECT at 0x801d0d44). Read as two scalar symbols: a RECT view lets
   GCC keep the struct address live in a saved register and changes the allocation. */
extern s16 g_require_party_palette_destination;
extern s16 g_require_party_palette_destination_y;
extern RECT g_require_party_palette_move_rect;
extern s16 g_require_party_portrait_destination;
extern RECT g_require_party_portrait_move_rect;
extern u8 g_require_party_removable_flags[];

/* Six consecutive 0x3c-byte menu records; the target derives entry [4]'s
   address from entry [2], so they are one array. */
extern world_menu_entry_t g_require_party_removal_menus[6];
extern s16 g_require_party_removal_result;
void require_party_apply_permanent_brave_faith_changes(void);
s32 require_party_find_low_brave_high_faith_departures(void);
void require_party_remove_low_brave_high_faith_units(void);
s32 require_party_sell_equipment_over_limit(s32 unit_id);
s32 require_party_store_equipment_or_sell_overflow(s32 unit_id);
void require_party_update_bio_variable_for_unit_class(s32 unit_class, s32 value);

s32 require_party_save_battle_units_and_find_join_candidates(void);
s32 require_party_get_free_slot_status(s32 unit_id);
s32 require_party_restore_item_counts(s32 party_index);

s32 require_party_select_unit_to_remove(void);

extern job_data_t* g_require_formation_job_data;
extern u8 g_require_join_menu_descriptor[];

s32 require_party_apply_permanent_brave_faith_changes_and_collect_warnings(void);
void require_party_apply_post_battle_unit_changes(void);

void require_party_report_brave_departure_stub(
    s32 unused_battle_index, s32 unused_unit_id, s32 unused_character_identity);

/* editor */
/* The status panels' unit: loaded from the party or from the battle, with its numbers and names. */

void require_editor_load_party_unit(s32 unused, s32 party_index);

extern u8 g_require_editor_unit_name[];
extern u8 g_require_editor_secondary_skillset_name[];
extern menu_number_entry_t g_require_editor_numeric_descriptor_a[];
extern menu_number_entry_t g_require_editor_numeric_descriptor_b[];
extern RECT g_require_editor_numeric_entries_a;
extern RECT g_require_editor_numeric_entries_b;
extern RECT g_require_editor_numeric_entries_c;
extern RECT g_require_editor_numeric_entries_d;
extern RECT g_require_editor_numeric_entries_e;
extern RECT g_require_editor_numeric_entries_f;
extern world_image_location_t g_require_editor_numeric_geometry[];
extern battle_menu_status_panel_numeric_buffer_t g_require_editor_numeric_state[2];
extern battle_menu_status_panel_numeric_geometry_t g_require_editor_numeric_table;
extern u8 g_require_editor_numeric_text_a[];
extern u8 g_require_editor_numeric_text_b[];
extern world_gfx_image_load_parameters_t g_require_editor_numeric_texture[];
extern u8 g_require_editor_job_name[];

extern u8 g_require_editor_primary_skillset_name[];
extern battle_menu_status_panel_slot_storage_t g_require_editor_unit_fields;

extern struct battle_stats g_require_editor_party_unit_stats;
void require_editor_load_selected_unit(void);
void require_editor_run_numeric_thread(void);

extern s32 g_require_numeric_editor_redraw_request;
extern s32 g_require_numeric_editor_thread_params;

/* panel */
/* The panels: the unit status banner thread, the character status panel with its frame, the
 * selected and comparison billboards, images, gauges and layouts, and the panel dimming. */

extern RECT g_require_character_status_frame_rect[];
extern RECT g_require_character_status_draw_area_rect;
extern s32 g_require_character_status_frame_config;
extern s32 g_require_character_status_redraw_request;

void require_panel_unit_status_banner_thread(void);

extern battle_unit_status_record_t g_require_panel_comparison_billboard;

extern s32 g_require_panel_dim_a;
extern s32 g_require_panel_dim_b;
extern s32 g_require_panel_dim_c;
extern battle_unit_status_record_t g_require_panel_selected_billboard;
extern u8 g_require_panel_status_animation[];
extern u8 g_require_panel_status_group_bounds[];
extern u8 g_require_panel_status_group_count;
extern u8 g_require_panel_status_group_ids[];
extern main_zodiac_draw_offset_t g_require_panel_status_offsets[];

extern point16_t g_require_panel_status_scales[];
extern main_zodiac_sprite_frame_t g_require_panel_status_uv_rects[];
extern const u16 g_require_panel_status_y_offsets[];
extern world_image_location_t g_require_panel_item_icon_texture[];
extern RECT g_require_panel_origin_offsets[];
extern RECT g_require_panel_text_upload_rect_a;
extern RECT g_require_panel_text_upload_rect_b;
extern world_gfx_image_load_parameters_t g_require_panel_label_layouts_mode0[];
extern world_gfx_image_load_parameters_t g_require_panel_label_layouts_mode1[];
extern world_gfx_image_load_parameters_t g_require_panel_label_layouts_mode2[];
extern world_gfx_image_load_parameters_t g_require_panel_label_layouts_mode3[];
extern world_gfx_image_load_parameters_t g_require_panel_item_icon_layouts[];
extern battle_menu_status_panel_buffer_t g_require_panel_frames_a[];
extern u8 g_require_panel_text_image_a[];
extern u8 g_require_panel_text_image_b[];
extern battle_menu_status_panel_editor_packet_t g_require_panel_comparison_editor_packets[2];
extern u8 g_require_panel_comparison_large_number_image[];
extern u8 g_require_panel_comparison_name_image[];
extern u8 g_require_panel_comparison_number_image[];
extern menu_number_entry_t g_require_panel_comparison_numeric_entries[];
extern battle_menu_status_panel_packet_t g_require_panel_comparison_packets[2];
extern u16 g_require_panel_comparison_portrait_image[];
extern RECT g_require_panel_comparison_portrait_rect;
extern battle_active_unit_t g_require_panel_comparison_unit_data;
extern world_gfx_image_load_parameters_t g_require_panel_editor_label_cells[];
extern u16 g_require_panel_editor_label_cluts[][2];
extern world_gfx_image_load_parameters_t g_require_panel_editor_mode_cell[];
extern world_gfx_image_load_parameters_t g_require_panel_editor_mode_cells[];
extern world_gfx_image_load_parameters_t g_require_panel_editor_value_cells[];
extern u16 g_require_panel_editor_value_cluts[][2];
extern RECT g_require_panel_frame_rect;
extern CVECTOR g_require_panel_gauge_bar_colors[];
extern world_gfx_image_load_parameters_t g_require_panel_portrait_cell[];
extern battle_menu_status_panel_editor_packet_t g_require_panel_selected_editor_packets[2];
extern u8 g_require_panel_selected_large_number_image[];
extern u8 g_require_panel_selected_name_image[];
extern u8 g_require_panel_selected_number_image[];
extern menu_number_entry_t g_require_panel_selected_numeric_entries[];
extern battle_menu_status_panel_packet_t g_require_panel_selected_packets[2];
extern u16 g_require_panel_selected_portrait_image[];
extern RECT g_require_panel_selected_portrait_rect;
extern world_gfx_image_load_parameters_t g_require_panel_sprite_cells[];
extern const world_image_location_t g_require_panel_status_texture;
void require_panel_copy_battle_stats_to_gauges(struct battle_stats* unit, battle_unit_status_record_t* output);

void require_panel_set_primitive_colors(
    battle_menu_status_panel_buffer_t* primitives, const world_status_thread_t* state);

void require_panel_run_character_status_thread(void);
void require_panel_set_transition_value(s32 value);

extern u8 g_require_work[];

extern s32 g_require_status_display_redraw_request;
extern s32 g_require_status_display_thread_params;

/* gfx */
/* Drawing: portraits and the party portrait textures, scaled sprites, frame tiles and lines, the
 * gradient grid, the Gouraud quad arrays, colour fades and draw areas. */

void require_gfx_prepare_party_portrait_textures(void);

extern const u16 g_require_gfx_draw_area_scale_percent[12];
extern RECT g_require_gfx_draw_area_template;
extern s16 g_require_gfx_draw_offset_y;
extern u8 g_require_gfx_fade_rgb[3];
extern u8 g_require_gfx_formation_sprite_groups[][3];
extern POLY_FT4 g_require_gfx_poly_ft4_buffers[2][30];
extern RECT g_require_gfx_portrait_origin;
extern u8 g_require_gfx_portrait_transition_offsets[];
extern u8 g_require_gfx_scaled_sprite_count;
extern point16_t g_require_gfx_scaled_sprite_scale[];
extern main_zodiac_sprite_frame_t g_require_gfx_scaled_sprite_uv_rects[];
extern world_image_location_t g_require_gfx_scaled_sprite_texture_page;
extern u8 g_require_portrait_cell_pixels[];
extern RECT g_require_portrait_cell_rect;
extern u16 g_require_portrait_index_by_sprite_set[];
extern RECT g_require_portrait_palette_rect;
extern RECT g_require_portrait_sheet_rect_0;
extern RECT g_require_portrait_sheet_rect_1;
extern RECT g_require_portrait_sheet_rect_2;
extern RECT g_require_portrait_sheet_rect_3;
extern POLY_GT4 g_require_gfx_poly_gt4_array_32[];
extern POLY_GT4 g_require_gfx_poly_gt4_array_8[];
extern POLY_GT4 g_require_gfx_poly_gt4_buffers[2][40];

void require_gfx_build_gradient_grid_primitives(POLY_GT4* poly);
void require_gfx_build_portrait_poly_ft4(s32 flags, POLY_FT4* output);

void require_gfx_build_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* packet, const RECT* source,
    s32 scale_index, s32 lower_half, const world_status_thread_t* thread);

void require_gfx_build_status_group_primitives(POLY_FT4* poly);
void require_gfx_clip_portrait_poly_from_left(POLY_FT4* poly, s32 amount);
void require_gfx_clip_portrait_poly_from_right(POLY_FT4* poly, s32 amount);
void require_gfx_fade_rgb_31_frames(s32 target_0, s32 target_1, s32 target_2);
void require_gfx_init_menu_tile_and_line_primitives(battle_menu_status_panel_buffer_t* menu);
void require_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* packet);
void require_gfx_set_clut_rect_from_id(RECT* rect, s32 packed);

void require_gfx_set_scaled_poly_ft4_geometry(POLY_FT4* poly, const world_image_location_t* texture_page,
    const RECT* position, main_zodiac_sprite_frame_t* sprite_rect, point16_t* scale, main_zodiac_draw_offset_t* base);

void require_gfx_set_scaled_poly_ft4_geometry_and_uv(POLY_FT4* poly, const world_image_location_t* texture,
    const RECT* position, const main_zodiac_sprite_frame_t* uv_rect, const point16_t* scale,
    const main_zodiac_draw_offset_t* offset);

void require_gfx_set_formation_icon_uv(POLY_FT4* poly, s32 index);

void require_gfx_build_portrait_transition_primitives(const RECT* texture_rect, s32* transition,
    const s32* first_portrait, const s32* second_portrait, u16* image, POLY_FT4* poly, s32 direction);

void require_gfx_init_poly_gt4_array_32(s32 x, s32 y);
void require_gfx_init_poly_gt4_array_8(s32 x, s32 y);
void require_gfx_run_suspended_thread_transition(void);
void require_gfx_scale_poly_gt4_vertex_colors(const u8* vertex_colors, s32 scale, POLY_GT4* poly);

extern RECT g_require_special_portrait_palette_rect;
extern RECT g_require_special_portrait_rect;

/* text */
/* REQUIRE's text file and its sections, decimal numbers in the panels, the Congratulations display and
 * the battle nicknames. */

typedef struct require_glyph {
    s16 source_x;
    s16 source_y;
    s16 width;
    s16 height;
    s16 source_stride;
} require_glyph_t;

/* Section offsets of the REQUIRE text file, relative to g_require_text_data (0x80 bytes before it). */
/* REQUIRE's text, which its post-battle routines bind over the resident
 * sections; it binds its job names as TEXT_SECTION_JOB_NAMES. */
typedef enum require_text_section {
    REQUIRE_TEXT_SECTION_BRAVE_FAITH_WARNINGS = 1,   /* a unit's Brave is low or its Faith high */
    REQUIRE_TEXT_SECTION_BRAVE_FAITH_DEPARTURES = 2, /* the unit leaves the party for it */
    REQUIRE_TEXT_SECTION_DISMISS_UNIT_REPLIES = 3,   /* what a unit turned away or dismissed says */
} require_text_section_e;

/* EVENT/JOIN.LZW, which REQUIRE.OUT carries (require_text_section_e). */
extern s32 g_require_text_section_offsets[32];
extern u8 g_require_text_data[];

extern require_glyph_t g_require_text_decimal_glyph;
void require_text_clear_string_buffer(u8* data);

void require_text_render_decimal_entry_list(
    u8* pixels, menu_number_entry_t* entries, world_glyph_blit_t* output, s32 count);

void require_text_render_decimal_value(s32 value, s32 flags, u8* pixels, world_glyph_blit_t* position);

void require_text_render_signed_decimal_entries(
    u8* pixels, menu_number_entry_t* entries, world_glyph_blit_t* output, s32 count);

void require_text_show_battle_congratulations(void);
void require_text_build_battle_nicknames(void);

extern char g_require_text_wait_for_file_allocation_message[];

/* menu */
/* The menu threads and primitives of the post-battle prompts: icon lists and strips, the message
 * entry, the status indicators and the selection results. */

void require_menu_select_primitive_color_palette(
    world_menu_palette_primitives_t* output, const world_status_thread_t* context);

extern s16 g_require_message_menu_result;

extern s32 g_require_menu_indicator_brightness[2];
extern s32 g_require_menu_indicator_fade_work[2];
extern s32 g_require_menu_indicator_packet_index[2];
extern s32 g_require_menu_indicator_state[2];
extern s16 g_require_menu_selection_result;
void require_menu_icon_strip_thread(void);
void require_menu_wait_selection_threads(void);

/* sound */
/* Waiting for the music load poll. */

void require_sound_wait_for_music_stub(void);

#endif
