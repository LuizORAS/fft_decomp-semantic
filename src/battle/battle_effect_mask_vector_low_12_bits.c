#include "fft/battle.h"
#include "psx/types.h"

/* Wrap three angles into one turn (0 to 0xfff). */
void battle_effect_mask_vector_low_12_bits(const SVECTOR* src, SVECTOR* dst) {
    dst->vx = src->vx & 0xFFF;
    dst->vy = src->vy & 0xFFF;
    dst->vz = src->vz & 0xFFF;
}
