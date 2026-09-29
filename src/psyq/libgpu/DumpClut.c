/* LIBGPU 80023b3c-80023b7c. */
#include "psx/libgpu.h"
void DumpClut(u16 clut) {
    g_psyq_gpu_printf(g_psyq_gpu_clut_format, (clut & 0x3f) << 4, clut >> 6);
}
