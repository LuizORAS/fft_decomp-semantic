#include "fft/event_equip.h"

/* Move a scrollable list by one page while preserving its visible row. */
void equip_menu_scroll_list_by_page(s32 direction, const menu_list_record_t* menu) {
    u8 unused[8];
    s32 previous_base;
    s16 base;
    s16 row_offset;
    s16 limit;
    s32 entry_count;
    s32 next;
    s16 count;

    base = g_equip_menu_scroll_base_index;
    previous_base = base;
    row_offset = g_equip_menu_selected_list_index - base;
    if (direction == -1) {
        g_equip_menu_scroll_base_index = base - menu->visible_rows;
        if (g_equip_menu_scroll_base_index < 0) {
            g_equip_menu_scroll_base_index = 0;
        }
    } else if (direction == 1) {
        g_equip_menu_scroll_base_index = menu->visible_rows + base;
        limit = g_equip_menu_list_entry_count;
        entry_count = limit;
        next = g_equip_menu_scroll_base_index + 1;
        if (entry_count - next < menu->visible_rows) {
            g_equip_menu_scroll_base_index = limit - menu->visible_rows;
        }
    }

    g_equip_menu_selected_list_index = g_equip_menu_scroll_base_index;
    equip_menu_draw_scrollable_list((const u8*)menu);
    g_equip_menu_selected_list_index = row_offset + g_equip_menu_scroll_base_index;
    count = g_equip_menu_list_entry_count;
    if (g_equip_menu_selected_list_index >= count) {
        g_equip_menu_selected_list_index = count - 1;
    }
    if (previous_base != g_equip_menu_scroll_base_index) {
        g_equip_sound_queued_effect_id = MAIN_SFX_CURSOR_MOVE;
    }
}
