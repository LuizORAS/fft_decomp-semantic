#ifndef EVENT_HELPMENU_H
#define EVENT_HELPMENU_H

/* EVENT/HELPMENU.OUT: menu screen overlay that runs with BATTLE. */

#include "fft/battle.h"

extern s16 g_helpmenu_menu_cursor_tile[];
extern u8 g_helpmenu_menu_text_id_tables[];
void helpmenu_menu_run_require_help(void);
void helpmenu_menu_close(void);

extern u8 g_helpmenu_active_banner[];
extern u8 g_helpmenu_active_unit_data[];

/* 0x40-byte billboard filled by battle_menu_copy_billboard_data_out; +0x0e is the monster-skillset flag. */
extern battle_menu_status_panel_slot_state_t g_helpmenu_billboard;
extern help_navigation_record_t g_helpmenu_require_navigation[];

/* 0x80 bytes of saved g_battle_text_section_pointers, i.e. 32 pointers. */
extern u8* g_helpmenu_text_saved_pointers[32];
extern s16 g_helpmenu_selected_unit_panel_mode;

/* Section offsets of the help text file, relative to g_helpmenu_text_data (0x80 bytes before it). */
extern s32 g_helpmenu_text_section_offsets[32];
extern u8 g_helpmenu_text_data[];
extern u32* g_helpmenu_controller_state_ptr;
extern u16* g_helpmenu_current_text_ids;
extern help_navigation_record_t g_helpmenu_view_mode_2_navigation[];
extern help_navigation_record_t g_helpmenu_view_mode_3_navigation[];
extern help_navigation_record_t g_helpmenu_view_mode_4_navigation[];
s32 helpmenu_thread_find_first_running_menu_thread(void);
void helpmenu_menu_run_battle_help(void);

#endif
