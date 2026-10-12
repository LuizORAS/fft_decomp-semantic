#include "fft/battle_script.h"
#include "fft/event_bunit.h"
#include "psx/types.h"

/* Keep the menu event speed and the event-speed step synchronized; a zero
 * argument uses the minimum event speed of 1. */
void bunit_menu_set_vsync_mode_and_event_speed(s32 speed) {
    g_bunit_menu_event_speed = speed;
    if (speed == 0) {
        speed = 1;
    }
    script_set_event_speed(speed);
}
