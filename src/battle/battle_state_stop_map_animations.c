#include "fft/battle.h"

/* Stop the map's texture animations and weather sound effects. */
void battle_state_stop_map_animations(void) {
    battle_map_stop_clut_cycles_and_weather_sfx();
}
