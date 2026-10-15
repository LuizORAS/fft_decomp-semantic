#include "fft/open.h"

typedef struct open_menu_packed_halfword_pair {
    s16 first;
    s16 second;
} open_menu_packed_halfword_pair_t;

/* Passed by value in $a3 + the first stack slot; copied whole into the
 * record tail (the retail code homes $a3 and reloads both words). */
typedef struct open_menu_render_record_tail_pair {
    s32 parameter;
    void* image;
} open_menu_render_record_tail_pair_t;

/* Prepare a 56-byte render record for an image that is already in VRAM.
 *
 * first_pair is the image's VRAM position and second_pair its size. The source
 * rectangle keeps x within the 64-word texture page and doubles x and the width
 * (4-bit texels). `pair` becomes the record's CLUT position and `flags` replaces
 * the 8-bit, blank and hidden bits. The WLDCORE twin
 * wldcore_window_init_vram_render_record stores a texture page id where this one
 * stores the VRAM position. Nothing in OPEN.BIN calls it. */
void open_menu_init_render_record_fields(s32 record_index, open_menu_packed_halfword_pair_t first_pair,
    open_menu_packed_halfword_pair_t second_pair, open_menu_render_record_tail_pair_t pair, u32 flags) {
    open_menu_render_record_tail_pair_t* tail_base;
    open_menu_render_record_tail_pair_t* tail;
    RECT* quad_base;
    RECT* quad;
    CVECTOR* color_base;
    CVECTOR* color;

    g_open_gfx_render_records_56[record_index].flags
        = (g_open_gfx_render_records_56[record_index].flags
              & ~(WLDCORE_DISPLAY_OBJECT_8BIT_CLUT | WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN))
        | flags;
    /* The retail code materialises each field base from the tail before
     * adding the record stride. */
    tail_base = (open_menu_render_record_tail_pair_t*)&g_open_gfx_render_records_56[0].tail;
    tail = tail_base + record_index * (sizeof(open_render_record_56_t) / sizeof(*tail_base));
    *tail = pair;
    g_open_gfx_render_records_56[record_index].vram_x = first_pair.first;
    g_open_gfx_render_records_56[record_index].vram_y = first_pair.second;
    quad_base = (RECT*)(tail_base + 1);
    quad = quad_base + record_index * (sizeof(open_render_record_56_t) / sizeof(RECT));
    quad->x = (first_pair.first & 0x3f) * 2;
    quad->y = first_pair.second & 0xFF;
    quad->w = second_pair.first * 2;
    quad->h = second_pair.second;
    color_base = (CVECTOR*)(tail_base + 2);
    color = color_base + record_index * (sizeof(open_render_record_56_t) / sizeof(CVECTOR));
    color->r = 0x80;
    color->g = 0x80;
    color->b = 0x80;
    g_open_gfx_render_records_56[record_index].pop_in_step = 0;
}
