/* LIBGPU 80024238-80024390. */
#include "psx/libc.h"
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Preserve the original volatile ordering relative to the GPU-type byte. */
extern volatile u8 g_psyq_gpu_queue_enabled;
int ResetGraph(int mode) {
    register int input __asm__("$5") = mode; /* The original copies the input before masking it. */
    int reset_mode;
    psyq_gpu_environment_t* state;
    void* draw;
    register int fill __asm__("$5");
    register u32 type __asm__("$2"); /* Reusing v0 preserves the original flag constant and indexed loads. */
    u16 width;
    __asm__("" : "=r"(input) : "0"(input)); /* Keep the original input copy in a1. */
    reset_mode = input & 7;
    if (reset_mode == 0 || reset_mode == 3) {
        printf(g_psyq_gpu_reset_addresses_format, &g_psyq_gpu_dispatch_table, &g_psyq_gpu_environment);
        state = &g_psyq_gpu_environment;
        memset2(state, 0, 128);
        ResetCallback();
        GPU_cw((u32)g_psyq_gpu_dispatch & PSYQ_GPU_DMA_ADDRESS_MASK);
        state->graph_type = _reset(reset_mode != 0);
        draw = &state->draw;
        __asm__("" : "=r"(draw) : "0"(draw)); /* Keep the original draw-address preparation before scalar stores. */
        /* The volatile view retains the queue-enable store before the type-index shift. */
        type = *(volatile u8*)&state->graph_type;
        __asm__("" : "=r"(type) : "0"(type));
        /* Retain the original shared type/index register. */
        g_psyq_gpu_queue_enabled = 1;
        type *= 2;
        width = g_psyq_gpu_vram_width_lookup[type];
        __asm__ volatile("" : : : "memory"); /* Preserve the original read before fetching the next type byte. */
        type = state->graph_type;
        fill = -1;
        type <<= 2;
        __asm__("" : "=r"(type) : "0"(type)); /* Keep the original scaled byte index before the width store. */
        g_psyq_gpu_vram_width = width;
        __asm__ volatile("" : : : "memory"); /* Preserve the width store before the second table read. */
        /* The equivalent u16 index changes the original scaled-index scheduling. */
        g_psyq_gpu_vram_height = *(const u16*)((const u8*)g_psyq_gpu_vram_height_lookup + type);
        memset2(draw, fill, 92);
        memset2(&state->display, -1, 20);
        return state->graph_type;
    }
    if (g_psyq_gpu_debug_level >= 2)
        g_psyq_gpu_printf(g_psyq_gpu_reset_graph_format, input);
    return g_psyq_gpu_dispatch->reset(1);
}
