#include "fft/battle.h"
#include "psx/types.h"

/* Step the screen colour-modulation fade once a frame: add each channel's Q8 step to its accumulator,
 * take the high byte as the colour in effect, and on the last frame set the target colour.
 *
 * The fade state: the target colour, the colour in effect, a Q8 accumulator and per-frame Q8 step per
 * channel, and the frames left. The three-element shape is what makes GCC hold the base addresses of the
 * current-colour and accumulator arrays in registers for element 0. */
void battle_gfx_update_screen_color_modulation_fade(void) {
    u16 count;
    u16 remaining;
    u16 red;
    u16 green;
    u16 blue;

    count = g_battle_gfx_screen_color_modulation_fade_frames;
    if (count != 0) {
        remaining = count - 1;
        g_battle_gfx_screen_color_modulation_fade_frames = remaining;
        red = g_battle_gfx_screen_color_modulation_value[0] + g_battle_gfx_screen_color_modulation_step[0];
        g_battle_gfx_screen_color_modulation_value[0] = red;
        green = g_battle_gfx_screen_color_modulation_value[1] + g_battle_gfx_screen_color_modulation_step[1];
        g_battle_gfx_screen_color_modulation_value[1] = green;
        blue = g_battle_gfx_screen_color_modulation_value[2] + g_battle_gfx_screen_color_modulation_step[2];
        g_battle_gfx_screen_color_modulation_value[2] = blue;
        g_battle_gfx_screen_color_modulation[0] = red >> 8;
        g_battle_gfx_screen_color_modulation[1] = green >> 8;
        g_battle_gfx_screen_color_modulation[2] = blue >> 8;
        if (remaining == 0) {
            g_battle_gfx_screen_color_modulation[0] = g_battle_gfx_screen_color_modulation_target[0];
            g_battle_gfx_screen_color_modulation[1] = g_battle_gfx_screen_color_modulation_target[1];
            g_battle_gfx_screen_color_modulation[2] = g_battle_gfx_screen_color_modulation_target[2];
        }
    }
}
