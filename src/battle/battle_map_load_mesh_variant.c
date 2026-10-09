#include "fft/battle.h"

enum {
    MAP_MESH_LOAD_IDLE = 0,
    MAP_MESH_LOAD_WAITING = 0x8d,
};

/* g_battle_map_gns_records viewed with the loader's halfword-aligned type,
 * which copies whole records with lwl/lwr pairs. */
extern gns_record_view_t g_battle_map_gns_records_view[150];
extern gns_record_view_t g_battle_map_selected_mesh_record;

/* Selects the last primary-mesh record, then substitutes an alternate whose
 * layout/weather/time comparison matches, and loads the selected record's
 * file into a temporary 0x20000-byte buffer. The loader's fourth argument,
 * 0x30, is forwarded as display suppression; battle_map_start_file_load does
 * not use it to index a resource slot. */
s32 battle_map_load_mesh_variant(s32 mesh_slot) {
    s32 i;
    s16 state;

    switch (g_battle_map_mesh_load_state) {
    case MAP_MESH_LOAD_IDLE:
        for (i = 0; g_battle_map_gns_records_view[i].resource_type < 0x80; i++) {
            if (g_battle_map_gns_records_view[i].resource_type == GNS_RESOURCE_MESH_PRIMARY) {
                g_battle_map_selected_mesh_record = g_battle_map_gns_records_view[i];
            }
        }
        for (i = 0; g_battle_map_gns_records_view[i].resource_type < 0x80; i++) {
            if (g_battle_map_gns_records_view[i].resource_type == GNS_RESOURCE_MESH_ALTERNATE) {
                state = battle_script_get_variable(g_battle_map_gns_records_view[i].script_variable_id) & 0xfff;
                state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER) & 7) << 12;
                state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY) & 1) << 15;
                if ((s16)battle_map_is_gns_record_matching_state(g_battle_map_gns_records_view[i].map_state_comparison,
                        g_battle_map_gns_records_view[i].map_state, state)) {
                    g_battle_map_selected_mesh_record = g_battle_map_gns_records_view[i];
                }
            }
        }
        g_battle_map_mesh_load_buffer = game_malloc(0x20000);
        if (g_battle_map_mesh_load_buffer == 0) {
            main_system_handle_malloc_exception(2, 0x3c9);
        }
        battle_map_start_file_load(g_battle_map_file_table, g_battle_map_selected_mesh_record.load_info,
            (s32)g_battle_map_mesh_load_buffer, 0x30);
        g_battle_map_mesh_load_state = MAP_MESH_LOAD_WAITING;
        break;

    case MAP_MESH_LOAD_WAITING:
        if (main_file_is_still_loading() != 0) {
            s32 mesh_data_offset;
            void* mesh_data;

            mesh_data = g_battle_map_mesh_load_buffer;
            /* Retail scales this byte offset as a word index. */
            mesh_data_offset = ((map_mesh_file_header_t*)mesh_data)->color_palette_offset * 4;
            mesh_data = (u8*)mesh_data + mesh_data_offset;
            battle_map_update_palette_colors(mesh_slot, 1, 0, 1, (const u16*)mesh_data);
            main_heap_free(g_battle_map_mesh_load_buffer);
            g_battle_map_mesh_load_state = MAP_MESH_LOAD_IDLE;
        }
        break;
    }
    return g_battle_map_mesh_load_state;
}
