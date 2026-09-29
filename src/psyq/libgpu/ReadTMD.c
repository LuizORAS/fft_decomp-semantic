#include "psx/libgpu.h"

/* LIBGPU, retail 0x8002705c-0x80027308. Resolves all four indices without a count test. */
psyq_tmd_primitive_t* ReadTMD(psyq_tmd_primitive_t* primitive) {
    int size = psyq_gpu_decode_tmd_primitive(g_psyq_gpu_tmd_cursor, primitive);
    SVECTOR* vertices;
    SVECTOR* normals;
    if (size < 0)
        return 0;
    vertices = g_psyq_gpu_tmd_vertices;
    normals = g_psyq_gpu_tmd_normals;
    primitive->vertex_base = vertices;
    primitive->normal_base = normals;
    primitive->n0.vx = normals[primitive->normal0].vx;
    primitive->n0.vy = normals[primitive->normal0].vy;
    primitive->n0.vz = normals[primitive->normal0].vz;
    primitive->n1.vx = normals[primitive->normal1].vx;
    primitive->n1.vy = normals[primitive->normal1].vy;
    primitive->n1.vz = normals[primitive->normal1].vz;
    primitive->n2.vx = normals[primitive->normal2].vx;
    primitive->n2.vy = normals[primitive->normal2].vy;
    primitive->n2.vz = normals[primitive->normal2].vz;
    g_psyq_gpu_tmd_cursor += size;
    primitive->n3.vx = normals[primitive->normal3].vx;
    primitive->n3.vy = normals[primitive->normal3].vy;
    primitive->n3.vz = normals[primitive->normal3].vz;
    primitive->x0.vx = vertices[primitive->vertex0].vx;
    primitive->x0.vy = vertices[primitive->vertex0].vy;
    primitive->x0.vz = vertices[primitive->vertex0].vz;
    primitive->x1.vx = vertices[primitive->vertex1].vx;
    primitive->x1.vy = vertices[primitive->vertex1].vy;
    primitive->x1.vz = vertices[primitive->vertex1].vz;
    primitive->x2.vx = vertices[primitive->vertex2].vx;
    primitive->x2.vy = vertices[primitive->vertex2].vy;
    primitive->x2.vz = vertices[primitive->vertex2].vz;
    primitive->x3.vx = vertices[primitive->vertex3].vx;
    primitive->x3.vy = vertices[primitive->vertex3].vy;
    primitive->x3.vz = vertices[primitive->vertex3].vz;
    return primitive;
}
