#include "fft/open.h"

/* Restore the date-selection records after leaving the confirmation prompt. */
void open_birthday_restore_date_menu(open_birthday_date_state_t* menu) {
    open_render_record_56_t* records_56 = g_open_gfx_render_records_56;
    /* The target reserves a 24-byte frame it never touches, as in the twin
     * open_birthday_build_confirmation_menu. */
    s32 unused_scratch[6];
    s32 cursor;
    s32 cursor_last;
    open_render_record_56_t* window;
    open_render_record_56_t* title;

    g_open_gfx_render_records_36[menu->cursor_record_36].sequence = 12;

    cursor = menu->cursor_record_36;
    g_open_gfx_render_records_36[cursor].anim_counter = 0;
    g_open_gfx_render_records_36[cursor].frame_index = 0;

    g_open_gfx_render_records_36[menu->label_record].color.r = 0x80;
    g_open_gfx_render_records_36[menu->label_record].color.g = 0x80;
    g_open_gfx_render_records_36[menu->label_record].color.b = 0x80;

    window = &records_56[menu->window_record_56];
    cursor_last = menu->cursor_record_36;
    title = &records_56[menu->title_record_56];
    window->palette = 0;
    title->palette = 0;
    g_open_gfx_render_records_36[cursor_last].palette = 0;
}
