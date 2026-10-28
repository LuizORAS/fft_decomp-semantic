#include "fft/battle_effect.h"

/* Copy the disc sector and the byte size of effect file effect_id from g_battle_effect_disc_entries into
 * lba and byte_length. */
void battle_effect_get_file_extent(s32 effect_id, s32* lba, s32* byte_length) {
    u32 stack_padding[2];

    *lba = g_battle_effect_disc_entries[effect_id].sector;
    *byte_length = g_battle_effect_disc_entries[effect_id].size;
}
