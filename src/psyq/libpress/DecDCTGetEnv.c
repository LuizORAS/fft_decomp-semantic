#include "psx/libpress.h"
#include "psx/types.h"

/* Copy the three cached MDEC tables out of the library's private storage
 * into the caller supplied environment block. */
DECDCTENV* DecDCTGetEnv(DECDCTENV* env) {
    u32* dst;
    u32* src;
    s32 i;

    dst = (u32*)env->iq_y;
    src = g_psyq_press_luma_quant_table;
    i = sizeof(g_psyq_press_luma_quant_table) / sizeof(u32) - 1;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    dst = (u32*)env->iq_c;
    src = g_psyq_press_chroma_quant_table;
    i = sizeof(g_psyq_press_chroma_quant_table) / sizeof(u32) - 1;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    dst = (u32*)env->dct;
    src = g_psyq_press_scale_table;
    i = sizeof(g_psyq_press_scale_table) / sizeof(u32) - 1;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    return env;
}
