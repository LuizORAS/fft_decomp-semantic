#include "fft/open.h"

/* Build the three-entry sound-type menu and select the saved option.
 *
 * The target reaches three fields through a pointer to the record or to its
 * x member (base register plus offset) and the others as plain array members
 * (absolute address plus index); the spellings below keep that split. The
 * selected render-record index is likewise read through the controller
 * record, while sound_mode_selection and step are addressed through
 * g_open_controller_selected_options. */
void open_menu_push_sound_type_controller(void) {
    s32 i;
    s32 record;
    /* Pin required: unpinned, the controller index and the stream base swap
     * $a0 and $a1. */
    register s32 controller_index __asm__("$4");
    s32 sound_type;
    s32 selected_record;
    open_sound_menu_state_t* menu;

    for (i = 0; i < 3; i++) {
        record = open_gfx_append_render_record_36(
            g_open_gfx_render_record_pointers, &g_open_gfx_render_record_pointer_count);
        ((open_sound_menu_state_t*)&g_open_controller_stream_start[g_open_current_controller_index])->render_records[i]
            = record;
        g_open_gfx_render_records_36[record].ot_layer = 4;
        g_open_gfx_render_records_36[record].anim_id = i + 7;
        (&g_open_gfx_render_records_36[record])->frame_timer = 0;
        g_open_gfx_render_records_36[record].frame_index = 0;
        ((wldcore_window_xy_t*)&g_open_gfx_render_records_36[record].x)->y = i * 0xc + 0x1e;
        ((wldcore_window_xy_t*)&g_open_gfx_render_records_36[record].x)->x = -0x14;
    }

    g_open_controller_selected_options[g_open_current_controller_index * 25 + 1] = 0;
    sound_type = main_sound_get_type();
    controller_index = g_open_current_controller_index;
    g_open_controller_selected_options[controller_index * 25] = sound_type;
    menu = (open_sound_menu_state_t*)&g_open_controller_stream_start[controller_index];
    sound_type <<= 2;
    /* The explicit byte offset preserves the target's index shift before the
     * menu-base addition; direct array indexing schedules those instructions
     * in the opposite order. */
    sound_type += (s32)menu;
    selected_record = *(s32*)sound_type;
    g_open_current_controller_index = controller_index + 1;
    g_open_gfx_render_records_36[selected_record].palette = 9;
    g_open_controller_handler_indices[controller_index] = 3;
}
