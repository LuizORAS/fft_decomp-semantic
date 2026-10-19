#include "psx/libgpu.h"

/* Start reading object `object_index` of the TMD model at `tmd` and return its primitive count. No code refers
 * to it. */
int OpenTMD(u32* tmd, int object_index) {
    int count = psyq_gpu_open_tmd_object((psyq_tmd_header_t*)tmd, object_index, &g_psyq_gpu_tmd_cursor,
        &g_psyq_gpu_tmd_vertices, &g_psyq_gpu_tmd_normals);
    g_psyq_gpu_tmd_primitive_count = count;
    return count;
}
