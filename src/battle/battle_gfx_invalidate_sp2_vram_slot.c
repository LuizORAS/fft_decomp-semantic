#include "fft/battle.h"

typedef struct battle_gfx_misc_data_header {
    u8 _unused_00[4];
    u8 unit_id;
} battle_gfx_misc_data_header_t;

/* Free the EVTCHR slots whose page holds the unit's SP2 image. */
void battle_gfx_invalidate_sp2_vram_slot(battle_gfx_misc_data_header_t* unit) {
    u32 invalid;
    u8* slot;
    s32 count;

    count = 0;
    invalid = BATTLE_GFX_VRAM_SLOT_OWNER_NONE;
    slot = (u8*)g_battle_gfx_vram_slots;
    do {
        u32 descriptor = *(u32*)slot;

        count++;
        if (((descriptor & BATTLE_GFX_VRAM_SLOT_OWNER_ID_MASK) == unit->unit_id)
            && ((descriptor & BATTLE_GFX_VRAM_SLOT_OWNER_SP2) != 0)) {
            *(u32*)slot = invalid;
        }
        slot += 0x7564;
    } while (count < 2);
}
