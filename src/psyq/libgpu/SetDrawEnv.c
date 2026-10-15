#include "psx/libgpu.h"

/* Build the draw-environment packet `dr_env` for `env`: draw area, offset, draw mode, texture window and mask
 * commands, then, when `env` asks for a background clear, a fill (64-pixel aligned) or a TILE over its clip
 * area in its colour. PutDrawEnv and DrawOTagEnv use it. */
void SetDrawEnv(DR_ENV* dr_env, DRAWENV* env) {
    extern volatile psyq_gpu_environment_t g_psyq_gpu_environment; /* The driver reads the environment as volatile. */
    psyq_gpu_rect_words_t rectangle;
    int index;
    u32* packet = (u32*)dr_env;
    packet[1] = get_cs(env->clip.x, env->clip.y);
    packet[2] = get_ce(env->clip.w + env->clip.x - 1, env->clip.y + env->clip.h - 1);
    packet[3] = get_ofs(env->ofs[0], env->ofs[1]);
    packet[4] = get_mode(env->dfe, env->dtd, env->tpage);
    packet[5] = get_tw(&env->tw);
    packet[6] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK);
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
            packet[index++] = rectangle.words[0];
            packet[index++] = rectangle.words[1];
            rectangle.rect.x += env->ofs[0];
            rectangle.rect.y += env->ofs[1];
        } else {
            packet[index++]
                = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_BLOCK_FILL) | (env->b0 << 16) | (env->g0 << 8) | env->r0;
            packet[index++] = rectangle.words[0];
            packet[index++] = rectangle.words[1];
        }
    }
    setlen(packet, index - 1);
}
