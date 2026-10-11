/* LIBGPU 800275c0-80028740. */
#include "psx/libc.h"
#include "psx/libgpu.h"

int psyq_gpu_decode_tmd_primitive(const psyq_tmd_packet_t* packet, TMD_PRIM* primitive) {
    memset(primitive, 0, sizeof(*primitive));
    primitive->id = packet->id;
    switch (primitive->id & PSYQ_TMD_PACKET_MODE_MASK) {
    case PSYQ_TMD_PACKET_F3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f3_lit_name);
        primitive->r0 = packet->f3_lit.r0;
        primitive->g0 = packet->f3_lit.g0;
        primitive->b0 = packet->f3_lit.b0;
        primitive->r1 = packet->f3_lit.r0;
        primitive->g1 = packet->f3_lit.g0;
        primitive->b1 = packet->f3_lit.b0;
        primitive->r2 = packet->f3_lit.r0;
        primitive->g2 = packet->f3_lit.g0;
        primitive->b2 = packet->f3_lit.b0;
        primitive->vert0 = packet->f3_lit.v0;
        primitive->vert1 = packet->f3_lit.v1;
        primitive->vert2 = packet->f3_lit.v2;
        primitive->norm0 = packet->f3_lit.n0;
        primitive->norm1 = packet->f3_lit.n0;
        primitive->norm2 = packet->f3_lit.n0;
        return sizeof(packet->f3_lit);
    case PSYQ_TMD_PACKET_G3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g3_lit_name);
        primitive->r0 = packet->g3_lit.r0;
        primitive->g0 = packet->g3_lit.g0;
        primitive->b0 = packet->g3_lit.b0;
        primitive->r1 = packet->g3_lit.r0;
        primitive->g1 = packet->g3_lit.g0;
        primitive->b1 = packet->g3_lit.b0;
        primitive->r2 = packet->g3_lit.r0;
        primitive->g2 = packet->g3_lit.g0;
        primitive->b2 = packet->g3_lit.b0;
        primitive->vert0 = packet->g3_lit.v0;
        primitive->vert1 = packet->g3_lit.v1;
        primitive->vert2 = packet->g3_lit.v2;
        primitive->norm0 = packet->g3_lit.n0;
        primitive->norm1 = packet->g3_lit.n1;
        primitive->norm2 = packet->g3_lit.n2;
        return sizeof(packet->g3_lit);
    case PSYQ_TMD_PACKET_FT3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft3_lit_name);
        primitive->tpage = packet->ft3_lit.tpage;
        primitive->clut = packet->ft3_lit.clut;
        primitive->u0 = packet->ft3_lit.tu0;
        primitive->v0 = packet->ft3_lit.tv0;
        primitive->u1 = packet->ft3_lit.tu1;
        primitive->v1 = packet->ft3_lit.tv1;
        primitive->u2 = packet->ft3_lit.tu2;
        primitive->v2 = packet->ft3_lit.tv2;
        primitive->vert0 = packet->ft3_lit.v0;
        primitive->vert1 = packet->ft3_lit.v1;
        primitive->vert2 = packet->ft3_lit.v2;
        primitive->norm0 = packet->ft3_lit.n0;
        primitive->norm1 = packet->ft3_lit.n0;
        primitive->norm2 = packet->ft3_lit.n0;
        return sizeof(packet->ft3_lit);
    case PSYQ_TMD_PACKET_GT3_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt3_lit_name);
        primitive->tpage = packet->gt3_lit.tpage;
        primitive->clut = packet->gt3_lit.clut;
        primitive->u0 = packet->gt3_lit.tu0;
        primitive->v0 = packet->gt3_lit.tv0;
        primitive->u1 = packet->gt3_lit.tu1;
        primitive->v1 = packet->gt3_lit.tv1;
        primitive->u2 = packet->gt3_lit.tu2;
        primitive->v2 = packet->gt3_lit.tv2;
        primitive->vert0 = packet->gt3_lit.v0;
        primitive->vert1 = packet->gt3_lit.v1;
        primitive->vert2 = packet->gt3_lit.v2;
        primitive->norm0 = packet->gt3_lit.n0;
        primitive->norm1 = packet->gt3_lit.n1;
        primitive->norm2 = packet->gt3_lit.n2;
        return sizeof(packet->gt3_lit);
    case PSYQ_TMD_PACKET_F3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f3_name);
        primitive->r0 = packet->f3.r0;
        primitive->g0 = packet->f3.g0;
        primitive->b0 = packet->f3.b0;
        primitive->r1 = packet->f3.r0;
        primitive->g1 = packet->f3.g0;
        primitive->b1 = packet->f3.b0;
        primitive->r2 = packet->f3.r0;
        primitive->g2 = packet->f3.g0;
        primitive->b2 = packet->f3.b0;
        primitive->vert0 = packet->f3.v0;
        primitive->vert1 = packet->f3.v1;
        primitive->vert2 = packet->f3.v2;
        return sizeof(packet->f3);
    case PSYQ_TMD_PACKET_G3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g3_name);
        primitive->r0 = packet->g3.r0;
        primitive->g0 = packet->g3.g0;
        primitive->b0 = packet->g3.b0;
        primitive->r1 = packet->g3.r1;
        primitive->g1 = packet->g3.g1;
        primitive->b1 = packet->g3.b1;
        primitive->r2 = packet->g3.r2;
        primitive->g2 = packet->g3.g2;
        primitive->b2 = packet->g3.b2;
        primitive->vert0 = packet->g3.v0;
        primitive->vert1 = packet->g3.v1;
        primitive->vert2 = packet->g3.v2;
        return sizeof(packet->g3);
    case PSYQ_TMD_PACKET_FT3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft3_name);
        primitive->tpage = packet->ft3.tpage;
        primitive->clut = packet->ft3.clut;
        primitive->u0 = packet->ft3.tu0;
        primitive->v0 = packet->ft3.tv0;
        primitive->u1 = packet->ft3.tu1;
        primitive->v1 = packet->ft3.tv1;
        primitive->u2 = packet->ft3.tu2;
        primitive->v2 = packet->ft3.tv2;
        primitive->r0 = packet->ft3.r0;
        primitive->g0 = packet->ft3.g0;
        primitive->b0 = packet->ft3.b0;
        primitive->r1 = packet->ft3.r0;
        primitive->g1 = packet->ft3.g0;
        primitive->b1 = packet->ft3.b0;
        primitive->r2 = packet->ft3.r0;
        primitive->g2 = packet->ft3.g0;
        primitive->b2 = packet->ft3.b0;
        primitive->vert0 = packet->ft3.v0;
        primitive->vert1 = packet->ft3.v1;
        primitive->vert2 = packet->ft3.v2;
        return sizeof(packet->ft3);
    case PSYQ_TMD_PACKET_GT3:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt3_name);
        primitive->tpage = packet->gt3.tpage;
        primitive->clut = packet->gt3.clut;
        primitive->u0 = packet->gt3.tu0;
        primitive->v0 = packet->gt3.tv0;
        primitive->u1 = packet->gt3.tu1;
        primitive->v1 = packet->gt3.tv1;
        primitive->u2 = packet->gt3.tu2;
        primitive->v2 = packet->gt3.tv2;
        primitive->vert0 = packet->gt3.v0;
        primitive->vert1 = packet->gt3.v1;
        primitive->vert2 = packet->gt3.v2;
        primitive->r0 = packet->gt3.r0;
        primitive->g0 = packet->gt3.g0;
        primitive->b0 = packet->gt3.b0;
        primitive->r1 = packet->gt3.r1;
        primitive->g1 = packet->gt3.g1;
        primitive->b1 = packet->gt3.b1;
        primitive->r2 = packet->gt3.r2;
        primitive->g2 = packet->gt3.g2;
        primitive->b2 = packet->gt3.b2;
        return sizeof(packet->gt3);
    case PSYQ_TMD_PACKET_F4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f4_lit_name);
        primitive->r0 = packet->f4_lit.r0;
        primitive->g0 = packet->f4_lit.g0;
        primitive->b0 = packet->f4_lit.b0;
        primitive->r1 = packet->f4_lit.r0;
        primitive->g1 = packet->f4_lit.g0;
        primitive->b1 = packet->f4_lit.b0;
        primitive->r2 = packet->f4_lit.r0;
        primitive->g2 = packet->f4_lit.g0;
        primitive->b2 = packet->f4_lit.b0;
        primitive->r3 = packet->f4_lit.r0;
        primitive->g3 = packet->f4_lit.g0;
        primitive->b3 = packet->f4_lit.b0;
        primitive->vert0 = packet->f4_lit.v0;
        primitive->vert1 = packet->f4_lit.v1;
        primitive->vert2 = packet->f4_lit.v2;
        primitive->vert3 = packet->f4_lit.v3;
        primitive->norm0 = packet->f4_lit.n0;
        primitive->norm1 = packet->f4_lit.n0;
        primitive->norm2 = packet->f4_lit.n0;
        primitive->norm3 = packet->f4_lit.n0;
        return sizeof(packet->f4_lit);
    case PSYQ_TMD_PACKET_G4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g4_lit_name);
        primitive->r0 = packet->g4_lit.r0;
        primitive->g0 = packet->g4_lit.g0;
        primitive->b0 = packet->g4_lit.b0;
        primitive->r1 = packet->g4_lit.r0;
        primitive->g1 = packet->g4_lit.g0;
        primitive->b1 = packet->g4_lit.b0;
        primitive->r2 = packet->g4_lit.r0;
        primitive->g2 = packet->g4_lit.g0;
        primitive->b2 = packet->g4_lit.b0;
        primitive->r3 = packet->g4_lit.r0;
        primitive->g3 = packet->g4_lit.g0;
        primitive->b3 = packet->g4_lit.b0;
        primitive->vert0 = packet->g4_lit.v0;
        primitive->vert1 = packet->g4_lit.v1;
        primitive->vert2 = packet->g4_lit.v2;
        primitive->vert3 = packet->g4_lit.v3;
        primitive->norm0 = packet->g4_lit.n0;
        primitive->norm1 = packet->g4_lit.n1;
        primitive->norm2 = packet->g4_lit.n2;
        primitive->norm3 = packet->g4_lit.n3;
        return sizeof(packet->g4_lit);
    case PSYQ_TMD_PACKET_FT4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft4_lit_name);
        primitive->tpage = packet->ft4_lit.tpage;
        primitive->clut = packet->ft4_lit.clut;
        primitive->u0 = packet->ft4_lit.tu0;
        primitive->v0 = packet->ft4_lit.tv0;
        primitive->u1 = packet->ft4_lit.tu1;
        primitive->v1 = packet->ft4_lit.tv1;
        primitive->u2 = packet->ft4_lit.tu2;
        primitive->v2 = packet->ft4_lit.tv2;
        primitive->u3 = packet->ft4_lit.tu3;
        primitive->v3 = packet->ft4_lit.tv3;
        primitive->vert0 = packet->ft4_lit.v0;
        primitive->vert1 = packet->ft4_lit.v1;
        primitive->vert2 = packet->ft4_lit.v2;
        primitive->vert3 = packet->ft4_lit.v3;
        primitive->norm0 = packet->ft4_lit.n0;
        primitive->norm1 = packet->ft4_lit.n0;
        primitive->norm2 = packet->ft4_lit.n0;
        primitive->norm3 = packet->ft4_lit.n0;
        return sizeof(packet->ft4_lit);
    case PSYQ_TMD_PACKET_GT4_LIT:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt4_lit_name);
        primitive->tpage = packet->gt4_lit.tpage;
        primitive->clut = packet->gt4_lit.clut;
        primitive->u0 = packet->gt4_lit.tu0;
        primitive->v0 = packet->gt4_lit.tv0;
        primitive->u1 = packet->gt4_lit.tu1;
        primitive->v1 = packet->gt4_lit.tv1;
        primitive->u2 = packet->gt4_lit.tu2;
        primitive->v2 = packet->gt4_lit.tv2;
        primitive->u3 = packet->gt4_lit.tu3;
        primitive->v3 = packet->gt4_lit.tv3;
        primitive->vert0 = packet->gt4_lit.v0;
        primitive->vert1 = packet->gt4_lit.v1;
        primitive->vert2 = packet->gt4_lit.v2;
        primitive->vert3 = packet->gt4_lit.v3;
        primitive->norm0 = packet->gt4_lit.n0;
        primitive->norm1 = packet->gt4_lit.n1;
        primitive->norm2 = packet->gt4_lit.n2;
        primitive->norm3 = packet->gt4_lit.n3;
        return sizeof(packet->gt4_lit);
    case PSYQ_TMD_PACKET_F4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_f4_name);
        primitive->r0 = packet->f4.r0;
        primitive->g0 = packet->f4.g0;
        primitive->b0 = packet->f4.b0;
        primitive->r1 = packet->f4.r0;
        primitive->g1 = packet->f4.g0;
        primitive->b1 = packet->f4.b0;
        primitive->r2 = packet->f4.r0;
        primitive->g2 = packet->f4.g0;
        primitive->b2 = packet->f4.b0;
        primitive->r3 = packet->f4.r0;
        primitive->g3 = packet->f4.g0;
        primitive->b3 = packet->f4.b0;
        primitive->vert0 = packet->f4.v0;
        primitive->vert1 = packet->f4.v1;
        primitive->vert2 = packet->f4.v2;
        primitive->vert3 = packet->f4.v3;
        return sizeof(packet->f4);
    case PSYQ_TMD_PACKET_G4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_g4_name);
        primitive->r0 = packet->g4.r0;
        primitive->g0 = packet->g4.g0;
        primitive->b0 = packet->g4.b0;
        primitive->r1 = packet->g4.r1;
        primitive->g1 = packet->g4.g1;
        primitive->b1 = packet->g4.b1;
        primitive->r2 = packet->g4.r2;
        primitive->g2 = packet->g4.g2;
        primitive->b2 = packet->g4.b2;
        primitive->r3 = packet->g4.r3;
        primitive->g3 = packet->g4.g3;
        primitive->b3 = packet->g4.b3;
        primitive->vert0 = packet->g4.v0;
        primitive->vert1 = packet->g4.v1;
        primitive->vert2 = packet->g4.v2;
        primitive->vert3 = packet->g4.v3;
        return sizeof(packet->g4);
    case PSYQ_TMD_PACKET_FT4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_ft4_name);
        primitive->tpage = packet->ft4.tpage;
        primitive->clut = packet->ft4.clut;
        primitive->u0 = packet->ft4.tu0;
        primitive->v0 = packet->ft4.tv0;
        primitive->u1 = packet->ft4.tu1;
        primitive->v1 = packet->ft4.tv1;
        primitive->u2 = packet->ft4.tu2;
        primitive->v2 = packet->ft4.tv2;
        primitive->u3 = packet->ft4.tu3;
        primitive->v3 = packet->ft4.tv3;
        primitive->r0 = packet->ft4.r0;
        primitive->g0 = packet->ft4.g0;
        primitive->b0 = packet->ft4.b0;
        primitive->r1 = packet->ft4.r0;
        primitive->g1 = packet->ft4.g0;
        primitive->b1 = packet->ft4.b0;
        primitive->r2 = packet->ft4.r0;
        primitive->g2 = packet->ft4.g0;
        primitive->b2 = packet->ft4.b0;
        primitive->r3 = packet->ft4.r0;
        primitive->g3 = packet->ft4.g0;
        primitive->b3 = packet->ft4.b0;
        primitive->vert0 = packet->ft4.v0;
        primitive->vert1 = packet->ft4.v1;
        primitive->vert2 = packet->ft4.v2;
        primitive->vert3 = packet->ft4.v3;
        return sizeof(packet->ft4);
    case PSYQ_TMD_PACKET_GT4:
        if (GetGraphDebug() == 2)
            printf(g_psyq_gpu_tmd_gt4_name);
        primitive->tpage = packet->gt4.tpage;
        primitive->clut = packet->gt4.clut;
        primitive->u0 = packet->gt4.tu0;
        primitive->v0 = packet->gt4.tv0;
        primitive->u1 = packet->gt4.tu1;
        primitive->v1 = packet->gt4.tv1;
        primitive->u2 = packet->gt4.tu2;
        primitive->v2 = packet->gt4.tv2;
        primitive->u3 = packet->gt4.tu3;
        primitive->v3 = packet->gt4.tv3;
        primitive->vert0 = packet->gt4.v0;
        primitive->vert1 = packet->gt4.v1;
        primitive->vert2 = packet->gt4.v2;
        primitive->vert3 = packet->gt4.v3;
        primitive->r0 = packet->gt4.r0;
        primitive->g0 = packet->gt4.g0;
        primitive->b0 = packet->gt4.b0;
        primitive->r1 = packet->gt4.r1;
        primitive->g1 = packet->gt4.g1;
        primitive->b1 = packet->gt4.b1;
        primitive->r2 = packet->gt4.r2;
        primitive->g2 = packet->gt4.g2;
        primitive->b2 = packet->gt4.b2;
        primitive->r3 = packet->gt4.r3;
        primitive->g3 = packet->gt4.g3;
        primitive->b3 = packet->gt4.b3;
        return sizeof(packet->gt4);
    default:
        printf(g_psyq_gpu_unsupported_tmd_format, primitive->id & PSYQ_TMD_PACKET_MODE_MASK);
        return -1;
    }
}
