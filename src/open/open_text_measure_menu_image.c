#include "fft/open.h"

void open_text_measure_menu_image(s32 combined_text_index, wldcore_text_dimensions_t* dimensions) {
    s16 columns;
    s16 rows;
    const u8* text;

    text = world_text_find_entry(combined_text_index);
    world_text_measure(&columns, &rows, text);
    dimensions->width = ((columns * 10) + 24) & 0xfffc;
    dimensions->height = (rows << 4) + 16;
}
