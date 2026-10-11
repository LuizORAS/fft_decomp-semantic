#include "fft/battle.h"

/* Return the frame tables that a unit frame id indexes and the frame itself:
 * a battle_gfx_source_frame_t pointer, as a word.
 *
 * A frame below BATTLE_SHP_FRAME_EVTCHR_FIRST is one of the unit's own SHP.
 * Its submerged table replaces the plain one for a rider that is not
 * jumping, and for a unit on a water tile whose surface is 12 or more above
 * the unit's feet; from 36 above, the unit shows submerged frame 0 whatever
 * the frame id. A unit that floats or moves on water, a crystal and a
 * treasure keep the plain table, and a chicken or a frog changes at 4.
 * Higher frame ids index the SHP of an EVTCHR slot: slot 0 for the sequences
 * below 600 and slot 1 for the rest, the submerged table on a water tile. */
void battle_gfx_select_unit_shp_frame(battle_unit_misc_data_t* unit, battle_unit_anim_state_t* state, u16 frame,
    u16 sequence_id, s32** out_table, s32* out_value) {
    map_tile_t* tile;
    s32* table;
    s32 value;
    s32 flags;
    s32 height;
    s32 z;

    tile = battle_map_get_tile_data_pointer((unit->screen.vx / 28), (unit->screen.vz / 28), unit->map_z);
    if (frame < BATTLE_SHP_FRAME_EVTCHR_FIRST) {
        table = (s32*)state->shp;
        if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_RIDER) {
            if (unit->status_flags_5_6 & (BATTLE_MISC_STATUS_JUMP | BATTLE_MISC_STATUS_JUMP_HEIGHT_ACTIVE)) {
                value = table[frame + 2];
            } else {
                value = table[frame + 0xd2];
            }
        } else if (tile->depth_half_height.value & MAP_TILE_DEPTH_MASK) {
            if (unit->battle_data != 0) {
                flags = battle_move_get_effective_flags(unit->battle_data);
                /* The combined test keeps the table pointer's reference count
                 * low enough for the target's s0/s1 allocation. */
                if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_FLOAT) {
                    value = table[frame + 2];
                } else if ((flags & BATTLE_EFFECTIVE_MOVEMENT_ON_WATER) || (flags & BATTLE_EFFECTIVE_MOVEMENT_FLOAT)) {
                    value = table[frame + 2];
                } else if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_CRYSTAL) {
                    value = table[frame + 2];
                } else if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_TREASURE) {
                    value = table[frame + 2];
                } else if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_CHICKEN
                    || unit->spritesheet_id == BATTLE_SPRITESHEET_ID_FROG) {
                    if ((tile->height + (tile->depth_half_height.value >> 5)) * 12 >= -unit->screen.vy + 4) {
                        value = table[frame + 0xd2];
                    } else {
                        value = table[frame + 2];
                    }
                } else {
                    height = (tile->height + (tile->depth_half_height.value >> 5)) * 12;
                    z = -unit->screen.vy;
                    if (height >= z + 0x24) {
                        value = table[0xd2];
                    } else if (height >= z + 0xc) {
                        value = table[frame + 0xd2];
                    } else {
                        value = table[frame + 2];
                    }
                }
            } else {
                height = (tile->height + (tile->depth_half_height.value >> 5)) * 12;
                z = -unit->screen.vy;
                if (height >= z + 0x24) {
                    value = table[0xd2];
                } else if (height >= z + 0xc) {
                    value = table[frame + 0xd2];
                } else {
                    value = table[frame + 2];
                }
            }
        } else {
            value = table[frame + 2];
        }
    } else {
        /* Flat word view of the second slot's SHP: header, then frames. The
         * first slot's is one slot record, 0x1d59 words, below it. */
        table = g_battle_gfx_vram_slots[1].shp.header;
        if (sequence_id < BATTLE_SEQUENCE_EVTCHR_SLOT_1) {
            table -= 0x1d59;
        }
        /* A shifted byte offset keeps the target's base-first addu. */
        if (tile->depth_half_height.value & MAP_TILE_DEPTH_MASK) {
            value = *(s32*)((u8*)table + (frame << 2));
        } else {
            value = *(s32*)((u8*)table + (frame << 2) - 0x340);
        }
    }
    *out_table = table;
    *out_value = value;
}
