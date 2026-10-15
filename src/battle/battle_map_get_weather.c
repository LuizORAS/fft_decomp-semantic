#include "fft/battle.h"
#include "psx/types.h"

/* The weather script variable as the event set it, before battle_map_get_effective_weather applies
 * the map's indoor and snow flags. No C code calls it. */
s32 battle_map_get_weather(void) {
    return g_battle_script_variables[EVENT_SCRIPT_VAR_WEATHER];
}
