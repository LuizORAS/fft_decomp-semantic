#include "fft/battle_ai.h"

/*
 * Build the considered origin's targetable-tile mask on both map levels.
 *
 * Modes 0, 1, and 2 select range, area, or their sum; other modes use radius.
 * Radius 16 or greater fills both masks without applying the linear filter.
 * Radii 8 through 15 read the following status-priority weights instead of
 * row templates. This helper does not guard those inputs; caller restrictions
 * are not established.
 */
void ai_build_targetable_tile_mask(ai_tile_mask_mode_e mode, s32 radius) {
    ai_data_t* ai = &g_ai_data_base;
    s32 i;
    s32 x;
    s32 y;

    /* Preserve the target's signed mode dispatch. */
    switch ((s32)mode) {
    case AI_TILE_MASK_RANGE:
        radius = g_ai_data_base.considered_ability.parameters.fields.range;
        break;
    case AI_TILE_MASK_AOE:
        radius = g_ai_data_base.considered_ability.parameters.fields.aoe;
        break;
    case AI_TILE_MASK_RANGE_PLUS_AOE:
        radius = g_ai_data_base.considered_ability.parameters.fields.range
            + g_ai_data_base.considered_ability.parameters.fields.aoe;
        break;
    }
    if (radius >= 16) {
        for (i = 0; i < AI_GRID_ROW_COUNT; i++) {
            ai->targetable_tiles[0][i] = 0xffff;
            ai->targetable_tiles[1][i] = 0xffff;
        }
        return;
    }
    for (i = AI_GRID_ROW_COUNT - 1; i >= 0; i--)
        ai->targetable_tiles[0][i] = 0;
    y = ai->considered_unit_coords.fields.y;
    x = ai->considered_unit_coords.fields.x;
    for (i = 0; i <= radius; i++) {
        if (y - i < 0)
            break;
        if (x < 8)
            ai->targetable_tiles[0][y - i] = g_ai_range_row_masks[radius - i] << (7 - x);
        else
            ai->targetable_tiles[0][y - i] = g_ai_range_row_masks[radius - i] >> (x - 7);
    }
    for (i = 1; i <= radius; i++) {
        if (y + i >= ai->map_max_y)
            break;
        if (x < 8)
            ai->targetable_tiles[0][y + i] = g_ai_range_row_masks[radius - i] << (7 - x);
        else
            ai->targetable_tiles[0][y + i] = g_ai_range_row_masks[radius - i] >> (x - 7);
    }
    if (ai->considered_ability.ai_flags.word & AI_ABILITY_LINEAR_TRAJECTORY) {
        for (y = 0; y < ai->map_max_y; y++) {
            i = ai->targetable_tiles[0][y];
            if (i != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    if ((i << x) & 0x8000) {
                        if (x != ai->considered_unit_coords.fields.x && y != ai->considered_unit_coords.fields.y)
                            ai->targetable_tiles[0][y] ^= 0x8000 >> x;
                    }
                }
            }
        }
    }
    ai_copy_u16_values(ai->targetable_tiles[1], ai->targetable_tiles[0], sizeof(ai->targetable_tiles[0]));
}
