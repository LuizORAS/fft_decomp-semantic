#include "psx/libgpu.h"

/* LIBGPU, retail 0x80025c44-0x80025e5c. Selects fill or tile clearing and restores drawing state. */
int _clr(void* rectangle, u32 color) {
    RECT* rect = rectangle;
    u32 rgb = color;
    int dimension;
    int value;
    /* Bindings preserve the clear-packet masks, coordinates and linked restore pointer. */
    register int maximum __asm__("$4");
    u16* limit;
    register u32 fill_color __asm__("$3");
    register u32 mask __asm__("$4");
    u32 area_start;
    u32 offset;
    u32 area_end;
    register u32* restore __asm__("$6");
    u32 terminator;
    register u32 coordinate __asm__("$2");
    dimension = rect->w;
    if (dimension >= 0) {
        limit = &g_psyq_gpu_vram_width;
        __asm__("" : "=r"(limit) : "0"(limit)); /* Retail materializes the signed clamp's unsigned field address. */
        value = *limit;
        __asm__("" : "=r"(value) : "0"(value));
        value = (s16)value;
        maximum = value - 1;
        if (maximum < dimension)
            dimension = maximum;
        value = dimension;
    } else
        value = 0;
    rect->w = value;
    dimension = rect->h;
    if (dimension >= 0) {
        limit = &g_psyq_gpu_vram_height;
        __asm__("" : "=r"(limit) : "0"(limit));
        value = *limit;
        __asm__("" : "=r"(value) : "0"(value));
        value = (s16)value;
        maximum = value - 1;
        if (maximum < dimension)
            dimension = maximum;
    } else
        dimension = 0;
    rect->h = dimension;
    if ((rect->x & 63) || (rect->w & 63)) {
        mask = PSYQ_GPU_DMA_ADDRESS_MASK;
        area_end = (PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_BOTTOM_RIGHT) | PSYQ_GPU_PARAMETER_MASK);
        terminator = 0x03ffffff;
        restore = &g_psyq_gpu_clear_restore_tag;
        coordinate = (u32)restore & mask;
        coordinate |= 0x07000000;
        area_start = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_TOP_LEFT);
        offset = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_OFFSET);
        __asm__(""
            : "=r"(coordinate)
            : "0"(coordinate), "r"(area_start),
            "r"(offset)); /* Preserve the two saved command constants before the tag store. */
        g_psyq_gpu_clear_packet_tag = coordinate;
        __asm__(""
            : "=r"(mask)
            : "0"(mask), "m"(g_psyq_gpu_clear_packet_tag)); /* Preserve the retail colour mask after the tag write. */
        g_psyq_gpu_clear_packet_word_1 = area_start;
        g_psyq_gpu_clear_packet_word_2 = area_end;
        g_psyq_gpu_clear_packet_word_3 = offset;
        coordinate = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK);
        g_psyq_gpu_clear_packet_word_4 = coordinate;
        mask = rgb & mask;
        g_psyq_gpu_clear_packet_word_5 = mask | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_TILE);
        coordinate = ((u32*)rect)[0];
        g_psyq_gpu_clear_packet_word_6 = coordinate;
        coordinate = ((u32*)rect)[1];
        *restore = terminator;
        g_psyq_gpu_clear_packet_word_7 = coordinate;
        g_psyq_gpu_clear_restore_area_start = _param(PSYQ_GPU_INFO_DRAW_AREA_TOP_LEFT) | area_start;
        g_psyq_gpu_clear_restore_area_end = _param(PSYQ_GPU_INFO_DRAW_AREA_BOTTOM_RIGHT)
            | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_AREA_BOTTOM_RIGHT);
        g_psyq_gpu_clear_restore_offset = _param(PSYQ_GPU_INFO_DRAW_OFFSET) | offset;
    } else {
        coordinate = 0x04ffffff;
        g_psyq_gpu_clear_packet_tag = coordinate;
        fill_color = PSYQ_GPU_RGB_MASK;
        coordinate = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_DRAW_MASK);
        g_psyq_gpu_clear_packet_word_1 = coordinate;
        fill_color = rgb & fill_color;
        g_psyq_gpu_clear_packet_word_2 = fill_color | PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_BLOCK_FILL);
        coordinate = ((u32*)rect)[0];
        g_psyq_gpu_clear_packet_word_3 = coordinate;
        coordinate = ((u32*)rect)[1];
        g_psyq_gpu_clear_packet_word_4 = coordinate;
    }
    _cwc(&g_psyq_gpu_clear_packet_tag);
    return 0;
}
