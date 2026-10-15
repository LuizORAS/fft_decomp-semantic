#include "fft/world.h"
#include "psx/types.h"

/* Return the weather script variable (EVENT_SCRIPT_VAR_WEATHER). No retail code calls it. */
s32 world_map_get_weather(void) {
    return g_world_script_variables[EVENT_SCRIPT_VAR_WEATHER];
}
