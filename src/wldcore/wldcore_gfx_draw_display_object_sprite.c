#include "fft/wldcore.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* Convert one display object into a libgs sprite primitive and sort it into the
 * ordering table handed down by wldcore_dispatch_display_object_list.
 *
 * Flag 0x004 selects the 8-bit colour mode (attribute 0x01000000); flag 0x400 copies
 * the object's colour verbatim, otherwise 0x80069718 applies the global fade
 * tint to it; flag 0x100 selects the animated path, where 0x8006b678 looks up
 * the frame rectangle for anim_counter/2 and the counter is then stepped, the
 * flag clearing itself once the halved counter reaches 4.
 *
 * A field-to-field copy narrows the load to the store's mode at expand time;
 * routing the value through a named local of the field's own width does not,
 * and combine cannot re-narrow it afterwards. The target narrows exactly where
 * no local is involved (`sprite.u = fx + object->bounds.position.x` is `lbu`), so the
 * locals and their absence both follow the target's load widths.
 *
 * - The anim test is `>= 4`: the target puts the counter-step arm at the
 *   branch target and the clear-and-toggle arm in the fall-through.
 * - `frame.position.x` and `frame.position.y` need their own locals, or the `sb` sites re-load
 *   them narrowed instead of reusing the `lhu` value.
 * - `object->palette` needs one local shared by the `!= 0` test and the
 *   arithmetic, or the arithmetic re-loads it.
 * - `ox = object->x` must precede `fx = frame.position.x`: the three loads form one
 *   scheduler ready list ordered by source, and the other order transposes
 *   two of them.
 */
void wldcore_gfx_draw_display_object_sprite(wldcore_window_render_record_t* object, GsOT* ot) {
    GsSPRITE sprite;
    wldcore_window_render_bounds16_t frame;
    CVECTOR color;
    s32 centre_x;
    s32 centre_y;
    s32 stepped_x;
    s32 page;
    s32 step;

    if (object->flags & WLDCORE_DISPLAY_OBJECT_8BIT_CLUT) {
        sprite.attribute = 0x01000000;
    } else {
        sprite.attribute = 0;
    }
    centre_x = object->clut.x;
    sprite.cx = centre_x;
    centre_y = object->clut.y;
    sprite.cy = centre_y;
    step = object->palette;
    if (step != 0) {
        stepped_x = object->clut.x;
        sprite.cx = stepped_x + ((step - 1) << 4);
    }
    if (object->flags & WLDCORE_DISPLAY_OBJECT_UNTINTED) {
        color = object->color;
    } else {
        wldcore_gfx_copy_color_with_tint(&object->color, &color);
    }
    sprite.r = color.r;
    sprite.g = color.g;
    sprite.b = color.b;
    page = object->tpage;
    sprite.tpage = page;
    if (object->flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        s32 fx;
        s32 fy;
        s32 ox;
        s32 oy;

        wldcore_gfx_calculate_scaled_rectangle(object->anim_counter / 2, &object->bounds, &frame);
        ox = object->base.x;
        fx = frame.position.x;
        fy = frame.position.y;
        sprite.x = fx + ox;
        oy = object->base.y;
        sprite.y = fy + oy;
        sprite.u = fx + object->bounds.position.x;
        sprite.v = fy + object->bounds.position.y;
        sprite.w = frame.dimensions.x;
        sprite.h = frame.dimensions.y;
        if (object->anim_counter / 2 >= 4) {
            object->anim_counter = 0;
            object->flags ^= WLDCORE_DISPLAY_OBJECT_POP_IN;
        } else {
            object->anim_counter = object->anim_counter + 1;
        }
    } else {
        s32 sx;
        s32 sy;
        s32 su;
        s32 sv;

        sx = object->base.x;
        sprite.x = sx;
        sy = object->base.y;
        sprite.y = sy;
        su = object->bounds.position.x;
        sprite.u = su;
        sv = object->bounds.position.y;
        sprite.v = sv;
        sprite.w = object->bounds.dimensions.x;
        sprite.h = object->bounds.dimensions.y;
    }
    GsSortFastSprite(&sprite, ot, object->priority);
}
