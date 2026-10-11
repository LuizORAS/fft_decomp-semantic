#include "fft/event_jobstts.h"

/* Rebase a scrollable list by one page while preserving its selected row. */
void jobstts_menu_scroll_list_by_page(jobstts_menu_page_scroll_direction_e direction, const menu_list_record_t* menu) {
    u8 unused[8];
    s32 previous_base;
    s16 base;
    s16 row_offset;
    s16 limit;
    s32 entry_count;
    s32 next;
    s16 count;

    base = g_jobstts_menu_scroll_base_index;
    previous_base = base;
    row_offset = g_jobstts_menu_list_selected_index - base;
    if (direction == JOBSTTS_PAGE_SCROLL_UP) {
        g_jobstts_menu_scroll_base_index = base - menu->visible_rows;
        if (g_jobstts_menu_scroll_base_index < 0) {
            g_jobstts_menu_scroll_base_index = 0;
        }
    } else if (direction == JOBSTTS_PAGE_SCROLL_DOWN) {
        g_jobstts_menu_scroll_base_index = menu->visible_rows + base;
        limit = g_jobstts_menu_list_entry_count;
        entry_count = limit;
        next = g_jobstts_menu_scroll_base_index + 1;
        if (entry_count - next < menu->visible_rows) {
            g_jobstts_menu_scroll_base_index = limit - menu->visible_rows;
        }
    }

    g_jobstts_menu_list_selected_index = g_jobstts_menu_scroll_base_index;
    jobstts_menu_draw_scrollable_list((const u8*)menu);
    g_jobstts_menu_list_selected_index = row_offset + g_jobstts_menu_scroll_base_index;
    count = g_jobstts_menu_list_entry_count;
    if (g_jobstts_menu_list_selected_index >= count) {
        g_jobstts_menu_list_selected_index = count - 1;
    }
    if (previous_base != g_jobstts_menu_scroll_base_index) {
        g_jobstts_sound_queued_effect_id = MAIN_SFX_CURSOR_MOVE;
    }
}
