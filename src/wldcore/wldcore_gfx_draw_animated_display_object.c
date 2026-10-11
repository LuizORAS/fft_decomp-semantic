#include "fft/wldcore.h"
#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* One sprite part of a cel, two packed words. */
typedef struct wldcore_sprite_part {
    u32 flags;
    u32 texture;
} wldcore_sprite_part_t;

extern wldcore_anim_sequence_t** g_wldcore_anim_sequence_table;

/* Draw one animated display object: advance its sequence, then convert every
 * part of the current cel into a libgs sprite primitive and sort it into the
 * ordering table.
 *
 * Flag 0x400 copies the object's colour verbatim, otherwise the global fade
 * tint is applied. Flag 0x800, or system flag 0x08000000 being clear, steps the
 * frame counter. Flag 0x20 overrides the part's own blend mode with the
 * object's (flags 0xc0).
 *
 * The parts are walked back to front as raw word indices: the target spells the
 * subscript ((frame_count - i) * 2) | 1 into the cel's word array, which is the
 * part's second word, and reaches the part itself from there. Indexing a part
 * array of records instead produces a different address computation, so
 * the word index is retained.
 *
 * Both high halves are masked before the shift. The code is the same as a
 * plain shift, but it matches the target's 0x88-byte frame: CSE shares the
 * 0xffff0000 constant between the two statements, so the first mask/shift
 * combine keeps that constant for the second and leaves its dead AND result
 * as a USE-only pseudo. That pseudo and the part loop's folded entry test
 * (combine turns it into `blez count`, keeps `i = 0`, and leaves the slt
 * pseudo only a USE) get the unreferenced reload slots at 0x50 and 0x58. */
void wldcore_gfx_draw_animated_display_object(world_animated_render_record_t* object, GsOT* ot) {
    GsSPRITE sprite;
    CVECTOR color;
    wldcore_anim_sequence_t* sequence;
    wldcore_sprite_part_t* part;
    u32** cels;
    u32* words;
    s32 count;
    s32 i;
    s32 index;
    s32 mode;
    s32 clut_x;
    s32 clut_y;
    s32 cx_base;
    s32 cy;
    s32 palette;
    s32 px;

    sequence = g_wldcore_anim_sequence_table[object->sequence];
    sprite.my = 0;
    sprite.mx = 0;
    sprite.scaley = ONE;
    sprite.scalex = ONE;
    sprite.rotate = 0;
    cels = g_wldcore_anim_cel_table;
    if (object->flags & WLDCORE_DISPLAY_OBJECT_UNTINTED) {
        color = object->color;
    } else {
        wldcore_gfx_copy_color_with_tint(&object->color, &color);
    }
    sprite.r = color.r;
    sprite.g = color.g;
    sprite.b = color.b;
    if ((object->flags & WLDCORE_DISPLAY_OBJECT_ANIMATE_WHILE_PAUSED)
        || !(g_main_system_flags & MAIN_SYSTEM_FLAG_FREEZE_ANIMATION)) {
        object->anim_counter = object->anim_counter + 1;
    }
    /* A goto loop: as a for/while loop, GCC's loop pass hoists the 0xffff
     * constant into a register, which the target does not. */
step_frame:
    index = ((wldcore_anim_frame_t*)sequence)[object->frame_index + 1].duration;
    if (index == object->anim_counter && index != 0xffff) {
        object->anim_counter = 0;
        object->frame_index = object->frame_index + 1;
        index = sequence->frame_count;
        if (index == object->frame_index) {
            object->frame_index = 0;
        }
        goto step_frame;
    }
    index = ((wldcore_anim_frame_t*)sequence)[object->frame_index + 1].cel;
    words = cels[index];
    clut_x = words[1] & 0xffff;
    clut_y = (words[1] & 0xffff0000) >> 16;
    cx_base = words[2] & 0xffff;
    count = words[0];
    cy = (words[2] & 0xffff0000) >> 16;
    for (i = 0; i < count; i++) {
        index = ((count - i) * 2) | 1;
        sprite.attribute = words[index] & 0x80000000;
        if (object->flags & WLDCORE_DISPLAY_OBJECT_SEMI_TRANSPARENT) {
            sprite.attribute |= 0x40000000;
            sprite.attribute |= (object->flags & WLDCORE_DISPLAY_OBJECT_BLEND_RATE_MASK) << 22;
            mode = (object->flags & WLDCORE_DISPLAY_OBJECT_BLEND_RATE_MASK) >> 6;
        } else {
            sprite.attribute |= words[index] & 0x70000000;
            mode = (words[index] >> 28) & 3;
        }
        part = (wldcore_sprite_part_t*)(index * 4 + (s32)words);
        sprite.attribute |= (part->flags & 0x07000000) >> 2;
        px = (u8)part->flags - 0x80;
        sprite.x = object->position.x + px;
        px = (u8)(part->flags >> 8) - 0x80;
        sprite.y = object->position.y + px;
        sprite.w = (u8)(part->texture >> 8);
        sprite.h = (u8)part->texture;
        sprite.tpage = GetTPage(0, mode, clut_x, clut_y);
        sprite.u = (u8)(part->texture >> 24);
        sprite.v = (part->texture & 0xff0000) >> 16;
        palette = object->palette;
        if (palette == 0 || (part->flags & 0x08000000)) {
            sprite.cx = (cx_base + (part->flags & 0xff0000)) >> 12;
        } else {
            sprite.cx = cx_base + ((palette - 1) << 4);
        }
        sprite.cy = cy;
        if (sprite.attribute & 0x800000) {
            sprite.u = sprite.u + 1;
        }
        if (sprite.attribute & 0x400000) {
            sprite.v = sprite.v + 1;
        }
        GsSortFlipSprite(&sprite, ot, object->priority);
    }
}
