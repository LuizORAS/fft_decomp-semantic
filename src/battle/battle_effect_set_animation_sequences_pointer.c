#include "fft/battle_effect.h"
#include "psx/types.h"

/* Point g_battle_effect_animation_sequences at the animation sequences of effect file effect_id, past
 * the section's leading 32-bit value: the frame scripts battle_effect_init_animation_sequence starts a sprite node on.
 */
void battle_effect_set_animation_sequences_pointer(s16 effect_id) {
    effect_file_header_t* header;

    header = g_battle_effect_file_headers[effect_id];
    g_battle_effect_animation_sequences = (u8*)header + header->animation_sequences + 4;
}
