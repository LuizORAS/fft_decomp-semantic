/* Keep the trap animation table as u8[]: another element type introduces an
 * 8-byte compiler frame and changes the load-delay and return slots. */
#include "fft/battle_effect.h"
#include "psx/types.h"

/* Start the animation script of trap group on slot: reset the colour and
 * script state and store the 16-bit CLUT value (callers pass CLUTs of
 * MAP_CLUT_ROW_WEAPONS) at 0x88. */
void battle_effect_init_trap_animation(s32 group, u16 clut, battle_effect_slot_t* slot) {
    u32 offset;

    slot->clut = clut;
    offset = *(
        u16*)(g_battle_effect_trap_animation_data + (g_battle_effect_trap_particle_configs[group].trap_animation << 1));
    slot->color.b = 0x80;
    slot->color.g = 0x80;
    slot->color.r = 0x80;
    slot->step_index = 0;
    slot->hold = 0;
    slot->angle = 0;
    slot->script = (battle_effect_anim_script_t*)(g_battle_effect_trap_animation_data + offset);
}
