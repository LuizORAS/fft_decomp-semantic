#include "fft/world.h"
#include "psx/types.h"

/* Render save slot `save_slot_index`'s two-line description into its 0x32 x 0x20 strip of VRAM:
 * slots 0..5 at x 0x240, 6..10 at x 0x1c0 and 11..14 at x 0x180, 0x20 rows apart. WORLD's copy of
 * card_save_render_slot_text, whose first column sits at x 0x100.
 *
 * Each column sets the whole rectangle; the compiler merges the three tails. */
void world_card_draw_slot_text(s32 save_slot_index) {
    RECT destination_rect;

    if (save_slot_index < 6) {
        destination_rect.x = 0x240;
        destination_rect.y = (save_slot_index << 5) + 0x130;
        destination_rect.w = 0x32;
        destination_rect.h = 0x20;
    } else if (save_slot_index < 11) {
        destination_rect.x = 0x1C0;
        destination_rect.y = (save_slot_index << 5) - 0x80;
        destination_rect.w = 0x32;
        destination_rect.h = 0x20;
    } else {
        destination_rect.x = 0x180;
        destination_rect.y = (save_slot_index << 5) - 0x100;
        destination_rect.w = 0x32;
        destination_rect.h = 0x20;
    }
    world_text_render_id_list_to_image_rows(g_world_card_save_slot_descriptions[save_slot_index],
        g_world_card_slot_description_row_ids, &destination_rect, 0);
}
