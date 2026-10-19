#include "fft/event_equip.h"
#include "fft/menu.h"
#include "fft/menu_script.h"
#include "psx/types.h"

/* Point the scrollable list menu at a new -1 terminated entry list and reset
   its render state. Type debt (QUIRKS.md): text_table is a u8* text section
   read as 16-bit values. */
void equip_menu_init_scrollable_list_core(s16* entries, s32 selected_index, u8* text_table) {
    g_equip_menu_list_entries = entries;
    g_equip_menu_list_text_table = text_table;

    if (entries != 0) {
        g_equip_menu_list_entry_count = 0;
        while (entries[g_equip_menu_list_entry_count] != -1) {
            g_equip_menu_list_entry_count = g_equip_menu_list_entry_count + 1;
        }
    }

    g_equip_menu_selected_list_index = selected_index;
    if (selected_index == 0) {
        g_menu_scroll_base_index = 0;
    }

    g_menu_gfx_sprite_color.r = 0x80;
    g_menu_gfx_sprite_color.g = 0x80;
    g_menu_gfx_sprite_color.b = 0x80;
    g_menu_gfx_otag_index = 0;
    g_menu_scroll_list_active = 0;
    g_menu_list_scroll_offset = 0;
    g_equip_menu_list_redraw_pending = 1;
    g_equip_gfx_transition_frame = 0;
    g_equip_gfx_zoom_draw_area_active = 0;
}
