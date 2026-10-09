#include "fft/event_card.h"
#include "psx/types.h"

typedef struct card_input_display_state {
    void* render_otag;
    u32 color; /* 0x04: copied into the poly r/g/b */
    u8 _unused_08[4];
    s16 scale_x; /* 0x0c; 4.12 sprite scale */
    u16 scale_y; /* 0x0e; 4.12 sprite scale */
    u8 _unused_10[0x10];
    u16 offset_x; /* 0x20: offset[4] of the +0x18 view, added to every quad x */
    u16 offset_y; /* 0x22: offset[5], added to every quad y */
} card_input_display_state_t;

extern card_input_display_state_t g_card_input_display_state;

void card_save_run_menu_graphic_thread(void) {
    s32 frame;

    if (g_battle_formation_screen_active != 0) {
        g_card_input_display_state.offset_x = 0;
        g_card_input_display_state.offset_y = 8;
    } else {
        g_card_input_display_state.offset_x = 0x100;
        g_card_input_display_state.offset_y = 0x80;
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
        g_card_input_display_state.render_otag = g_card_gfx_render_otag;
        if (g_card_input_display_state.scale_x > 0x1000) {
            s32 delta = g_battle_event_speed << 8;

            g_card_input_display_state.scale_x -= delta;
            g_card_input_display_state.scale_y -= delta;
        }
        g_card_input_display_state.color = 0x80;
        /* The chain builder reads this record through the shared scaled-sprite
         * view, which loads the colour word at +0x04 as a byte intensity and
         * scale_y as s16. */
        card_gfx_build_zodiac_primitives((battle_menu_status_panel_scaled_sprite_t*)&g_card_input_display_state,
            g_card_gfx_scaled_sprite_primitive_buffers[frame & 1]);
        battle_thread_yield();
        if (battle_thread_get_current_parameter_3() != 0) {
            battle_thread_exit_current();
        }
    }
}
