#include "psx/libpress.h"

/* Install the caller supplied quantisation tables into the library's cache
 * and push them and the IDCT table to the MDEC. */
u32* DecDCTPutEnv(u32* env) {
    u32* src;
    u32* dst;
    s32 i;

    src = env;
    dst = g_psyq_press_luma_quant_table;
    i = 15;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    dst = g_psyq_press_chroma_quant_table;
    src = env + 16;
    i = 15;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    MDEC_in(g_psyq_press_quant_table_command, 0x20);
    MDEC_in(g_psyq_press_scale_table_command, 0x20);
    return env;
}
