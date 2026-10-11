#include "psx/libpress.h"

/* Install the quantisation tables and upload the cached IDCT table; env->dct is ignored. */
DECDCTENV* DecDCTPutEnv(DECDCTENV* env) {
    u32* src;
    u32* dst;
    s32 i;

    src = (u32*)env->iq_y;
    dst = g_psyq_press_luma_quant_table;
    i = sizeof(g_psyq_press_luma_quant_table) / sizeof(u32) - 1;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    dst = g_psyq_press_chroma_quant_table;
    src = (u32*)env->iq_c;
    i = sizeof(g_psyq_press_chroma_quant_table) / sizeof(u32) - 1;
    do {
        *dst++ = *src++;
    } while (--i != -1);

    MDEC_in(g_psyq_press_quant_table_command,
        (sizeof(g_psyq_press_luma_quant_table) + sizeof(g_psyq_press_chroma_quant_table)) / sizeof(u32));
    MDEC_in(g_psyq_press_scale_table_command, sizeof(g_psyq_press_scale_table) / sizeof(u32));
    return env;
}
