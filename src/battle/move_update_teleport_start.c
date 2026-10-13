#include "fft/battle_move.h"
struct battle_unit;

/* Step handler of the TELEPORT_START phase. */
void move_update_teleport_start(struct battle_unit* unit) {
    move_start_teleport_animation(unit);
}
