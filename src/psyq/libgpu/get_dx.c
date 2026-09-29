/* LIBGPU 80025a88-80025b44. */
#include "psx/libgpu.h"
int get_dx(RECT* rect) {
    RECT* rectangle = rect;
    int width;
    int x;
    register int value __asm__("$2");
    u8* type = &g_psyq_gpu_graph_type;
    __asm__(""
        : "=r"(rectangle)
        : "0"(rectangle)); /* Keep the incoming rectangle pointer in a0 until its final load. */
    /* The type-selector temporary retains v0 until coordinate dispatch. */
    __asm__("" : "=r"(type) : "0"(type)); /* The original materializes the GPU-type address. */
    value = *type;
    __asm__("" : "=r"(value) : "0"(value)); /* The original narrows an explicitly loaded value for dispatch. */
    switch ((u8)value) {
    case 1:
        if (g_psyq_gpu_graph_reverse) {
            value = 1024;
            width = rectangle->w;
            x = rectangle->x;
        mirrored:
            value -= width;
            return value - x;
        }
        break;
    case 2:
        if (g_psyq_gpu_graph_reverse) {
            value = (u16)rectangle->w;
            x = rectangle->x;
            width = (s16)value / 2;
            value = 1024;
            goto mirrored;
        }
        value = (u16)rectangle->x;
        __asm__("" : "=r"(value) : "0"(value)); /* Preserve the original unsigned load before signed division. */
        return (s16)value / 2;
    }
    return rectangle->x;
}
