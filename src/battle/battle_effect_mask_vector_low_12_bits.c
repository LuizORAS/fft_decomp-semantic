#include "fft/battle.h"
#include "psx/types.h"

/* Wrap three angles into one turn (0 to 0xfff). */
void battle_effect_mask_vector_low_12_bits(const s16* src, s16* dst) {
    dst[0] = src[0] & 0xFFF;
    dst[1] = src[1] & 0xFFF;
    dst[2] = src[2] & 0xFFF;
}
