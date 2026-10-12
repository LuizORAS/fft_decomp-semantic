#include "fft/battle.h"
#include "fft/battle_effect.h"

/* Release an effect-frame sprite block and update the live sprite count. */
void effect_free_sprite_block(effect_sprite_block_t* block) {
    g_effect_sprite_count -= block->header.sprite_count;
    battle_heap_free_block(block);
}
