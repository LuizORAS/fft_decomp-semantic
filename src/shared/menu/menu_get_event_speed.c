#include "fft/menu.h"

/* Return the event speed, the VSync interval of the screen's frames (g_menu_event_speed; 1 when
 * unset): cursor, fade and scroll timers advance by it, so they keep their pace when the screen runs at
 * 30 frames a second. */
s32 menu_get_event_speed(void) {
    s32 speed;

    speed = g_menu_event_speed;
    if (speed == 0) {
        return 1;
    }
    return speed;
}
