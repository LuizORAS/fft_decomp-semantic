#include "fft/world.h"

/* Set the event speed (g_world_event_speed). Unlike battle_script_set_event_speed, it leaves the
 * animation speed alone. */
void world_script_set_event_speed(s32 speed) {
    g_world_event_speed = speed;
}
