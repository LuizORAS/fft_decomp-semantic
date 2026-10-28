#include "fft/battle_move.h"

/* Selects the slope bits of the two step tiles that face the movement
 * direction (top two bits of g_battle_move_step_value) and stores them as the unit's
 * per-step scales; a rider or a g_battle_move_step_value bit 0x10 step clears them. */
void battle_move_set_unit_step_slope_scales(battle_unit_t* unit) {
    battle_move_get_current_and_destination_tiles(unit, &g_battle_move_current_tile, &g_battle_move_destination_tile);
    battle_gfx_check_tile_status_palette_mod(unit, g_battle_move_destination_tile->flags.shading, 1);
    switch (g_battle_move_step_value.direction) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        unit->destination_edge_height = g_battle_move_destination_edge_height
            = (g_battle_move_destination_tile->slope_type & 0xC) >> 2;
        unit->current_edge_height = g_battle_move_current_edge_height = g_battle_move_current_tile->slope_type & 3;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        unit->destination_edge_height = g_battle_move_destination_edge_height
            = g_battle_move_destination_tile->slope_type & 3;
        unit->current_edge_height = g_battle_move_current_edge_height
            = (g_battle_move_current_tile->slope_type & 0xC) >> 2;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        unit->destination_edge_height = g_battle_move_destination_edge_height
            = g_battle_move_destination_tile->slope_type >> 6;
        unit->current_edge_height = g_battle_move_current_edge_height
            = (g_battle_move_current_tile->slope_type & 0x30) >> 4;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        unit->destination_edge_height = g_battle_move_destination_edge_height
            = (g_battle_move_destination_tile->slope_type & 0x30) >> 4;
        unit->current_edge_height = g_battle_move_current_edge_height = g_battle_move_current_tile->slope_type >> 6;
        break;
    }
    if (unit->standing_on_battle_id_plus_one != 0) {
        unit->current_edge_height = g_battle_move_current_edge_height = 0;
    }
    if (g_battle_move_step_value.mount) {
        unit->destination_edge_height = g_battle_move_destination_edge_height = 0;
    }
}
