#include "fft/open.h"
#include "psx/pad.h"

/* Per-frame handler of the memory card warning (handler 12), which New Game pushes when no card answers.
 *
 * Cross, or Circle or Start, records the choice and fades the screen to black. Once the fade has ended the
 * warning and the controller are released: Cross copies the title picture back from g_open_work_buffer_0 and
 * rebuilds the title menu, Circle or Start starts the prologue without a card. */
void open_title_update_card_warning(open_title_card_warning_t* warning) {
    s32 state;
    u32 buttons;

    if ((g_open_gfx_overlay_fade.flags & OPEN_OVERLAY_FADE_ACTIVE) != 0) {
        return;
    }

    if (warning->state != OPEN_TITLE_CARD_WARNING_INPUT) {
        g_open_gfx_next_render_record_36 -= 1;
        g_open_gfx_render_record_pointer_count -= 1;
        g_open_current_controller_index -= 1;
        state = warning->state;
        if (state == OPEN_TITLE_CARD_WARNING_BACK) {
            s32* source;
            s32* destination;
            s32 index;

            SetDispMask(0);
            index = 0;
            source = (s32*)g_open_work_buffer_0;
            g_open_system_runtime_flags |= OPEN_SYSTEM_RUNTIME_FLAG_DISPLAY_BLANKED;
            destination = g_open_file_destination;
            do {
                *destination = *source;
                source++;
                index++;
                destination++;
            } while (index < 0x7800);
            open_title_push_menu_controller(0);
        } else if (state == OPEN_TITLE_CARD_WARNING_CONTINUE) {
            open_title_start_new_game_transition();
        }
        return;
    }

    buttons = g_open_input_new_button_presses;
    if ((buttons & PSX_PAD_CROSS) != 0) {
        main_sound_play_sfx(MAIN_SFX_CANCEL);
        open_gfx_start_overlay_fade_in(0x20);
        warning->state = OPEN_TITLE_CARD_WARNING_BACK;
    } else if ((buttons & (PSX_PAD_CIRCLE | PSX_PAD_START)) != 0) {
        main_sound_play_sfx(MAIN_SFX_CONFIRM);
        open_gfx_start_overlay_fade_in(0x20);
        warning->state = OPEN_TITLE_CARD_WARNING_CONTINUE;
    }
}
