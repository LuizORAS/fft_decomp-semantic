#ifndef PSX_TMD_H
#define PSX_TMD_H

#include "psx/types.h"

/* Serialized TMD primitives shared by the GPU converter and GS renderer. */
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_F3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_G3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 v0, v1;
    u16 v2, p;
} TMD_P_NF3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u8 r1, g1, b1, p1;
    u8 r2, g2, b2, p2;
    u16 v0, v1;
    u16 v2, p;
} TMD_P_NG3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_TF3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_TG3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 r0, g0, b0, p1;
    u16 v0, v1;
    u16 v2, p2;
} TMD_P_TNF3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 r0, g0, b0, p1;
    u8 r1, g1, b1, p2;
    u8 r2, g2, b2, p3;
    u16 v0, v1;
    u16 v2, p4;
} TMD_P_TNG3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p;
} TMD_P_F4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_G4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 v0, v1;
    u16 v2, v3;
} TMD_P_NF4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u8 r1, g1, b1, p1;
    u8 r2, g2, b2, p2;
    u8 r3, g3, b3, p3;
    u16 v0, v1;
    u16 v2, v3;
} TMD_P_NG4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p2;
} TMD_P_TF4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_TG4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u8 r0, g0, b0, p2;
    u16 v0, v1;
    u16 v2, v3;
} TMD_P_TNF4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u8 r0, g0, b0, p2;
    u8 r1, g1, b1, p3;
    u8 r2, g2, b2, p4;
    u8 r3, g3, b3, p5;
    u16 v0, v1;
    u16 v2, v3;
} TMD_P_TNG4;

#endif
