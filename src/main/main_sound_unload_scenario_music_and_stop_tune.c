#include "fft/main.h"

/* Stop the playing scenario music, unload both scenario slots (1 and 2) and stop the tune at once
 * (main_sound_set_tune_volume(0)). The music loaders of BATTLE, WORLD, ATTACK and REQUIRE, the Game Over
 * and congratulations screens and the world map's start and end call it, as do the reset, the title and the
 * ending. */
void main_sound_unload_scenario_music_and_stop_tune(void) {
    main_sound_stop_current_music();
    main_sound_unload_scenario_mus(1);
    main_sound_unload_scenario_mus(2);
    main_sound_set_tune_volume(0);
}
