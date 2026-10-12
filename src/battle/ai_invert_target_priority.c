#include "fft/battle_ai.h"
#include "psx/types.h"

enum {
    AI_BASE_HIT_PERCENT = 100,
    AI_TILE_ROW_BYTE_STRIDE = AI_GRID_COLUMN_COUNT * sizeof(s16),
    AI_TILE_LEVEL_BYTE_STRIDE = AI_GRID_ROW_COUNT * AI_TILE_ROW_BYTE_STRIDE,
};

/* 0x5f4; kept as a separate integer term so the target's addu/addiu order
 * survives (a typed &ai->tile_priority[...] folds the offset first). */
#define AI_TILE_PRIORITY_OFFSET ((u32) & ((ai_data_t*)0)->tile_priority)

/*
 * Copies the candidate tile's priority into the inverted-priority scratch
 * action with its sign flipped.  tile_priority is a
 * grid of 16 columns x 18 signed 16-bit values per map level. The lookup reads
 * the stored 16-bit pattern before negating them.
 */
void ai_invert_target_priority(void) {
    ai_data_t* ai;
    ai_coords_t* coords;
    u8* level_row;
    u8 scenario;
    u32 row;

    ai = &g_ai_data_base;
    scenario = g_ai_data_base.movement_scenario;
    g_ai_data_base.inverted_priority_action.rank.decided = 0;
    g_ai_data_base.inverted_priority_action.rank.base_hit_percent = AI_BASE_HIT_PERCENT;
    coords = &ai->candidate_coords[scenario];
    level_row = (u8*)ai + coords->bytes.elevation * AI_TILE_LEVEL_BYTE_STRIDE + AI_TILE_PRIORITY_OFFSET;
    row = coords->bytes.y * AI_TILE_ROW_BYTE_STRIDE + (u32)level_row;
    g_ai_data_base.inverted_priority_action.rank.priority = -*(u16*)(coords->bytes.x * 2 + row);
}
