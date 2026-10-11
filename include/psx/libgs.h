#ifndef PSX_LIBGS_H
#define PSX_LIBGS_H

#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* libgs ordering-table handle. Five words, 0x14 bytes; the 0x14 stride is what
 * the per-display-buffer descriptor arrays are indexed with. `org` points at
 * the OT root, `tag` at the last entry, and `offset`/`point` carry the z
 * origin and resolution the sort helpers bias against. */
typedef struct {
    s32 log2n;  /* 0x00; the SDK spells this field `length`, but every
                 * accepted source here uses log2n, which is what it holds */
    u32* org;   /* 0x04 */
    s32 offset; /* 0x08 */
    s32 point;  /* 0x0C */
    u32* tag;   /* 0x10 */
} GsOT;

typedef char gs_ot_size_must_be_0x14[(sizeof(GsOT) == 0x14) ? 1 : -1];

/* libgs sprite descriptor (Psy-Q GsSPRITE), 0x24 bytes; 0x17 is implicit
 * padding. As a stack local GCC rounds it to 0x28, which is what the wldcore
 * drawing frames show. */
typedef struct {
    u32 attribute; /* 0x00 */
    s16 x;         /* 0x04 */
    s16 y;         /* 0x06 */
    u16 w;         /* 0x08 */
    u16 h;         /* 0x0A */
    u16 tpage;     /* 0x0C */
    u8 u;          /* 0x0E */
    u8 v;          /* 0x0F */
    s16 cx;        /* 0x10 */
    s16 cy;        /* 0x12 */
    u8 r;          /* 0x14 */
    u8 g;          /* 0x15 */
    u8 b;          /* 0x16 */
    s16 mx;        /* 0x18 */
    s16 my;        /* 0x1A */
    s16 scalex;    /* 0x1C */
    s16 scaley;    /* 0x1E */
    s32 rotate;    /* 0x20 */
} GsSPRITE;

typedef char gs_sprite_size_must_be_0x24[(sizeof(GsSPRITE) == 0x24) ? 1 : -1];

/* libgs flat-shaded line primitive descriptor, 0x10 bytes; 0x0F is implicit
 * padding. */
typedef struct {
    s32 attribute; /* 0x00 */
    s16 x0;        /* 0x04 */
    s16 y0;        /* 0x06 */
    s16 x1;        /* 0x08 */
    s16 y1;        /* 0x0A */
    u8 r;          /* 0x0C */
    u8 g;          /* 0x0D */
    u8 b;          /* 0x0E */
} GsLINE;

typedef char gs_line_size_must_be_0x10[(sizeof(GsLINE) == 0x10) ? 1 : -1];

/* libgs filled-rectangle descriptor (Psy-Q GsBOXF), 0x10 bytes; 0x0F is
 * implicit padding. */
typedef struct {
    u32 attribute; /* 0x00 */
    s16 x;         /* 0x04 */
    s16 y;         /* 0x06 */
    u16 w;         /* 0x08 */
    u16 h;         /* 0x0A */
    u8 r;          /* 0x0C */
    u8 g;          /* 0x0D */
    u8 b;          /* 0x0E */
} GsBOXF;

typedef char gs_boxf_size_must_be_0x10[(sizeof(GsBOXF) == 0x10) ? 1 : -1];

/* libgs TIM image descriptor (Psy-Q GsIMAGE), 0x1C bytes, as GsGetTimInfo
 * fills it: pixel mode, then VRAM rectangle and data pointer for the pixels
 * and for the CLUT. Widths are in 16-bit VRAM units. */
typedef struct {
    u32 pmode;  /* 0x00 */
    s16 px;     /* 0x04 */
    s16 py;     /* 0x06 */
    u16 pw;     /* 0x08 */
    u16 ph;     /* 0x0A */
    u32* pixel; /* 0x0C */
    s16 cx;     /* 0x10 */
    s16 cy;     /* 0x12 */
    u16 cw;     /* 0x14 */
    u16 ch;     /* 0x16 */
    u32* clut;  /* 0x18 */
} GsIMAGE;

typedef char gs_image_size_must_be_0x1c[(sizeof(GsIMAGE) == 0x1C) ? 1 : -1];

/* 2D screen offset (Psy-Q _GsPOSITION). */
typedef struct {
    s16 offx; /* 0x00 */
    s16 offy; /* 0x02 */
} _GsPOSITION;

extern RECT CLIP2;           /* clip rectangle relative to the draw buffer base */
extern s16 PSDBASEX[2];      /* frame-buffer x of each draw buffer */
extern s16 PSDBASEY[2];      /* frame-buffer y of each draw buffer */
extern s16 PSDIDX;           /* active draw buffer */
extern s32 PSDCNT;           /* frame counter; GsSwapDispBuff skips zero */
extern _GsPOSITION POSITION; /* 2D offset */
extern DRAWENV GsDRAWENV;
extern DISPENV GsDISPENV;
extern s32 HWD0; /* horizontal resolution */
extern s32 VWD0; /* vertical resolution */
extern MATRIX GsIDMATRIX;
extern MATRIX GsIDMATRIX2;   /* identity scaled by the aspect ratio */
extern void* GsOUT_PACKET_P; /* packet work area cursor; PACKET* in the SDK */
extern s32 GsLIGHT_MODE;

void GsInitGraph(u16 x, u16 y, u16 intmode, u16 dith, u16 varmmode);
void GsInitGraph2(u16 x, u16 y, u16 intmode, u16 dith, u16 varmmode);
void GsInit3D(void);
void GsDefDispBuff(u16 x0, u16 y0, u16 x1, u16 y1);
void GsSwapDispBuff(void);
s32 GsGetActiveBuff(void);
void GsSetDrawBuffClip(void);
void GsSetDrawBuffOffset(void);
void GsSetClip2D(RECT* rectp);
void GsClearOt(u16 offset, u16 point, GsOT* otp);
void GsDrawOt(GsOT* ot);
void GsSetWorkBase(void* base);
void GsSortSprite(GsSPRITE* sp, GsOT* ot, u16 pri);
void GsSortFastSprite(const GsSPRITE* sp, GsOT* ot, u16 pri);
void GsSortFlipSprite(const GsSPRITE* sp, GsOT* ot, u16 pri);
void GsSortBoxFill(GsBOXF* bp, GsOT* ot, u16 pri);
void GsSortLine(GsLINE* lp, GsOT* ot, u16 pri);
void GsSortPoly(void* pp, GsOT* ot, u16 pri);
void GsSortClear(u8 r, u8 g, u8 b, GsOT* otp);
void GsGetTimInfo(u32* tim, GsIMAGE* im);

/* driver internals */

/* DR_TPAGE and SPRT commands under one DMA tag. */
typedef struct {
    DR_TPAGE tpage; /* 0x00: tag and texture-page word */
    u32 mode;       /* 0x08: r | g << 8 | b << 16 | code << 24 */
    u32 xy;         /* 0x0c */
    u32 uv_clut;    /* 0x10 */
    u32 wh;         /* 0x14 */
} psyq_gs_sprite_packet_t;

typedef char psyq_gs_sprite_packet_size_must_be_0x18[(sizeof(psyq_gs_sprite_packet_t) == 0x18) ? 1 : -1];

/* POLY_FT4 packet as the packed words the sprite sorters write. */
typedef struct {
    u32 tag;       /* 0x00 */
    u32 mode;      /* 0x04: r | g << 8 | b << 16 | code << 24 */
    u32 xy0;       /* 0x08 */
    u32 uv0_clut;  /* 0x0c */
    u32 xy1;       /* 0x10 */
    u32 uv1_tpage; /* 0x14 */
    u32 xy2;       /* 0x18 */
    u32 uv2;       /* 0x1c */
    u32 xy3;       /* 0x20 */
    u32 uv3;       /* 0x24 */
} psyq_gs_poly_ft4_packet_t;

extern const char g_psyq_gs_sort_sprite_overflow_message[];
extern TILE g_psyq_gs_clear_tile[2]; /* GsSortClear's tile for each draw buffer */

/* Draw-buffer bases as GsDefDispBuff last stored them, zero in GPU offset
 * mode; no linked function reads them. */
extern s16 g_psyq_gs_buffer_offset_x[2];
extern s16 g_psyq_gs_buffer_offset_y[2];

/* Offset the sorters add to 2D coordinates: zero in GPU offset mode, else
 * the GTE offset GsSetDrawBuffOffset programmed. */
extern s16 g_psyq_gs_offset_x;
extern s16 g_psyq_gs_offset_y;
extern s16 g_psyq_gs_offset_draw_active; /* GsOFSGPU: offsets go through the draw environment */

/* Both are zero matrices after GsInitGraph and no linked function reads
 * them; which SDK light matrix each one is has not been established. */
extern MATRIX g_psyq_gs_ls_matrix;
extern MATRIX g_psyq_gs_light_ws_matrix;

/* Set by GsInit3D to 0x3FFF and 10; no linked function reads them. */
extern s32 D_801CD7DC;
extern s32 D_801CD854;

u32* psyq_gs_sort_sprite_bg(u32* tag, GsOT* ot, u16 z, s32 len);
void psyq_gs_init_gpu(u16 x, u16 y, u16 intmode, u16 dith, u16 varmmode);
void psyq_gs_init_gte(void);
void psyq_gs_reset_screen_state(u16 width, u16 height);

#endif /* PSX_LIBGS_H */
