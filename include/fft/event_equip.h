#ifndef EVENT_EQUIP_H
#define EVENT_EQUIP_H

/* EVENT/EQUIP.OUT: menu screen overlay that runs with BATTLE. */

#include "fft/battle.h"

/* bits */
extern s32 g_equip_bits_reader_1_index;
extern u8 g_equip_bits_reader_1_reset;
extern s32 g_equip_bits_reader_2_index;
extern u8 g_equip_bits_reader_2_reset;
extern u8* g_equip_bits_reader_1_stream;
extern u8* g_equip_bits_reader_2_stream;
void equip_bits_init_primary_reader(const u8* data);
s32 equip_bits_read_primary(s32 count);
void equip_bits_init_secondary_reader(const u8* data);
s32 equip_bits_read_secondary(s32 count);

/* command */
/* Render-command stream callbacks: every handler consumes and returns a
 * command pointer; every row callback is indexed by a list row. */
typedef u8* (*equip_command_handler_t)(u8*);

/* The opcode that ends an EQUIP menu script stream (menu_script_opcode_e
 * names the others). */
enum {
    EQUIP_CMD_END = 0x19,
};

extern equip_command_handler_t g_equip_cmd_handlers[];
extern u8 g_equip_cmd_row_sprite_body[];

/* EQUIP's 0x3c-byte menu records: the main commands, the item actions and
 * (in equip_run_item_type_order_mode) the item-type order menus. Each is
 * started through equip_thread_start_managed, which makes it the current
 * g_battle_menu_thread_menu_data. */
extern world_menu_entry_t g_equip_command_menu;
extern s32 g_equip_cmd_stream_input;
extern world_menu_window_command_t g_equip_cmd_window_interior_command;
u8* equip_cmd_draw_textured_quad_handler(u8* command);
void equip_cmd_run_stream(u8* stream, s32 input);
world_menu_window_command_t* equip_cmd_draw_window_frame_handler(world_menu_window_command_t* command);
u8* equip_cmd_draw_tiled_rectangle_handler(u8* command);

/* editor */
extern world_image_location_t g_equip_editor_numeric_geometry[];
extern menu_number_entry_t g_equip_editor_numeric_descriptor_a[];
extern menu_number_entry_t g_equip_editor_numeric_descriptor_b[];
extern RECT g_equip_editor_numeric_entries_a;
extern RECT g_equip_editor_numeric_entries_b;
extern RECT g_equip_editor_numeric_entries_c;
extern RECT g_equip_editor_numeric_entries_d;
extern RECT g_equip_editor_numeric_entries_e;
extern RECT g_equip_editor_numeric_entries_f;
extern battle_menu_status_panel_numeric_buffer_t g_equip_editor_numeric_state_a[2];
extern battle_menu_status_panel_numeric_buffer_t g_equip_editor_numeric_state_b[2];

/* Per-thread-slot window rect, upload rects, sprite descriptors and number
 * entries; the first set belongs to thread 12. */
extern battle_menu_status_panel_numeric_geometry_t g_equip_editor_numeric_table;
extern u8 g_equip_editor_numeric_text_a[];
extern u8 g_equip_editor_numeric_text_b[];
extern world_gfx_image_load_parameters_t g_equip_editor_numeric_texture[];
void equip_editor_run_numeric_thread(void);

/* equip */
extern u16 g_equip_equip_item_cursor_anim[];
extern s8 g_equip_equip_item_list_open;
extern s8 g_equip_equip_item_list_refresh;
extern u8 g_equip_equip_item_menu_script[];
extern s8 g_equip_equip_item_preview_active;
extern s16 g_equip_equip_item_previewed_cursor;
extern s8 g_equip_equip_item_slot;
extern s8 g_equip_equip_mode_initialized;

/* equipment */
extern u8 g_equip_equipment_panel_params[];
extern s32 g_equip_equipment_panel_redraw;
extern s32 g_equip_equipment_panel_style;
extern u8 g_equip_equipment_render_commands[];

/* gfx */

/* Per-buffer primitive-pool pointers used by EQUIP.OUT's renderer. The 25
 * stores at 0x801cda80 establish the offsets in this 0x64-byte table. */
typedef struct equip_gfx_context {
    u32* otag;                /* 0x00 */
    POLY_F3* poly_f3;         /* 0x04 */
    POLY_FT3* poly_ft3;       /* 0x08 */
    POLY_F4* poly_f4;         /* 0x0c */
    POLY_FT4* textured_quads; /* 0x10 */
    POLY_G3* poly_g3;         /* 0x14 */
    POLY_GT3* poly_gt3;       /* 0x18 */
    POLY_G4* poly_g4;         /* 0x1c */
    POLY_GT4* poly_gt4;       /* 0x20 */
    LINE_F2* lines;           /* 0x24 */
    LINE_F3* line_f3;         /* 0x28 */
    LINE_F4* line_f4;         /* 0x2c */
    LINE_G2* line_g2;         /* 0x30 */
    LINE_G3* line_g3;         /* 0x34 */
    LINE_G4* line_g4;         /* 0x38 */
    TILE* tiles;              /* 0x3c */
    u8 (*tiles_1)[0xc];       /* 0x40: TILE_1 */
    u8 (*tiles_8)[0xc];       /* 0x44: TILE_8 */
    u8 (*tiles_16)[0xc];      /* 0x48: TILE_16 */
    SPRT* sprites;            /* 0x4c */
    SPRT_8* sprites_8;        /* 0x50 */
    u8 (*sprites_16)[0x10];   /* 0x54: SPRT_16 */
    DR_MOVE* moves;           /* 0x58 */
    DR_AREA* draw_areas;      /* 0x5c */
    DR_MODE* draw_modes;      /* 0x60 */
} equip_gfx_context_t;
typedef char equip_gfx_context_size_must_be_0x64[(sizeof(equip_gfx_context_t) == 0x64) ? 1 : -1];

/* One of the two double-buffered graphics contexts: the pool-pointer table
 * followed by per-buffer state. equip_gfx_swap_context_and_clear_otag and
 * equip_gfx_init_contexts step between the two by 0xec. */
typedef struct equip_gfx_buffer {
    equip_gfx_context_t context;
    u8 _unused_64[0x88];
} equip_gfx_buffer_t;
typedef char equip_gfx_buffer_size_must_be_0xec[(sizeof(equip_gfx_buffer_t) == 0xec) ? 1 : -1];

extern u16 g_equip_gfx_clut_id;
extern equip_gfx_context_t* g_equip_gfx_context;
extern equip_gfx_buffer_t* g_equip_gfx_context_base;
extern u32* g_equip_gfx_current_ot;
extern u16 g_equip_gfx_draw_area_count;
extern const u16 g_equip_gfx_draw_area_scale_percent[12];
extern u16 g_equip_gfx_draw_mode_count;
extern u16 g_equip_gfx_draw_move_capacity;
extern u16 g_equip_gfx_draw_move_count;
extern u16 g_equip_gfx_drawenv_y;
extern u16 g_equip_gfx_line_f2_capacity;
extern u16 g_equip_gfx_line_f2_count;
extern u16 g_equip_gfx_otag_length;
extern u16 g_equip_gfx_poly_f4_capacity;
extern u16 g_equip_gfx_poly_ft4_capacity;
extern u16 g_equip_gfx_poly_ft4_count;
extern RECT g_equip_gfx_portrait_origin;
extern u8 g_equip_gfx_portrait_transition_offsets[];
extern s16 g_equip_gfx_semitransparency;
extern u8 g_equip_gfx_sprite_color[3];
extern s16 g_equip_gfx_sprite_ot_index;
extern u16 g_equip_gfx_texture_page;
extern u16 g_equip_gfx_tile_capacity;
extern u16 g_equip_gfx_tile_count;

/* Tiled-rectangle bounds inside an unnamed command buffer; the four s16
 * fields are packed at a 2-byte stride (the CARD twin is g_card_gfx_tiled_rect). */
extern RECT g_equip_gfx_tiled_rect;
extern s8 g_equip_gfx_transition_frame;
extern s8 g_equip_gfx_zoom_draw_area_active;
extern RECT g_equip_gfx_draw_area_template;
extern RECT g_equip_gfx_draw_area;
extern s16 g_equip_gfx_window_zoom_percent[];
extern POLY_FT4 g_equip_gfx_poly_ft4_buffer[];
extern DR_MOVE g_equip_gfx_draw_move_buffer[];
extern DR_AREA g_equip_gfx_draw_area_buffer[];
extern u32 g_equip_gfx_otag_buffer[];
extern equip_gfx_buffer_t g_equip_gfx_contexts[];
extern DR_MODE g_equip_gfx_draw_mode_buffer[];
extern LINE_F2 g_equip_gfx_line_f2_buffer[];
extern TILE g_equip_gfx_tile_buffer[];
extern u16 g_equip_gfx_draw_offset_y;
extern u16 g_equip_gfx_poly_f3_capacity;
extern u16 g_equip_gfx_poly_g3_capacity;
extern u16 g_equip_gfx_poly_g4_capacity;
extern u16 g_equip_gfx_poly_f3_count;
extern u16 g_equip_gfx_poly_g3_count;
extern u16 g_equip_gfx_poly_f4_count;
extern u16 g_equip_gfx_poly_g4_count;
extern u16 g_equip_gfx_sprite_capacity;
extern volatile u16 g_equip_gfx_line_g2_capacity; /* LINE_G2 capacity; load order matters in 0x801cda80 */
extern u16 g_equip_gfx_line_f3_capacity;
extern volatile u16 g_equip_gfx_line_g3_capacity; /* LINE_G3 capacity; load order matters in 0x801cda80 */
extern u16 g_equip_gfx_line_f4_capacity;
extern u16 g_equip_gfx_line_g4_capacity;
extern u16 g_equip_gfx_poly_ft3_capacity;
extern u16 g_equip_gfx_poly_gt3_capacity;
extern u16 g_equip_gfx_line_g2_count;
extern u16 g_equip_gfx_line_f3_count;
extern u16 g_equip_gfx_poly_gt4_capacity;
extern u16 g_equip_gfx_line_g3_count;
extern u16 g_equip_gfx_line_f4_count;
extern u16 g_equip_gfx_line_g4_count;
extern u16 g_equip_gfx_poly_ft3_count;
extern u16 g_equip_gfx_poly_gt3_count;
extern u16 g_equip_gfx_poly_gt4_count;
extern u16 g_equip_gfx_sprite_8_capacity;
extern u16 g_equip_gfx_sprite_16_capacity;
extern u16 g_equip_gfx_draw_area_capacity;
extern u16 g_equip_gfx_draw_mode_capacity;
extern u16 g_equip_gfx_sprite_count;
extern u16 g_equip_gfx_tile_1_capacity;
extern u16 g_equip_gfx_tile_8_capacity;
extern u16 g_equip_gfx_tile_1_count;
extern u16 g_equip_gfx_tile_8_count;
extern u16 g_equip_gfx_sprite_8_count;
extern u16 g_equip_gfx_tile_16_capacity;
extern u16 g_equip_gfx_tile_16_count;
extern u16 g_equip_gfx_sprite_16_count;

/* The scratch marker rect this routine drives; uncatalogued, but the same
 * descriptor equip_menu_draw_equipment_slot_marker feeds to the oriented quad
 * enqueue. */
extern world_oriented_quad_t g_equip_gfx_blank_cell_rect;
void equip_menu_select_primitive_color_palette(
    world_menu_palette_primitives_t* primitives, const world_status_thread_t* thread);
void equip_gfx_build_portrait_poly_ft4(s32 portrait_id, void* poly);

void equip_gfx_build_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* packet, const void* source,
    s32 scale_index, s32 lower_half, const world_status_thread_t* thread);

void equip_gfx_clip_portrait_poly_from_left(POLY_FT4* poly, s32 amount);
void equip_gfx_clip_portrait_poly_from_right(POLY_FT4* poly, s32 amount);
void equip_gfx_enqueue_draw_area(RECT* rect, s32 idx);
void equip_gfx_enqueue_draw_mode(s32 dfe, s32 dtd, s32 tpage, RECT* tw, s32 idx);
void equip_gfx_enqueue_line_f2(const RECT* rect, const u8* color, s32 semitrans, s32 otag_index);

void equip_gfx_enqueue_oriented_textured_quad(
    const world_oriented_quad_t* descriptor, const u8* color, s32 orientation, s32 semitrans, s32 otag_index);

void equip_gfx_enqueue_tile(const RECT* rect, const u8* color, s32 semitrans, s32 otag_index);

void equip_gfx_init_contexts(equip_gfx_buffer_t* base, u32* otag, POLY_F3* poly_f3, POLY_FT3* poly_ft3,
    POLY_F4* poly_f4, POLY_FT4* textured_quads, POLY_G3* poly_g3, POLY_GT3* poly_gt3, POLY_G4* poly_g4,
    POLY_GT4* poly_gt4, LINE_F2* line_f2, LINE_F3* line_f3, LINE_F4* line_f4, LINE_G2* line_g2, LINE_G3* line_g3,
    LINE_G4* line_g4, TILE* tiles, u8 (*tiles_1)[0xc], u8 (*tiles_8)[0xc], u8 (*tiles_16)[0xc], SPRT* sprites,
    SPRT_8* sprites_8, u8 (*sprites_16)[0x10], DR_MOVE* draw_moves, DR_AREA* draw_areas, DR_MODE* draw_modes);

u8* equip_gfx_get_draw_color(void);
s32 equip_gfx_get_transition_frame(void);
void equip_gfx_init_menu_tile_and_line_primitives(battle_menu_status_panel_buffer_t* menu);
void equip_gfx_init_primitive_buffers(equip_gfx_context_t* context);
void equip_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* data);
void equip_gfx_load_image_and_wait(RECT* rect, void* data);
void equip_gfx_set_clut_rect_from_id(RECT* rect, s32 clut_id);
void equip_gfx_set_otag_index(s32 ot_index);
void equip_gfx_set_transition_frame(s32 value);
void equip_gfx_store_image_and_wait(RECT* rect, void* data);
void equip_gfx_swap_context_and_clear_otag(s32 first_otag, s32 unused);
void equip_gfx_build_item_graphic_descriptor(battle_menu_status_panel_graphic_descriptor_t* descriptor, s32 item_id);

void equip_gfx_build_portrait_transition_primitives(const RECT* texture_rect, s32* transition,
    const s32* first_portrait, const s32* second_portrait, u16* image, POLY_FT4* poly, s32 direction);

void equip_gfx_enqueue_draw_move(const RECT* rect, s32 u, s32 v, s32 otag_index);

void equip_gfx_enqueue_textured_quad(
    const RECT* rect, s32 u, s32 v, const u8* color, s32 semitrans, s32 texture_page, s32 clut, s32 otag_index);

void equip_gfx_enqueue_textured_quad_current_ot(
    const RECT* rect, s32 u, s32 v, const u8* color, s32 semitrans, s32 texture_page, s32 clut, s32 unused_priority);

/* input */
extern u8 g_equip_input_activation_timer;
extern u32* g_equip_input_controller;
extern u8 g_equip_input_lock_timer;
extern u16 g_equip_input_newly_pressed;
extern u32 g_equip_input_primary_repeat;
extern s32 g_equip_input_repeat_counters[16];
extern u32 g_equip_input_secondary_repeat;
extern u8 g_equip_input_page_scroll_disabled;
extern u16 g_equip_input_page_scroll_hold_frames;
extern u16 g_equip_input_latched_button;
extern u16 g_equip_input_previous;
void equip_input_clear_state(void);
s32 equip_input_read_page_scroll_direction(void);
void equip_input_update_with_message_state(void);
void equip_update_controller_input(void);

/* item */
enum {
    EQUIP_ITEM_LIST_END = 0xFF,
    EQUIP_ITEM_ID_MASK = 0x3FF,
    EQUIP_ITEM_LIST_ENTRY_ITEM_ID_MASK = 0x3FF,
    EQUIP_ITEM_LIST_ENTRY_EQUIPPED = 0x4000,
    EQUIP_ITEM_COUNT_EQUIPPED_FLAG = 0x40000000,
};

extern u8* g_equip_item_category_lists[];
extern battle_menu_status_panel_graphic_descriptor_t g_equip_item_graphic_descriptor;
extern u8 g_equip_item_inventory_totals[];
extern s16 g_equip_item_list_entries[];
extern u8 g_equip_item_type_icon_coords[][2];
extern u8* g_equip_item_type_order_lists[];
extern s32 g_equip_item_numeric_panel_params;
extern s32 g_equip_item_numeric_panel_y;
extern s32 g_equip_item_numeric_panel_redraw;
extern s32 g_equip_item_numeric_panel_style;
extern s32 g_equip_item_numeric_thread_enabled;
extern struct world_item_stat_detail g_equip_item_preview_stat_detail;
extern world_menu_entry_t g_equip_item_action_menu;
extern world_menu_entry_t g_equip_item_action_menu_single_item;
extern s8 g_equip_item_action_menu_active;
extern u8 g_equip_item_list_has_multiple_entries;
extern s16 g_equip_item_type_order[];
extern u8 g_equip_item_type_order_names[];
extern world_menu_entry_t* g_equip_item_action_active_menu;
extern world_item_icon_source_t g_equip_item_row_icon_rect;
extern world_menu_entry_t* g_equip_item_type_order_active_menu;
extern u16 g_equip_remove_item_cursor_anim[];
extern s16 g_equip_remove_item_equipment[];
extern point16_t g_equip_remove_item_marker_rect;
extern s8 g_equip_remove_item_previewed_slot;
void equip_item_clear_stat_sums(world_item_stat_summary_t* summary, world_item_stat_detail_t* detail);
s32 equip_item_adjust_inventory_count(s32 item_entry, s32 delta);
void equip_item_build_inventory_totals(void);
world_item_icon_source_t* equip_item_build_type_icon_source(s32 index);

void equip_item_calculate_swap_stat_delta(
    struct world_item_stat_detail* output, struct world_item_stat_summary* out, s16 from, s16 to, s32 slot);

void equip_menu_load_sort_key_list(s32 list_index, s16* destination);
s32 equip_item_get_available_count(s32 item_entry);
s32 equip_menu_get_entry_item_equipped_count(s32 row);
item_menu_category_e equip_item_get_menu_category(s32 item_id);
s32 equip_item_get_total_count(s32 item_id);
s32 equip_menu_get_entry_item_count(s32 row);
s32 equip_item_get_type(s32 item_id);
void equip_item_get_type_icon_rect(s32 item_type, world_item_icon_source_t* output);
s32 equip_item_is_in_category_list(s32 item_id, s32 category_index);
void equip_item_prepend_to_category_list(s32 item_id, s32 category_index);
void equip_item_rebuild_category_list(s32 category);
void equip_item_remove_from_category_list(s32 item_id, s32 category_index);
s32 equip_item_sort_list_by_category_order(item_menu_category_e category, s16* list);
void equip_item_store_category_list(item_menu_category_e category, const s16* source);
void equip_menu_store_sort_key_list(s32 list_index, const s16* source);
s32 equip_run_item_type_order_mode(void);
void equip_item_sort_list_by_criteria(s32 sort_key, s16* list);
s32 equip_item_build_filtered_list(s32 unit_index, s32 sort_mode, s32 category, s16* list, s32 equip_filter);
battle_menu_status_panel_graphic_descriptor_t* equip_menu_get_entry_item_icon_source(s32 row);
s32 equip_item_get_ranking_value(s32 item_id);

void equip_item_subtract_scaled_stats(struct world_item_stat_detail* out, struct world_item_stat_detail* scaled,
    struct world_item_stat_detail* base, s32 scale);

/* menu */
/* g_equip_menu_screen_state: the index of the screen's handler in
 * g_equip_menu_screen_state_handlers. The command menu enters its picked row
 * + EQUIP: Equip, Remove and the item list are rows 0, 2 and 3 (row 1 is
 * refused). */
typedef enum equip_screen_state {
    EQUIP_SCREEN_STATE_INIT = 0, /* equip_entrypoint builds the screen */
    EQUIP_SCREEN_STATE_COMMAND_MENU = 1,
    EQUIP_SCREEN_STATE_EQUIP = 2,     /* equip_menu_run_equip_mode */
    EQUIP_SCREEN_STATE_REMOVE = 4,    /* equip_menu_run_remove_mode */
    EQUIP_SCREEN_STATE_ITEM_LIST = 5, /* equip_menu_update_equipment */
    EQUIP_SCREEN_STATE_CLOSED = 0xff, /* equip_entrypoint returns its result */
} equip_screen_state_e;

typedef s32 (*equip_row_callback_t)(s32 row);

extern point16_t g_equip_menu_clear_all_prompt_draw_params;
extern volatile u16 g_equip_menu_clear_all_prompt_frame;
extern s8 g_equip_menu_event_speed;
extern s16* g_equip_menu_list_entries;
extern s16 g_equip_menu_list_entry_count;
extern equip_row_callback_t g_equip_menu_list_row_callbacks[];
extern s16 g_equip_menu_list_row_height;
extern s32 g_equip_menu_list_row_index;
extern s16 g_equip_menu_list_row_mode;
extern s16 g_equip_menu_list_scroll_offset;
extern u8* g_equip_menu_list_text_table;
extern s16 g_equip_menu_list_visible_rows;
extern u16 g_equip_menu_list_vram_width;
extern u8 g_equip_menu_screen_state; /* equip_screen_state_e */
extern s16 g_equip_menu_scroll_base_index;
extern world_oriented_quad_t g_equip_menu_secondary_slot_marker;

/* Signed halfword: EQUIP reads this state with lh at 0x801c4584,
 * 0x801c45bc, and 0x801c4614. */
extern s16 g_equip_menu_selected_list_index;
extern bunit_menu_selection_record_t g_equip_menu_selection_entries[];
extern s16 g_equip_menu_selection_values[4];
extern world_oriented_quad_t g_equip_menu_slot_marker_rects[];
extern u8 g_equip_menu_slot_marker_window[];
extern u8 g_equip_menu_slot_marker_x[];
extern s8 g_equip_menu_thread_running;
extern u16 g_equip_menu_palette_image[];
extern s16 g_equip_menu_text_redraw_flag;
extern u16 g_equip_menu_cursor_texture_page;
extern u16 g_equip_menu_cursor_mode0_foreground_clut;
extern u16 g_equip_menu_cursor_mode0_background_clut;
extern u16 g_equip_menu_cursor_mode1_foreground_clut;
extern u16 g_equip_menu_cursor_mode1_background_clut;
extern s8 g_equip_menu_cursor_forward_offset_keyframes[];
extern s8 g_equip_menu_cursor_reverse_offset_keyframes[];
extern s16 g_equip_menu_list_row_group_count;
extern s8 g_equip_menu_list_redraw_pending; /* read with lb (equip_cmd_draw_scrollable_list_body_handler) */
extern u8 g_equip_menu_list_glyph_width;
extern u8 g_equip_menu_list_text_columns;
extern s8 g_equip_menu_list_scroll_direction; /* -1/0/1; read with lb */
extern s16 g_equip_menu_result_rows[4];       /* [2]: type-order row picked in the sub-menu, or -1 */
extern s32 g_equip_menu_indicator_brightness[2];

/* Double-buffer index, current level, fade level and state, one pair each for
 * the two status panels. */
extern s32 g_equip_menu_indicator_buffer_index[2];
extern s32 g_equip_menu_indicator_edge_brightness[2];
extern s32 g_equip_menu_indicator_state[2];
extern u16 g_equip_menu_list_cursor_anim_state[];
extern s16 g_equip_menu_list_scroll_entry_ids;
extern u16 g_equip_menu_list_scroll_progress;
extern s8 g_equip_menu_ready;
extern void (*g_equip_menu_screen_state_handlers[])(void);
extern s8 g_equip_menu_substate;
extern s16 g_equip_menu_window_right_x;
void equip_menu_clear_selection_records(void);
void equip_menu_clear_selection_values(void);
void equip_menu_dispatch_with_override(u8* menu, s32 state, s32 override);
void equip_menu_draw_equipment_slot_marker(s32 unused, s32 slot, s32 override);
void equip_menu_draw_scrollable_list(const u8* script);
void equip_menu_draw_weapon_hand_icons(s16 weapon_id);
s32 equip_menu_get_event_speed(void);
void equip_menu_init_screen(s32 battle_id);

void equip_menu_init_scrollable_list(s16* entries, s32 selected_index, s32 scroll_base_index, u8* text_table);

void equip_menu_init_scrollable_list_core(s16* entries, s32 selected_index, u8* text_table);
void equip_menu_load_images_and_reset_lists(void);
void equip_menu_reset_selection_indices(void);
void equip_menu_reset_state_flags(void);
void equip_menu_restore_list_selection(s32 idx, s16* p1, s16* p2, s16* list);
s32 equip_item_run_rearrange_mode(void);
void equip_menu_icon_list_thread(void);
void equip_menu_scroll_list_by_page(s32 direction, const u8* menu);
void equip_menu_set_selection_record(s32 index, s32 selected_index, s32 scroll_index, s16* items);
s32 equip_menu_set_selection_value(s32 index, s32 value);
s32 equip_menu_set_selection_value_2(s32 index, s32 value);
void equip_menu_update_and_draw_animated_cursor(const point16_t* anchor, u16* state, s32 mode);
s32 equip_menu_update_clear_all_equipment_prompt(void);
s32 equip_menu_update_equipment(void);
s32 equip_menu_update_item_action_state(void);
s32 equip_menu_run_equip_mode(void);
s32 equip_menu_run_remove_mode(void);
s32 equip_menu_run_slot_item_browser(void);
void equip_menu_draw_item_status_list(s16 item_id);
s32 equip_menu_get_preview_hp_bonus(s32 unused_row);
s32 equip_menu_get_preview_hp_bonus_display(s32 unused_row);
s32 equip_menu_get_preview_mp_bonus(s32 unused_row);
s32 equip_menu_get_preview_mp_bonus_display(s32 unused_row);

s32 equip_menu_update_horizontal_selection_and_mark_change(
    s32 entry_count, s32 selection_index, s32 input_mask, s8 sound_id);

s32 equip_menu_update_vertical_selection_and_mark_change(
    s32 entry_count, s32 selection_index, s32 input_mask, s8 sound_id);

s16 equip_menu_update_wrapped_horizontal_selection(u16 entry_count, u8 selection_index, s32 input_mask);
s16 equip_menu_update_wrapped_vertical_selection(u16 entry_count, u8 selection_index, s32 input_mask);

/* panel */
extern s16 g_equip_panel_selected_unit_data[];
extern world_image_location_t g_equip_panel_item_icon_texture[];
extern RECT g_equip_panel_origin_offsets[];
extern RECT g_equip_panel_text_upload_rect_a;
extern RECT g_equip_panel_text_upload_rect_b;
extern world_gfx_image_load_parameters_t g_equip_panel_label_layouts_mode0[];
extern world_gfx_image_load_parameters_t g_equip_panel_label_layouts_mode1[];
extern world_gfx_image_load_parameters_t g_equip_panel_label_layouts_mode2[];
extern world_gfx_image_load_parameters_t g_equip_panel_label_layouts_mode3[];
extern world_gfx_image_load_parameters_t g_equip_panel_item_icon_layouts[];
extern battle_menu_status_panel_buffer_t g_equip_panel_frames_a[];
extern u8 g_equip_panel_text_image_a[];
extern u8 g_equip_panel_text_image_b[];
extern battle_unit_status_record_t g_equip_panel_comparison_billboard;
extern battle_menu_status_panel_editor_packet_t g_equip_panel_comparison_editor_packets[2];
extern u8 g_equip_panel_comparison_large_number_image[];
extern u8 g_equip_panel_comparison_name_image[];
extern u8 g_equip_panel_comparison_number_image[];
extern menu_number_entry_t g_equip_panel_comparison_numeric_entries[];
extern battle_menu_status_panel_packet_t g_equip_panel_comparison_packets[2];
extern u16 g_equip_panel_comparison_portrait_image[];
extern RECT g_equip_panel_comparison_portrait_rect;
extern s16 g_equip_panel_comparison_unit_data[];
extern world_gfx_image_load_parameters_t g_equip_panel_editor_label_cells[];
extern u16 g_equip_panel_editor_label_cluts[][2];
extern world_gfx_image_load_parameters_t g_equip_panel_editor_mode_cell[];
extern world_gfx_image_load_parameters_t g_equip_panel_editor_mode_cells[];
extern world_gfx_image_load_parameters_t g_equip_panel_editor_value_cells[];
extern u16 g_equip_panel_editor_value_cluts[][2];
extern RECT g_equip_panel_frame_rect;
extern battle_menu_status_panel_buffer_t g_equip_panel_frames_b[];
extern CVECTOR g_equip_panel_gauge_bar_colors[];
extern world_gfx_image_load_parameters_t g_equip_panel_portrait_cell[];
extern battle_menu_status_panel_editor_packet_t g_equip_panel_selected_editor_packets[2];
extern u8 g_equip_panel_selected_large_number_image[];
extern u8 g_equip_panel_selected_name_image[];
extern u8 g_equip_panel_selected_number_image[];
extern menu_number_entry_t g_equip_panel_selected_numeric_entries[];
extern battle_menu_status_panel_packet_t g_equip_panel_selected_packets[2];
extern u16 g_equip_panel_selected_portrait_image[];
extern RECT g_equip_panel_selected_portrait_rect;
extern s16 g_equip_panel_slide_down_y[];
extern s16 g_equip_panel_slide_up_y[];
extern world_gfx_image_load_parameters_t g_equip_panel_sprite_cells[];

void equip_panel_set_primitive_colors(
    battle_menu_status_panel_buffer_t* primitives, const world_status_thread_t* state);

void equip_panel_toggle_equipment_threads(s32 enable);
void equip_panel_toggle_item_numeric_thread(s32 enable);
void equip_panel_toggle_numeric_editor_thread(s32 enable);
void equip_panel_toggle_unit_status_thread(s32 enable);
void equip_panel_run_equipment_list_thread(void);

/* rearrange */
extern s8 g_equip_rearrange_active;
extern u8 g_equip_rearrange_anim_frame;
extern u16 g_equip_rearrange_picked_cursor;
extern u16 g_equip_rearrange_picked_scroll;
extern u16 g_equip_rearrange_preview_cursor;

/* slot */
typedef enum equip_slot {
    EQUIP_SLOT_RIGHT_HAND = 0,
    EQUIP_SLOT_LEFT_HAND = 1,
    EQUIP_SLOT_HEAD = 2,
    EQUIP_SLOT_BODY = 3,
    EQUIP_SLOT_ACCESSORY = 4,
    EQUIP_SLOT_HAND_COUNT = 2,
    EQUIP_SLOT_COUNT = 5,
} equip_unit_equipment_slot_e;

typedef enum equip_slot_item_result {
    EQUIP_SLOT_ITEM_CURRENT_LOCKED = -2,
    EQUIP_SLOT_ITEM_CANNOT_EQUIP = -1,
    EQUIP_SLOT_ITEM_INCOMPATIBLE_PAIR = 0,
    EQUIP_SLOT_ITEM_ALLOWED = 1,
} equip_item_slot_result_e;

extern u8 g_equip_selected_slot;

/* One-shot entry flag for this mode. */
extern s8 g_equip_slot_item_browser_initialized;

/* List index whose stat preview was last built; -1 forces a rebuild. */
extern s8 g_equip_slot_item_browser_previewed_index;
extern point16_t g_equip_slot_marker_rect;

/* text */
extern u8 g_equip_text_compact_layout;
extern u16 g_equip_text_digit_texture_page;
extern s32 g_equip_text_help_message_id;
extern s16 g_equip_text_message_thread_active;
extern u16 g_equip_text_metric_2;
extern u8 g_equip_text_thread_running_state;
extern u8 g_equip_text_data[];
extern u8 g_equip_numeric_panel_params[];
extern s32 g_equip_numeric_panel_redraw;
extern s32 g_equip_numeric_panel_style;
extern u16 g_equip_text_clut_2_mode0;
extern u16 g_equip_text_clut_2_mode1;
extern u16 g_equip_text_clut_1_mode1;
extern u16 g_equip_text_clut_1_mode0;
extern u16 g_equip_text_clut_3_mode0;
extern u16 g_equip_text_clut_3_mode1;
extern u16 g_equip_text_clut_0_mode0;
extern u16 g_equip_text_clut_0_mode1;
extern u8 g_equip_text_restore_pending;
extern u16 g_equip_text_metric_3;
extern u16 g_equip_text_metric_1;
extern u16 g_equip_text_metric_0;
extern u16 g_equip_text_metric_4;
extern u16 g_equip_text_metric_5;

/* The "%d" format string at the head of the overlay. */
extern const char g_equip_text_decimal_format[];
extern battle_menu_status_panel_glyph_t g_equip_text_decimal_glyph;

void equip_text_render_decimal_entry_list(u8* pixels, menu_number_entry_t* entries, world_glyph_blit_t* out, s32 count);

void equip_text_render_decimal_value(s32 value, s32 flags, u8* pixels, world_glyph_blit_t* position);

s32 equip_text_render_glyph_to_4bpp_image(
    s32 glyph_id, u8* image, const bunit_text_image_position_t* position, s32 style);

void equip_text_set_palette_and_metrics(s32 compact);
void equip_text_start_help_thread(s32 text_id);
void equip_text_concatenate_entries(const u8* text_table, u8* dst, s16* list, s32 separate);

void equip_text_render_encoded_ids_to_image(u8* image, battle_menu_text_image_bounds_t* rect, s32 unused, s32 max_chars,
    u8* font, s16* ids, s32 count, s16 glyph, s32 unused_flags);

void equip_text_render_id_rows_to_vram(u8* text_table, s16* list, RECT* rect, s32 style);

void equip_text_render_signed_decimal_entries(
    u8* pixels, menu_number_entry_t* entry, world_glyph_blit_t* out, s32 count);

void equip_text_show_message_with_sound(s32 message_id, s32 sound_id);
const u8* equip_text_find_entry_by_index(const u8* data, s16 count, s32 unused_2);

/* thread */
/* Slots of the equip screen's own threads. The WORLD formation screen runs
 * the same threads in other slots (world_thread_slot_e and
 * native_thread_slot_e); here BATTLE's suspended menu threads stay in 7 and 8. */
typedef enum equip_thread_slot {
    EQUIP_THREAD_SLOT_POPUP_MENU = 3,         /* the item action and type order menus */
    EQUIP_THREAD_SLOT_COMMAND_MENU_CHILD = 4, /* a wait-forever thread while a command's mode runs */
    EQUIP_THREAD_SLOT_COMMAND_MENU = 5,       /* equip_menu_icon_list_thread */
    EQUIP_THREAD_SLOT_EQUIPMENT_LIST = 10,    /* equip_panel_run_equipment_list_thread */
    EQUIP_THREAD_SLOT_ITEM_NUMERIC = 11,      /* the numeric editor thread of the item panel */
    EQUIP_THREAD_SLOT_UNIT_STATUS = 13,       /* equip_render_unit_status_panel_thread */
    EQUIP_THREAD_SLOT_COMPARISON_STATUS = 14, /* the second banner slot the indicator draw watches */
} equip_thread_slot_e;

extern u8 g_equip_thread_idle_stop_pending;
extern s32 g_equip_thread_state;
extern battle_menu_status_panel_indicator_prims_t g_equip_thread_indicator_packets[2][2];
void equip_thread_request_stop(s32 id);
void equip_text_start_character_thread_if_idle(s32 a, s32 b, s32 c, s32 d);
s32 equip_thread_start_managed(s32 thread_id, world_menu_entry_t* desc);
void equip_thread_stop_and_clear_state(s32 id);
void equip_thread_wait_forever(void);

/* unit */
/* EQUIP.OUT's 0x7A-byte unit preview, built from battle_stats_t by
 * equip_unit_build_data_from_battle_stats (0x801C6920). Field names follow
 * the battle_stats_t source; evasion_N maps to the corresponding
 * battle_stats_t equipment-stat slot. The
 * first 0x1E bytes share the unit-status-billboard layout. */
typedef struct equip_unit_data {
    s16 level;                   /* 0x00; capped at 99 */
    s16 team_kind;               /* 0x02; 1 enemy, 2 neutral */
    s16 list_index;              /* 0x04; AT-list lookup + 1, or -1 */
    s16 unit_count;              /* 0x06; units still present, set by equip_unit_load_data_from_battle_stats */
    s16 experience;              /* 0x08; capped at 99 */
    s16 unit_index;              /* 0x0a; battle_stats_t.entd_slot */
    u16 hp;                      /* 0x0c; capped at 999 */
    s16 _unused_0e;              /* 0x0e; HP gauge bonus slot */
    u16 max_hp;                  /* 0x10; capped at 999 */
    u16 mp;                      /* 0x12; capped at 999 */
    s16 _unused_14;              /* 0x14 */
    u16 max_mp;                  /* 0x16; capped at 999 */
    s16 ct;                      /* 0x18; 100 while the unit has its turn */
    s16 _unused_1a;              /* 0x1a */
    s16 max_ct;                  /* 0x1c; always 100 */
    s16 battle_id;               /* 0x1e */
    s16 _unused_20;              /* 0x20 */
    s16 entd_slot;               /* 0x22 */
    s16 job_id;                  /* 0x24 */
    s16 brave;                   /* 0x26 */
    s16 faith;                   /* 0x28 */
    s16 zodiac;                  /* 0x2a; birthday high nibble */
    s16 formation_index;         /* 0x2c */
    s16 _unused_2e;              /* 0x2e */
    s16 move;                    /* 0x30 */
    s16 speed;                   /* 0x32 */
    s16 jump;                    /* 0x34 */
    s16 right_weapon_power;      /* 0x36 */
    s16 left_weapon_power;       /* 0x38 */
    s16 right_weapon_evade;      /* 0x3a */
    s16 left_weapon_evade;       /* 0x3c */
    s16 _unused_3e;              /* 0x3e; monster-skillset flag slot */
    s16 two_handing;             /* 0x40; equip_unit_is_two_handing_weapon result */
    s16 physical_attack;         /* 0x42 */
    s16 physical_class_evade;    /* 0x44 */
    s16 physical_shield_evade;   /* 0x46 */
    s16 _unused_48;              /* 0x48; accessory evade slot */
    s16 _unused_4a;              /* 0x4a */
    s16 magic_attack;            /* 0x4c */
    s16 magical_class_evade;     /* 0x4e; cleared (lower billboard M-C-Ev) */
    s16 magical_shield_evade;    /* 0x50 */
    s16 magical_accessory_evade; /* 0x52 */
    u16 equipment[5];            /* 0x54; equip_unit_equipment_slot_e order */
    u8 _unused_5e[0x12];         /* 0x5e; ability ids and the spare tail of the stat-detail block, left zero */
    u8 equipment_categories[4];  /* 0x70 */
    u8 support_abilities[4];     /* 0x74; copy of battle_stats_t.support_abilities */
    u8 party_index;              /* 0x78; battle_stats_t.formation_index */
    u8 _padding_79;              /* tail padding to 2-byte alignment */
} equip_unit_data_t;
typedef char equip_unit_data_size_must_be_0x7a[(sizeof(equip_unit_data_t) == 0x7A) ? 1 : -1];

/* Provisional: the 0x7A-byte preview records for the selected unit (index 0)
 * and its compare slot (index 1); several callers view one as raw bytes. */
extern equip_unit_data_t* g_equip_unit_data[];
extern s16 g_equip_unit_initial_equipment[];
extern u8 g_equip_unit_selected_index;
extern u8 g_equip_unit_status_panel_params[];
extern s32 g_equip_unit_status_panel_flags;
extern s32 g_equip_unit_status_panel_redraw;
extern s8 g_equip_unit_banner_enabled;
extern world_item_stat_detail_t g_equip_unit_editor_stats;
extern s16 g_equip_unit_saved_hp;
extern s16 g_equip_unit_saved_mp;
extern equip_unit_data_t g_equip_unit_records[];
extern struct world_item_stat_summary g_equip_selected_unit_stat_summary;

void equip_item_calculate_equipment_swap_stat_delta(
    struct world_item_stat_detail* detail_total, struct world_item_stat_summary* acc, u16* froms, u16* tos);

s32 equip_unit_can_equip_item_id(s16 unit_index, s32 item_entry);
s32 equip_unit_has_two_hands(s16 unit_index);
s32 equip_unit_has_two_swords(s16 unit_index);
void equip_unit_load_data_from_battle_stats(s32 battle_id);
void equip_unit_load_selected_data(void);
void equip_unit_copy_data_to_compare_slot(s32 unused_selected_index, s32 unused_1);
s32 equip_unit_set_slot_item(s16 unit_index, s16 slot, s32 item_id);
equip_item_slot_result_e equip_unit_validate_slot_item(s16 unit_index, s16 slot, s32 item_id);
void equip_unit_build_data_from_battle_stats(struct battle_stats* stats, equip_unit_data_t* unit);
void equip_unit_commit_loadout_to_battle_stats(void);
void equip_item_sum_equipment_stat_details(world_item_stat_detail_t* total, u16* equipment);
void equip_render_unit_status_panel_thread(void);
s32 equip_unit_is_two_handing_weapon(weapon_pair_t* slots, s32 two_hands_support);

/* other */
extern u8 g_equip_sound_queued_effect_id; /* main_sfx_e */
extern u8 g_equip_sound_suppress_queued;

/* RECT[1] (right panel) of g_equip_panel_origin_offsets. Kept as its own symbol: spelling it as an element lets GCC
 * derive the table base from this address and changes equip_panel_run_ability_list_thread. */
extern RECT g_equip_right_panel_frame_rect;
extern RECT g_equip_character_status_frame_rect[];
extern RECT g_equip_character_status_draw_area_rect;
extern u8 g_equip_status_label_image[];
extern u8 g_equip_armor_bonus_render_commands[];
extern RECT g_equip_status_label_image_rect;
extern u32 g_equip_help_text_table[];
extern u16 g_equip_remove_all_equipment_ids[];
extern s8 g_equip_remove_all_equipment_prompt_active;
extern s8 g_equip_remove_mode_initialized;

/* Status image rects, the EQUIP.OUT copy of WORLD's
 * g_world_status_display_image_rects (0x8018cf68). */
extern RECT g_equip_status_display_image_rects[];
extern world_menu_entry_t g_equip_type_order_menu_hand_body;
extern world_menu_entry_t g_equip_type_order_menu_other_slots;
extern s8 g_equip_type_order_ready;

void equip_collect_item_stat_deltas(
    s32 raw_item, struct world_item_stat_summary* summary, struct world_item_stat_detail* detail, s32 alternate);

s32 equip_entrypoint(s32 unit, void* otag);

/* unnamed */
extern s16 D_801d86ac;

#endif
