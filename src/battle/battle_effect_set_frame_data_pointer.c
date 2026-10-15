#include "fft/battle.h"
#include "psx/types.h"

/* Point g_battle_effect_frame_data at the frame data of effect file effect_id, past the section's
 * leading word: the sprite frames battle_effect_render_particle_sprite draws. */
void battle_effect_set_frame_data_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_file_headers[effect_id];
    g_battle_effect_frame_data = (u8*)header + header->frame_data + 4;
}
