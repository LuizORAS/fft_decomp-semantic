#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Debug print: FntPrint `value` and then `value` + 1 in decimal, as battle_text_print_fnt_value_and_next
 * does. Nothing calls it. */
void world_text_print_fnt_value_and_next(s32 value) {
    char* fmt = &g_world_text_decimal_format[0];

    FntPrint(fmt, value);
    value = value + 1;
    FntPrint(fmt, value);
}
