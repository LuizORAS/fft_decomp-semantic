#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Thread body of the zodiac backdrop behind the card screen and WLDCORE's screen transitions:
 * it draws the zodiac (world_gfx_build_zodiac_primitives) every frame, zooming from double size to
 * normal when the first thread parameter is 0, until the third parameter asks it to stop. It sits
 * higher when the menu frame runs inside WLDCORE or OPEN (g_world_menu_hosted_frame). */
void world_menu_zodiac_backdrop_thread(void) {
    s32 frame;

    if (g_world_menu_hosted_frame != 0) {
        g_world_menu_zodiac_backdrop.offset.position.x = 0;
        g_world_menu_zodiac_backdrop.offset.position.y = 8;
    } else {
        g_world_menu_zodiac_backdrop.offset.position.x = 0x100;
        g_world_menu_zodiac_backdrop.offset.position.y = 0x80;
    }
    if (world_thread_get_current_parameter_1() == 0) {
        g_world_menu_zodiac_backdrop.scale_x = 0x2000;
        g_world_menu_zodiac_backdrop.scale_y = 0x2000;
    } else {
        g_world_menu_zodiac_backdrop.scale_x = ONE;
        g_world_menu_zodiac_backdrop.scale_y = ONE;
    }
    for (frame = 0;; frame++) {
        g_world_menu_zodiac_backdrop.ot = g_world_menu_zodiac_backdrop_otag;
        if (g_world_menu_zodiac_backdrop.scale_x > ONE) {
            s32 step = g_script_event_speed << 8;
            g_world_menu_zodiac_backdrop.scale_x -= step;
            g_world_menu_zodiac_backdrop.scale_y -= step;
        }
        g_world_menu_zodiac_backdrop.brightness = g_world_menu_color_red;
        world_gfx_build_zodiac_primitives(&g_world_menu_zodiac_backdrop, g_world_menu_zodiac_backdrop_quads[frame & 1]);
        thread_yield();
        if (world_thread_get_current_parameter_3() != 0) {
            thread_exit_current();
        }
    }
}
