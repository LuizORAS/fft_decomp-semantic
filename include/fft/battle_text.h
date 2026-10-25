#ifndef BATTLE_TEXT_H
#define BATTLE_TEXT_H

/* Battle and event-overlay text buffers, glyph rendering and messages. */

#include "fft/menu.h"
#include "fft/unit.h"

/* heap */
/* The battle heap: a K&R-style free list in 8-byte units from the effect palette buffer up to MAIN's game heap. */

extern const char g_battle_heap_dp_format[];     /* "DP %x\n" */
extern const char g_battle_heap_remain_format[]; /* "HEAP REMAIN %x MAX %x\n" */
extern const char g_battle_heap_task_format[];   /* "TASK %d %x\n" */
extern const char g_battle_heap_min_format[];    /* "MIN HEAP %x " */

/* menu */
/* The battle menus (battle_menu_*): menu entries run as threads in slots 3 to 8, the turn menus and
 * their lists, the system-function prompts, the windows, text images and cursors they draw, the unit
 * panels and the companion overlays they open. WORLD carries a copy (world_menu_*). */

extern char g_battle_menu_wait_for_allocation_message[];
extern world_glyph_blit_t g_battle_menu_small_number_glyph_rect; /* glyph source rectangle in the large number font */
void battle_menu_draw_small_number_glyphs(s32 value, s32 digits, u8* pixels, world_glyph_blit_t* pos);
s32 battle_menu_read_packed_nibble(const u8* data, s32 index, s32 row, s32 stride);

/* The caller leaves 0x16f in $a1, but the body never reads $a1. */
void battle_menu_mirror_packed_nibble_region(u8* data, const world_glyph_blit_t* region);

void battle_menu_write_packed_nibble(u8* data, s32 index, s32 row, s32 stride, s32 value);

/* text */
/* BATTLE's text engine (battle_text_*): the 32 text sections and the menu text BATTLE.BIN carries
 * (EVENT/SAMPLE.LZW), entry lookup and measuring, the substitution values, the dialog thread and its
 * typewriter, and the spell quotes. The text format itself is in menu.h. */

extern s32 g_battle_spell_quote_last_ability_id;
s32 text_is_skillset_in_spell_quote_exception_list(s32 skillset_id);
void text_select_ability_announcement(action_command_t* action, s32 unit_id, s32 enabled);
extern u8 g_battle_spell_quote_exception_skillsets[];

/* Origin and row geometry used by the EVENT menu text image renderers. */
typedef struct battle_menu_text_image_bounds {
    u16 x;
    u16 y;
    u16 row_stride;
    s16 line_height;
} battle_menu_text_image_bounds_t;

/* Pen of the BUNIT, CARD, EQUIP and JOBSTTS *_text_render_glyph_to_4bpp_image: x in pixels (two per byte),
 * y in rows, and the image's row stride in 16-bit values. */
typedef struct bunit_text_image_position {
    s16 x;          /* 0x00 */
    s16 y;          /* 0x02 */
    s16 row_stride; /* 0x04 */
} bunit_text_image_position_t;
typedef char bunit_text_image_position_size_must_be_6[(sizeof(bunit_text_image_position_t) == 6) ? 1 : -1];

/* Text variables $27 and $28 (rows of glyph pixels drawn, 0x10 by default),
 * bound separately: battle_menu_display_text also takes the table base, and
 * element spellings let GCC address them from it. */
extern s32 g_battle_text_substitution_value_27;

extern s32 g_battle_text_substitution_value_28;
extern u8 g_battle_text_glyph_bitmaps[];
extern u16 g_battle_text_speaker_swap_unit_a;
extern u16 g_battle_text_speaker_swap_unit_b;
void text_build_display_message(s32 offset, s32 flags, s32* out_width);
extern const char g_battle_text_decimal_format[];
extern u16 g_battle_text_message_duration_frames;
extern s32 g_battle_text_awaiting_input;
extern s32 g_battle_text_current_line;
extern u32 g_battle_text_decimal_divisors[];
extern u8* g_battle_text_message_buffer;

/* Text substitutions $7E/$7F (0x801660ec, table index 0x7e from 0x80165ef4)
 * as the four 16-bit values battle_text_set_substitution_values_7e_7f stores. */
extern RECT g_battle_text_substitution_rect;

void battle_text_run_spell_quote_thread(void);
void battle_text_select_ability_announcement(action_command_t* action, s32 unit_id, s32 enabled);

void battle_text_render_glyph_to_4bpp_image(
    const u8* glyph_bitmap, u8* image, world_glyph_blit_t* origin, s32 palette_offset);

void battle_text_build_display_message(s32 offset, s32 flags, s32* out_width);
void battle_text_build_unit_name_list(void);

/* MAIN selects the BATTLE address of text_find_entry through this existing alias. */
u8* battle_text_find_entry(u32 text_id);

void battle_text_init_menu_section_pointers(void);
void battle_text_set_message_duration_frames(u16 frames);
void battle_text_relocate_pointer_table(const u32* offsets);
void battle_text_restore_section_pointers(void);
void battle_text_save_section_pointers(void);
void battle_text_run_dialog_thread(void);
void battle_text_configure_sprite_vram(RECT* rect, s16 width, s16 height, void* packet, s32 clut_offset);
void battle_text_draw_glyph_with_typewriter_delay(s32 pen_x, s32 pen_y, s32 delay);
void battle_text_print_fnt_value_and_next(s32 value);
s32 battle_text_is_skillset_in_spell_quote_exception_list(s32 skillset_id);
void text_blit_glyph(const u8* source, u8* pixels, const world_glyph_blit_t* glyph, const world_glyph_blit_t* position);
extern char g_battle_script_reading_character_message[]; /* "reading charactor%d \n" */

/* Text: glyphs and number fields rendered into 4bpp images, text-id rows uploaded to VRAM, and the
 * help text thread. */

void menu_text_render_encoded_ids_to_image(u8* image, battle_menu_text_image_bounds_t* rect, s32 unused, s32 max_chars,
    u8* font, s16* ids, s32 count, s16 glyph, s32 unused_flags);

/* Text: glyphs rendered into 4bpp images, text-id rows uploaded to VRAM and the text thread starters. */

s32 menu_text_render_glyph_to_4bpp_image(
    s32 glyph_id, u8* image, const bunit_text_image_position_t* position, s32 style);

void menu_text_render_id_rows_to_vram(u8* text_table, s16* list, RECT* rect, s32 style);

#endif
