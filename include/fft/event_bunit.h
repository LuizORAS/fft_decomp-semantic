#ifndef EVENT_BUNIT_H
#define EVENT_BUNIT_H

/* EVENT/BUNIT.OUT: menu screen overlay that runs with BATTLE. */

#include "fft/battle.h"

/* thread */
/* The screen's menus and panels run as BATTLE threads: the running test, the stop request (third
 * thread parameter 1), the thread-status snapshot and the indicator packets. */

extern u8 g_bunit_thread_status_snapshot[16];
void bunit_thread_request_stop(s32 thread_id);

/* bits */
/* The bit reader for glyph pixels and the unlocked-job bits. */

extern const u8* g_bunit_bits_cursor;
extern s32 g_bunit_bits_index;
extern u8 g_bunit_bits_reader_reset;
void bunit_bits_init_reader(const u8* data);
u32 bunit_bits_read(s32 bit_count);

/* script */
/* BUNIT's handlers of the menu display scripts (menu_script_opcode_e), named after the opcode like
 * WORLD's: bunit_menu_run_display_script runs a script through menu_script_handlers. */

/* The opcode that ends a BUNIT menu display stream. */
enum {
    BUNIT_MENU_SCRIPT_END = 0x1c,
};

extern menu_sprite_command_t g_bunit_menu_script_quad_command;
extern u32 g_bunit_menu_display_script_input;
menu_instruction_t* bunit_menu_script_reregister_temporary_draw_area(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_advance_window_scale_step(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_draw_tiled_rectangle(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_draw_plain_number(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_draw_callback_quad(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_draw_textured_quad(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_add_blend_draw_mode(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_clear_temporary_draw_area(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_draw_scrollable_list(menu_instruction_t* instruction);
void bunit_menu_run_display_script(menu_instruction_t* instruction, s32 flags);
menu_instruction_t* bunit_menu_script_set_draw_area(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_set_draw_priority(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_set_semi_trans(menu_instruction_t* instruction);
menu_instruction_t* bunit_menu_script_draw_opening_window_frame(menu_instruction_t* instruction);

/* cursor */
/* The rhombus cursor of the unit grid: its two sprite halves and its trail. */

/* Oriented-quad descriptors for the two cursor halves. */
extern world_oriented_quad_t g_bunit_cursor_shadow_sprite;

extern world_oriented_quad_t g_bunit_cursor_sprite;

/* Rhombus-cursor trail, oldest point first; x == -1 marks an empty slot
 * (bunit_gfx_init_rhombus_cursor_tpages clears all eight). */
extern point16_t g_bunit_cursor_trail_points[8];

extern u8 g_bunit_cursor_trail_brightness[];

/* character */
/* The character status panel (screen thread slot CHARACTER_STATUS,
 * bunit_menu_equipment_and_ability_panel_thread): the selected unit's equipment and abilities. */

extern wldcore_menu_status_thread_t g_bunit_character_status_frame_config;
extern s32 g_bunit_character_status_redraw_request;
extern s32 g_bunit_character_status_dimmed;
extern u8 g_bunit_character_status_ability_text_image[];
extern RECT g_bunit_character_status_ability_text_rect;
extern RECT g_bunit_character_status_draw_area_rect;
extern u8 g_bunit_character_status_equipment_text_image[];
extern RECT g_bunit_character_status_equipment_text_rect;
extern RECT g_bunit_character_status_frame_rect;
extern battle_menu_status_panel_buffer_t g_bunit_character_status_frames[2];
extern image_load_parameters_t g_bunit_character_status_item_icon_layout[5];
extern image_load_parameters_t g_bunit_character_status_layout_mode0[19];
extern image_load_parameters_t g_bunit_character_status_layout_mode1[19];
extern image_load_parameters_t g_bunit_character_status_layout_mode2[19];
extern image_load_parameters_t g_bunit_character_status_layout_mode3[19];
extern RECT g_bunit_character_status_sprite_origin;

/* ability */
/* The job page's ability list: the abilities a job lists (bunit_ability_build_list), its row
 * conditions and values (tab, learned, CT, MP and JP cost) and its menu. */

extern menu_instruction_t g_bunit_ability_list_menu_script;
extern s8 g_bunit_ability_list_confirm_prompt_state;
extern s16 g_bunit_ability_list_job_name_row[];
extern u8 g_bunit_ability_list_ready;
s32 bunit_ability_build_list(s16 unit_index, s16 job_id, s32 ability_category, s16* ability_list, s32 mode);
s32 bunit_ability_run_list_menu(void);

/* job */
/* The job list of BUNIT's unit screen: a unit's selectable jobs and the numbers its rows show (job
 * level, JP, total JP, JP to the next level, mastered), which the job list caches for the cursor job
 * and the ability list reads back. */

extern u32 g_bunit_job_current_jp;
extern s32 g_bunit_job_current_mastered;
extern u32 g_bunit_job_current_total_jp;
extern u8 g_bunit_job_list_initialized;
extern s8 g_bunit_job_menu_active;
extern s8 g_bunit_job_menu_phase; /* menu_job_list_step_e */
extern s16 g_bunit_job_selected_generic_index;
extern s16 g_bunit_job_selected_id;
extern menu_instruction_t g_bunit_job_list_menu_script;
s32 bunit_job_build_unit_job_list(s32 unit_id, s16* job_ids, s32 unused_2);
s32 bunit_job_calculate_current_jp(s32 index);
s32 bunit_job_calculate_current_level(s32 index);
s32 bunit_job_calculate_current_mastered(s32 index);
s32 bunit_job_calculate_current_total_jp(s32 index);
s32 bunit_job_get_base(s16 unit_id);
s32 bunit_job_get_current_jp(s32 unused_row);
s32 bunit_job_get_current_total_jp(s32 unused_row);
s32 bunit_job_is_current_mastered(s32 unused_row);

/* status */
/* The status banner (bunit_panel_unit_status_banner_thread in screen thread slot UNIT_STATUS): the selected
 * unit's portrait and gauges at the top or the bottom of the screen, slid between the two. */

extern u8 g_bunit_status_banner_enabled;
extern u32 g_bunit_status_display_thread_params;
extern s32 g_bunit_status_display_offset_y;
extern u32 g_bunit_status_display_flags; /* menu_status_panel_flags_e */
extern s32 g_bunit_status_display_redraw_request;
extern s32 g_bunit_status_display_dimmed;
extern u8 g_bunit_status_banner_at_bottom;
extern u8 g_bunit_status_banner_at_bottom_latch;
extern s8 g_bunit_status_banner_slide_step;

/* comparison */
/* The second banner (slot COMPARISON_STATUS) that slides out with the previous unit while the browser changes unit. */

extern u32 g_bunit_comparison_display_thread_params;
extern s32 g_bunit_comparison_display_redraw_request;
extern s32 g_bunit_comparison_display_flags;
extern s32 g_bunit_comparison_display_offset_y;

/* editor */
/* The numeric panel (bunit_menu_run_numeric_editor_thread, screen thread slot NUMERIC_EDITOR): the
 * selected unit's numbers, drawn with the numeric editor code that DEBUGCHR shares. */

/* Copy of the selected unit's 0x40 bytes from +0x30; only the leading
 * slot-state fields are read. */
extern world_item_stat_detail_t g_bunit_editor_unit_fields;

extern point16_t g_bunit_editor_numeric_geometry[];
extern battle_menu_status_panel_numeric_geometry_t g_bunit_editor_numeric_table;
extern RECT g_bunit_editor_numeric_entries_a;
extern RECT g_bunit_editor_numeric_entries_b;
extern RECT g_bunit_editor_numeric_entries_c;
extern RECT g_bunit_editor_numeric_entries_d;
extern RECT g_bunit_editor_numeric_entries_e;
extern RECT g_bunit_editor_numeric_entries_f;
extern image_load_parameters_t g_bunit_editor_numeric_texture[];
extern menu_number_entry_t g_bunit_editor_numeric_descriptor_a[];
extern menu_number_entry_t g_bunit_editor_numeric_descriptor_b[];
extern battle_menu_status_panel_numeric_buffer_t g_bunit_editor_numeric_state_a[2];
extern battle_menu_status_panel_numeric_buffer_t g_bunit_editor_numeric_state_b[2];
extern menu_unit_stat_number_images_t g_bunit_editor_numeric_text_a;
extern menu_unit_stat_number_images_t g_bunit_editor_numeric_text_b;

/* help */
/* The help cursor that Select starts: the navigation nodes of the three layouts (unit browser, banner
 * at the top, banner at the bottom) and the help text table. */

extern u32 g_bunit_help_text_table[];
extern u8 g_bunit_help_text_state;
extern u16 g_bunit_help_cursor_anim_state;
extern u8 g_bunit_help_cursor_initialized;

/* g_bunit_help_nodes_unit_browse[23].help_base and [24].help_base as their own symbols: as element
 * accesses GCC derives the second store and the later table address from one base register. */
extern s16 g_bunit_help_node_23_help_base;

extern s16 g_bunit_help_node_24_help_base;
extern u8 g_bunit_help_node_index;
extern u8 g_bunit_help_node_index_reload;
extern u8 g_bunit_help_node_index_reload_2;
extern help_navigation_record_t* g_bunit_help_nodes;
extern help_navigation_record_t g_bunit_help_nodes_banner_bottom[];
extern help_navigation_record_t g_bunit_help_nodes_banner_top[];

/* Aliases of g_bunit_help_nodes/g_bunit_help_node_index; the target reloads them at each use
 * instead of reusing the cached copies, which separate symbols reproduce. */
extern help_navigation_record_t* g_bunit_help_nodes_reload;

extern help_navigation_record_t g_bunit_help_nodes_unit_browse[];
extern s32 g_bunit_help_screen_id; /* menu_grid_help_id_e */

/* gfx */
/* Drawing: the packet pools and the two frame buffers, the fades, portraits, quads, tiles, draw
 * areas and VRAM transfers, the distance-shaded rows and the cursor trail. */

typedef enum bunit_vertical_scroll_direction {
    BUNIT_VERTICAL_SCROLL_UP = -1,
    BUNIT_VERTICAL_SCROLL_NONE = 0,
    BUNIT_VERTICAL_SCROLL_DOWN = 1,
    BUNIT_VERTICAL_SCROLL_WAIT = 2,
} bunit_menu_vertical_scroll_direction_e;

extern world_textured_quad_source_t g_bunit_bouncing_cursor_quad;

/* Scratch descriptor for the blank cells of
 * bunit_menu_script_draw_plain_number; only x/y are written, the rest
 * comes from its static initializer. */
extern world_oriented_quad_t g_bunit_gfx_blank_cell_rect;

extern u8 g_bunit_gfx_decreasing_fade_active;
extern RECT g_bunit_gfx_draw_area;
extern u16 g_bunit_gfx_draw_area_capacity;
extern u16 g_bunit_gfx_draw_mode_capacity;
extern u16 g_bunit_gfx_draw_move_capacity;
extern rgb24_t g_bunit_gfx_fade_color;
extern s16 g_bunit_gfx_fade_intensity;
extern RECT g_bunit_gfx_fade_rect[];
extern u8 g_bunit_gfx_increasing_fade_active;
extern u16 g_bunit_gfx_line_f2_capacity;
extern u16 g_bunit_gfx_otag_count;
extern s16 g_bunit_gfx_falloff_focus_x;
extern s16 g_bunit_gfx_falloff_focus_y;
extern s16 g_bunit_gfx_window_zoom_percent[];
extern u16 g_bunit_gfx_draw_offset_y;
extern u16 g_bunit_gfx_poly_g4_count;
extern u16 g_bunit_gfx_poly_gt3_capacity;
extern u16 g_bunit_gfx_tile_count;

/* Base of the two frame buffers; g_menu_gfx_context alternates between them. */
extern bunit_menu_gfx_context_t* g_bunit_gfx_context_base;

extern u16 g_bunit_gfx_background_clut;
extern u16 g_bunit_gfx_background_tile_layout[2][8]; /* per row parity: four (x, u) column pairs */
extern s16 g_bunit_gfx_line_f2_count;
extern u16 g_bunit_gfx_line_f3_capacity;
extern s16 g_bunit_gfx_line_f3_count;
extern u16 g_bunit_gfx_line_f4_capacity;
extern s16 g_bunit_gfx_line_f4_count;
extern s16 g_bunit_gfx_line_g2_count;
extern s16 g_bunit_gfx_line_g3_count;
extern u16 g_bunit_gfx_line_g4_capacity;
extern s16 g_bunit_gfx_line_g4_count;
extern s8 g_bunit_gfx_otag_index_locked;
extern u16 g_bunit_gfx_poly_f3_capacity;
extern s16 g_bunit_gfx_poly_f3_count;
extern u16 g_bunit_gfx_poly_f4_capacity;
extern s16 g_bunit_gfx_poly_f4_count;
extern u16 g_bunit_gfx_poly_ft3_capacity;
extern s16 g_bunit_gfx_poly_ft3_count;
extern u16 g_bunit_gfx_poly_ft4_capacity;
extern u16 g_bunit_gfx_poly_g3_capacity;
extern s16 g_bunit_gfx_poly_g3_count;
extern u16 g_bunit_gfx_poly_g4_capacity;
extern s16 g_bunit_gfx_poly_gt3_count;
extern u16 g_bunit_gfx_poly_gt4_capacity;
extern u16 g_bunit_gfx_sprite_16_capacity;
extern s16 g_bunit_gfx_sprite_16_count;
extern u16 g_bunit_gfx_sprite_8_capacity;
extern s16 g_bunit_gfx_sprite_8_count;
extern u16 g_bunit_gfx_sprite_capacity;
extern s16 g_bunit_gfx_sprite_count;
extern u16 g_bunit_gfx_tile_16_capacity;
extern s16 g_bunit_gfx_tile_16_count;
extern u16 g_bunit_gfx_tile_1_capacity;
extern s16 g_bunit_gfx_tile_1_count;
extern u16 g_bunit_gfx_tile_8_capacity;
extern s16 g_bunit_gfx_tile_8_count;
extern u16 g_bunit_gfx_tile_capacity;
extern s8 g_bunit_gfx_transition_frame;
extern u8 g_bunit_sprite_cell_sizes[][2];
void bunit_gfx_init_packet_pools(bunit_menu_gfx_context_t* buffer);
s32 bunit_gfx_calculate_distance_falloff(s32 x, s32 y, s32 base_value, s32 minimum);

void bunit_gfx_enqueue_gouraud_textured_quad(
    const world_textured_rect_source_t* rect, const rgb24_t* color, s32 semitrans, s32 otag_index);

void bunit_gfx_enqueue_textured_quad_corners(
    const world_textured_quad_source_t* src, const rgb24_t* color, s32 semi, s32 idx);

void bunit_gfx_enqueue_tile(const RECT* rect, const rgb24_t* color, u8 semi, s32 idx);
s32 bunit_gfx_get_fade_state(void);
void bunit_gfx_init_rhombus_cursor_tpages(void);
void bunit_gfx_set_transition_frame(s32 frame);
void bunit_gfx_start_decreasing_fade(void);
void bunit_gfx_start_increasing_fade(void);
s32 bunit_gfx_update_increasing_fade(void);
void bunit_gfx_draw_falloff_shaded_rows(s32 y);
void bunit_gfx_draw_unit_sprite_and_name_plate(s16 unit, s32 x, s32 y);
void bunit_gfx_enqueue_gouraud_quad(const upoint16_t* pts, const rgb24_t* color, s32 semi, s32 idx);

void bunit_gfx_init_double_packet_buffers(bunit_menu_gfx_context_t* buffers, u32* otags, POLY_F3* flat_triangles,
    POLY_FT3* textured_triangles, POLY_F4* flat_quads, POLY_FT4* textured_quads, POLY_G3* gouraud_triangles,
    POLY_GT3* gouraud_textured_triangles, POLY_G4* gouraud_quads, POLY_GT4* gouraud_textured_quads, LINE_F2* flat_lines,
    LINE_F3* flat_lines_3, LINE_F4* flat_lines_4, LINE_G2* gouraud_lines, LINE_G3* gouraud_lines_3,
    LINE_G4* gouraud_lines_4, TILE* tiles, TILE_1* tiles_1, TILE_8* tiles_8, TILE_16* tiles_16, SPRT* sprites,
    SPRT_8* sprites_8, SPRT_16* sprites_16, DR_MOVE* draw_moves, DR_AREA* draw_areas, DR_MODE* draw_modes);

void bunit_gfx_init_vram_and_start_fade(void);
void bunit_gfx_set_draw_color(const rgb24_t* color);
void bunit_gfx_set_otag_index(s16 value);
void bunit_gfx_submit_frame_and_swap_buffers(s32 first_otag, s32 draw_otag);
s32 bunit_gfx_update_decreasing_fade(void);
void bunit_gfx_update_rhombus_cursor_trail(point16_t* target);

/* input */
/* The pad: newly pressed buttons, the two repeat rates, the input lock timer and the event input
 * BATTLE's threads read. */

extern u8 g_bunit_input_activation_timer;
extern u32 g_bunit_input_current;
extern u32 g_bunit_input_event;

/* Named fields hoist the address; keep the 32-bit event-input alias for scalar accesses. */
extern world_menu_input_state_t g_bunit_input_event_state;

extern u32 g_bunit_input_initial_repeat_counter;
extern u16 g_bunit_input_latched_button;
extern u8 g_bunit_input_lock_timer;
extern u8 g_bunit_input_page_scroll_disabled;
extern u16 g_bunit_input_page_scroll_hold_frames;
extern u16 g_bunit_input_previous;
extern u32 g_bunit_input_repeat_counter;
extern s32 g_bunit_input_repeat_counters[16];
extern u32 g_bunit_input_secondary_repeat_counter;
extern u32 g_bunit_previous_input;
extern u32 g_bunit_input_primary_repeat;
extern u32 g_bunit_input_secondary_repeat;
void bunit_input_clear_state(void);
s32 bunit_input_get_lock_timer(void);
bunit_menu_vertical_scroll_direction_e bunit_input_read_page_scroll_direction(void);
void bunit_input_update_controller(void);
s32 bunit_input_read_pad1_unless_locked(void);
void bunit_input_update_event_state(u32* otag, u32 input, s32 frame_arg);
void bunit_input_update_lock_timer(void);

/* text */
/* Text: glyphs and number fields rendered into 4bpp images, text-id rows uploaded to VRAM, and the
 * help text thread. */

/* Slots of the unit list screen's own menus; thread_screen_slot_e
 * has the shared ones. */
typedef enum bunit_thread_slot {
    BUNIT_THREAD_SLOT_MENU = 5,          /* the unit list's triangle menu */
    BUNIT_THREAD_SLOT_REORDER_MENU = 15, /* the reorder list */
} bunit_thread_slot_e;

extern const char g_bunit_text_decimal_format[];
extern s32 g_bunit_text_selection_id;
extern wldcore_menu_status_thread_t g_bunit_numeric_editor_thread_params;
extern s32 g_bunit_numeric_editor_redraw_request;
extern s32 g_bunit_numeric_editor_dimmed;

/* Text-section buffers: bunit_menu_run_reorder_list passes g_bunit_text_reorder_list_buffer
 * straight to bunit_text_concatenate_entries as its u8* output string and
 * g_bunit_text_menu_section as the text-section base; bunit_gfx_init_vram_and_start_fade
 * installs both addresses as u8* text-section pointers. */
extern u8 g_bunit_text_menu_section[];

extern u8 g_bunit_text_reorder_list_buffer[];

s32 bunit_text_render_glyph_to_4bpp_image(
    s32 glyph_id, u8* image, const bunit_text_image_position_t* position, s32 style);

void bunit_text_render_signed_decimal_entries(
    u8* pixels, menu_number_entry_t* entries, world_glyph_blit_t* out, s32 count);
void bunit_text_concatenate_entries(const u8* text_table, u8* out, s16* list, s32 separate);
void bunit_text_render_id_rows_to_vram(u8* text_table, s16* list, RECT* rect, s32 style);
void bunit_text_start_selection_thread(s32 thread_data);

/* unit */
/* The units the screen lists (bunit_unit_t, built from BATTLE's units), the shown and reorderable
 * index lists and their sort, and the stat values the grid's row callbacks return. */

/* Unit List menu snapshot built from battle_stats_t, with display values and
 * the battle_id used to return the selected unit. */
typedef struct bunit_unit {
    s16 level;      /* 0x00; getters at 0x801c4054..0x801c4163 read level, experience, brave and faith */
    s16 team_kind;  /* 0x02 */
    s16 list_index; /* 0x04 */
    s16 unit_count; /* 0x06; shown units plus eggs in each billboard record */
    s16 experience; /* 0x08 */
    s16 entd_slot;  /* 0x0a */
    battle_menu_unit_status_gauge_t gauges[3]; /* 0x0c; HP, MP and CT */
    s16 formation_index;                       /* 0x1e */
    u8 _unused_020[2];
    s16 name_index; /* 0x22 */
    s16 job_id;     /* 0x24; used when character_identity is 0x82 */
    s16 brave;      /* 0x26 */
    s16 faith;      /* 0x28 */
    s16 zodiac;     /* 0x2a */
    s16 battle_id;  /* 0x2C; searched by bunit_unit_find_index_by_battle_id */
    u8 _unused_02e[2];
    s16 move;                     /* 0x30 */
    s16 speed;                    /* 0x32 */
    s16 jump;                     /* 0x34 */
    s16 right_weapon_power;       /* 0x36 */
    s16 left_weapon_power;        /* 0x38 */
    s16 right_weapon_evade;       /* 0x3a */
    s16 left_weapon_evade;        /* 0x3c */
    s16 uses_monster_skillset;    /* 0x3e; nonzero restricts the job list to its base entry */
    s16 two_handed;               /* 0x40 */
    s16 physical_attack;          /* 0x42 */
    s16 physical_class_evade;     /* 0x44 */
    s16 physical_shield_evade;    /* 0x46 */
    s16 physical_accessory_evade; /* 0x48 */
    u8 _unused_04a[2];
    s16 magical_attack;          /* 0x4c */
    s16 magical_class_evade;     /* 0x4e */
    s16 magical_shield_evade;    /* 0x50 */
    s16 magical_accessory_evade; /* 0x52 */
    s16 equipment[5];            /* 0x54 */
    s16 abilities[5];            /* 0x5e; primary skillset, secondary, reaction, support, movement (filled by
                                  * bunit_unit_build_data_from_battle_stats; bunit_ability_build_list reads [0]) */
    u8 _unused_068[8];           /* 0x68; spare tail of the stat-detail block */
    u8 gender_flags;             /* 0x70; Bard/Dancer restrictions */
    u8 runtime_id;               /* 0x71 */
    u8 initial_team_flags;       /* 0x72 */
    u8 mount_info;               /* 0x73; battle_stats_t.mount_info */
    u8 character_identity;       /* 0x74; special character, generic male/female/monster */
    u8 unlocked_jobs[UNIT_UNLOCKED_JOB_BYTE_COUNT]; /* 0x75; bit reader consumes Squire through Mime */
    u8 learned_abilities[UNIT_LEARNED_ABILITY_JOB_COUNT][UNIT_LEARNED_ABILITY_BYTES_PER_JOB]; /* 0x78 */
    u8 job_levels[UNIT_JOB_LEVEL_BYTE_COUNT]; /* 0xb1; two packed job-level nibbles per byte */
    u8 _padding_0bb;                          /* aligns job_points */
    unit_job_points_t job_points;             /* 0xbc; current JP followed by total JP */
} bunit_unit_t;
typedef char bunit_unit_mount_info_must_be_0x73[((unsigned long)&((bunit_unit_t*)0)->mount_info == 0x73) ? 1 : -1];
typedef char bunit_unit_battle_id_must_be_0x2c[((unsigned long)&((bunit_unit_t*)0)->battle_id == 0x2C) ? 1 : -1];
typedef char bunit_unit_faith_must_be_0x28[((unsigned long)&((bunit_unit_t*)0)->faith == 0x28) ? 1 : -1];
typedef char bunit_unit_brave_must_be_0x26[((unsigned long)&((bunit_unit_t*)0)->brave == 0x26) ? 1 : -1];
typedef char bunit_unit_experience_must_be_0x08[((unsigned long)&((bunit_unit_t*)0)->experience == 0x08) ? 1 : -1];
typedef char bunit_unit_uses_monster_skillset_must_be_0x3e
    [((unsigned long)&((bunit_unit_t*)0)->uses_monster_skillset == 0x3e) ? 1 : -1];
typedef char
    bunit_unit_unlocked_jobs_must_be_0x75[((unsigned long)&((bunit_unit_t*)0)->unlocked_jobs == 0x75) ? 1 : -1];
typedef char bunit_unit_gender_flags_must_be_0x70[((unsigned long)&((bunit_unit_t*)0)->gender_flags == 0x70) ? 1 : -1];
typedef char bunit_unit_character_identity_must_be_0x74[((unsigned long)&((bunit_unit_t*)0)->character_identity == 0x74)
        ? 1
        : -1];
typedef char bunit_unit_size_must_be_268[(sizeof(bunit_unit_t) == 268) ? 1 : -1];

extern s16 g_bunit_unit_count;
extern s16 g_bunit_unit_current_stat_value;
extern bunit_unit_t* g_bunit_unit_pointers[];
extern u8 g_bunit_unit_list_count;
extern u8 g_bunit_unit_list_index_map[];
extern s8 g_bunit_unit_list_page_offset;
extern s8 g_bunit_unit_list_scroll_step;
extern s16 g_bunit_unit_max_stat_value;
extern u8 g_bunit_unit_reorderable_indices[];
extern s16 g_bunit_unit_selected_index;
extern u8 g_bunit_unit_stat_mode; /* menu_unit_readout_e */
extern s16 g_bunit_unit_comparison_index;
extern u8 g_bunit_unit_browse_enabled;
extern u8 g_bunit_unit_grid_selected_cell;
extern u8 g_bunit_unit_list_initialized;
extern u8 g_bunit_unit_list_triangle_menu_open;
extern u8 g_bunit_unit_reorderable_count;
extern bunit_unit_t g_bunit_unit_records[];
extern s8 g_bunit_unit_dot_glow_direction;
extern s8 g_bunit_unit_dot_glow_timer;
extern world_oriented_quad_t g_bunit_unit_dot_sprite;
extern menu_draw_number_command_t g_bunit_unit_grid_brave_number;
extern menu_draw_number_command_t g_bunit_unit_grid_brave_number_wide;
extern u16 g_bunit_unit_grid_ct_gauge_clut;
extern rgb24_t g_bunit_unit_grid_ct_gauge_colors[4];
extern point16_t g_bunit_unit_grid_cursor_target;
extern rgb24_t g_bunit_unit_grid_dimmed_color;
extern menu_draw_number_command_t g_bunit_unit_grid_experience_number;
extern menu_draw_number_command_t g_bunit_unit_grid_faith_number;
extern menu_draw_number_command_t g_bunit_unit_grid_faith_number_wide;
extern s8 g_bunit_unit_grid_gauge_offsets[];
extern u16 g_bunit_unit_grid_hp_gauge_clut;
extern rgb24_t g_bunit_unit_grid_hp_gauge_colors[4];
extern battle_menu_sprite_cell_t g_bunit_unit_grid_label_tiles[];
extern battle_menu_sprite_cell_t g_bunit_unit_grid_label_tiles_bottom[];
extern menu_draw_number_command_t g_bunit_unit_grid_level_number;
extern menu_draw_number_command_t g_bunit_unit_grid_max_stat_number;
extern u16 g_bunit_unit_grid_mp_gauge_clut;
extern rgb24_t g_bunit_unit_grid_mp_gauge_colors[4];
extern rgb24_t g_bunit_unit_grid_normal_color;
extern menu_instruction_t g_bunit_unit_grid_stat_bar_script_bottom;
extern menu_instruction_t g_bunit_unit_grid_stat_bar_script_top;
extern menu_draw_number_command_t g_bunit_unit_grid_stat_number;
extern world_menu_entry_t g_bunit_unit_list_triangle_menu;
extern world_oriented_quad_t g_bunit_unit_shadow_sprite;

/* 16-bit field index, per sort key, into the 0x10C-byte formation record
 * built by bunit_unit_build_data_list. The name is provisional. */
extern s8 g_bunit_unit_sort_field_indices[];

void bunit_unit_sort_index_list_by_order_keys(s32 sort_mode);
void bunit_unit_build_list_index_map(void);
s32 bunit_unit_build_reorderable_index_list(void);
void bunit_unit_copy_orders_to_reorder_list(s32 unused, s16* entries);
void bunit_unit_copy_selected_data(void);
s32 bunit_unit_find_index_by_battle_id(s16 battle_id);
s32 bunit_unit_get_current_stat(s32 unit_index);
s32 bunit_unit_get_level(s32 unit_id);
s32 bunit_unit_get_experience(s32 unit_id);
s32 bunit_unit_get_brave(s32 unit_id);
s32 bunit_unit_get_faith(s32 unit_id);
s32 bunit_unit_get_max_stat(s32 unit_index);
void bunit_unit_copy_reorder_list_to_orders(s32 unused, const s16* entries);
s32 bunit_unit_build_data_list(s32 mode, s32 unit_data, s32 sort_mode);
void bunit_unit_build_data_from_battle_stats(const struct battle_stats* unit, bunit_unit_t* record, s32 unused_slot);

/* panel */
/* Starting and stopping the panel threads, and the colours of the status panel's sprites. */

void bunit_panel_toggle_comparison_status_thread(s32 enable);
void bunit_panel_toggle_unit_status_thread(s32 enable);
void bunit_panel_toggle_equipment_and_numeric_editor_threads(s32 enable);
void bunit_panel_unit_status_banner_thread(void);
extern battle_menu_unit_status_record_t g_bunit_panel_selected_billboard;
extern battle_menu_unit_status_record_t g_bunit_panel_comparison_billboard;
extern s16 g_bunit_panel_selected_unit_data[];
extern s16 g_bunit_panel_comparison_unit_data[];
extern battle_menu_status_panel_editor_packet_t g_bunit_panel_comparison_editor_packets[2];
extern u8 g_bunit_panel_comparison_large_number_image[];
extern u8 g_bunit_panel_comparison_name_image[];
extern u8 g_bunit_panel_comparison_number_image[];
extern menu_number_entry_t g_bunit_panel_comparison_numeric_entries[];
extern battle_menu_status_panel_packet_t g_bunit_panel_comparison_packets[2];
extern palette_t g_bunit_panel_comparison_portrait_palettes[2];
extern RECT g_bunit_panel_comparison_portrait_rect;
extern image_load_parameters_t g_bunit_panel_editor_label_cells[];
extern u16 g_bunit_panel_editor_label_cluts[][2];
extern image_load_parameters_t g_bunit_panel_editor_mode_cell[];
extern image_load_parameters_t g_bunit_panel_editor_mode_cells[];
extern image_load_parameters_t g_bunit_panel_editor_value_cells[];
extern u16 g_bunit_panel_editor_value_cluts[][2];
extern RECT g_bunit_panel_frame_rect;
extern CVECTOR g_bunit_panel_gauge_bar_colors[];
extern point16_t g_bunit_panel_item_icon_texture[];
extern image_load_parameters_t g_bunit_panel_portrait_cell[];
extern battle_menu_status_panel_editor_packet_t g_bunit_panel_selected_editor_packets[2];
extern u8 g_bunit_panel_selected_large_number_image[];
extern u8 g_bunit_panel_selected_name_image[];
extern u8 g_bunit_panel_selected_number_image[];
extern menu_number_entry_t g_bunit_panel_selected_numeric_entries[];
extern battle_menu_status_panel_packet_t g_bunit_panel_selected_packets[2];
extern palette_t g_bunit_panel_selected_portrait_palettes[2];
extern RECT g_bunit_panel_selected_portrait_rect;
extern s16 g_bunit_panel_slide_down_y[];
extern s16 g_bunit_panel_slide_up_y[];
extern image_load_parameters_t g_bunit_panel_sprite_cells[];

/* menu */
/* The screen's menus: the unit list frame and its grid, the sort-order list, the job page, the
 * scrollable lists with their saved positions, the selectors, the cursors, the palette mode and the menu
 * threads. */

void bunit_menu_run_numeric_editor_thread(void);
void bunit_menu_set_palette_mode(s32 mode);
void bunit_menu_equipment_and_ability_panel_thread(void);

/* Display opcodes use .value for predicates and numbers, .icon for sprite sources. */
enum { BUNIT_MENU_SELECTION_RECORD_COUNT = 14 };

/* g_bunit_menu_current_menu: what bunit_entrypoint runs. */
typedef enum bunit_menu {
    BUNIT_MENU_UNIT_LIST = -1,   /* bunit_menu_run_unit_list_frame: the grid and its triangle menu */
    BUNIT_MENU_REORDER_LIST = 0, /* bunit_menu_run_reorder_list, the triangle menu's first row */
    BUNIT_MENU_JOB = 2,          /* bunit_menu_update_job: the selected unit's jobs and abilities */
} bunit_menu_e;

/* Idle and active vertical bounce curves of (frame threshold, pixel offset)
 * byte pairs, shared by the animated and bouncing cursor renderers. */
extern s8 g_bunit_menu_cursor_forward_offset_keyframes[];

extern s8 g_bunit_menu_cursor_reverse_offset_keyframes[];
extern s16 g_bunit_menu_list_visible_rows;
extern u16 g_bunit_menu_list_vram_width;
extern s16 g_bunit_menu_primary_result;
extern s16 g_bunit_menu_reorder_list_entries[];
extern s16 g_bunit_menu_reorder_list_index;
extern u8 g_bunit_menu_reorder_list_initialized;
extern s16 g_bunit_menu_reorder_result;
extern bunit_menu_selection_record_t g_bunit_menu_selection_records[BUNIT_MENU_SELECTION_RECORD_COUNT];
extern world_menu_button_piece_t g_bunit_slot_sprite_entries[];
extern u8 g_bunit_menu_thread_running;
extern s32 g_bunit_menu_current_menu; /* bunit_menu_e */
extern s16 g_bunit_menu_reorder_list_redraw_request;
extern world_menu_entry_t g_bunit_menu_reorder_list_desc[];
extern u16 g_bunit_menu_icon_texture_page;
extern s16 g_bunit_menu_list_row_group_count;
extern s8 g_bunit_menu_list_redraw_pending; /* read with lb (bunit_menu_script_draw_scrollable_list) */
extern u8 g_bunit_menu_list_glyph_width;
extern u8 g_bunit_menu_list_text_columns;
extern s8 g_bunit_menu_list_scroll_direction; /* read with lb */
extern u16 g_bunit_menu_button_clut;

/* Signed 4.12 x/y scale pair per sprite entry; a negative scale flips the quad. */
extern s16 g_bunit_menu_button_piece_scales[][2];

extern u8 g_bunit_menu_button_piece_starts[];

/* Per-slot blink phase, and the half-open span of sprite entries the slot draws. */
extern s8 g_bunit_menu_button_press_timers[];

extern clut_t bunit_menu_clut;
extern u32 g_bunit_menu_input_active_mask;
extern u16 g_bunit_menu_input_mask;
extern u32 g_bunit_menu_input_repeat_mask;
extern u16 g_bunit_menu_list_cursor_anim_state;
extern s16* g_bunit_menu_list_entries;
extern s16 g_bunit_menu_list_entry_count;
extern s16 g_bunit_menu_list_row_height;
extern s16 g_bunit_menu_list_scroll_entry_ids[];
extern u16 g_bunit_menu_list_scroll_progress;
extern s16 g_bunit_menu_list_selected_index;
extern u8* g_bunit_menu_list_text_table;
menu_instruction_t* bunit_menu_script_draw_formatted_number(menu_instruction_t* instruction);
void bunit_menu_clear_selection_record(s32 index);
void bunit_menu_clear_selection_records(void);
void bunit_menu_draw_scrollable_list(const menu_instruction_t* script);
void bunit_menu_get_selection_record(s32 index, s16* selected_index, s16* scroll_base_index, s16* entries);

void bunit_menu_init_and_draw_scrollable_list(
    s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table, const menu_instruction_t* script);

void bunit_menu_init_and_draw_scrollable_list_simple(
    s16* entries, s32 selected_index, u8* text_table, const menu_instruction_t* script);

s32 bunit_menu_is_selected_unit_in_upper_rows(s32 page_offset);
s32 bunit_menu_is_unit_list_scroll_needed(void);
void bunit_menu_reset_results(void);
s32 bunit_menu_run_descriptor_thread(s32 id, world_menu_entry_t* desc);
s32 bunit_menu_run_job_list(void);
void bunit_menu_run_reorder_list(void);
void bunit_menu_scroll_list_by_page(bunit_menu_vertical_scroll_direction_e direction, const menu_instruction_t* list);
s32 bunit_menu_update_wrapped_grid_selection(s32 step, s32 count, s32 max, s32 current, s32 input);
void bunit_menu_dispatch_with_override(menu_instruction_t* script, s32 state, s32 override);
void bunit_menu_draw_pressable_button(s16 slot, s16 x, s16 y, s16 pressed, s16 busy, u16 sound_id);
void bunit_menu_init_scrollable_list(s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table);
void bunit_menu_init_scrollable_list_core(s16* entries, s32 selected_index, u8* text_table);
void bunit_menu_init_unit_list(s32 unused_selected_index);
s32 bunit_menu_is_active_or_transitioning(void);
s32 bunit_menu_run_unit_list_frame(void);
void bunit_menu_set_selection_record(s32 index, s32 selected_index, s32 scroll_base_index, s16* entries);
s32 bunit_menu_set_selection_value(u8 index, s16 value);
void bunit_menu_update_and_draw_animated_cursor(point16_t* pos, u16* state, s32 mode);
void bunit_menu_update_and_draw_bouncing_cursor(point16_t* pos, u16* state, s32 mode);
void bunit_menu_update_event_thread_state(void);

s32 bunit_menu_update_horizontal_selection_and_mark_change(
    u16 entry_count, u8 selection_index, s32 input_mask, u8 changed_state);

void bunit_menu_update_job(void);
void bunit_menu_update_node_cursor(s16 mode);
void bunit_menu_update_unit_browser(void);

s32 bunit_menu_update_unit_grid(s16 allow_banner_toggle, s16 allow_browse, s16 show_stat_panel, s16 allow_stat_cycle,
    s16 stat_mode, s16 buttons, s16 locked);

void bunit_menu_update_unit_list_scroll(s8* step, s8* offset);

/* screen */
/* The entry point and the state it keeps between frames: the unit picked from the list's menu, the
 * saved banner and browse settings, the shown-unit indices and the queued sound. */

extern s8 g_bunit_selected_battle_id;
extern u8 g_bunit_saved_unit_browse_enabled;
extern u8 g_bunit_saved_status_banner_enabled;
extern u16 g_bunit_frame_arg;
extern s16 g_bunit_shown_unit_indices[];
extern u8 g_bunit_sound_queued_effect_id; /* main_sfx_e */
s32 bunit_entrypoint(s32 unused_unit, void* unused_otag);

/* added from build-2 (merge dry run) */
extern RECT g_bunit_menu_portrait_origin;

#endif
