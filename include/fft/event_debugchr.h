#ifndef EVENT_DEBUGCHR_H
#define EVENT_DEBUGCHR_H

/* EVENT/DEBUGCHR.OUT: menu screen overlay that runs with BATTLE. */

#include "fft/battle.h"

/* editor */
/* Inclusive value range for one editable field row. */
typedef struct debug_range {
    s32 lo; /* 0x00 */
    s32 hi; /* 0x04 */
} debug_range_t;

extern s32 g_debugchr_editor_active;
extern u8 g_debugchr_editor_unit_name[];
extern s16 g_debugchr_editor_first_maximum;
extern u8 g_debugchr_editor_secondary_skillset_name[];
extern u8 g_debugchr_editor_jump;
extern u8 g_debugchr_editor_move;
extern menu_number_entry_t g_debugchr_editor_numeric_descriptor_a[];
extern menu_number_entry_t g_debugchr_editor_numeric_descriptor_b[];
extern RECT g_debugchr_editor_numeric_entries_a;
extern RECT g_debugchr_editor_numeric_entries_b;
extern RECT g_debugchr_editor_numeric_entries_c;
extern RECT g_debugchr_editor_numeric_entries_d;
extern RECT g_debugchr_editor_numeric_entries_e;
extern RECT g_debugchr_editor_numeric_entries_f;
extern point16_t g_debugchr_editor_numeric_geometry[];
extern menu_status_panel_numeric_buffer_t g_debugchr_editor_numeric_state_a[2];
extern menu_status_panel_numeric_buffer_t g_debugchr_editor_numeric_state_b[2];
extern battle_menu_status_panel_numeric_geometry_t g_debugchr_editor_numeric_table;
extern u8 g_debugchr_editor_numeric_text_a[];
extern u8 g_debugchr_editor_numeric_text_b[];
extern image_load_parameters_t g_debugchr_editor_numeric_texture[];
extern u8 g_debugchr_editor_job_name[];
extern s16 g_debugchr_editor_second_maximum;
extern s32 g_debugchr_editor_selected_unit_slot;
extern u8 g_debugchr_editor_speed;

/* 0x801c5e34; per-row value ranges, copied from the _normal or _special table. */
extern debug_range_t g_debugchr_editor_template[21];
extern u8 g_debugchr_editor_template_normal[];
extern u8 g_debugchr_editor_template_special[];
extern u8 g_debugchr_editor_primary_skillset_name[];
extern s16 g_debugchr_editor_third_maximum;
extern menu_status_panel_slot_storage_t g_debugchr_editor_unit_fields;
extern u8 g_debugchr_editor_unit_ids[];

/* Views of g_debugchr_panel_selected_billboard.bars[i].value (values[3 * i])
 * and bars[i].limit (the *_maximum words): debugchr_editor_apply_values_to_battle_unit
 * reaches the values from their own base register and the limits by absolute
 * address, which the member spelling does not reproduce. */
extern s16 g_debugchr_editor_values[];
extern help_navigation_record_t* g_debugchr_editor_current_field;
extern POLY_FT4 g_debugchr_editor_cursor_polys[];
extern POLY_FT4 g_debugchr_editor_cursor_shade_polys[];
extern s32 g_debugchr_editor_field_edit_active;
extern help_navigation_record_t* g_debugchr_editor_field_table;
extern help_navigation_record_t g_debugchr_editor_fields[];
void debugchr_editor_load_selected_unit(void);
void debugchr_editor_run_numeric_thread(void);
void debugchr_editor_run_unit_thread(s32 unit_id);
void debugchr_editor_apply_values_to_battle_unit(s32 unit_id);

/* gfx */
extern const u16 g_debugchr_gfx_draw_area_scale_percent[12];
extern RECT g_debugchr_gfx_draw_area_template;
extern u16 g_debugchr_gfx_draw_offset_y;
extern RECT g_debugchr_gfx_portrait_origin;
extern u8 g_debugchr_gfx_portrait_transition_offsets[];
extern POLY_FT4 g_debugchr_gfx_render_buffers[2][30];
extern u8 g_debugchr_gfx_scaled_sprite_count;
extern point16_t g_debugchr_gfx_scaled_sprite_scale[];
extern main_zodiac_sprite_frame_t g_debugchr_gfx_scaled_sprite_uv_rects[];
extern point16_t g_debugchr_gfx_scaled_sprite_texture_page;
void debugchr_gfx_build_item_graphic_descriptor(menu_icon_source_t* output, u32 item_id);
void debugchr_gfx_build_portrait_poly_ft4(s32 portrait_id, POLY_FT4* poly);

void debugchr_gfx_build_scaled_draw_area_packets(scaled_draw_area_pair_t* packet, const RECT* source, s32 scale_index,
    s32 lower_half, const menu_status_thread_t* thread);

void debugchr_gfx_build_status_group_primitives(POLY_FT4* poly);
void debugchr_gfx_init_menu_tile_and_line_primitives(menu_status_panel_buffer_t* menu);
void debugchr_gfx_init_scaled_draw_area_packets(scaled_draw_area_pair_t* packet);

void debugchr_gfx_set_scaled_poly_ft4_geometry(POLY_FT4* poly, const point16_t* texture_page, const RECT* position,
    main_zodiac_sprite_frame_t* sprite_rect, point16_t* scale, main_zodiac_draw_offset_t* base);

void debugchr_gfx_set_scaled_poly_ft4_geometry_and_uv(POLY_FT4* poly, const point16_t* texture, const RECT* position,
    const main_zodiac_sprite_frame_t* uv_rect, const point16_t* scale, const main_zodiac_draw_offset_t* offset);

void debugchr_gfx_clip_portrait_poly_from_left(POLY_FT4* poly, s32 amount);
void debugchr_gfx_clip_portrait_poly_from_right(POLY_FT4* poly, s32 amount);
void debugchr_gfx_set_clut_rect_from_id(RECT* rect, s32 packed);

void debugchr_menu_select_primitive_color_palette(
    menu_palette_primitives_t* output, const menu_status_thread_t* context);

void debugchr_gfx_build_portrait_transition_primitives(const RECT* texture_rect, s32* transition,
    const s32* first_portrait, const s32* second_portrait, palette_t palettes[2], POLY_FT4* poly, s32 direction);

/* menu */
extern s32 g_debugchr_menu_indicator_brightness[2];
extern s32 g_debugchr_menu_indicator_fade_work[2];
extern s32 g_debugchr_menu_indicator_packet_index[2];
extern s32 g_debugchr_menu_indicator_state[2];

/* panel */
extern menu_unit_status_record_t g_debugchr_panel_comparison_billboard;
extern menu_status_thread_t g_debugchr_status_display_thread_params;
extern menu_status_thread_t g_debugchr_numeric_editor_thread_params;
extern menu_status_thread_t g_debugchr_character_status_frame_config;
extern menu_unit_status_record_t g_debugchr_panel_selected_billboard;
extern u8 g_debugchr_panel_status_animation[];
extern u8 g_debugchr_panel_status_group_bounds[];
extern u8 g_debugchr_panel_status_group_count;
extern u8 g_debugchr_panel_status_group_ids[];
extern main_zodiac_draw_offset_t g_debugchr_panel_status_offsets[];
extern point16_t g_debugchr_panel_status_scales[];
extern main_zodiac_sprite_frame_t g_debugchr_panel_status_uv_rects[];
extern const u16 g_debugchr_panel_status_y_offsets[];

/* Selected-unit panel record at 0x801c5324; the comparison panel's twin
 * follows at 0x801c5334. */
extern menu_active_unit_t g_debugchr_panel_selected_unit_data;
extern menu_active_unit_t g_debugchr_panel_comparison_unit_data;
extern s16 g_debugchr_panel_slide_up_y[];
extern point16_t g_debugchr_panel_item_icon_texture[];
extern s16 g_debugchr_panel_slide_down_y[];
extern image_load_parameters_t g_debugchr_panel_editor_label_cells[];
extern image_load_parameters_t g_debugchr_panel_editor_mode_cell[];
extern image_load_parameters_t g_debugchr_panel_editor_value_cells[];
extern RECT g_debugchr_panel_frame_rect;
extern image_load_parameters_t g_debugchr_panel_sprite_cells[];
extern image_load_parameters_t g_debugchr_panel_portrait_cell[];
extern u16 g_debugchr_panel_editor_value_cluts[][2];
extern u16 g_debugchr_panel_editor_label_cluts[][2];
extern image_load_parameters_t g_debugchr_panel_editor_mode_cells[];
extern RECT g_debugchr_panel_selected_portrait_rect;
extern RECT g_debugchr_panel_comparison_portrait_rect;
extern RECT g_debugchr_panel_origin_offsets[];
extern RECT g_debugchr_panel_text_upload_rect_a;
extern RECT g_debugchr_panel_text_upload_rect_b;
extern image_load_parameters_t g_debugchr_panel_label_layouts_mode0[];
extern image_load_parameters_t g_debugchr_panel_label_layouts_mode1[];
extern image_load_parameters_t g_debugchr_panel_label_layouts_mode2[];
extern image_load_parameters_t g_debugchr_panel_label_layouts_mode3[];
extern image_load_parameters_t g_debugchr_panel_item_icon_layouts[];
extern menu_status_panel_editor_packet_t g_debugchr_panel_selected_editor_packets[2];
extern menu_status_panel_packet_t g_debugchr_panel_selected_packets[2];
extern menu_status_panel_editor_packet_t g_debugchr_panel_comparison_editor_packets[2];
extern menu_status_panel_packet_t g_debugchr_panel_comparison_packets[2];
extern u8 g_debugchr_panel_selected_number_image[];
extern u8 g_debugchr_panel_selected_name_image[];
extern u8 g_debugchr_panel_selected_large_number_image[];
extern u8 g_debugchr_panel_comparison_number_image[];
extern u8 g_debugchr_panel_comparison_name_image[];
extern u8 g_debugchr_panel_comparison_large_number_image[];
extern palette_t g_debugchr_panel_selected_portrait_palettes[2];
extern palette_t g_debugchr_panel_comparison_portrait_palettes[2];
extern menu_status_panel_buffer_t g_debugchr_panel_frames_a[];
extern u8 g_debugchr_panel_text_image_a[];
extern u8 g_debugchr_panel_text_image_b[];
extern menu_number_entry_t g_debugchr_panel_comparison_numeric_entries[];
extern CVECTOR g_debugchr_panel_gauge_bar_colors[];
extern menu_number_entry_t g_debugchr_panel_selected_numeric_entries[];
extern const point16_t g_debugchr_panel_status_texture;
void debugchr_menu_copy_unit_data_to_status_billboard(battle_stats_t* unit, menu_unit_status_record_t* output);

void debugchr_panel_set_primitive_colors(menu_status_panel_buffer_t* primitives, const menu_status_thread_t* state);

void debugchr_panel_run_character_status_thread(void);

/* text */
extern glyph_blit_t g_debugchr_text_decimal_glyph;
void debugchr_text_clear_string_buffer(u8* output);

void debugchr_text_render_decimal_entry_list(u8* pixels, menu_number_entry_t* entries, glyph_blit_t* output, s32 count);

void debugchr_text_render_decimal_value(s32 value, s32 flags, u8* pixels, glyph_blit_t* position);

void debugchr_text_render_signed_decimal_entries(
    u8* pixels, menu_number_entry_t* entries, glyph_blit_t* output, s32 count);

/* thread */
extern menu_gradient_panel_prims_t g_debugchr_thread_indicator_packets[2][2];
s32 debugchr_thread_is_running(s32 thread_id);

/* other */
extern u32* g_debugchr_input_controller;
extern RECT g_debugchr_character_status_frame_rect[];
extern RECT g_debugchr_character_status_draw_area_rect;
void debugchr_render_unit_status_panel_thread(void);
void debugchr_run_character_debug_session(void);

#endif
