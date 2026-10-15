#include "fft/wldcore.h"

/* The window size text `value` needs: its widest line plus 24, rounded down to a multiple of 4, and 16
 * pixels per line plus 16. Nothing calls it.
 *
 * The mask is written 0xfffc (not ~3) to keep the target's `andi`. */
void wldcore_text_get_padded_dimensions(s32 value, wldcore_text_dimensions_t* output) {
    s16 maximum_line_width;
    s16 line_count;
    s32 height;

    world_text_measure_pixels(&maximum_line_width, &line_count, world_text_find_entry(value));
    height = line_count * 16 + 16;
    output->width = (maximum_line_width + 24) & 0xfffc;
    output->height = height;
}
