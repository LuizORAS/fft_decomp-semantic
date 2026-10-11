#ifndef EFFECT_H
#define EFFECT_H

/* EFFECT/E*.BIN: ability effect overlays that run with BATTLE. */

#include "fft/battle.h"

/* helpers */
/* Reads animation curve `nibble` (0 = none, otherwise the curve index + 1) at `frame`. The
 * separate decrement keeps the target's compare against -1. */
#define READ_CURVE(result, index, nibble, frame)                                                                       \
    {                                                                                                                  \
        index = (nibble);                                                                                              \
        index--;                                                                                                       \
        if (index != -1) {                                                                                             \
            result = g_effect_palette_table[index].factor[frame];                                                      \
        } else {                                                                                                       \
            result = 0;                                                                                                \
        }                                                                                                              \
    }

/* Scales a scratch record's colour by a 4.12 brightness and packs it with its
 * primitive code into one RGB-code word. */
#define PACK_RGB(scratch, brightness)                                                                                  \
    ((((brightness) * (scratch)->colour[0]) >> 12) | (scratch)->code                                                   \
        | ((((brightness) * (scratch)->colour[1]) >> 12) << 8)                                                         \
        | ((((brightness) * (scratch)->colour[2]) >> 12) << 16))

/* The RGB-code word of vertex `n` of a primitive. */
#define POLY_RGB(poly, n) (*(u32*)&(poly)->r##n)

/* A random value in [-|x|, |x|). */
#define RAND_SPREAD(x) ((x) == -(x) ? (x) : (-(x) < (x) ? rand() % ((x) * 2) - (x) : rand() % (-(x) - (x)) + (x)))

/* A random value between `a` and `b`, in either order. */
#define RAND_BETWEEN(a, b) ((a) == (b) ? (a) : ((b) < (a) ? (b) + rand() % ((a) - (b)) : (a) + rand() % ((b) - (a))))

/* particle ribbon */
/* Shared records of the homing particle-ribbon renderer that 16 EFFECT
 * overlays compile from one source (for example E000 at 0x801c2500). It comes
 * in two sizes: the short variant keeps 8 ribbons with 9-point trails (E000,
 * E242, E317, E336, E355, E371, E402, E409) and the long variant 4 ribbons
 * with 17-point trails (E070, E403, E412, E461, E462, E463, E465, E466). */
enum {
    EFFECT_PARTICLE_RIBBON_SHORT_SLOTS = 8,
    EFFECT_PARTICLE_RIBBON_SHORT_POINTS = 9,
    EFFECT_PARTICLE_RIBBON_LONG_SLOTS = 4,
    EFFECT_PARTICLE_RIBBON_LONG_POINTS = 17,
};

/* One trail point, in world units. */
typedef struct effect_particle_ribbon_point {
    s16 x;
    s16 y;
    s16 z;
} effect_particle_ribbon_point_t;
typedef char assert_effect_particle_ribbon_point_size[sizeof(effect_particle_ribbon_point_t) == 6 ? 1 : -1];

/* One short ribbon: a particle and the double-buffered quad strip drawn along
 * its recent path, one quad per pair of adjacent trail points. */
typedef struct effect_particle_ribbon_short {
    SVECTOR target;                                                            /* 0x000: homing destination */
    VECTOR velocity;                                                           /* 0x008: 20.12 fixed */
    VECTOR position;                                                           /* 0x018: 20.12 fixed */
    POLY_GT4 quads[2][EFFECT_PARTICLE_RIBBON_SHORT_POINTS - 1];                /* 0x028 */
    effect_particle_ribbon_point_t trail[EFFECT_PARTICLE_RIBBON_SHORT_POINTS]; /* 0x368: ring buffer indexed by age */
    s16 age;                                                                   /* 0x39e */
    s16 lifetime;                                                              /* 0x3a0 */
    s16 active;                                                                /* 0x3a2 */
    u8 _unused_3a4[0x02];
    s16 frame;             /* 0x3a6: texture animation frame */
    s16 frame_timer;       /* 0x3a8 */
    u8 _padding_3aa[0x02]; /* tail padding to 4-byte alignment */
} effect_particle_ribbon_short_t;
typedef char assert_effect_particle_ribbon_short_size[sizeof(effect_particle_ribbon_short_t) == 0x3ac ? 1 : -1];

/* One long ribbon: the short layout with 16 quads per buffer and a 17-point
 * trail. */
typedef struct effect_particle_ribbon_long {
    SVECTOR target;                                                           /* 0x000: homing destination */
    VECTOR velocity;                                                          /* 0x008: 20.12 fixed */
    VECTOR position;                                                          /* 0x018: 20.12 fixed */
    POLY_GT4 quads[2][EFFECT_PARTICLE_RIBBON_LONG_POINTS - 1];                /* 0x028 */
    effect_particle_ribbon_point_t trail[EFFECT_PARTICLE_RIBBON_LONG_POINTS]; /* 0x6a8: ring buffer indexed by age */
    s16 age;                                                                  /* 0x70e */
    s16 lifetime;                                                             /* 0x710 */
    s16 active;                                                               /* 0x712 */
    u8 _unused_714[0x02];
    s16 frame;             /* 0x716: texture animation frame */
    s16 frame_timer;       /* 0x718 */
    u8 _padding_71a[0x02]; /* tail padding to 4-byte alignment */
} effect_particle_ribbon_long_t;
typedef char assert_effect_particle_ribbon_long_size[sizeof(effect_particle_ribbon_long_t) == 0x71c ? 1 : -1];

/* Work block allocated by the short variant. */
typedef struct effect_particle_ribbon_short_work {
    s32 buffer;                                                                 /* 0x0000 */
    s16 count;                                                                  /* 0x0004: active ribbons */
    u8 _padding_0006[0x02];                                                     /* aligns ribbons */
    effect_particle_ribbon_short_t ribbons[EFFECT_PARTICLE_RIBBON_SHORT_SLOTS]; /* 0x0008 */
    MATRIX matrix;                                                              /* 0x1d68 */
    s32 destroy_delay;                                                          /* 0x1d88 */
} effect_particle_ribbon_short_work_t;
typedef char
    assert_effect_particle_ribbon_short_work_size[sizeof(effect_particle_ribbon_short_work_t) == 0x1d8c ? 1 : -1];

/* Work block allocated by the long variant. */
typedef struct effect_particle_ribbon_long_work {
    s32 buffer;                                                               /* 0x0000 */
    s16 count;                                                                /* 0x0004: active ribbons */
    u8 _padding_0006[0x02];                                                   /* aligns ribbons */
    effect_particle_ribbon_long_t ribbons[EFFECT_PARTICLE_RIBBON_LONG_SLOTS]; /* 0x0008 */
    MATRIX matrix;                                                            /* 0x1c78 */
    s32 destroy_delay;                                                        /* 0x1c98 */
} effect_particle_ribbon_long_work_t;
typedef char
    assert_effect_particle_ribbon_long_work_size[sizeof(effect_particle_ribbon_long_work_t) == 0x1c9c ? 1 : -1];

extern u32 g_effect_e070_particle_ribbon_long_brightness_table[][EFFECT_PARTICLE_RIBBON_LONG_POINTS];
extern u32 g_effect_e403_particle_ribbon_long_brightness_table[][EFFECT_PARTICLE_RIBBON_LONG_POINTS];
extern u32 g_effect_e461_particle_ribbon_long_brightness_table[][EFFECT_PARTICLE_RIBBON_LONG_POINTS];
extern u32 g_effect_particle_ribbon_long_brightness_table[][EFFECT_PARTICLE_RIBBON_LONG_POINTS];
extern u32 g_effect_e000_particle_ribbon_short_brightness_table[][EFFECT_PARTICLE_RIBBON_SHORT_POINTS];
extern u32 g_effect_e242_particle_ribbon_short_brightness_table[][EFFECT_PARTICLE_RIBBON_SHORT_POINTS];
extern u32 g_effect_e317_particle_ribbon_short_brightness_table[][EFFECT_PARTICLE_RIBBON_SHORT_POINTS];
extern u32 g_effect_e371_particle_ribbon_short_brightness_table[][EFFECT_PARTICLE_RIBBON_SHORT_POINTS];
extern u32 g_effect_e409_particle_ribbon_short_brightness_table[][EFFECT_PARTICLE_RIBBON_SHORT_POINTS];
extern u32 g_effect_particle_ribbon_short_brightness_table[][EFFECT_PARTICLE_RIBBON_SHORT_POINTS];

/* polar mesh */
/* Shared records of the 6,012-byte polar quad mesh renderer that 23 EFFECT
 * overlays compile from one source (for example E461 at 0x801c44a0). */
/* Scratchpad (0x1f800000) layout used while one polar mesh is rebuilt.
 * points[ring][0..3] sweep one quadrant in 0x100 steps; points[ring][4] is
 * points[ring][0] rotated by 0x400, closing the quadrant. The renderer never
 * touches the unknown ranges. */
typedef struct effect_polar_mesh_scratch {
    s32 colour[3]; /* 0x00: r/g/b curve factors, 0x80 when colour curves are off */
    u8 _unused_00c[0x04];
    u32 rgb;  /* 0x10: packed colour word of the current ring */
    u32 code; /* 0x14: primitive code byte of the first quad, kept in bits 24-31 */
    u8 u0;    /* 0x18 */
    u8 u1;    /* 0x19 */
    u8 v0;    /* 0x1a */
    u8 v1;    /* 0x1b */
    u8 _unused_01c[0x20];
    s32 translucent;       /* 0x3c */
    u16 tpage;             /* 0x40 */
    u8 _padding_042[0x02]; /* aligns half_width */
    s32 half_width;        /* 0x44 */
    s32 half_height;       /* 0x48 */
    s32 brightness_row;    /* 0x4c */
    u8 _unused_050[0x0c];
    s32 velocity_angle;     /* 0x5c */
    s32 acceleration_angle; /* 0x60 */
    s32 center[3];          /* 0x64: interpolated emitter spawn position; z is written but unused */
    u8 _unused_070[0x34];
    s32 velocity[2]; /* 0xa4: centre drift per ring, 20.12 fixed */
    u8 _unused_0ac[0x08];
    s32 acceleration[2]; /* 0xb4 */
    u8 _unused_0bc[0x1c];
    s32 radius;             /* 0xd8: 20.12 fixed */
    s32 radius_step;        /* 0xdc */
    s32 radius_step_delta;  /* 0xe0 */
    s32 angle;              /* 0xe4 */
    s32 angle_step;         /* 0xe8 */
    s32 angle_step_delta;   /* 0xec */
    s32 ot_depth;           /* 0xf0: clamped to 4..0x17f */
    point16_t points[6][5]; /* 0xf4 */
} effect_polar_mesh_scratch_t;

/* Scratchpad (0x1f800000) layout of the unit-anchored variant (the 6,492-byte
 * renderer in E467, E468, E469, E471 and E472). Its fields shift from
 * effect_polar_mesh_scratch_t, so it is a separate layout, not a view. */
typedef struct effect_anchored_polar_mesh_scratch {
    s32 colour[3]; /* 0x00: r/g/b curve factors, 0x80 when colour curves are off */
    u8 r;          /* 0x0c: colour bytes of the current ring */
    u8 _unused_00d;
    u8 g; /* 0x0e */
    u8 _unused_00f;
    u8 b; /* 0x10 */
    u8 _unused_011;
    u8 u0; /* 0x12 */
    u8 u1; /* 0x13 */
    u8 v0; /* 0x14 */
    u8 v1; /* 0x15 */
    u8 _unused_016[0x22];
    s32 translucent;       /* 0x38 */
    u16 tpage;             /* 0x3c */
    u8 _padding_03e[0x02]; /* aligns half_width */
    s32 half_width;        /* 0x40 */
    s32 half_height;       /* 0x44 */
    s32 brightness_row;    /* 0x48 */
    u8 _unused_04c[0x0c];
    s32 velocity_angle;     /* 0x58 */
    s32 acceleration_angle; /* 0x5c */
    s32 center[3];          /* 0x60: interpolated emitter spawn position */
    u8 _unused_06c[0x34];
    s32 velocity[2]; /* 0xa0 */
    u8 _unused_0a8[0x08];
    s32 acceleration[2]; /* 0xb0 */
    u8 _unused_0b8[0x18];
    s32 target_index; /* 0xd0 */
    u8 _unused_0d4[0x14];
    s32 radius;             /* 0xe8 */
    s32 radius_step;        /* 0xec */
    s32 radius_step_delta;  /* 0xf0 */
    s32 angle;              /* 0xf4 */
    s32 angle_step;         /* 0xf8 */
    s32 angle_step_delta;   /* 0xfc */
    s32 ot_depth;           /* 0x100 */
    SVECTOR position;       /* 0x104 */
    SVECTOR* unit_coords;   /* 0x10c */
    s32 screen[3];          /* 0x110 */
    long flag;              /* 0x11c */
    point16_t points[6][5]; /* 0x120 */
} effect_anchored_polar_mesh_scratch_t;

/* Work block: two POLY_GT4 buffers of four quadrants of 5x4 quads. */
typedef struct effect_polar_mesh_work {
    s32 buffer;            /* 0x0000: 0 or 1, flipped after each update */
    POLY_GT4 quads[2][80]; /* 0x0004: quadrant * 20 + ring * 4 + segment */
    u8 _unused_2084[0x60];
    s32 scroll_u; /* 0x20e4: 20.12 fixed, wrapped to half_width */
    s32 scroll_v; /* 0x20e8: 20.12 fixed, wrapped to half_height */
    u8 _unused_20ec[0x04];
    s16 base_u;        /* 0x20f0 */
    s16 base_v;        /* 0x20f2 */
    s32 frame;         /* 0x20f4: curve column, advanced once per update */
    s32 destroy_delay; /* 0x20f8: the destroy phase waits one call before freeing */
} effect_polar_mesh_work_t;
typedef char assert_effect_polar_mesh_work_size[sizeof(effect_polar_mesh_work_t) == 0x20fc ? 1 : -1];

/* Overlay-local vertex brightness rows (12-bit fixed): brightness_row selects
 * the row, the ring (0-5) the column. */
extern u32 g_effect_anchored_polar_mesh_brightness_table[][6];
extern u32 g_effect_e467_anchored_polar_mesh_brightness_table[][6];

/* Overlay-local vertex brightness rows (12-bit fixed): brightness_row selects
 * the row, the ring (0-5) the column. */
extern u32 g_effect_e004_polar_mesh_brightness_table[][6];
extern u32 g_effect_e033_polar_mesh_brightness_table[][6];
extern u32 g_effect_e065_polar_mesh_brightness_table[][6];
extern u32 g_effect_e070_polar_mesh_brightness_table[][6];
extern u32 g_effect_e077_polar_mesh_brightness_table[][6];
extern u32 g_effect_e241_polar_mesh_brightness_table[][6];
extern u32 g_effect_e242_polar_mesh_brightness_table[][6];
extern u32 g_effect_e245_polar_mesh_brightness_table[][6];
extern u32 g_effect_e384_polar_mesh_brightness_table[][6];
extern u32 g_effect_e409_polar_mesh_brightness_table[][6];
extern u32 g_effect_e453_polar_mesh_brightness_table[][6];
extern u32 g_effect_e454_polar_mesh_brightness_table[][6];
extern u32 g_effect_e461_polar_mesh_brightness_table[][6];
extern u32 g_effect_polar_mesh_brightness_table[][6];

/* quad grid */
/* Work blocks of the scrolling textured quad grid renderer that EFFECT
 * overlays compile from one source in 2x2, 4x4 and 8x8 builds (for example
 * effect_e047_update_textured_quad_grid_4x4 in E047). Each block is allocated in
 * phase 1 and freed in phase 3. The 2x2 and 4x4 builds leave a 0x48-byte gap
 * after the quads that the 8x8 build does not have. */
typedef struct effect_textured_quad_grid_2x2_work {
    s32 buffer;           /* 0x000: quads[buffer] is drawn this frame */
    POLY_GT4 quads[2][4]; /* 0x004: row-major 2x2 grid, double buffered */
    u8 _unused_1a4[0x48];
    s32 scroll_u; /* 0x1ec: 20.12 texture scroll, wrapped to texture_width / 2 */
    s32 scroll_v; /* 0x1f0: 20.12 texture scroll, wrapped to texture_height / 2 */
    u8 _unused_1f4[4];
    u16 texture_u;       /* 0x1f8: from the emitter record; read back as u8 */
    u16 texture_v;       /* 0x1fa */
    s32 frame;           /* 0x1fc: column of the animation curves */
    s32 destroy_latched; /* 0x200: phase 3 frees the block on its second call */
} effect_textured_quad_grid_2x2_work_t;
typedef char
    assert_effect_textured_quad_grid_2x2_work_size[sizeof(effect_textured_quad_grid_2x2_work_t) == 0x204 ? 1 : -1];

typedef struct effect_textured_quad_grid_4x4_work {
    s32 buffer;            /* 0x000: quads[buffer] is drawn this frame */
    POLY_GT4 quads[2][16]; /* 0x004: row-major 4x4 grid, double buffered */
    u8 _unused_684[0x48];
    s32 scroll_u; /* 0x6cc: 20.12 texture scroll, wrapped to texture_width / 2 */
    s32 scroll_v; /* 0x6d0: 20.12 texture scroll, wrapped to texture_height / 2 */
    u8 _unused_6d4[4];
    u16 texture_u;       /* 0x6d8: from the emitter record; read back as u8 */
    u16 texture_v;       /* 0x6da */
    s32 frame;           /* 0x6dc: column of the animation curves */
    s32 destroy_latched; /* 0x6e0: phase 3 frees the block on its second call */
} effect_textured_quad_grid_4x4_work_t;
typedef char
    assert_effect_textured_quad_grid_4x4_work_size[sizeof(effect_textured_quad_grid_4x4_work_t) == 0x6e4 ? 1 : -1];

typedef struct effect_textured_quad_grid_8x8_work {
    s32 buffer;            /* 0x000: quads[buffer] is drawn this frame */
    POLY_GT4 quads[2][64]; /* 0x004: row-major 8x8 grid, double buffered */
    s32 scroll_u;          /* 0x1a04: 20.12 texture scroll, wrapped to texture_width / 2 */
    s32 scroll_v;          /* 0x1a08: 20.12 texture scroll, wrapped to texture_height / 2 */
    u8 _unused_1a0c[4];
    u16 texture_u;       /* 0x1a10: from the emitter record; read back as u8 */
    u16 texture_v;       /* 0x1a12 */
    s32 frame;           /* 0x1a14: column of the animation curves */
    s32 destroy_latched; /* 0x1a18: phase 3 frees the block on its second call */
} effect_textured_quad_grid_8x8_work_t;
typedef char
    assert_effect_textured_quad_grid_8x8_work_size[sizeof(effect_textured_quad_grid_8x8_work_t) == 0x1a1c ? 1 : -1];

/* Overlay-local vertex brightness rows (12-bit fixed) for the 2x2 grid:
 * brightness_row selects the row, the column is the quad row. */
extern u32 g_effect_electricity_grid_brightness_table[][3];

/* Overlay-local vertex brightness rows (12-bit fixed) for the 8x8 grid:
 * brightness_row selects the row, the column is the quad row. */
extern u32 g_effect_textured_grid_brightness_table[][9];

/* ring mesh */
/* Shared records of the 5,584-byte ring mesh renderer that 32 EFFECT overlays
 * compile from one source (for example E450 at 0x801c2d08). */
enum {
    RING_MESH_SEGMENTS = 32,  /* quads around one ring */
    RING_MESH_ARC_POINTS = 9, /* lattice points of one 90-degree arc */
};

/* Work block: two POLY_GT4 buffers of two rings of 32 quads. */
typedef struct effect_ring_mesh_work {
    s32 buffer;                                /* 0x0000: 0 or 1, flipped after each update */
    POLY_GT4 quads[2][RING_MESH_SEGMENTS * 2]; /* 0x0004: ring * 32 + segment */
    s16 spin;                                  /* 0x1a04: ring rotation, wrapped to 0xfff */
    s16 v_scroll;                              /* 0x1a06: 8.8 fixed, wrapped to v_span */
    s32 radius_offset;                         /* 0x1a08: 24.8 fixed, added to the radius */
    s32 radius_speed;                          /* 0x1a0c */
    u8 _unused_1a10[0x08];
    s32 destroy_delay; /* 0x1a18: the destroy phase waits one call before freeing */
} effect_ring_mesh_work_t;
typedef char assert_effect_ring_mesh_work_size[sizeof(effect_ring_mesh_work_t) == 0x1a1c ? 1 : -1];

/* Scratchpad (0x1f800000) layout while the ring is rebuilt. points[row][0..8]
 * sweep one quadrant in 0x80 steps; the rotating add helpers reuse them for
 * the other three quadrants. */
typedef struct effect_ring_mesh_scratch {
    VECTOR points[3][RING_MESH_ARC_POINTS]; /* 0x000 */
    VECTOR origin;                          /* 0x1b0 */
    SVECTOR vertex;                         /* 0x1c0 */
    VECTOR screen[4];                       /* 0x1c8: projected quad corners; RotTrans writes its flag to pad */
    s16 depth[3][RING_MESH_SEGMENTS + 1];   /* 0x208 */
} effect_ring_mesh_scratch_t;

/* Work block of the single-ring variant (4,948-byte renderer in E411, E464
 * and E483): two POLY_GT4 buffers of one ring of 32 quads. */
typedef struct effect_single_ring_mesh_work {
    s32 buffer;                            /* 0x000: 0 or 1, flipped after each update */
    POLY_GT4 quads[2][RING_MESH_SEGMENTS]; /* 0x004 */
    s16 spin;                              /* 0xd04: ring rotation, wrapped to 0xfff */
    s16 v_scroll;                          /* 0xd06: 8.8 fixed, wrapped to v_span */
    s32 radius_offset;                     /* 0xd08: 24.8 fixed, added to the radius */
    s32 radius_speed;                      /* 0xd0c */
    u8 _unused_d10[0x08];
    s32 destroy_delay; /* 0xd18: the destroy phase waits one call before freeing */
} effect_single_ring_mesh_work_t;
typedef char assert_effect_single_ring_mesh_work_size[sizeof(effect_single_ring_mesh_work_t) == 0xd1c ? 1 : -1];

/* Scratchpad (0x1f800000) layout of the single-ring variant: two lattice
 * rows of one quadrant. */
typedef struct effect_single_ring_mesh_scratch {
    VECTOR points[2][RING_MESH_ARC_POINTS]; /* 0x000 */
} effect_single_ring_mesh_scratch_t;

/* Overlay-local vertex brightness rows (12-bit fixed): brightness_row selects
 * the row, the lattice row (0-2) the column. */
extern u32 g_effect_e015_ring_mesh_brightness_table[][3];
extern u32 g_effect_e080_ring_mesh_brightness_table[][3];
extern u32 g_effect_e134_ring_mesh_brightness_table[][3];
extern u32 g_effect_e235_ring_mesh_brightness_table[][3];
extern u32 g_effect_e256_ring_mesh_brightness_table[][3];
extern u32 g_effect_e371_ring_mesh_brightness_table[][3];
extern u32 g_effect_e373_ring_mesh_brightness_table[][3];
extern u32 g_effect_e383_ring_mesh_brightness_table[][3];
extern u32 g_effect_e450_ring_mesh_brightness_table[][3];
extern u32 g_effect_e451_ring_mesh_brightness_table[][3];
extern u32 g_effect_e452_ring_mesh_brightness_table[][3];
extern u32 g_effect_e461_ring_mesh_brightness_table[][3];
extern u32 g_effect_e474_ring_mesh_brightness_table[][3];
extern u32 g_effect_e480_ring_mesh_brightness_table[][3];
extern u32 g_effect_ring_mesh_brightness_table[][3];

/* Overlay-local vertex brightness rows (12-bit fixed): brightness_row selects
 * the row, the lattice row (0-8) the column. */
extern u32 g_effect_e004_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e015_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e065_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e067_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e073_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e074_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e077_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e134_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e242_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e256_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e259_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_e383_ring_stack_mesh_brightness_table[][9];
extern u32 g_effect_ring_stack_mesh_brightness_table[][9];

/* Overlay-local vertex brightness rows (12-bit fixed). */
extern u32 g_effect_e464_single_ring_mesh_brightness_table[][2];
extern u32 g_effect_single_ring_mesh_brightness_table[][2];

/* Overlay-local vertex brightness rows (12-bit fixed): brightness_row selects
 * the row, the lattice row (0-8) the column. */
extern u32 g_effect_e317_tube_mesh_brightness_table[][9];
extern u32 g_effect_e383_tube_mesh_brightness_table[][9];
extern u32 g_effect_tube_mesh_brightness_table[][9];

/* Overlay-local ring scale per lattice row pair (12-bit fixed): row 4 - i and
 * row 4 + i share g_effect_tube_mesh_ring_scales[i]. */
extern s32 g_effect_e317_tube_mesh_ring_scales[5];
extern s32 g_effect_e383_tube_mesh_ring_scales[5];
extern s32 g_effect_tube_mesh_ring_scales[5];

/* summon mesh */
/* Shared records of the 4,744-byte summon dome mesh renderer that 12 EFFECT
 * overlays compile from one source (for example E474 at 0x801c42c4). */
/* Work block: two POLY_GT4 buffers of 8 rows x 16 columns. */
typedef struct effect_summon_mesh_work {
    s32 buffer;             /* 0x0000: 0 or 1, flipped after each update */
    POLY_GT4 quads[2][128]; /* 0x0004: row * 16 + column */
    s16 spin;               /* 0x3404: lattice rotation, wrapped to 0xfff */
    s16 v_scroll;           /* 0x3406: 8.8 fixed, wrapped to v_span */
    s32 radius_offset;      /* 0x3408: 24.8 fixed, added to the radius */
    s32 radius_speed;       /* 0x340c */
    u8 _unused_3410[0x08];
    s32 destroy_delay; /* 0x3418: the destroy phase waits one call before freeing */
} effect_summon_mesh_work_t;
typedef char assert_effect_summon_mesh_work_size[sizeof(effect_summon_mesh_work_t) == 0x341c ? 1 : -1];

/* Leading flags word of the effect section at *g_battle_effect_flags_section. */
typedef struct effect_summon_mesh_effect_header {
    u32 flags; /* 0x00: bit 3 is passed as the add_depth argument */
} effect_summon_mesh_effect_header_t;

/* Overlay-local vertex brightness rows (12-bit fixed): brightness_row selects
 * the row, the lattice row (0-8) the column. */
extern u32 g_effect_e071_summon_mesh_brightness_table[][9];
extern u32 g_effect_e074_summon_mesh_brightness_table[][9];
extern u32 g_effect_e259_summon_mesh_brightness_table[][9];
extern u32 g_effect_e338_summon_mesh_brightness_table[][9];
extern u32 g_effect_e373_summon_mesh_brightness_table[][9];
extern u32 g_effect_e452_summon_mesh_brightness_table[][9];
extern u32 g_effect_e474_summon_mesh_brightness_table[][9];
extern u32 g_effect_summon_mesh_brightness_table[][9];

/* system */
/* The native handlers of the EFFECT/E###.BIN overlays.
 *
 * Effect script opcode 06 puts a handler in one of the callback slots, and a
 * particle keyframe whose action word names the slot binds it to the record.
 * While a keyframe that names the slot runs, the particle channel calls
 *     handler(record_index, callback_slot, emitter_index, frame)
 * once a frame in place of the particle emitter: emitter_index is the
 * keyframe's emitter record, which the handler reads through its own view,
 * and frame counts from the start of the keyframe. The slot's phase
 * (effect_record_t.callback_phase, effect_phase_e) starts at INIT, where the
 * handler allocates its work block into work_slots; it then stays at UPDATE
 * until the channel sets DESTROY two frames before the keyframe ends. */

typedef struct effect_particle_vertex_location {
    u8 slot;
    u8 row;
} effect_particle_vertex_location_t;
extern effect_particle_vertex_location_t g_effect_e454_particle_vertex_locations[12];
extern effect_particle_vertex_location_t g_effect_e464_particle_vertex_locations[3];

/* tentacle mesh */
/* Shared records of two emitter-driven EFFECT renderers compiled from one
 * source family: the 8,668-byte tentacle mesh (E376, E381, E382, E384, E401
 * and E453) that draws up to eight textured strips, and the 5,624-byte particle
 * trail (E077, E161, E451 and E485) that tracks up to four point trails. Both
 * read their effect_geometry_entry_t through
 * effect_tentacle_mesh_geometry_view_t. */
/* One point of a segment's ring of trailing positions. */
typedef struct effect_tentacle_mesh_point {
    s16 x;
    s16 y;
    s16 z;
} effect_tentacle_mesh_point_t;
typedef char assert_effect_tentacle_mesh_point_size[sizeof(effect_tentacle_mesh_point_t) == 6 ? 1 : -1];

typedef struct effect_tentacle_mesh_segment {
    SVECTOR tip;                            /* 0x000 */
    VECTOR velocity;                        /* 0x008: 20.12 fixed */
    VECTOR position;                        /* 0x018: 20.12 fixed */
    POLY_GT4 quads[4][8];                   /* 0x028: sets buffer and buffer + 2 */
    effect_tentacle_mesh_point_t points[9]; /* 0x6a8 */
    s16 head;                               /* 0x6de */
    s16 length;                             /* 0x6e0 */
    s16 active;                             /* 0x6e2 */
    u8 _unused_6e4[0x02];
    s16 frame;             /* 0x6e6: texture animation frame */
    s16 frame_timer;       /* 0x6e8 */
    u8 _padding_6ea[0x02]; /* tail padding to 4-byte alignment */
} effect_tentacle_mesh_segment_t;
typedef char assert_effect_tentacle_mesh_segment_size[sizeof(effect_tentacle_mesh_segment_t) == 0x6ec ? 1 : -1];

typedef struct effect_tentacle_mesh_work {
    s32 buffer;                                 /* 0x0000 */
    s16 count;                                  /* 0x0004 */
    u8 _padding_0006[0x02];                     /* aligns segments */
    effect_tentacle_mesh_segment_t segments[8]; /* 0x0008 */
    MATRIX matrix;                              /* 0x3768 */
    s32 destroy_delay;                          /* 0x3788 */
} effect_tentacle_mesh_work_t;
typedef char assert_effect_tentacle_mesh_work_size[sizeof(effect_tentacle_mesh_work_t) == 0x378c ? 1 : -1];

/* A 17-point particle trail; spawning seeds only points 0-8 (the target's loop bound is 9). */
typedef struct effect_trail_segment {
    SVECTOR tip;                             /* 0x00 */
    VECTOR velocity;                         /* 0x08: 20.12 fixed */
    VECTOR position;                         /* 0x18: 20.12 fixed */
    effect_tentacle_mesh_point_t points[17]; /* 0x28 */
    s16 head;                                /* 0x8e */
    s16 length;                              /* 0x90 */
    s16 active;                              /* 0x92 */
    u8 _unused_94[0x04];
} effect_trail_segment_t;
typedef char assert_effect_trail_segment_size[sizeof(effect_trail_segment_t) == 0x98 ? 1 : -1];

typedef struct effect_trail_work {
    s32 buffer;                         /* 0x000 */
    s16 count;                          /* 0x004 */
    u8 _padding_006[0x02];              /* aligns segments */
    effect_trail_segment_t segments[4]; /* 0x008 */
    MATRIX matrix;                      /* 0x268 */
    s32 destroy_delay;                  /* 0x288 */
} effect_trail_work_t;
typedef char assert_effect_trail_work_size[sizeof(effect_trail_work_t) == 0x28c ? 1 : -1];

extern u32 g_effect_e384_tentacle_mesh_brightness_table[][9];
extern u32 g_effect_e453_tentacle_mesh_brightness_table[][9];
extern u32 g_effect_tentacle_mesh_brightness_table[][9];

/* wave mesh */
/* Shared records of the 2,924-byte 8x8 wave-distorted quad mesh renderer that
 * eight EFFECT overlays compile from one source (for example E033 at
 * 0x801c3c7c). */
/* Work block of the 8x8 wave mesh, allocated in phase 1. The four wave phases
 * are 4096-per-turn angles; the emitter record's spawn spread (x, y) and
 * wave_phase_speed curves advance them once per update. */
typedef struct effect_wave_mesh_work {
    s32 buffer;            /* 0x0000: quads[buffer] is drawn this frame */
    POLY_GT4 quads[2][64]; /* 0x0004: row-major 8x8 grid, double buffered */
    s32 wave_y_phase;      /* 0x1a04: sine phase of the vertical wave at point (0, 0) */
    u8 _unused_1a08[0x0c];
    s32 wave_y_amplitude_phase; /* 0x1a14: cosine phase scaling row 0's vertical amplitude */
    u8 _unused_1a18[0x0c];
    s32 wave_x_phase; /* 0x1a24: sine phase of the horizontal wave at point (0, 0) */
    u8 _unused_1a28[0x0c];
    s32 wave_x_amplitude_phase; /* 0x1a34: cosine phase scaling column 0's horizontal amplitude */
    u8 _unused_1a38[0x0c];
    s32 scroll_u; /* 0x1a44 */
    s32 scroll_v; /* 0x1a48 */
    u8 _unused_1a4c[4];
    u16 texture_u;       /* 0x1a50 */
    u16 texture_v;       /* 0x1a52 */
    s32 frame;           /* 0x1a54 */
    s32 destroy_latched; /* 0x1a58 */
} effect_wave_mesh_work_t;
typedef char assert_effect_wave_mesh_work_size[sizeof(effect_wave_mesh_work_t) == 0x1a5c ? 1 : -1];

/* Scratchpad (0x1f800000) layout while the mesh is rebuilt. */
typedef struct effect_wave_mesh_scratch {
    point16_t points[9][9]; /* 0x000: wave-displaced lattice */
} effect_wave_mesh_scratch_t;

/* Overlay-local vertex brightness rows (12-bit fixed): brightness_row selects
 * the row, the quad row (0-8) the column. */
extern u32 g_effect_e035_wave_mesh_brightness_table[][9];
extern u32 g_effect_e073_wave_mesh_brightness_table[][9];
extern u32 g_effect_e080_wave_mesh_brightness_table[][9];
extern u32 g_effect_e230_wave_mesh_brightness_table[][9];
extern u32 g_effect_e453_wave_mesh_brightness_table[][9];
extern u32 g_effect_wave_mesh_brightness_table[][9];

/* other */

/* Overlay-local vertex brightness rows (12-bit fixed) for the 4x4 grid-mesh
 * family: brightness_row selects the row, the column is the quad row. */
extern u32 g_effect_gfx_brightness_table[][5];
extern battle_effect_tick_channels_t* g_effect_timeline_channel_base;
void effect_add_scratchpad_xyz_components(s32 group, s32 point, const VECTOR* offset, SVECTOR* output);

#endif
