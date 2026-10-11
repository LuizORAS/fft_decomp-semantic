#ifndef EVENT_OPTION_H
#define EVENT_OPTION_H

/* EVENT/OPTION.OUT: menu screen overlay that runs with BATTLE. */

#include "fft/battle.h"

/* dead unit */

extern u32* g_option_dead_unit_controller_input;
extern dead_unit_menu_t g_option_dead_unit_menu;
extern dead_unit_entry_counts_t g_option_dead_unit_menu_entries;
extern s16 g_option_dead_unit_menu_flags[];
extern s16 g_option_dead_unit_primary_result;
extern s16 g_option_dead_unit_secondary_result;

/* gfx */
extern const u16 g_option_gfx_draw_area_scale_percent[12];
extern RECT g_option_gfx_draw_area_template;

void option_gfx_build_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* packet, const void* source,
    s32 scale_index, s32 lower_half, const world_status_thread_t* thread);

void option_gfx_init_scaled_draw_area_packets(world_gfx_scaled_draw_area_pair_t* packet);

/* menu */
/* Record types hoisted out of the defining .c files so that callers and
 * definitions agree on one spelling. */

/* OPTION's view of the 0x3c-byte menu record (g_battle_menu_thread_menu_data);
 * field names follow world_menu_entry_t. */
typedef struct option_at_menu {
    s16 vram_x;       /* 0x00: text-image VRAM x */
    s16 vram_y;       /* 0x02: text-image VRAM y */
    s16 inner_width;  /* 0x04 */
    s16 inner_height; /* 0x06 */
    s16 window_x;     /* 0x08 */
    s16 window_y;     /* 0x0a */
    s16 window_width; /* 0x0c */
    u8 _unused_0e[6];
    s16 overall_width; /* 0x14 */
    u8 _unused_16[0x1a];
    world_menu_text_layout_t* layout; /* 0x30 */
} option_at_menu_t;

extern const u8 g_option_menu_at_list_entry_table[];
extern const void* g_option_menu_at_list_entry_table_pointer;
extern s16 g_option_menu_at_list_flags[];
extern s16 g_option_menu_at_list_indices[];
extern s16 g_option_menu_at_list_menu_type;
extern s16 g_option_menu_at_list_primary_values[];
extern s16 g_option_menu_at_list_secondary_values[];
extern option_at_menu_t g_option_menu_at_list_state[];
extern u8 g_option_menu_at_list_text[];
extern battle_at_entry_t g_option_menu_at_list_descriptors[40];
extern world_menu_text_layout_t g_option_menu_entry_table[1];
extern world_panel_frame_t g_option_menu_panel_frames[2];
extern u8 g_option_menu_panel_pixels[];

/* One libgpu texture window per window frame DR_MODE packet. The static
 * tables already hold each window's w/h (0x10x0x10, 8x0x10, 0x10x8 and two
 * full 0x100x0x100 windows); the panels fill in only the runtime x/y.
 * Indexing the array is what reproduces the target's symbol-plus-byte-offset
 * addressing. */
extern RECT g_option_menu_panel_texture_windows[7];
extern RECT g_option_menu_thin_panel_texture_windows[7];
extern u8 g_option_menu_render_buffer[];
extern s32 g_option_menu_state;
extern u16 g_option_menu_transition_duration;
extern s16 g_option_menu_transition_timer;
extern RECT g_option_menu_entry_panel_rect;
extern world_menu_entry_t g_option_menu_icon_thread_param;
extern RECT g_option_menu_title_image_rect;
extern RECT g_option_menu_value_image_rect;
void option_menu_init_at_list(option_at_menu_t* menu);
void option_menu_draw_text_columns(world_menu_entry_t* entry, s32* row_offset, u8* buffer);
void option_menu_start_at_list_thread(void);
void option_menu_start_dead_unit_threads(void);
void option_menu_window_frame_thread(void);
void* option_menu_redraw_text_page_on_scroll(
    world_menu_entry_t* menu, s32* first_row, s32* render_pending, s32* unused_cursor);

/* text */

extern glyph_blit_t g_option_text_glyph_dest;
extern glyph_blit_t g_option_text_glyph_source;
void option_text_set_blit_destination(s16 destination_x, s16 destination_y);
void option_text_set_blit_source_rect(s16 source_x, s16 source_y, s16 width, s16 height);

/* other */
extern s32 g_option_current_values[GAME_OPTION_UNCHANGED + 1];
extern s32 g_option_reference_values[];
extern s32 g_option_choice_counts[];
extern s32 g_option_choice_window_widths[];
extern world_menu_icon_record_t g_option_picker_icon_records[2];
extern s32 g_option_value_text_id_bases[];
extern world_input_timing_profile_t g_option_input_repeat_timings[];
void option_build_at_list(void);
void option_build_options_menu(void);
void option_thread_wait_forever(void);
void option_run_option_choice_menu(void);
void option_entrypoint(s32 menu_type);

#endif
