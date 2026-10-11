#include "psx/libgpu.h"

/* LIBGPU, retail 0x80025524-0x800257c8. Builds environment and optional background commands. */
void SetDrawEnv(DR_ENV* dr_env, DRAWENV* env) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* The driver reads the environment as volatile. */
    psyq_gpu_rect_words_t rectangle;
    int index;
    /* Code-member indexing reverses the ADDU operands of the variable-length tail. */
    u32* packet = (u32*)dr_env;
    dr_env->code[0] = get_cs(env->clip.x, env->clip.y);
    dr_env->code[1] = get_ce(env->clip.w + env->clip.x - 1, env->clip.y + env->clip.h - 1);
    dr_env->code[2] = get_ofs(env->ofs[0], env->ofs[1]);
    dr_env->code[3] = get_mode(env->dfe, env->dtd, env->tpage);
    dr_env->code[4] = get_tw(&env->tw);
    dr_env->code[5] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK);
    index = 7;
    if (env->isbg) {
        rectangle.rect.x = env->clip.x;
        rectangle.rect.y = env->clip.y;
        rectangle.rect.w = env->clip.w;
        rectangle.rect.h = env->clip.h;
        limitRange(rectangle.rect.w, 0, g_psyq_gpu_environment.vram_width - 1);
        limitRange(rectangle.rect.h, 0, g_psyq_gpu_environment.vram_height - 1);
        if ((rectangle.rect.x & 63) || (rectangle.rect.w & 63)) {
            rectangle.rect.x -= env->ofs[0];
            rectangle.rect.y -= env->ofs[1];
            packet[index++] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_TILE) | (env->b0 << 16) | (env->g0 << 8) | env->r0;
            packet[index++] = rectangle.packed.coordinates;
            packet[index++] = rectangle.packed.dimensions;
            rectangle.rect.x += env->ofs[0];
            rectangle.rect.y += env->ofs[1];
        } else {
            packet[index++]
                = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_BLOCK_FILL) | (env->b0 << 16) | (env->g0 << 8) | env->r0;
            packet[index++] = rectangle.packed.coordinates;
            packet[index++] = rectangle.packed.dimensions;
        }
    }
    setlen(packet, index - 1);
}
