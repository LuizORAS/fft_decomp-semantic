#include "fft/wldcore.h"
#include "psx/types.h"

/* Step the screen fade one frame. Fading out, the screen brightness (WORLD's menu brightness and
 * g_wldcore_screen_brightness_rgb) falls from 0x70 to 0 while the subtractive fill box 0 rises from
 * 0x20 to 0xFF; fading in, the brightness rises from 0x10 to 0x80 and the box falls from 0xC0 to 0. A
 * WLDCORE_SCREEN_FADE_REDRAW_MAP fade sets MAIN_SYSTEM_FLAG_DRAW_MAP each step. One step before the
 * end it clears MAIN_SYSTEM_FLAG_FADING and waits for the audio queue; the end clears RUNNING,
 * REDRAW_MAP and BOX_VISIBLE. Returns 1 while a step ran. */
s32 wldcore_gfx_step_screen_fade(void) {
    u32 flags;
    s32 elapsed;
    s32 brightness;
    s32 box_level;

    flags = g_wldcore_screen_fade_state.flags[0];
    if (flags & WLDCORE_SCREEN_FADE_RUNNING) {
        if (g_wldcore_screen_fade_state.elapsed == g_wldcore_screen_fade_state.duration) {
            g_wldcore_screen_fade_state.flags[0] = flags
                & ~(WLDCORE_SCREEN_FADE_RUNNING | WLDCORE_SCREEN_FADE_REDRAW_MAP | WLDCORE_SCREEN_FADE_BOX_VISIBLE);
        } else {
            if (g_wldcore_screen_fade_state.elapsed + 1 == g_wldcore_screen_fade_state.duration) {
                g_main_system_flags &= ~MAIN_SYSTEM_FLAG_FADING;
                wldcore_sound_wait_for_queue_drain();
            }
            if (g_wldcore_screen_fade_state.flags[0] & WLDCORE_SCREEN_FADE_OUT) {
                elapsed = g_wldcore_screen_fade_state.elapsed;
                brightness = 0x70 - (elapsed << 7) / g_wldcore_screen_fade_state.duration;
                box_level = (elapsed << 8) / g_wldcore_screen_fade_state.duration + 0x20;
                if (brightness < 0) {
                    brightness = 0;
                }
                if (box_level >= 0x100) {
                    box_level = 0xFF;
                }
                if (elapsed < 0) {
                    brightness = 0x80;
                    box_level = 0;
                }
            } else {
                elapsed = g_wldcore_screen_fade_state.elapsed;
                brightness = (elapsed << 7) / g_wldcore_screen_fade_state.duration + 0x10;
                box_level = 0xC0 - (elapsed << 8) / g_wldcore_screen_fade_state.duration;
                if (brightness > 0x80) {
                    brightness = 0x80;
                }
                if (box_level < 0) {
                    box_level = 0;
                }
                if (elapsed < 0) {
                    brightness = 0;
                    box_level = 0x80;
                }
            }
            g_wldcore_screen_fade_state.elapsed++;
            world_menu_set_brightness(brightness, brightness, brightness);
            g_wldcore_screen_brightness_rgb.r = brightness;
            g_wldcore_screen_brightness_rgb.g = brightness;
            g_wldcore_screen_brightness_rgb.b = brightness;
            g_wldcore_screen_fade_state.boxes[0].r = box_level;
            g_wldcore_screen_fade_state.boxes[0].g = box_level;
            g_wldcore_screen_fade_state.boxes[0].b = box_level;
            if (g_wldcore_screen_fade_state.flags[0] & WLDCORE_SCREEN_FADE_REDRAW_MAP) {
                g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
            }
            return 1;
        }
    }
    return 0;
}
