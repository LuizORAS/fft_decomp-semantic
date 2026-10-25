#include "fft/battle_move.h"
#include "psx/types.h"

/* Place a unit at the tile center (x, y) moved by delta along g_battle_move_displacement_direction
 * (0 -y, 1 +x, 2 +y, 3 -x; y is the depth axis, world_coords.vz), at the height of the ground there,
 * and mark its shadow for redraw. Dead, crystallized, treasure and jumping units stay put. */
void battle_move_displace_unit_along_step_direction(battle_unit_t* unit, s32 x, s32 y, s32 delta) {
    s16 axis;
    s16 screen_z;

    if ((unit->status_flags_1_4 & BATTLE_UNIT_STATUS_DEAD) != 0) {
        return;
    }
    if ((unit->status_flags_5_6 & (BATTLE_UNIT_STATUS_CRYSTAL | BATTLE_UNIT_STATUS_TREASURE | BATTLE_UNIT_STATUS_JUMP))
        != 0) {
        return;
    }
    switch (g_battle_move_displacement_direction) {
    case BATTLE_MOVE_DIRECTION_SOUTH:
        axis = y - delta;
        unit->world_coords.vz = axis;
        unit->real.vz = axis * ONE;
        break;
    case BATTLE_MOVE_DIRECTION_NORTH:
        axis = y + delta;
        unit->world_coords.vz = axis;
        unit->real.vz = axis * ONE;
        break;
    case BATTLE_MOVE_DIRECTION_EAST:
        axis = x + delta;
        unit->world_coords.vx = axis;
        unit->real.vx = axis * ONE;
        break;
    case BATTLE_MOVE_DIRECTION_WEST:
        axis = x - delta;
        unit->world_coords.vx = axis;
        unit->real.vx = axis * ONE;
        break;
    }
    screen_z = battle_gfx_calculate_world_height_from_world_coords(unit);
    unit->world_coords.vy = screen_z;
    unit->real.vy = screen_z * ONE;
    unit->shadow_dirty |= 1;
}
