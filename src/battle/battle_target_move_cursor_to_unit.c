#include "fft/battle.h"
#include "fft/battle_target.h"

/* Put the map cursor on the unit's square; NULL leaves it where it is. */
void battle_target_move_cursor_to_unit(const battle_unit_t* unit) {
    if (unit != 0) {
        g_battle_cursor_x = unit->map_x;
        g_battle_cursor_y = unit->map_y;
        g_battle_cursor_z = unit->map_z;
    }
}
