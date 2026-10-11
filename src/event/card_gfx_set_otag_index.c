#include "fft/event_card.h"
#include "psx/types.h"

/* Select the ordering-table bucket used by subsequent menu packets. */
void card_gfx_set_otag_index(s32 value) {
    g_card_text_otag_index = value;
}
