#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Weather severity, the difficulty of moving through weather-affected tiles:
 * 1 for none/rain/snow, 2 for a storm or snowstorm, 3 for a strong storm or
 * snowstorm. */
s32 move_get_weather_terrain_cost(void) {
    s32 weather;

    weather = map_get_effective_weather();
    if (weather < BATTLE_WEATHER_STORM || weather == BATTLE_WEATHER_SNOW) {
        return 1;
    }
    if (weather == BATTLE_WEATHER_STORM || weather == BATTLE_WEATHER_SNOWSTORM) {
        return 2;
    }
    return 3;
}
