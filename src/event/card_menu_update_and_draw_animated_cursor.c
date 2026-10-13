#include "fft/event_card.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Advances the animated cursor at `position` and draws it, sliding
 * horizontally, with its semi-transparent shadow. `mode` bit 0 starts the
 * reverse curve; mode 0 cancels it.
 *
 * CARD copy of bunit_menu_update_and_draw_animated_cursor. The 16-bit state
 * packs the reverse flag in bit 15 and a frame counter in bits 0-6; the
 * counter is scaled by the event speed before the keyframe lookup and
 * clamped to the end of the curve in use (0x24 reverse, wrapping at 0x2f
 * forward). */
void card_menu_update_and_draw_animated_cursor(const point16_t* position, u16* animation_state, s32 mode) {
    RECT rect;
    s32 reverse;
    u32 state_value;
    s32 frame;
    s32 table_offset;
    s32 x_offset;
    s8 raw_offset;
    u16 foreground_clut;
    u16 background_clut;

    state_value = *animation_state;
    state_value >>= 15;
    reverse = state_value;
    if (mode == 0) {
        if (state_value != 0) {
            *animation_state = 0;
            reverse = 0;
        }
    }
    state_value = reverse;
    if (state_value == 0 && (mode & 1) != 0) {
        *animation_state = 0x8000;
        reverse = 1;
    }

    frame = *animation_state;
    frame &= 0x7f;
    frame *= card_menu_get_event_speed();
    state_value = reverse;
    if (state_value != 0) {
        state_value = frame < 0x25;
        if (state_value == 0) {
            frame = 0x24;
        }
    } else {
        state_value = frame < 0x2f;
        if (state_value == 0) {
            frame = 0;
        }
    }
    state_value = reverse;
    if (state_value != 0) {
        for (table_offset = 0; g_card_menu_cursor_reverse_offset_keyframes[table_offset] < frame; table_offset += 2) { }
        raw_offset = g_card_menu_cursor_reverse_offset_keyframes[table_offset + 1];
    } else {
        for (table_offset = 0; g_card_menu_cursor_forward_offset_keyframes[table_offset] < frame; table_offset += 2) { }
        raw_offset = g_card_menu_cursor_forward_offset_keyframes[table_offset + 1];
    }
    x_offset = raw_offset;

    frame += card_menu_get_event_speed();
    state_value = reverse;
    if (state_value != 0) {
        state_value = frame < 0x25;
        if (state_value != 0) {
            (*animation_state)++;
        }
    } else {
        state_value = frame < 0x2f;
        if (state_value != 0) {
            (*animation_state)++;
        } else {
            *animation_state = 0;
        }
    }

    if (mode != 0) {
        foreground_clut = g_menu_cursor_mode1_foreground_clut;
        background_clut = g_menu_cursor_mode1_background_clut;
    } else {
        foreground_clut = g_menu_cursor_mode0_foreground_clut;
        background_clut = g_menu_cursor_mode0_background_clut;
    }

    rect.x = x_offset + position->x - 2;
    rect.y = position->y;
    rect.w = 0x10;
    rect.h = 0x10;
    card_gfx_enqueue_textured_quad(&rect, 0xa8, 0, 0, 0, g_card_menu_cursor_texture_page, foreground_clut, 0x3c);

    rect.x += 2;
    rect.y += 2;
    card_gfx_enqueue_textured_quad(&rect, 0xb8, 0, 0, 1, 0x5f, background_clut, 0x3b);
}
