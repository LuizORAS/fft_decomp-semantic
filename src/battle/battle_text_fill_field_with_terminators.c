#include "fft/battle.h"
#include "psx/types.h"

/* Fill a 16-byte text field with the terminator byte 0xfe. */
void battle_text_fill_field_with_terminators(u8* text) {
    s32 index;
    u8* cursor = text;

    /* GCC reverses this count-up loop; this form preserves the target's
     * terminator-before-counter initialization order. */
    for (index = 0; index < 16; index++) {
        *cursor++ = 0xfe;
    }
}
