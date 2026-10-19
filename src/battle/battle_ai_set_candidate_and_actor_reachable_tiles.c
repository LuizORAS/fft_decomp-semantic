#include "fft/battle_ai.h"

/* Make the current scenario's reachable tiles only its destination (candidate_coords) and the
 * unit's own tile.
 *
 * Integer address stages preserve the target's addition order; direct typed indexing introduces extra
 * instructions. Field offsets come from the shared workspace declaration. */
void battle_ai_set_candidate_and_actor_reachable_tiles(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 row;

    for (row = 0; row < BATTLE_AI_GRID_ROW_COUNT; row++) {
        u32 row_address = row * sizeof(u16);
        u32 scenario_address = (u32)ai + ai->movement_scenario * sizeof(ai->reachable_tiles[0]);
        *(u16*)(row_address + scenario_address + ((u32)&ai->reachable_tiles[0][0][0] - (u32)ai)) = 0;
        scenario_address = (u32)ai + ai->movement_scenario * sizeof(ai->reachable_tiles[0]);
        row_address += scenario_address;
        *(u16*)(row_address + ((u32)&ai->reachable_tiles[0][1][0] - (u32)ai)) = 0;
    }
    {
        u32 scenario_address = ai->movement_scenario * sizeof(ai->reachable_tiles[0]) + (u32)ai;
        u32 coords_address = (u32)ai + ai->movement_scenario * sizeof(ai->candidate_coords[0]);
        u32 level_address;
        u32 row_address;
        scenario_address += (u32)&ai->reachable_tiles - (u32)ai;
        level_address = *(u8*)(coords_address + ((u32)&ai->candidate_coords[0].fields.elevation - (u32)ai))
                * sizeof(ai->reachable_tiles[0][0])
            + scenario_address;
        row_address = *(u8*)(coords_address + ((u32)&ai->candidate_coords[0].fields.y - (u32)ai))
                * sizeof(ai->reachable_tiles[0][0][0])
            + level_address;
        *(u16*)row_address = 0x8000 >> *(u8*)(coords_address + ((u32)&ai->candidate_coords[0].fields.x - (u32)ai));
    }
    {
        u32 scenario_address = ai->movement_scenario * sizeof(ai->reachable_tiles[0]) + (u32)ai;
        u32 level_address;
        u32 row_address;
        scenario_address += (u32)&ai->reachable_tiles - (u32)ai;
        level_address = ai->acting_unit_coords.fields.elevation * sizeof(ai->reachable_tiles[0][0]) + scenario_address;
        row_address = ai->acting_unit_coords.fields.y * sizeof(ai->reachable_tiles[0][0][0]) + level_address;
        *(u16*)row_address |= 0x8000 >> ai->acting_unit_coords.fields.x;
    }
}
