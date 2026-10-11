#include "fft/event_require.h"
#include "psx/types.h"

/* Render gauge values with an explicit sign, colored by sign, and a zero placeholder. */
void require_text_render_decimal_entry_list(u8* pixels, menu_number_entry_t* entries, glyph_blit_t* output, s32 count) {
    s32 i;

    for (i = 0; i < count; entries++, i++, output->color = 0) {
        s32 value = *entries->value;
        s32 flags;
        output->rect.x = entries->x;
        output->rect.y = entries->y;
        flags = entries->format;
        flags &= MENU_DECIMAL_SIGN_CLEAR_MASK;
        if (value < 0) {
            value = -value;
            flags |= MENU_DECIMAL_SIGN_MINUS;
            output->color = 0x77777777;
        } else if (value > 0) {
            flags |= MENU_DECIMAL_SIGN_PLUS;
            output->color = 0xbbbbbbbb;
        } else {
            output->color = 0;
            flags &= MENU_DECIMAL_FIELD_WIDTH_CLEAR_MASK;
            flags |= MENU_DECIMAL_ZERO_PLACEHOLDER_FLAGS;
        }
        require_text_render_decimal_value(value, flags, pixels, output);
    }
}
