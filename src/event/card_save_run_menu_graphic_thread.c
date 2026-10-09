#include "fft/event_card.h"
#include "psx/types.h"

void card_save_run_menu_graphic_thread(void) {
    s32 frame;

    if (g_battle_formation_screen_active != 0) {
        g_card_input_display_state.offset.x = 0;
        g_card_input_display_state.offset.y = 8;
    } else {
        g_card_input_display_state.offset.x = 0x100;
        g_card_input_display_state.offset.y = 0x80;
    }

    /* Start at 2.0 (4.12) and shrink to 1.0 below, or start at 1.0. */
    if (battle_thread_get_current_parameter_1() != 0) {
        g_card_input_display_state.scale_x = 0x1000;
        g_card_input_display_state.scale_y = 0x1000;
    } else {
        g_card_input_display_state.scale_x = 0x2000;
        g_card_input_display_state.scale_y = 0x2000;
    }

    for (frame = 0;; frame++) {
        g_card_input_display_state.ot = g_card_gfx_render_otag;
        if (g_card_input_display_state.scale_x > 0x1000) {
            s32 delta = g_battle_event_speed << 8;

            g_card_input_display_state.scale_x -= delta;
            g_card_input_display_state.scale_y -= delta;
        }
        g_card_input_display_state.brightness = 0x80;
        card_gfx_build_zodiac_primitives(
            &g_card_input_display_state, g_card_gfx_scaled_sprite_primitive_buffers[frame & 1]);
        battle_thread_yield();
        if (battle_thread_get_current_parameter_3() != 0) {
            battle_thread_exit_current();
        }
    }
}
