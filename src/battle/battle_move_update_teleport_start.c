#include "fft/battle_move.h"

/* Step handler of the TELEPORT_START phase. */
void battle_move_update_teleport_start(battle_unit_t* unit) {
    battle_move_start_teleport_animation(unit);
}
