#include "fft/battle_move.h"
#include "psx/types.h"

/* Place a unit that shares a tile with a mover delta screen units from the point
 * (x, y) along g_move_displacement_direction, at the ground height there,
 * and mark its shadow for redrawing.
 *
 * y is the map depth axis (screen.vz). Dead, crystal, treasure and jumping units
 * are left where they are. */
void move_displace_unit_along_step_direction(battle_unit_t* unit, s32 x, s32 y, s32 delta) {
    s16 axis;
    s16 screen_z;

    if ((unit->status_flags_1_4 & BATTLE_UNIT_STATUS_DEAD) != 0) {
        return;
    }
    if ((unit->status_flags_5_6 & (BATTLE_UNIT_STATUS_CRYSTAL | BATTLE_UNIT_STATUS_TREASURE | BATTLE_UNIT_STATUS_JUMP))
        != 0) {
        return;
    }
    switch (g_move_displacement_direction) {
    case MOVE_DIRECTION_SOUTH:
        axis = y - delta;
        unit->screen.vz = axis;
        unit->real.vz = axis << 12;
        break;
    case MOVE_DIRECTION_NORTH:
        axis = y + delta;
        unit->screen.vz = axis;
        unit->real.vz = axis << 12;
        break;
    case MOVE_DIRECTION_EAST:
        axis = x + delta;
        unit->screen.vx = axis;
        unit->real.vx = axis << 12;
        break;
    case MOVE_DIRECTION_WEST:
        axis = x - delta;
        unit->screen.vx = axis;
        unit->real.vx = axis << 12;
        break;
    }
    screen_z = battle_gfx_calculate_screen_z_from_misc_screen_data(unit);
    unit->screen.vy = screen_z;
    unit->real.vy = screen_z << 12;
    unit->shadow_dirty |= 1;
}
