#include "fft/battle.h"
#include "fft/battle_effect.h"

/* Allocate the variable-length sprite block for one effect frame.
 *
 * The eight-byte prefix is followed by one pointer per sprite part. The renderer tracks
 * both current and peak occupancy for the shared effect pool. */
effect_sprite_block_t* effect_alloc_sprite_block(u8 sprite_count) {
    effect_sprite_block_t* block;
    s32 total_count;

    block = battle_heap_alloc_block(sprite_count * sizeof(block->frames[0]) + sizeof(*block), 0);
    block->header.sprite_count = sprite_count;
    block->header.color.b = EFFECT_COLOR_NEUTRAL;
    block->header.color.g = EFFECT_COLOR_NEUTRAL;
    block->header.color.r = EFFECT_COLOR_NEUTRAL;
    total_count = g_effect_sprite_count + sprite_count;
    g_effect_sprite_count = total_count;
    if (g_effect_sprite_count_peak < total_count) {
        g_effect_sprite_count_peak = total_count;
    }
    return block;
}
