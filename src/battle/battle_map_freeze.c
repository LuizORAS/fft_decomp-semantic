#include "fft/battle.h"

/* Pause every map texture animation (their modes are saved and cleared) until
 * battle_map_unfreeze. Ability effects and the event script use the pair. */
void battle_map_freeze(void) {
    battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_DISABLE_ALL_TEXTURE_ANIMATIONS, 0, 0, 0);
}
