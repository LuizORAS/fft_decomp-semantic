#include "fft/event_equip.h"
#include "psx/types.h"

/* Select the ordering-table bucket used by subsequent menu packets. */
void equip_gfx_set_otag_index(s32 value) {
    g_equip_gfx_sprite_ot_index = value;
}
