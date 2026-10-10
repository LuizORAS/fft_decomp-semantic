#include "fft/battle.h"
#include "psx/types.h"

enum {
    BATTLE_AI_BASE_HIT_PERCENT = 100,
    BATTLE_AI_TILE_ROW_BYTE_STRIDE = 32,
    BATTLE_AI_TILE_LEVEL_BYTE_STRIDE = 576,
};

/* 0x5f4; kept as a separate integer term so the target's addu/addiu order
 * survives (a typed &ai->tile_priority[...] folds the offset first). */
#define BATTLE_AI_TILE_PRIORITY_OFFSET ((u32) & ((battle_ai_data_t*)0)->tile_priority)

/* Make the movement baseline the score of only moving to the current scenario's destination: rank 0,
 * 100% hit and the tile's standing score (tile_priority holds it negated).
 * battle_ai_insert_ranked_action keeps only actions that rank above it. */
void battle_ai_set_movement_baseline(void) {
    battle_ai_data_t* ai;
    battle_ai_coords_t* coords;
    u8* level_row;
    u8 scenario;
    u32 row;

    ai = &g_battle_ai_data_base;
    scenario = g_battle_ai_data_base.movement_scenario;
    g_battle_ai_data_base.movement_baseline_action.plan_chosen = 0;
    g_battle_ai_data_base.movement_baseline_action.base_hit_percent = BATTLE_AI_BASE_HIT_PERCENT;
    coords = &ai->candidate_coords[scenario];
    level_row = (u8*)ai + coords->bytes.elevation * BATTLE_AI_TILE_LEVEL_BYTE_STRIDE + BATTLE_AI_TILE_PRIORITY_OFFSET;
    row = coords->bytes.y * BATTLE_AI_TILE_ROW_BYTE_STRIDE + (u32)level_row;
    g_battle_ai_data_base.movement_baseline_action.priority = -*(u16*)(coords->bytes.x * 2 + row);
}
