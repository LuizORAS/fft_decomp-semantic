#ifndef GFX_H
#define GFX_H

/* Graphics records shared across modules: points, rectangles, textures and sprites. */

#include "psx/libgpu.h"
#include "psx/types.h"

/* color */
/* Packed RGB triples used by Gouraud menu graphics. CVECTOR adds a fourth
 * byte and cannot describe tables whose colors have a three-byte stride. */
typedef struct rgb24 {
    u8 r;
    u8 g;
    u8 b;
} rgb24_t;
typedef char assert_rgb24_size[sizeof(rgb24_t) == 3 ? 1 : -1];

/* Packed PS1 colour: the high bit enables semi-transparency for textured pixels. */
typedef struct rgba5551 {
    u16 red : 5;
    u16 green : 5;
    u16 blue : 5;
    u16 semi_transparent : 1;
} rgba5551_t;
typedef char assert_rgba5551_size[sizeof(rgba5551_t) == 2 ? 1 : -1];

enum {
    PALETTE_COLOR_COUNT = 16,
    CLUT_PALETTE_COUNT = 16,
};

/* Sixteen packed texture colours. */
typedef rgba5551_t palette_t[PALETTE_COLOR_COUNT];
typedef char assert_palette_size[sizeof(palette_t) == 32 ? 1 : -1];

/* Sixteen palette banks. */
typedef palette_t clut_t[CLUT_PALETTE_COUNT];
typedef char assert_clut_size[sizeof(clut_t) == 512 ? 1 : -1];

/* geometry */
/* Use DVECTOR for GTE screen coordinates, SVECTOR/VECTOR for padded GTE
 * vectors, RECT for signed rectangles and CVECTOR for r/g/b plus a code byte.
 * The point types below cover other field widths and signedness. Battle real
 * positions use VECTOR (vx = x, vy = height, vz = map depth); screen positions
 * and rotations use SVECTOR in the same order. */
/* Signed 16-bit x/y pair: screen offsets, sprite positions and per-axis
 * factors that are not GTE output. */
typedef struct point16 {
    s16 x;
    s16 y;
} point16_t;
typedef char assert_point16_size[sizeof(point16_t) == 4 ? 1 : -1];

/* Two signed 32-bit coordinates, without VECTOR's 32-bit z and padding fields. */
typedef struct point32 {
    s32 x;
    s32 y;
} point32_t;
typedef char assert_point32_size[sizeof(point32_t) == 8 ? 1 : -1];

/* Signed 32-bit width/height pair. */
typedef struct size32 {
    s32 width;
    s32 height;
} size32_t;
typedef char size32_size_must_be_8[sizeof(size32_t) == 8 ? 1 : -1];

/* Unsigned 16-bit x/y pair, e.g. VRAM or texture-page positions. */
typedef struct upoint16 {
    u16 x;
    u16 y;
} upoint16_t;
typedef char assert_upoint16_size[sizeof(upoint16_t) == 4 ? 1 : -1];

/* Unpadded signed 16-bit x/y/z triple (6 bytes; SVECTOR is 8). */
typedef struct vec3s16 {
    s16 x;
    s16 y;
    s16 z;
} vec3s16_t;
typedef char assert_vec3s16_size[sizeof(vec3s16_t) == 6 ? 1 : -1];

/* Three signed 32-bit components without VECTOR's trailing 32-bit padding field. */
typedef struct vec3s32 {
    s32 x;
    s32 y;
    s32 z;
} vec3s32_t;
typedef char assert_vec3s32_size[sizeof(vec3s32_t) == 12 ? 1 : -1];

/* RECT layout with unsigned fields, for rectangles the target reads with lhu. */
typedef struct urect16 {
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} urect16_t;
typedef char assert_urect16_size[sizeof(urect16_t) == 8 ? 1 : -1];

/* Round x, y, width and height from texture pixels to 16-pixel cells. */
static inline void texture_rect_pixels_to_cells(const s32 pixels[4], s32 cells[4]) {
    s32 i;
    s32 value;

    for (i = 0; i < 4; i++) {
        value = pixels[i] >> 4;
        cells[i] = value;
        if (pixels[i] & 0xf) {
            cells[i] = value + 1;
        }
    }
}

/* gfx */
/* Sprite-sheet frame: sampled rectangle and scaled screen offset. */
typedef struct sprite_sheet_frame {
    RECT source;
    point16_t offset;
} sprite_sheet_frame_t;
typedef char assert_sprite_sheet_frame_size[sizeof(sprite_sheet_frame_t) == 12 ? 1 : -1];

/* Screen translation used by the sprite geometry helpers. */
typedef struct sprite_draw_offset {
    u8 _unused_00[8];
    upoint16_t position;
} sprite_draw_offset_t;
typedef char sprite_draw_offset_size_must_be_12[sizeof(sprite_draw_offset_t) == 12 ? 1 : -1];

/* Eight-bit texture coordinates within a PS1 texture page. */
typedef struct uv8 {
    u8 u;
    u8 v;
} uv8_t;
typedef char assert_uv8_size[sizeof(uv8_t) == 2 ? 1 : -1];

typedef enum texture_uv_flip {
    TEXTURE_UV_FLIP_NONE = 0,
    TEXTURE_UV_FLIP_X = 1,
    TEXTURE_UV_FLIP_Y = 2,
    TEXTURE_UV_FLIP_XY = TEXTURE_UV_FLIP_X | TEXTURE_UV_FLIP_Y,
} texture_uv_flip_e;

/* event character graphics */
/* One of the 137 fixed-size blocks in EVTCHR.BIN. No native pointer tables
 * or widened 32-bit unit-animation headers are present in the file. */
typedef struct evtchr_file_record {
    u8 sequence_offsets[0x40][4]; /* 0x0000; signed 32-bit offsets, -1 resolves to zero */
    u8 sequence_data[0x400];      /* 0x0100 */
    u8 frame_offsets[0x28][4];    /* 0x0500; signed 32-bit offsets, -1 resolves to zero */
    u8 frame_data[0x1e0];         /* 0x05a0; copied into both native frame sets */
    u8 cluts[16 * 16 * 2];        /* 0x0780; 16 palettes of 16 little-endian colors */
    u8 pixels[200 * 128];         /* 0x0980; 256x200, 4bpp */
    u8 _unused_6d80[0xa80];       /* unread tail of the 0x7800-byte block */
} evtchr_file_record_t;
typedef char evtchr_file_record_size_must_be_30720[sizeof(evtchr_file_record_t) == 30720 ? 1 : -1];

/* image loading */
/* Image load parameters consumed by both *_gfx_init_image_loading routines
 * (0xc bytes; BATTLE's table at 0x80155838 holds one per menu sprite). */
typedef struct image_load_parameters {
    s16 x_load;
    s16 y_load;
    u16 width;
    u16 height;
    u16 x_screen_offset;
    u16 y_screen_offset;
} image_load_parameters_t;

/* Draw-area pair built by the menu graphics helpers: two DR_AREA packets whose
 * rectangles are scaled around their centre by a percentage from
 * g_world_menu_zoom_percentages (indexed by g_world_event_speed-adjusted step).
 * The BATTLE menus and the overlay status panels build the same pair: the
 * *_gfx_build_scaled_draw_area_packets helpers call SetDrawArea(areas[i],
 * rects[i]), the *_gfx_init_scaled_draw_area_packets helpers clear rects[0].w
 * and rects[1].h and copy the menu template into rects[1]. */
typedef struct scaled_draw_area_pair {
    DR_AREA areas[2]; /* 0x00 */
    RECT rects[2];    /* 0x18 */
} scaled_draw_area_pair_t;
typedef char scaled_draw_area_pair_size_must_be_40[sizeof(scaled_draw_area_pair_t) == 40 ? 1 : -1];

/* The 0x10-byte glyph blit descriptor used by the window frame threads and filled
 * by world_gfx_set_image_blit_source_rect / _destination_origin. The stride is
 * the surface width in pixels. */
typedef struct glyph_blit {
    RECT rect;  /* 0x00 */
    s32 stride; /* 0x08 */
    u32 color;  /* 0x0c: 32-bit colour value the decimal renderers set by value sign
                 * (0x77777777, 0xbbbbbbbb, ...) and clear after each draw */
} glyph_blit_t;
typedef char glyph_blit_size_must_be_16[sizeof(glyph_blit_t) == 16 ? 1 : -1];

/* Texture primitive used by the WORLD/BATTLE image-loading and
 * texture-page allocation helpers: a SPRT
 * (code 0x64-0x67) or a POLY_FT4. Both carry the CLUT at 0x0e, which the icon
 * slot helpers overwrite. */
typedef union texture_primitive {
    SPRT sprt;
    POLY_FT4 ft4;
} texture_primitive_t;

/* Textured menu quad source for world_gfx_enqueue_textured_quad_corners (0x8012ccd0):
 * four screen points, four texture points (only the low byte of each
 * 16-bit value is used), then the CLUT and texture page. */
typedef struct textured_quad_source {
    s16 x0, y0; /* 0x00 */
    s16 x1, y1; /* 0x04 */
    s16 x2, y2; /* 0x08 */
    s16 x3, y3; /* 0x0c */
    u16 u0;     /* 0x10 */
    u16 v0;     /* 0x12 */
    u16 u1;     /* 0x14 */
    u16 v1;     /* 0x16 */
    u16 u2;     /* 0x18 */
    u16 v2;     /* 0x1a */
    u16 u3;     /* 0x1c */
    u16 v3;     /* 0x1e */
    u16 clut;   /* 0x20 */
    u16 tpage;  /* 0x22 */
} textured_quad_source_t;

/* Twelve-byte texture cell copied by the formation portrait producers and
 * embedded in oriented menu quads. */
typedef struct texture_source {
    s16 u;     /* 0x00 */
    u16 v;     /* 0x02 */
    s16 w;     /* 0x04 */
    s16 h;     /* 0x06 */
    u16 clut;  /* 0x08 */
    s16 tpage; /* 0x0a */
} texture_source_t;
typedef char texture_source_size_must_be_12[sizeof(texture_source_t) == 12 ? 1 : -1];

/* Screen rectangle and texture source, passed to the oriented-quad readers.
 * Equipment-slot marker arrays have a 0x14-byte stride. */
typedef struct oriented_quad {
    point16_t position;      /* 0x00 */
    u16 w;                   /* 0x04 */
    u16 h;                   /* 0x06 */
    texture_source_t source; /* 0x08 */
} oriented_quad_t;
typedef char oriented_quad_size_must_be_20[sizeof(oriented_quad_t) == 20 ? 1 : -1];

/* Gouraud-textured rectangle source for world_gfx_enqueue_gouraud_textured_quad
 * (0x8012cfd4) and bunit_gfx_enqueue_gouraud_textured_quad: screen rectangle,
 * texture origin (only the low byte of u/v is used; the texture extent is the
 * screen extent), CLUT and texture page. The caller also passes four RGB
 * triples, one per corner. */
typedef struct textured_rect_source {
    s16 x, y;  /* 0x00 */
    s16 w, h;  /* 0x04 */
    s16 u;     /* 0x08 */
    u16 v;     /* 0x0a */
    u16 clut;  /* 0x0c */
    u16 tpage; /* 0x0e */
} textured_rect_source_t;
typedef char textured_rect_source_size_must_be_16[sizeof(textured_rect_source_t) == 16 ? 1 : -1];

/* Full-screen fade primitive pair: a TILE followed by the DR_MODE that
 * selects its texture page and window. */
typedef struct fade_overlay {
    TILE tile;    /* 0x00 */
    DR_MODE mode; /* 0x10 */
} fade_overlay_t;
typedef char fade_overlay_size_must_be_28[(sizeof(fade_overlay_t) == 28) ? 1 : -1];

/* camera */
enum {
    CAMERA_FUSION_HOLD_SENTINEL = 0x2710,
};

/* Provisional camera interpolation key: frame time and target value. */
typedef struct camera_key {
    s32 time;  /* 0x00; cumulative frame of this key */
    s32 value; /* 0x04 */
    u8 _unused_08[8];
} camera_key_t;

/* Provisional per-component camera fusion track (0xa4 bytes), seeded by the
 * *_camera_fusion_thread and advanced by *_camera_step_track. */
typedef struct camera_track {
    camera_key_t keys[8]; /* 0x00 */
    s32 key_count;        /* 0x80 */
    s32 segment;          /* 0x84; first key of the active segment */
    s32 frame;            /* 0x88; frames since the track started */
    s32 segment_frame;    /* 0x8c; frames into the active segment */
    s32 prev_time;        /* 0x90; Q12 */
    s32 prev_value;       /* 0x94; Q12 */
    s32 start_value;      /* 0x98; Q12 */
    s32 segment_duration; /* 0x9c */
    s32 finished;         /* 0xa0 */
} camera_track_t;
typedef char camera_track_size_must_be_164[sizeof(camera_track_t) == 164 ? 1 : -1];

#endif
