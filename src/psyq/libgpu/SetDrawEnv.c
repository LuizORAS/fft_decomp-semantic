#include "psx/libgpu.h"

/* LIBGPU, retail 0x80025524-0x800257c8. Builds environment and optional background commands. */
void SetDrawEnv(DR_ENV* input_packet, DRAWENV* input_env) {
    DRAWENV* env = input_env;
    DR_ENV* packet = input_packet;
    psyq_gpu_rect_words_t rectangle;
    int index;
    int value;
    int dimension;
    /* The shared clamp limit remains in a0 during background-packet construction. */
    register int maximum __asm__("$4");
    u16* limit;
    __asm__(""
        : "=r"(env), "=r"(packet)
        : "0"(env), "1"(packet)); /* Preserve the original two saved argument copies. */
    packet->code[0] = get_cs(env->clip.x, env->clip.y);
    packet->code[1] = get_ce((s16)(env->clip.w + env->clip.x - 1), (s16)(env->clip.y + env->clip.h - 1));
    packet->code[2] = get_ofs(env->ofs[0], env->ofs[1]);
    packet->code[3] = get_mode(env->dfe, env->dtd, env->tpage);
    packet->code[4] = get_tw(&env->tw);
    packet->code[5] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK);
    index = 7;
    if (env->isbg) {
        rectangle.rect.x = env->clip.x;
        rectangle.rect.y = env->clip.y;
        value = (u16)env->clip.w;
        rectangle.rect.w = value;
        dimension = (u16)env->clip.h;
        rectangle.rect.h = dimension;
        value = (u32)value << 16;
        dimension = value >> 16;
        value = 0;
        if (dimension >= 0) {
            limit = &g_psyq_gpu_vram_width;
            __asm__("" : "=r"(limit) : "0"(limit)); /* Retail materializes the dimension limit address. */
            value = *limit;
            __asm__("" : "=r"(value) : "0"(value));
            value = (s16)value;
            maximum = value - 1;
            if (maximum < dimension)
                dimension = maximum;
            value = dimension;
        }
        rectangle.rect.w = value;
        dimension = rectangle.rect.h;
        if (dimension >= 0) {
            limit = &g_psyq_gpu_vram_height;
            __asm__("" : "=r"(limit) : "0"(limit));
            value = *limit;
            __asm__("" : "=r"(value) : "0"(value));
            value = (s16)value;
            maximum = value - 1;
            if (maximum < dimension)
                dimension = maximum;
            value = dimension;
        } else
            value = 0;
        rectangle.rect.h = value;
        if ((rectangle.rect.x & 63) || (rectangle.rect.w & 63)) {
            rectangle.rect.x -= env->ofs[0];
            rectangle.rect.y -= env->ofs[1];
            ((u32*)packet)[index++]
                = (((((u32)env->b0 << 16) | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_TILE)) | ((u32)env->g0 << 8))
                    | env->r0);
            ((u32*)packet)[index++] = rectangle.words[0];
            ((u32*)packet)[index++] = rectangle.words[1];
            rectangle.rect.x += env->ofs[0];
            rectangle.rect.y += env->ofs[1];
        } else {
            ((u32*)packet)[index++]
                = (((((u32)env->b0 << 16) | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_BLOCK_FILL)) | ((u32)env->g0 << 8))
                    | env->r0);
            ((u32*)packet)[index++] = rectangle.words[0];
            ((u32*)packet)[index++] = rectangle.words[1];
        }
    }
    ((P_TAG*)packet)->len = index - 1;
}
