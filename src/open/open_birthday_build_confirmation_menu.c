#include "fft/open.h"

/* Dim the date entry behind the confirmation prompt: the date cursor switches to animation 10 in palette 7,
 * the label drops to half brightness (0x40), and the title and date windows take palette 2.
 * open_birthday_restore_date_menu undoes it.
 *
 * The target reserves a 24-byte frame it never touches (the stack adjustment
 * is even scheduled into the last load-delay slot), so the original body
 * declared an unused aggregate local of 17..24 bytes. */
void open_birthday_build_confirmation_menu(open_birthday_date_state_t* state) {
    s32 unused_scratch[6];

    g_open_gfx_render_records_36[state->cursor_record_36].anim_id = 10;
    g_open_gfx_render_records_36[state->cursor_record_36].frame_index
        = g_open_gfx_render_records_36[state->cursor_record_36].frame_timer = 0;

    g_open_gfx_render_records_36[state->label_record].color.r = 0x40;
    g_open_gfx_render_records_36[state->label_record].color.g = 0x40;
    g_open_gfx_render_records_36[state->label_record].color.b = 0x40;

    g_open_gfx_render_records_36[state->cursor_record_36].palette = 7;

    g_open_gfx_render_records_56[state->title_record_56].palette = 2;
    g_open_gfx_render_records_56[state->window_record_56].palette = 2;
}
