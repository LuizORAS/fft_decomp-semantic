/* LIBGPU 800275c0-80028740. */
#include "psx/libc.h"
#include "psx/libgpu.h"

int psyq_gpu_decode_tmd_primitive(const u8* source, psyq_tmd_primitive_t* primitive) {
    const psyq_tmd_packet_t* packet = (const psyq_tmd_packet_t*)source;
    memset(primitive, 0, sizeof(*primitive));
    primitive->id = packet->id;
    switch (primitive->id & PSYQ_TMD_PACKET_MODE_MASK) {
    case PSYQ_TMD_PACKET_F3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f3_lit_name);
        primitive->color0.r = packet->f3_lit.color0.r;
        primitive->color0.g = packet->f3_lit.color0.g;
        primitive->color0.b = packet->f3_lit.color0.b;
        primitive->color1.r = packet->f3_lit.color0.r;
        primitive->color1.g = packet->f3_lit.color0.g;
        primitive->color1.b = packet->f3_lit.color0.b;
        primitive->color2.r = packet->f3_lit.color0.r;
        primitive->color2.g = packet->f3_lit.color0.g;
        primitive->color2.b = packet->f3_lit.color0.b;
        primitive->vertex0 = packet->f3_lit.vertex0;
        primitive->vertex1 = packet->f3_lit.vertex1;
        primitive->vertex2 = packet->f3_lit.vertex2;
        primitive->normal0 = packet->f3_lit.normal0;
        primitive->normal1 = packet->f3_lit.normal0;
        primitive->normal2 = packet->f3_lit.normal0;
        return sizeof(packet->f3_lit);
    case PSYQ_TMD_PACKET_G3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g3_lit_name);
        primitive->color0.r = packet->g3_lit.color0.r;
        primitive->color0.g = packet->g3_lit.color0.g;
        primitive->color0.b = packet->g3_lit.color0.b;
        primitive->color1.r = packet->g3_lit.color0.r;
        primitive->color1.g = packet->g3_lit.color0.g;
        primitive->color1.b = packet->g3_lit.color0.b;
        primitive->color2.r = packet->g3_lit.color0.r;
        primitive->color2.g = packet->g3_lit.color0.g;
        primitive->color2.b = packet->g3_lit.color0.b;
        primitive->vertex0 = packet->g3_lit.vertex0;
        primitive->vertex1 = packet->g3_lit.vertex1;
        primitive->vertex2 = packet->g3_lit.vertex2;
        primitive->normal0 = packet->g3_lit.normal0;
        primitive->normal1 = packet->g3_lit.normal1;
        primitive->normal2 = packet->g3_lit.normal2;
        return sizeof(packet->g3_lit);
    case PSYQ_TMD_PACKET_FT3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft3_lit_name);
        primitive->tpage = packet->ft3_lit.tpage;
        primitive->clut = packet->ft3_lit.clut;
        primitive->u0 = packet->ft3_lit.u0;
        primitive->v0 = packet->ft3_lit.v0;
        primitive->u1 = packet->ft3_lit.u1;
        primitive->v1 = packet->ft3_lit.v1;
        primitive->u2 = packet->ft3_lit.u2;
        primitive->v2 = packet->ft3_lit.v2;
        primitive->vertex0 = packet->ft3_lit.vertex0;
        primitive->vertex1 = packet->ft3_lit.vertex1;
        primitive->vertex2 = packet->ft3_lit.vertex2;
        primitive->normal0 = packet->ft3_lit.normal0;
        primitive->normal1 = packet->ft3_lit.normal0;
        primitive->normal2 = packet->ft3_lit.normal0;
        return sizeof(packet->ft3_lit);
    case PSYQ_TMD_PACKET_GT3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt3_lit_name);
        primitive->tpage = packet->gt3_lit.tpage;
        primitive->clut = packet->gt3_lit.clut;
        primitive->u0 = packet->gt3_lit.u0;
        primitive->v0 = packet->gt3_lit.v0;
        primitive->u1 = packet->gt3_lit.u1;
        primitive->v1 = packet->gt3_lit.v1;
        primitive->u2 = packet->gt3_lit.u2;
        primitive->v2 = packet->gt3_lit.v2;
        primitive->vertex0 = packet->gt3_lit.vertex0;
        primitive->vertex1 = packet->gt3_lit.vertex1;
        primitive->vertex2 = packet->gt3_lit.vertex2;
        primitive->normal0 = packet->gt3_lit.normal0;
        primitive->normal1 = packet->gt3_lit.normal1;
        primitive->normal2 = packet->gt3_lit.normal2;
        return sizeof(packet->gt3_lit);
    case PSYQ_TMD_PACKET_F3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f3_name);
        primitive->color0.r = packet->f3.color0.r;
        primitive->color0.g = packet->f3.color0.g;
        primitive->color0.b = packet->f3.color0.b;
        primitive->color1.r = packet->f3.color0.r;
        primitive->color1.g = packet->f3.color0.g;
        primitive->color1.b = packet->f3.color0.b;
        primitive->color2.r = packet->f3.color0.r;
        primitive->color2.g = packet->f3.color0.g;
        primitive->color2.b = packet->f3.color0.b;
        primitive->vertex0 = packet->f3.vertex0;
        primitive->vertex1 = packet->f3.vertex1;
        primitive->vertex2 = packet->f3.vertex2;
        return sizeof(packet->f3);
    case PSYQ_TMD_PACKET_G3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g3_name);
        primitive->color0.r = packet->g3.color0.r;
        primitive->color0.g = packet->g3.color0.g;
        primitive->color0.b = packet->g3.color0.b;
        primitive->color1.r = packet->g3.color1.r;
        primitive->color1.g = packet->g3.color1.g;
        primitive->color1.b = packet->g3.color1.b;
        primitive->color2.r = packet->g3.color2.r;
        primitive->color2.g = packet->g3.color2.g;
        primitive->color2.b = packet->g3.color2.b;
        primitive->vertex0 = packet->g3.vertex0;
        primitive->vertex1 = packet->g3.vertex1;
        primitive->vertex2 = packet->g3.vertex2;
        return sizeof(packet->g3);
    case PSYQ_TMD_PACKET_FT3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft3_name);
        primitive->tpage = packet->ft3.tpage;
        primitive->clut = packet->ft3.clut;
        primitive->u0 = packet->ft3.u0;
        primitive->v0 = packet->ft3.v0;
        primitive->u1 = packet->ft3.u1;
        primitive->v1 = packet->ft3.v1;
        primitive->u2 = packet->ft3.u2;
        primitive->v2 = packet->ft3.v2;
        primitive->color0.r = packet->ft3.color0.r;
        primitive->color0.g = packet->ft3.color0.g;
        primitive->color0.b = packet->ft3.color0.b;
        primitive->color1.r = packet->ft3.color0.r;
        primitive->color1.g = packet->ft3.color0.g;
        primitive->color1.b = packet->ft3.color0.b;
        primitive->color2.r = packet->ft3.color0.r;
        primitive->color2.g = packet->ft3.color0.g;
        primitive->color2.b = packet->ft3.color0.b;
        primitive->vertex0 = packet->ft3.vertex0;
        primitive->vertex1 = packet->ft3.vertex1;
        primitive->vertex2 = packet->ft3.vertex2;
        return sizeof(packet->ft3);
    case PSYQ_TMD_PACKET_GT3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt3_name);
        primitive->tpage = packet->gt3.tpage;
        primitive->clut = packet->gt3.clut;
        primitive->u0 = packet->gt3.u0;
        primitive->v0 = packet->gt3.v0;
        primitive->u1 = packet->gt3.u1;
        primitive->v1 = packet->gt3.v1;
        primitive->u2 = packet->gt3.u2;
        primitive->v2 = packet->gt3.v2;
        primitive->vertex0 = packet->gt3.vertex0;
        primitive->vertex1 = packet->gt3.vertex1;
        primitive->vertex2 = packet->gt3.vertex2;
        primitive->color0.r = packet->gt3.color0.r;
        primitive->color0.g = packet->gt3.color0.g;
        primitive->color0.b = packet->gt3.color0.b;
        primitive->color1.r = packet->gt3.color1.r;
        primitive->color1.g = packet->gt3.color1.g;
        primitive->color1.b = packet->gt3.color1.b;
        primitive->color2.r = packet->gt3.color2.r;
        primitive->color2.g = packet->gt3.color2.g;
        primitive->color2.b = packet->gt3.color2.b;
        return sizeof(packet->gt3);
    case PSYQ_TMD_PACKET_F4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f4_lit_name);
        primitive->color0.r = packet->f4_lit.color0.r;
        primitive->color0.g = packet->f4_lit.color0.g;
        primitive->color0.b = packet->f4_lit.color0.b;
        primitive->color1.r = packet->f4_lit.color0.r;
        primitive->color1.g = packet->f4_lit.color0.g;
        primitive->color1.b = packet->f4_lit.color0.b;
        primitive->color2.r = packet->f4_lit.color0.r;
        primitive->color2.g = packet->f4_lit.color0.g;
        primitive->color2.b = packet->f4_lit.color0.b;
        primitive->color3.r = packet->f4_lit.color0.r;
        primitive->color3.g = packet->f4_lit.color0.g;
        primitive->color3.b = packet->f4_lit.color0.b;
        primitive->vertex0 = packet->f4_lit.vertex0;
        primitive->vertex1 = packet->f4_lit.vertex1;
        primitive->vertex2 = packet->f4_lit.vertex2;
        primitive->vertex3 = packet->f4_lit.vertex3;
        primitive->normal0 = packet->f4_lit.normal0;
        primitive->normal1 = packet->f4_lit.normal0;
        primitive->normal2 = packet->f4_lit.normal0;
        primitive->normal3 = packet->f4_lit.normal0;
        return sizeof(packet->f4_lit);
    case PSYQ_TMD_PACKET_G4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g4_lit_name);
        primitive->color0.r = packet->g4_lit.color0.r;
        primitive->color0.g = packet->g4_lit.color0.g;
        primitive->color0.b = packet->g4_lit.color0.b;
        primitive->color1.r = packet->g4_lit.color0.r;
        primitive->color1.g = packet->g4_lit.color0.g;
        primitive->color1.b = packet->g4_lit.color0.b;
        primitive->color2.r = packet->g4_lit.color0.r;
        primitive->color2.g = packet->g4_lit.color0.g;
        primitive->color2.b = packet->g4_lit.color0.b;
        primitive->color3.r = packet->g4_lit.color0.r;
        primitive->color3.g = packet->g4_lit.color0.g;
        primitive->color3.b = packet->g4_lit.color0.b;
        primitive->vertex0 = packet->g4_lit.vertex0;
        primitive->vertex1 = packet->g4_lit.vertex1;
        primitive->vertex2 = packet->g4_lit.vertex2;
        primitive->vertex3 = packet->g4_lit.vertex3;
        primitive->normal0 = packet->g4_lit.normal0;
        primitive->normal1 = packet->g4_lit.normal1;
        primitive->normal2 = packet->g4_lit.normal2;
        primitive->normal3 = packet->g4_lit.normal3;
        return sizeof(packet->g4_lit);
    case PSYQ_TMD_PACKET_FT4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft4_lit_name);
        primitive->tpage = packet->ft4_lit.tpage;
        primitive->clut = packet->ft4_lit.clut;
        primitive->u0 = packet->ft4_lit.u0;
        primitive->v0 = packet->ft4_lit.v0;
        primitive->u1 = packet->ft4_lit.u1;
        primitive->v1 = packet->ft4_lit.v1;
        primitive->u2 = packet->ft4_lit.u2;
        primitive->v2 = packet->ft4_lit.v2;
        primitive->u3 = packet->ft4_lit.u3;
        primitive->v3 = packet->ft4_lit.v3;
        primitive->vertex0 = packet->ft4_lit.vertex0;
        primitive->vertex1 = packet->ft4_lit.vertex1;
        primitive->vertex2 = packet->ft4_lit.vertex2;
        primitive->vertex3 = packet->ft4_lit.vertex3;
        primitive->normal0 = packet->ft4_lit.normal0;
        primitive->normal1 = packet->ft4_lit.normal0;
        primitive->normal2 = packet->ft4_lit.normal0;
        primitive->normal3 = packet->ft4_lit.normal0;
        return sizeof(packet->ft4_lit);
    case PSYQ_TMD_PACKET_GT4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt4_lit_name);
        primitive->tpage = packet->gt4_lit.tpage;
        primitive->clut = packet->gt4_lit.clut;
        primitive->u0 = packet->gt4_lit.u0;
        primitive->v0 = packet->gt4_lit.v0;
        primitive->u1 = packet->gt4_lit.u1;
        primitive->v1 = packet->gt4_lit.v1;
        primitive->u2 = packet->gt4_lit.u2;
        primitive->v2 = packet->gt4_lit.v2;
        primitive->u3 = packet->gt4_lit.u3;
        primitive->v3 = packet->gt4_lit.v3;
        primitive->vertex0 = packet->gt4_lit.vertex0;
        primitive->vertex1 = packet->gt4_lit.vertex1;
        primitive->vertex2 = packet->gt4_lit.vertex2;
        primitive->vertex3 = packet->gt4_lit.vertex3;
        primitive->normal0 = packet->gt4_lit.normal0;
        primitive->normal1 = packet->gt4_lit.normal1;
        primitive->normal2 = packet->gt4_lit.normal2;
        primitive->normal3 = packet->gt4_lit.normal3;
        return sizeof(packet->gt4_lit);
    case PSYQ_TMD_PACKET_F4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f4_name);
        primitive->color0.r = packet->f4.color0.r;
        primitive->color0.g = packet->f4.color0.g;
        primitive->color0.b = packet->f4.color0.b;
        primitive->color1.r = packet->f4.color0.r;
        primitive->color1.g = packet->f4.color0.g;
        primitive->color1.b = packet->f4.color0.b;
        primitive->color2.r = packet->f4.color0.r;
        primitive->color2.g = packet->f4.color0.g;
        primitive->color2.b = packet->f4.color0.b;
        primitive->color3.r = packet->f4.color0.r;
        primitive->color3.g = packet->f4.color0.g;
        primitive->color3.b = packet->f4.color0.b;
        primitive->vertex0 = packet->f4.vertex0;
        primitive->vertex1 = packet->f4.vertex1;
        primitive->vertex2 = packet->f4.vertex2;
        primitive->vertex3 = packet->f4.vertex3;
        return sizeof(packet->f4);
    case PSYQ_TMD_PACKET_G4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g4_name);
        primitive->color0.r = packet->g4.color0.r;
        primitive->color0.g = packet->g4.color0.g;
        primitive->color0.b = packet->g4.color0.b;
        primitive->color1.r = packet->g4.color1.r;
        primitive->color1.g = packet->g4.color1.g;
        primitive->color1.b = packet->g4.color1.b;
        primitive->color2.r = packet->g4.color2.r;
        primitive->color2.g = packet->g4.color2.g;
        primitive->color2.b = packet->g4.color2.b;
        primitive->color3.r = packet->g4.color3.r;
        primitive->color3.g = packet->g4.color3.g;
        primitive->color3.b = packet->g4.color3.b;
        primitive->vertex0 = packet->g4.vertex0;
        primitive->vertex1 = packet->g4.vertex1;
        primitive->vertex2 = packet->g4.vertex2;
        primitive->vertex3 = packet->g4.vertex3;
        return sizeof(packet->g4);
    case PSYQ_TMD_PACKET_FT4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft4_name);
        primitive->tpage = packet->ft4.tpage;
        primitive->clut = packet->ft4.clut;
        primitive->u0 = packet->ft4.u0;
        primitive->v0 = packet->ft4.v0;
        primitive->u1 = packet->ft4.u1;
        primitive->v1 = packet->ft4.v1;
        primitive->u2 = packet->ft4.u2;
        primitive->v2 = packet->ft4.v2;
        primitive->u3 = packet->ft4.u3;
        primitive->v3 = packet->ft4.v3;
        primitive->color0.r = packet->ft4.color0.r;
        primitive->color0.g = packet->ft4.color0.g;
        primitive->color0.b = packet->ft4.color0.b;
        primitive->color1.r = packet->ft4.color0.r;
        primitive->color1.g = packet->ft4.color0.g;
        primitive->color1.b = packet->ft4.color0.b;
        primitive->color2.r = packet->ft4.color0.r;
        primitive->color2.g = packet->ft4.color0.g;
        primitive->color2.b = packet->ft4.color0.b;
        primitive->color3.r = packet->ft4.color0.r;
        primitive->color3.g = packet->ft4.color0.g;
        primitive->color3.b = packet->ft4.color0.b;
        primitive->vertex0 = packet->ft4.vertex0;
        primitive->vertex1 = packet->ft4.vertex1;
        primitive->vertex2 = packet->ft4.vertex2;
        primitive->vertex3 = packet->ft4.vertex3;
        return sizeof(packet->ft4);
    case PSYQ_TMD_PACKET_GT4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt4_name);
        primitive->tpage = packet->gt4.tpage;
        primitive->clut = packet->gt4.clut;
        primitive->u0 = packet->gt4.u0;
        primitive->v0 = packet->gt4.v0;
        primitive->u1 = packet->gt4.u1;
        primitive->v1 = packet->gt4.v1;
        primitive->u2 = packet->gt4.u2;
        primitive->v2 = packet->gt4.v2;
        primitive->u3 = packet->gt4.u3;
        primitive->v3 = packet->gt4.v3;
        primitive->vertex0 = packet->gt4.vertex0;
        primitive->vertex1 = packet->gt4.vertex1;
        primitive->vertex2 = packet->gt4.vertex2;
        primitive->vertex3 = packet->gt4.vertex3;
        primitive->color0.r = packet->gt4.color0.r;
        primitive->color0.g = packet->gt4.color0.g;
        primitive->color0.b = packet->gt4.color0.b;
        primitive->color1.r = packet->gt4.color1.r;
        primitive->color1.g = packet->gt4.color1.g;
        primitive->color1.b = packet->gt4.color1.b;
        primitive->color2.r = packet->gt4.color2.r;
        primitive->color2.g = packet->gt4.color2.g;
        primitive->color2.b = packet->gt4.color2.b;
        primitive->color3.r = packet->gt4.color3.r;
        primitive->color3.g = packet->gt4.color3.g;
        primitive->color3.b = packet->gt4.color3.b;
        return sizeof(packet->gt4);
    default:
        printf(g_psyq_gpu_unsupported_tmd_format, primitive->id & PSYQ_TMD_PACKET_MODE_MASK);
        return -1;
    }
}
