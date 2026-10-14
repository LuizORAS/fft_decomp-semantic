#include "fft/open.h"

/* New Game on the title menu. With a memory card in either slot (open_card_check_slots) Alazlam's
 * prologue starts at once (open_title_start_new_game_transition). With none, the title picture (0x7800 words of
 * the frame image) is saved to g_open_work_buffer_0 and blanked, and the memory card warning (animation 0x12:
 * "You need a Memory card (with at least 1 block free) to save your current game") is shown on the black
 * screen under the card warning controller (handler 12). */
void open_title_start_new_game(void) {
    s32* source;
    s32* destination;
    s32 index;
    s32 record;
    s32* position;
    s32 controller;
    s32 _card_status;
    s32 frame_padding[2];

    _card_status = open_card_check_slots();
    index = 0;
    if (_card_status != 0) {
        open_title_start_new_game_transition();
        return;
    }

    source = g_open_file_destination;
    destination = (s32*)g_open_work_buffer_0;
    do {
        *destination = *source;
        index++;
        *source = 0;
        source++;
        destination++;
    } while (index < 0x7800);

    open_gfx_start_overlay_fade_out(0x10);
    record = open_gfx_append_animated_render_record(
        g_open_gfx_render_record_pointers, &g_open_gfx_render_record_pointer_count);
    g_open_gfx_render_records_36[record].anim_id = 0x12;
    g_open_gfx_render_records_36[record].frame_timer = 0;
    g_open_gfx_render_records_36[record].frame_index = 0;
    position = &g_open_gfx_render_records_36[record].x;
    controller = g_open_current_controller_index;
    position[0] = -4;
    position[1] = -16;
    g_open_controller_records_next[controller].card_warning.state = 0;
    g_open_controller_handler_indices[controller] = OPEN_CONTROLLER_CARD_WARNING;
    g_open_current_controller_index = controller + 1;
}
