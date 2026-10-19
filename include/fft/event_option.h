#ifndef EVENT_OPTION_H
#define EVENT_OPTION_H

/* EVENT/OPTION.OUT: menu screen overlay that runs with BATTLE. */

#include "fft/battle.h"

/* thread */
/* The stand-in thread that yields forever. */

void option_thread_wait_forever(void);

/* text */
/* The frame-piece blit: the source and destination rectangles of the copies the window frame threads
 * make from the menu glyph image. */

extern world_glyph_blit_t g_option_text_glyph_dest;
extern world_glyph_blit_t g_option_text_glyph_source;
void option_text_set_blit_destination(s16 destination_x, s16 destination_y);
void option_text_set_blit_source_rect(s16 source_x, s16 source_y, s16 width, s16 height);

/* menu */
/* The list windows: the scrolling list thread, its text columns and page scroll, the window frame
 * threads and their frame images, and the AT list's menu records, rows and unit view. */

extern world_menu_text_layout_t g_option_menu_at_list_entry_table[1];
extern world_menu_text_layout_t* g_option_menu_at_list_entry_table_pointer;
extern s16 g_option_menu_at_list_flags[];
extern s16 g_option_menu_at_list_indices[];
extern s16 g_option_menu_at_list_menu_type;
extern s16 g_option_menu_at_list_primary_values[];
extern s16 g_option_menu_at_list_secondary_values[];
extern world_menu_entry_t g_option_menu_at_list_state[1];
extern u8 g_option_menu_at_list_text[];
extern world_menu_panel_frame_t g_option_menu_panel_frames[2];
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
void option_menu_init_at_list(world_menu_entry_t* menu);
void option_menu_draw_text_columns(world_menu_entry_t* entry, s32* row_offset, u8* buffer);
void option_menu_start_at_list_thread(void);
void option_menu_window_frame_thread(void);
u8* option_menu_redraw_text_page_on_scroll(
    world_menu_entry_t* menu, s32* first_row, s32* render_pending, s32* unused_cursor);
void option_menu_run_list_thread(void);

/* options */
/* The game options: each option's current and default value, its value count, picker window and
 * text ids, the options list and the value picker. */

extern world_menu_text_layout_t g_option_menu_entry_table[1];
extern world_options_menu_page_t g_option_menu_pages[2];
extern world_menu_entry_t g_option_menu_icon_thread_param;
extern RECT g_option_menu_title_image_rect;
extern RECT g_option_menu_value_image_rect;
extern s32 g_option_current_values[GAME_OPTION_UNCHANGED + 1];
extern s32 g_option_reference_values[];
extern s32 g_option_choice_counts[];
extern s32 g_option_choice_window_widths[];
extern world_menu_icon_record_t g_option_picker_icon_records[2];
extern s32 g_option_value_text_id_bases[];
extern world_input_timing_profile_t g_option_input_repeat_timings[];
void option_menu_run_options_thread(void);
void option_menu_run_option_choice_thread(void);
extern world_menu_list_page_t g_option_at_list_pages[2];
extern battle_at_entry_t g_option_menu_at_list_descriptors[BATTLE_AT_LIST_CAPACITY];

/* crystal pickup */
/* The panel of the crystal or chest a dead unit leaves: its menu record, its ability list and
 * the choices it returns to BATTLE (g_battle_crystal_pickup_action). */

void option_menu_start_crystal_pickup_threads(void);
extern u32* g_option_crystal_pickup_controller_input;
extern world_menu_entry_t g_option_crystal_pickup_menu[CRYSTAL_PICKUP_MENU_ENTRY_COUNT];
extern world_menu_text_layout_t g_option_crystal_pickup_menu_entries;
extern s16 g_option_crystal_pickup_menu_flags[];
extern s16 g_option_crystal_pickup_primary_result;
extern s16 g_option_crystal_pickup_secondary_result;

/* screen */
/* The entry point that BATTLE's menu threads call with the menu to run (option_menu_type_e). */

void option_entrypoint(s32 menu_type);

#endif
