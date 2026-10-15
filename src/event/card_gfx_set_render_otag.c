#include "fft/event_card.h"
#include "psx/types.h"

/* Set the ordering-table entry that CARD's graphic thread gives the input display
 * (g_card_gfx_render_otag); card_save_run_menu sets entry 1 of the current buffer. */
void card_gfx_set_render_otag(u32* otag) {
    g_card_gfx_render_otag = otag;
}
