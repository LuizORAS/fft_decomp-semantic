#include "fft/battle_script.h"
#include "fft/menu.h"

/* Store `speed` unchanged as the menu event speed and set the script event
 * speed to it, with 0 raised to the minimum of 1. */
void menu_set_vsync_mode_and_event_speed(s32 speed) {
    g_menu_event_speed = speed;
    if (speed == 0) {
        speed = 1;
    }
    battle_script_set_event_speed(speed);
}
