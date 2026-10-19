#include "fft/script.h"
#include "fft/world.h"

/* Set the event speed (g_script_event_speed). Unlike battle_script_set_event_speed, it leaves the
 * animation speed alone. */
void world_script_set_event_speed(s32 speed) {
    g_script_event_speed = speed;
}
