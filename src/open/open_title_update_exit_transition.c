#include "fft/open.h"
#include "psx/pad.h"

/* Per-frame handler of the title-exit controller (handler 12), pushed when New
 * Game is chosen with no memory card inserted.
 *
 * Cross, or Circle or Start, records the choice and fades the screen to black.
 * When the fade has ended the notice record and the controller are released: on
 * Cross the frame image saved in g_open_work_buffer_0 is copied back and the
 * title menu is pushed again; on Circle or Start the new-game prologue starts. */
void open_title_update_exit_transition(open_controller_record_t* record) {
    s32 state;
    u32 buttons;

    if ((g_open_gfx_overlay_fade.flags & OPEN_OVERLAY_FADE_ACTIVE) != 0) {
        return;
    }

    if (record->title_exit.state != OPEN_TITLE_EXIT_INPUT) {
        g_open_gfx_next_render_record_36 -= 1;
        g_open_gfx_render_record_pointer_count -= 1;
        g_open_current_controller_index -= 1;
        state = record->title_exit.state;
        if (state == OPEN_TITLE_EXIT_CANCEL) {
            s32* source;
            s32* destination;
            s32 index;

            SetDispMask(0);
            source = (s32*)g_open_work_buffer_0;
            g_open_system_runtime_flags |= OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
            destination = g_open_file_destination;
            for (index = 0; index < 0x7800; index++) {
                destination[index] = source[index];
            }
            open_title_push_menu_controller(0);
        } else if (state == OPEN_TITLE_EXIT_CONFIRM) {
            open_title_start_new_game_transition();
        }
        return;
    }

    buttons = g_open_input_new_button_presses;
    if ((buttons & PSX_PAD_CROSS) != 0) {
        main_sound_play_sfx(MAIN_SFX_CANCEL);
        open_gfx_start_overlay_fade_in(0x20);
        record->title_exit.state = OPEN_TITLE_EXIT_CANCEL;
    } else if ((buttons & (PSX_PAD_CIRCLE | PSX_PAD_START)) != 0) {
        main_sound_play_sfx(MAIN_SFX_CONFIRM);
        open_gfx_start_overlay_fade_in(0x20);
        record->title_exit.state = OPEN_TITLE_EXIT_CONFIRM;
    }
}
