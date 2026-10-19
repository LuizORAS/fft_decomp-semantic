#include "fft/event_debugchr.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Render signed gauge values with their positive, negative, or zero color. */
void debugchr_text_render_signed_decimal_entries(
    u8* pixels, menu_number_entry_t* entries, world_glyph_blit_t* out, s32 count) {
    s32 i;
    for (i = 0; i < count;) {
        s32 amount = *entries->value;
        s32 flags;
        out->rect.x = entries->x;
        out->rect.y = entries->y;
        entries->format &= MENU_DECIMAL_SIGN_CLEAR_MASK;
        if (amount < 0) {
            out->color = 0x88888888;
            amount = -amount;
            entries->format |= MENU_DECIMAL_SIGN_MINUS;
        } else if (amount > 0) {
            out->color = 0xcccccccc;
            entries->format |= MENU_DECIMAL_SIGN_PLUS;
        } else {
            out->color = 0;
            entries->format |= MENU_DECIMAL_SIGN_MINUS;
        }
        flags = entries->format;
        entries++;
        i++;
        menu_text_render_decimal_value(amount, flags, pixels, out);
        out->color = 0;
    }
}
