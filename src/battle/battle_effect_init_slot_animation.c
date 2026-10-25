#include "fft/battle_effect.h"
#include "psx/types.h"

/* Start the animation script of particle group `group` on effect slot `slot`: reset its color and
 * script state and store the CLUT at 0x88 (callers pass CLUTs of MAP_CLUT_ROW_WEAPONS).
 * battle_effect_start_group and the charge, weapon-hit and poof handlers call it.
 *
 * The script table is declared `u8[]` and cast only at the 16-bit read. The target has no stack frame.
 * Casting an array symbol away from its declared element type (`(u8*)table + offset` off a `u16[]`, or
 * `(u32)`, `&table[0]` or a local `u8* p`) allocates an expand-time stack temp that later optimization
 * deletes, and gcc 2.6.3 never shrinks the frame afterwards: an 8-byte frame appears whose `addiu sp` pair
 * displaces the load-delay nop and the `jr ra` delay-slot store. No zero-instruction constraint reaches that
 * temp; only keeping every symbol at its own type avoids it. */
void battle_effect_init_slot_animation(s32 group, u16 clut, battle_effect_slot_t* slot) {
    u32 offset;

    slot->clut = clut;
    offset = *(
        u16*)(g_battle_effect_slot_animation_scripts + (g_battle_effect_particle_groups[group].animation_script << 1));
    slot->color.b = 0x80;
    slot->color.g = 0x80;
    slot->color.r = 0x80;
    slot->step_index = 0;
    slot->hold = 0;
    slot->angle = 0;
    slot->script = (battle_effect_anim_script_t*)(g_battle_effect_slot_animation_scripts + offset);
}
