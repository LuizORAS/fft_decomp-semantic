#include "fft/battle.h"
#include "psx/types.h"

/* Point a sprite animation node at parameter set animation_index and restart
 * it: clear the sequence position and hold time, store frame_group doubled and
 * set kind bit 0 to mark the frame dirty. */
void battle_effect_init_animation_sequence(effect_list_node_t* state, s32 frame_group, s32 animation_index) {
    u8* sequences;
    u8* entry;
    s32 offset;
    u16 flags;

    sequences = g_battle_effect_animation_sequences;
    entry = (u8*)(((animation_index << 16) >> 15) + (s32)sequences);
    flags = state->kind | EFFECT_LIST_NODE_FRAME_CHANGED;
    offset = entry[0] + (entry[1] << 8);
    state->sequence_offset = 0;
    state->frame_group_index = (frame_group & 0xFF) * 2;
    state->frame_timer = 0;
    state->kind = flags;
    /* Integer addition preserves the original register order. */
    state->sequence_data = (u8*)((s16)offset + (s32)sequences);
}
