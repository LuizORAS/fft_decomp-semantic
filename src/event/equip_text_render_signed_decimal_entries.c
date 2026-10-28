#include "fft/event_equip.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Render signed gauge values with their positive, negative, or zero color. */
void equip_text_render_signed_decimal_entries(
    u8* pixels, menu_number_entry_t* entry, world_glyph_blit_t* out, s32 count) {
    s32 i;
    s32 value;

    for (i = 0; i < count; i++) {
        value = *entry->value;
        out->rect.x = entry->x;
        out->rect.y = entry->y;
        entry->format &= MENU_DECIMAL_SIGN_CLEAR_MASK;
        if (value < 0) {
            out->color = 0x88888888;
            value = -value;
            entry->format |= MENU_DECIMAL_SIGN_MINUS;
        } else if (value > 0) {
            out->color = 0xcccccccc;
            entry->format |= MENU_DECIMAL_SIGN_PLUS;
        } else {
            out->color = 0;
            entry->format |= MENU_DECIMAL_SIGN_MINUS;
        }
        menu_text_render_decimal_value(value, entry->format, pixels, out);
        out->color = 0;
        entry++;
    }
}
