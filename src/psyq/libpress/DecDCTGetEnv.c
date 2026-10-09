#include "psx/libpress.h"
#include "psx/types.h"

/* Copy the three cached MDEC tables out of the library's private storage
 * into the caller supplied environment block. */
u32* DecDCTGetEnv(u32* env) {
    u32* dst;
    u32* src;
    s32 i;

    dst = env;
    src = g_psyq_press_luma_quant_table;
    i = 15;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    dst = env + 16;
    src = g_psyq_press_chroma_quant_table;
    i = 15;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    dst = env + 32;
    src = g_psyq_press_scale_table;
    i = 31;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    return env;
}
