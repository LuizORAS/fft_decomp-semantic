#include "psx/libgpu.h"

/* LIBGPU, retail 0x8002705c-0x80027308. Resolves all four indices without a count test. */
TMD_PRIM* ReadTMD(TMD_PRIM* primitive) {
    int size = psyq_gpu_decode_tmd_primitive(g_psyq_gpu_tmd_cursor, primitive);
    SVECTOR* vertices;
    SVECTOR* normals;
    if (size < 0)
        return 0;
    vertices = g_psyq_gpu_tmd_vertices;
    normals = g_psyq_gpu_tmd_normals;
    primitive->v_ofs = vertices;
    primitive->n_ofs = normals;
    primitive->n0.vx = normals[primitive->norm0].vx;
    primitive->n0.vy = normals[primitive->norm0].vy;
    primitive->n0.vz = normals[primitive->norm0].vz;
    primitive->n1.vx = normals[primitive->norm1].vx;
    primitive->n1.vy = normals[primitive->norm1].vy;
    primitive->n1.vz = normals[primitive->norm1].vz;
    primitive->n2.vx = normals[primitive->norm2].vx;
    primitive->n2.vy = normals[primitive->norm2].vy;
    primitive->n2.vz = normals[primitive->norm2].vz;
    g_psyq_gpu_tmd_cursor = (psyq_tmd_packet_t*)((u8*)g_psyq_gpu_tmd_cursor + size);
    primitive->n3.vx = normals[primitive->norm3].vx;
    primitive->n3.vy = normals[primitive->norm3].vy;
    primitive->n3.vz = normals[primitive->norm3].vz;
    primitive->x0.vx = vertices[primitive->vert0].vx;
    primitive->x0.vy = vertices[primitive->vert0].vy;
    primitive->x0.vz = vertices[primitive->vert0].vz;
    primitive->x1.vx = vertices[primitive->vert1].vx;
    primitive->x1.vy = vertices[primitive->vert1].vy;
    primitive->x1.vz = vertices[primitive->vert1].vz;
    primitive->x2.vx = vertices[primitive->vert2].vx;
    primitive->x2.vy = vertices[primitive->vert2].vy;
    primitive->x2.vz = vertices[primitive->vert2].vz;
    primitive->x3.vx = vertices[primitive->vert3].vx;
    primitive->x3.vy = vertices[primitive->vert3].vy;
    primitive->x3.vz = vertices[primitive->vert3].vz;
    return primitive;
}
