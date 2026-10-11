#include "fft/battle.h"

/* Advances the unit SHP/SEQ loader by one step and returns the file kind index.
 *
 * g_battle_gfx_unit_graphics_load_state[0] selects the file kind (sprite SHP/SEQ, WEP SHP/SEQ, EFF SHP/SEQ,
 * WEP.SPR, OTHER.SPR), g_battle_gfx_unit_graphics_load_state[1] the phase (0 allocate and start the read,
 * 1 wait, 2 copy into the RAM tables, 3 free and advance) and g_battle_gfx_unit_graphics_load_state[2] the
 * file index within the kind.
 *
 * The target keeps several of these globals' addresses in registers; only a
 * single-assignment pointer local reproduces that (a plain `g++` folds %lo into
 * each access), and a block-scoped one per case also lets the scheduler hoist
 * the g_battle_gfx_unit_graphics_load_state[0] load above the counter store, as the target does. */
s32 battle_gfx_step_load_unit_shp_seq_data(void) {
    s32* phase;
    s32* index;
    s32* slot;
    s32 buffer;

    phase = &g_battle_gfx_unit_graphics_load_state[1];
    switch (*phase) {
    case BATTLE_GFX_UNIT_LOAD_PHASE_START_READ:
        switch (g_battle_gfx_unit_graphics_load_state[0]) {
        case BATTLE_GFX_SHARED_LOAD_SPRITE_SHP:
            index = &g_battle_gfx_unit_graphics_load_state[2];
            buffer = (s32)main_heap_alloc(g_battle_gfx_sprite_shp_files[*index].size);
            g_battle_gfx_unit_graphics_load_state[3] = buffer;
            if (buffer == 0) {
                break;
            }
            if (main_file_request_read_bytes(g_battle_gfx_sprite_shp_files[*index].sector,
                    g_battle_gfx_sprite_shp_files[*index].size, (void*)buffer)
                == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)g_battle_gfx_unit_graphics_load_state[3]);
            }
            break;
        case BATTLE_GFX_SHARED_LOAD_SPRITE_SEQ:
            index = &g_battle_gfx_unit_graphics_load_state[2];
            buffer = (s32)main_heap_alloc(g_battle_gfx_sprite_seq_files[*index].size);
            g_battle_gfx_unit_graphics_load_state[3] = buffer;
            if (buffer == 0) {
                break;
            }
            if (main_file_request_read_bytes(g_battle_gfx_sprite_seq_files[*index].sector,
                    g_battle_gfx_sprite_seq_files[*index].size, (void*)buffer)
                == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)g_battle_gfx_unit_graphics_load_state[3]);
            }
            break;
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SHP:
            index = &g_battle_gfx_unit_graphics_load_state[2];
            buffer = (s32)main_heap_alloc(g_battle_gfx_weapon_shp_files[*index].size);
            g_battle_gfx_unit_graphics_load_state[3] = buffer;
            if (buffer == 0) {
                break;
            }
            if (main_file_request_read_bytes(g_battle_gfx_weapon_shp_files[*index].sector,
                    g_battle_gfx_weapon_shp_files[*index].size, (void*)buffer)
                == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)g_battle_gfx_unit_graphics_load_state[3]);
            }
            break;
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SEQ:
            index = &g_battle_gfx_unit_graphics_load_state[2];
            buffer = (s32)main_heap_alloc(g_battle_gfx_weapon_seq_files[*index].size);
            g_battle_gfx_unit_graphics_load_state[3] = buffer;
            if (buffer == 0) {
                break;
            }
            if (main_file_request_read_bytes(g_battle_gfx_weapon_seq_files[*index].sector,
                    g_battle_gfx_weapon_seq_files[*index].size, (void*)buffer)
                == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)g_battle_gfx_unit_graphics_load_state[3]);
            }
            break;
        case BATTLE_GFX_SHARED_LOAD_EFFECT_SHP:
            index = &g_battle_gfx_unit_graphics_load_state[2];
            buffer = (s32)main_heap_alloc(g_battle_gfx_effect_shp_files[*index].size);
            g_battle_gfx_unit_graphics_load_state[3] = buffer;
            if (buffer == 0) {
                break;
            }
            if (main_file_request_read_bytes(g_battle_gfx_effect_shp_files[*index].sector,
                    g_battle_gfx_effect_shp_files[*index].size, (void*)buffer)
                == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)g_battle_gfx_unit_graphics_load_state[3]);
            }
            break;
        case BATTLE_GFX_SHARED_LOAD_EFFECT_SEQ:
            index = &g_battle_gfx_unit_graphics_load_state[2];
            buffer = (s32)main_heap_alloc(g_battle_gfx_effect_seq_files[*index].size);
            g_battle_gfx_unit_graphics_load_state[3] = buffer;
            if (buffer == 0) {
                break;
            }
            if (main_file_request_read_bytes(g_battle_gfx_effect_seq_files[*index].sector,
                    g_battle_gfx_effect_seq_files[*index].size, (void*)buffer)
                == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)g_battle_gfx_unit_graphics_load_state[3]);
            }
            break;
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SPR:
            *(slot = &g_battle_gfx_unit_graphics_load_state[3]) = (s32)main_heap_alloc(0x15000);
            if (*slot == 0) {
                break;
            }
            if (main_file_request_read_bytes(DISC_SECTOR_BATTLE_WEP_SPR, 0x15000, (void*)*slot) == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)*slot);
            }
            break;
        case BATTLE_GFX_SHARED_LOAD_OTHER_SPR:
            *(slot = &g_battle_gfx_unit_graphics_load_state[3]) = (s32)main_heap_alloc(0x8800);
            if (*slot == 0) {
                break;
            }
            if (main_file_request_read_bytes(DISC_SECTOR_BATTLE_OTHER_SPR, 0x8800, (void*)*slot) == 0) {
                g_battle_gfx_unit_graphics_load_state[1]++;
            } else {
                main_heap_call_free((void*)*slot);
            }
            break;
        }
        break;
    case BATTLE_GFX_UNIT_LOAD_PHASE_WAIT:
        if (main_file_is_still_loading() != 0) {
            break;
        }
        (*phase)++;
        break;
    case BATTLE_GFX_UNIT_LOAD_PHASE_INSTALL:
        switch (g_battle_gfx_unit_graphics_load_state[0]) {
        case BATTLE_GFX_SHARED_LOAD_SPRITE_SHP:
            battle_gfx_unpack_unit_shp_data(
                &g_battle_gfx_unit_shp_frame_tables[g_battle_gfx_unit_graphics_load_state[2]],
                g_battle_gfx_unit_graphics_load_state[3]);
            break;
        case BATTLE_GFX_SHARED_LOAD_SPRITE_SEQ:
            battle_gfx_unpack_unit_seq_data(
                g_battle_gfx_unit_seq_tables + g_battle_gfx_unit_graphics_load_state[2] * 0x408,
                g_battle_gfx_unit_graphics_load_state[3]);
            break;
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SHP:
            battle_gfx_copy_shp_data_to_ram((struct battle_gfx_shp_frame_table*)(g_battle_gfx_weapon_shp_data
                                                + g_battle_gfx_unit_graphics_load_state[2] * 0xc40),
                (u8*)g_battle_gfx_unit_graphics_load_state[3]);
            break;
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SEQ:
            battle_gfx_resolve_packed_offset_table(
                (u8**)(g_battle_gfx_weapon_seq_data + g_battle_gfx_unit_graphics_load_state[2] * 0xc40),
                (struct battle_gfx_packed_offset_table*)g_battle_gfx_unit_graphics_load_state[3]);
            break;
        case BATTLE_GFX_SHARED_LOAD_EFFECT_SHP:
            battle_gfx_copy_shp_data_to_ram((struct battle_gfx_shp_frame_table*)(g_battle_gfx_effect_shp_data
                                                + g_battle_gfx_unit_graphics_load_state[2] * 0xc40),
                (u8*)g_battle_gfx_unit_graphics_load_state[3]);
            break;
        case BATTLE_GFX_SHARED_LOAD_EFFECT_SEQ:
            battle_gfx_resolve_packed_offset_table(
                (u8**)(g_battle_gfx_effect_seq_data + g_battle_gfx_unit_graphics_load_state[2] * 0xc40),
                (struct battle_gfx_packed_offset_table*)g_battle_gfx_unit_graphics_load_state[3]);
            break;
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SPR:
            battle_gfx_load_wep_spr_data((u8*)g_battle_gfx_unit_graphics_load_state[3]);
            break;
        case BATTLE_GFX_SHARED_LOAD_OTHER_SPR:
            battle_gfx_copy_other_spr_palette_data_to_ram((u8*)g_battle_gfx_unit_graphics_load_state[3]);
            break;
        }
        {
            s32* next_phase = &g_battle_gfx_unit_graphics_load_state[1];

            (*next_phase)++;
        }
        break;
    case BATTLE_GFX_UNIT_LOAD_PHASE_FINISH:
        main_heap_call_free((void*)g_battle_gfx_unit_graphics_load_state[3]);
        *phase = BATTLE_GFX_UNIT_LOAD_PHASE_START_READ;
        switch (g_battle_gfx_unit_graphics_load_state[0]) {
        case BATTLE_GFX_SHARED_LOAD_SPRITE_SHP: {
            s32* counter = &g_battle_gfx_unit_graphics_load_state[2];

            if (++*counter >= 5) {
                *counter = 0;
                g_battle_gfx_unit_graphics_load_state[0]++;
            }
            break;
        }
        case BATTLE_GFX_SHARED_LOAD_SPRITE_SEQ: {
            s32* counter = &g_battle_gfx_unit_graphics_load_state[2];

            if (++*counter >= 5) {
                *counter = 0;
                g_battle_gfx_unit_graphics_load_state[0]++;
            }
            break;
        }
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SHP:
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SEQ: {
            s32* counter = &g_battle_gfx_unit_graphics_load_state[2];

            if (++*counter >= 2) {
                *counter = 0;
                g_battle_gfx_unit_graphics_load_state[0]++;
            }
            break;
        }
        case BATTLE_GFX_SHARED_LOAD_EFFECT_SHP:
        case BATTLE_GFX_SHARED_LOAD_EFFECT_SEQ: {
            s32* counter = &g_battle_gfx_unit_graphics_load_state[2];

            if (++*counter >= 1) {
                *counter = 0;
                g_battle_gfx_unit_graphics_load_state[0]++;
            }
            break;
        }
        case BATTLE_GFX_SHARED_LOAD_WEAPON_SPR:
        case BATTLE_GFX_SHARED_LOAD_OTHER_SPR: {
            s32* kind = &g_battle_gfx_unit_graphics_load_state[0];

            g_battle_gfx_unit_graphics_load_state[2] = 0;
            (*kind)++;
            break;
        }
        }
        break;
    }
    return g_battle_gfx_unit_graphics_load_state[0];
}
