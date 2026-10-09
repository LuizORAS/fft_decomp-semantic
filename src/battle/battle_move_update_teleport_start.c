#include "fft/battle.h"
struct battle_unit_misc_data;

/* Step handler of the TELEPORT_START phase. */
void battle_move_update_teleport_start(struct battle_unit_misc_data* unit) {
    battle_move_start_teleport_animation(unit);
}
