#include "fft/event_require.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Render signed gauge values with their positive, negative, or zero color. */
void require_text_render_signed_decimal_entries(
    u8* pixels, menu_number_entry_t* entries, glyph_blit_t* output, s32 count) {
    s32 i;

    for (i = 0; i < count;) {
        s32 value = *entries->value;
        s32 flags;
        output->rect.x = entries->x;
        output->rect.y = entries->y;
        entries->format &= MENU_DECIMAL_SIGN_CLEAR_MASK;
        if (value < 0) {
            output->color = 0x88888888;
            value = -value;
            entries->format |= MENU_DECIMAL_SIGN_MINUS;
        } else if (value > 0) {
            output->color = 0xcccccccc;
            entries->format |= MENU_DECIMAL_SIGN_PLUS;
        } else {
            output->color = 0;
            entries->format |= MENU_DECIMAL_SIGN_MINUS;
        }
        flags = entries->format;
        entries++;
        i++;
        menu_text_render_decimal_value(value, flags, pixels, output);
        output->color = 0;
    }
}
