#ifndef GFX_H
#define GFX_H

/* Graphics records shared across modules: points, rectangles, textures and sprites. */

#include "psx/libgpu.h"
#include "psx/types.h"

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

/* Sixteen packed texture colours. */
typedef rgba5551_t palette_t[16];
typedef char assert_palette_size[sizeof(palette_t) == 32 ? 1 : -1];

/* Sixteen palette banks. */
typedef palette_t clut_t[16];
typedef char assert_clut_size[sizeof(clut_t) == 512 ? 1 : -1];

/* geometry */
/* Neutral 2D/3D point, rectangle and colour records for shapes the Psy-Q SDK
 * does not define.
 *
 * Use the SDK type whenever its layout and signedness fit exactly: DVECTOR
 * for GTE screen coordinates (SXY), SVECTOR/VECTOR for padded GTE vectors,
 * RECT for signed x/y/w/h rectangles and CVECTOR for r/g/b plus a code or
 * padding byte. These types cover the remaining cases. Field signedness is
 * part of the choice, not a detail: s16 fields load with lh and u16 fields
 * with lhu, so pick the variant whose loads match the target.
 *
 * Battle real positions are VECTORs (vx = x, vy = height, vz = map depth) and
 * screen positions and rotations are SVECTORs in the same order; keep those
 * in the SDK types rather than vec3s16_t/vec3u16_t. */
/* Signed 16-bit x/y pair: screen offsets, sprite positions and per-axis
 * factors that are not GTE output. */
typedef struct point16 {
    s16 x;
    s16 y;
} point16_t;
typedef char assert_point16_size[sizeof(point16_t) == 4 ? 1 : -1];

/* Two signed 32-bit coordinates, without VECTOR's z and padding words. */
typedef struct point32 {
    s32 x;
    s32 y;
} point32_t;
typedef char assert_point32_size[sizeof(point32_t) == 8 ? 1 : -1];

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

/* Three signed 32-bit components without VECTOR's trailing padding word. */
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

/* gfx */
typedef enum texture_uv_flip {
    TEXTURE_UV_FLIP_NONE = 0,
    TEXTURE_UV_FLIP_X = 1,
    TEXTURE_UV_FLIP_Y = 2,
    TEXTURE_UV_FLIP_XY = TEXTURE_UV_FLIP_X | TEXTURE_UV_FLIP_Y,
} texture_uv_flip_e;

/* Image load parameters consumed by world_gfx_init_image_loading (0xc bytes;
 * the table at 0x80155838 holds one per menu sprite). */
typedef struct world_gfx_image_load_parameters {
    s16 x_load;
    s16 y_load;
    u16 width;
    u16 height;
    u16 x_screen_offset;
    u16 y_screen_offset;
} world_gfx_image_load_parameters_t;

/* The 0x13c-byte panel fade packet group world_menu_init_panel_fade_tiles
 * (0x800ef2fc) and battle_menu_init_panel_fade_tiles (0x8013b6e4) initialize and
 * copy to the second frame buffer: independent nine-tile strips at +0x1c and
 * +0xac, each ordered left to right. */
typedef struct world_fade_tile_frame {
    DR_MODE draw_mode; /* 0x00 */
    TILE center;       /* 0x0c */
    TILE backdrop[9];  /* 0x1c */
    TILE bar[9];       /* 0xac */
} world_fade_tile_frame_t;
typedef char world_fade_tile_frame_size_must_be_0x13c[sizeof(world_fade_tile_frame_t) == 0x13c ? 1 : -1];

/* Draw-area pair built by world_screen_gfx_build_scaled_draw_area_packets: two DR_AREA packets whose
 * rectangles are scaled around their centre by a percentage from
 * g_world_menu_zoom_percentages (indexed by g_world_event_speed-adjusted step).
 * The BATTLE menus and the overlay status panels build the same pair: the
 * *_gfx_build_scaled_draw_area_packets helpers call SetDrawArea(areas[i],
 * rects[i]), the *_gfx_init_scaled_draw_area_packets helpers clear rects[0].w
 * and rects[1].h and copy the menu template into rects[1]. */
typedef struct world_gfx_scaled_draw_area_pair {
    DR_AREA areas[2]; /* 0x00 */
    RECT rects[2];    /* 0x18 */
} world_gfx_scaled_draw_area_pair_t;
typedef char world_scaled_draw_area_pair_size_must_be_0x28[sizeof(world_gfx_scaled_draw_area_pair_t) == 0x28 ? 1 : -1];

/* The 0x10-byte glyph blit descriptor used by the window frame threads and filled
 * by world_gfx_set_image_blit_source_rect / _destination_origin. The stride is
 * the surface width in pixels. */
typedef struct glyph_blit {
    RECT rect;  /* 0x00 */
    s32 stride; /* 0x08 */
    u32 color;  /* 0x0c: colour word the decimal renderers set by value sign
                 * (0x77777777, 0xbbbbbbbb, ...) and clear after each draw */
} glyph_blit_t;
typedef char glyph_blit_size_must_be_0x10[sizeof(glyph_blit_t) == 0x10 ? 1 : -1];

/* Texture primitive used by the WORLD/BATTLE image-loading and
 * texture-page allocation helpers: a SPRT
 * (code 0x64-0x67) or a POLY_FT4. Both carry the CLUT at 0x0e, which the icon
 * slot helpers overwrite. */
typedef union world_texture_prim {
    SPRT sprt;
    POLY_FT4 ft4;
} world_texture_prim_t;

/* Textured menu quad source for world_gfx_enqueue_textured_quad_corners (0x8012ccd0):
 * four screen points, four texture points (only the low byte of each
 * halfword is used), then the CLUT and texture page. */
typedef struct world_gfx_textured_quad_source {
    s16 x0, y0;    /* 0x00 */
    s16 x1, y1;    /* 0x04 */
    s16 x2, y2;    /* 0x08 */
    s16 x3, y3;    /* 0x0c */
    u8 u0, u0_pad; /* 0x10 */
    u8 v0, v0_pad; /* 0x12 */
    u8 u1, u1_pad; /* 0x14 */
    u8 v1, v1_pad; /* 0x16 */
    u8 u2, u2_pad; /* 0x18 */
    u8 v2, v2_pad; /* 0x1a */
    u8 u3, u3_pad; /* 0x1c */
    u8 v3, v3_pad; /* 0x1e */
    u16 clut;      /* 0x20 */
    u16 tpage;     /* 0x22 */
} world_gfx_textured_quad_source_t;

/* Sprite placement record passed to the *_gfx_enqueue_oriented_textured_quad
 * readers: screen rectangle, texture origin and extent, CLUT and texture page
 * (0x14 bytes; equip_menu_draw_equipment_slot_marker proves the stride of its
 * marker arrays). Every member is a halfword in the writers; the readers
 * narrow the texture members to their low byte. */
typedef struct world_oriented_quad {
    u16 x;     /* 0x00 */
    s16 y;     /* 0x02; the save-slot lists sign-test it */
    u16 w;     /* 0x04 */
    u16 h;     /* 0x06 */
    u16 u;     /* 0x08 */
    u16 v;     /* 0x0a */
    s16 uw;    /* 0x0c; world_formation_draw_sprite_with_shadow compares it signed */
    u16 vh;    /* 0x0e */
    u16 clut;  /* 0x10 */
    u16 tpage; /* 0x12 */
} world_oriented_quad_t;
typedef char world_oriented_quad_size_must_be_0x14[sizeof(world_oriented_quad_t) == 0x14 ? 1 : -1];

/* Provisional: 12-byte textured-sprite source record (texture rectangle, CLUT
 * and texture page) filled by world_gfx_build_item_graphic_descriptor,
 * world_item_build_type_icon_source and equip_item_build_type_icon_source,
 * returned through the list-row callback tables and consumed by the menu icon
 * strip at 0x801c8464 and jobstts_gfx_enqueue_textured_quad_rect. */
typedef struct world_item_icon_source {
    s16 u;     /* 0x00 */
    s16 v;     /* 0x02 */
    s16 w;     /* 0x04 */
    s16 h;     /* 0x06 */
    u16 clut;  /* 0x08 */
    u16 tpage; /* 0x0a */
} world_item_icon_source_t;
typedef char world_item_icon_source_size_must_be_12[sizeof(world_item_icon_source_t) == 12 ? 1 : -1];

/* Gouraud-textured rectangle source for world_gfx_enqueue_gouraud_textured_quad
 * (0x8012cfd4) and bunit_gfx_enqueue_gouraud_textured_quad: screen rectangle,
 * texture origin (only the low byte of u/v is used; the texture extent is the
 * screen extent), CLUT and texture page. The caller also passes four RGB
 * triples, one per corner. */
typedef struct world_gfx_textured_rect_source {
    s16 x, y;  /* 0x00 */
    s16 w, h;  /* 0x04 */
    s16 u;     /* 0x08 */
    u16 v;     /* 0x0a */
    u16 clut;  /* 0x0c */
    u16 tpage; /* 0x0e */
} world_gfx_textured_rect_source_t;
typedef char world_textured_rect_source_size_must_be_0x10[sizeof(world_gfx_textured_rect_source_t) == 0x10 ? 1 : -1];

/* Stack working set of world_formation_draw_background_tiles and
 * bunit_gfx_draw_falloff_shaded_rows: the tile rectangle, its four corner RGB
 * triples and the per-grid-point shade table (rows 32 pixels apart, columns
 * 64; five samples per row). */
typedef struct world_formation_background_draw_state {
    world_gfx_textured_rect_source_t rect; /* 0x00 */
    u8 _unused_10[8];                      /* 0x10; reserved in the frame, never read */
    rgb24_t rgb[4];                        /* 0x18 */
    u8 _unused_24[4];                      /* reserved in the frame */
    u8 shades[16][5];                      /* 0x28 */
} world_formation_background_draw_state_t;

/* Full-screen fade primitive pair: a TILE followed by the DR_MODE that
 * selects its texture page and window. */
typedef struct battle_gfx_fade_overlay {
    TILE tile;    /* 0x00 */
    DR_MODE mode; /* 0x10 */
} battle_gfx_fade_overlay_t;
typedef char battle_gfx_fade_overlay_size_must_be_0x1c[(sizeof(battle_gfx_fade_overlay_t) == 0x1c) ? 1 : -1];

/* other */
enum {
    BATTLE_CAMERA_PITCH_STEEP_MASK_THRESHOLD = 0x178,
    BATTLE_CAMERA_FUSION_HOLD_SENTINEL = 0x2710,
};

/* Base texture/screen location paired with the parameters above by
 * world_gfx_init_image_loading: the x is sign-tested (an s16) while the y is
 * only ever added as an unsigned VRAM row. Hoisted here from the three
 * identical local copies in the WORLD and BATTLE unit-editor panels. */
typedef struct world_image_location {
    s16 x; /* 0x00 */
    u16 y; /* 0x02 */
} world_image_location_t;

/* Provisional camera interpolation key: frame time and target value. */
typedef struct battle_camera_key {
    s32 time;  /* 0x00; cumulative frame of this key */
    s32 value; /* 0x04 */
    u8 _unused_08[8];
} battle_camera_key_t;

/* Provisional per-component camera fusion track (0xa4 bytes), seeded by the
 * *_camera_fusion_thread and advanced by *_camera_step_track. */
typedef struct battle_camera_track {
    battle_camera_key_t keys[8]; /* 0x00 */
    s32 key_count;               /* 0x80 */
    s32 segment;                 /* 0x84; first key of the active segment */
    s32 frame;                   /* 0x88; frames since the track started */
    s32 segment_frame;           /* 0x8c; frames into the active segment */
    s32 prev_time;               /* 0x90; Q12 */
    s32 prev_value;              /* 0x94; Q12 */
    s32 start_value;             /* 0x98; Q12 */
    s32 segment_duration;        /* 0x9c */
    s32 finished;                /* 0xa0 */
} battle_camera_track_t;
typedef char battle_camera_track_size_must_be_0xa4[sizeof(battle_camera_track_t) == 0xa4 ? 1 : -1];

#endif
