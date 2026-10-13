#ifndef EVENT_ETC_H
#define EVENT_ETC_H

/* EVENT/ETC.OUT: the full-screen graphics of event opcode ShowGraphic (battle_script_show_graphic_thread,
 * world_script_show_graphic_thread): the chapter titles, the game-over screen and the five ending
 * images. */

#include "fft/battle.h"

/* graphic */
/* The graphics table (file, sector, size, VRAM area, dimensions, texture mode), the polygons of
 * the chapter-title and game-over reveals, and the show routines. */

/* ETC uses byte loads for texture coordinates and halfword loads for screen
 * geometry. Preserve both views of each little-endian dimension component. */
typedef union etc_graphic_dimension_component {
    u16 value;
    s16 signed_value;
    struct {
        u8 low;
        u8 high;
    } bytes;
} etc_graphic_dimension_component_t;

/* EVENT/ETC.OUT tables at 0x801c0668 and 0x801c072c. The chapter builder
 * (0x801bf3e0) establishes these axes; the game-over builder (0x801bfc58)
 * swaps their screen interpretation. Both advance records by 12 bytes. */
typedef struct etc_graphic_dimensions {
    etc_graphic_dimension_component_t u;      /* 0x00 */
    etc_graphic_dimension_component_t v;      /* 0x02 */
    etc_graphic_dimension_component_t width;  /* 0x04 */
    etc_graphic_dimension_component_t height; /* 0x06: signed clipping limit */
    etc_graphic_dimension_component_t x;      /* 0x08 */
    etc_graphic_dimension_component_t y;      /* 0x0a */
} etc_graphic_dimensions_t;
typedef char etc_graphic_dimensions_size_check[sizeof(etc_graphic_dimensions_t) == 12 ? 1 : -1];

/* Index into g_etc_graphics, named after the image file: 0 to 6 are chapter
 * titles (CHAPTER1.BIN to CHAPTER4.BIN), 7 is GAMEOVER.BIN and 8 to 12 are
 * END1.BIN to END5.BIN. Only the ones etc_graphic_open treats specially are
 * named. */
typedef enum etc_graphic_id {
    ETC_GRAPHIC_END1 = 8, /* g_battle_screen_fade rises from -0xff; for the others it falls from 0xff */
    ETC_GRAPHIC_END3 = 10,
    ETC_GRAPHIC_END4 = 11,
    ETC_GRAPHIC_END5 = 12,
} etc_graphic_id_e;

typedef struct etc_graphic {
    const char* path;
    s32 lba;
    s32 size;
    RECT* framebuffer_rect;
    etc_graphic_dimensions_t* dimensions;
    void* _unused_14;
    void* _unused_18;
    s32 texture_mode;
} etc_graphic_t;
typedef char etc_graphic_size_check[sizeof(etc_graphic_t) == 0x20 ? 1 : -1];

extern POLY_GT4 g_etc_graphic_chapter_primitives_a[2][4];
extern POLY_GT4 g_etc_graphic_chapter_primitives_b[2][4];
extern RECT g_etc_graphic_game_over_palette_rect;
extern etc_graphic_t g_etc_graphics[13];

void etc_graphic_build_chapter_polygons(
    s32 graphic_id, s32 fade, s32 dimension_set, s32 layer, POLY_GT4* primitives, s32 color);

void etc_graphic_build_game_over_polygons(
    s32 graphic_id, s32 fade, s32 dimension_set, s32 reverse_order, POLY_GT4* primitives, s32 color);

void etc_graphic_open(s32 graphic_id);
void etc_graphic_show_async(void);
void etc_graphic_show_chapter_title(s32 graphic_id);
void etc_graphic_show_game_over(s32 graphic_id);

extern const char g_etc_allocation_wait_message[];

#endif
