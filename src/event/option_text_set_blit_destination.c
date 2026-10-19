#include "fft/event_option.h"
#include "psx/types.h"

/* Set the destination origin of the frame-piece blit (g_option_text_glyph_dest) in the frame's
 * scratch image. */
void option_text_set_blit_destination(s16 destination_x, s16 destination_y) {
    g_option_text_glyph_dest.rect.x = destination_x;
    g_option_text_glyph_dest.rect.y = destination_y;
}
