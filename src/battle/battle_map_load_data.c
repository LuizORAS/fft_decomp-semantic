#include "fft/battle.h"
#include "psx/types.h"

s32 battle_map_load_data(s32 map_id, s32 command) {
    s32 i;
    s32 r;
    s32 j;
    s16 state;
    u8 diff;

    if (g_battle_map_gns_load_phase == MAP_GNS_LOAD_PHASE_IDLE) {
        switch (command) {
        case MAP_LOAD_COMMAND_LOAD_GNS:
            if (main_file_is_still_loading() != 0) {
                return 0;
            }
            g_battle_map_data_load_buffer = game_malloc(0x20000);
            if (g_battle_map_data_load_buffer == 0) {
                main_system_handle_malloc_exception(2, 0x3c2);
            }
            battle_map_load_gns_and_move_find_items(map_id & 0xffff);
            g_battle_map_resource_load_state = MAP_LOAD_STATE_GNS_FILE;
            g_battle_map_loaded_id = map_id;
            g_battle_map_gns_load_phase = MAP_GNS_LOAD_PHASE_START;
            battle_map_reset_mesh_render_state();
            break;
        case MAP_LOAD_COMMAND_START_HELD:
            g_battle_map_gns_dispatch_held = 1;
        case MAP_LOAD_COMMAND_START:
            if (main_file_is_still_loading() != 0) {
                return 0;
            }
            g_battle_map_data_load_buffer = game_malloc(0x20000);
            if (g_battle_map_data_load_buffer == 0) {
                main_system_handle_malloc_exception(2, 0x3c2);
            }
            g_battle_map_texture_quarter_0_pending = 0;
            g_battle_map_texture_quarter_1_pending = 0;
            g_battle_map_texture_quarter_2_pending = 0;
            g_battle_map_texture_quarter_3_pending = 0;
            g_battle_map_texture_pending = 0;
            g_battle_map_palette_pending = 0;
            g_battle_map_lighting_pending = 0;
            if (map_id != g_battle_map_loaded_id) {
                if (battle_map_load_gns_and_move_find_items(map_id & 0xffff) != g_battle_map_gns_records) {
                    return 0;
                }
                g_battle_map_resource_load_state = MAP_LOAD_STATE_GNS_FILE;
                g_battle_map_loaded_id = map_id;
                g_battle_map_gns_load_phase = MAP_GNS_LOAD_PHASE_START;
                battle_map_reset_mesh_render_state();
            } else {
                g_battle_map_resource_load_state = MAP_LOAD_STATE_IDLE;
                g_battle_map_gns_load_phase = MAP_GNS_LOAD_PHASE_START;
            }
            break;
        case MAP_LOAD_STATE_GNS_FILE:
        case MAP_LOAD_COMMAND_POLL_HELD:
            break;
        }
    }

    if (g_battle_map_resource_load_state == MAP_LOAD_STATE_IDLE) {
        if (g_battle_map_gns_load_phase != MAP_GNS_LOAD_PHASE_RESOURCES) {
            if (g_battle_map_gns_load_phase == MAP_GNS_LOAD_PHASE_START) {
                g_battle_map_gns_record_cursor = 0;
                g_battle_map_gns_load_phase = MAP_GNS_LOAD_PHASE_RESOURCES;
                {
                    s32 k;

                    for (k = 0; k < GNS_RESOURCE_END; k++) {
                        g_battle_map_pending_gns_records[k].resource_type = GNS_RESOURCE_NONE;
                    }
                }
                for (r = 0; g_battle_map_gns_records[r].resource_type < 0x80; r++) {
                    if (g_battle_map_gns_records[r].resource_type == GNS_RESOURCE_MESH_PRIMARY) {
                        state = battle_script_get_variable(g_battle_map_gns_records[r].script_variable_id) & 0xfff;
                        state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER) & 7) << 12;
                        state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY) & 1) << 15;
                        if ((s16)battle_map_is_gns_record_matching_state(
                                g_battle_map_gns_records[r].map_state_comparison, g_battle_map_gns_records[r].map_state,
                                state)) {
                            /* No braces: a braced body's block note lengthens this loop by one
                             * luid, and loop.c then hoists the 0x48 out of the record loop. */
                            j = 0;
                            do
                                g_battle_map_loaded_gns_records[g_battle_map_gns_records[r].resource_type].load_info[j]
                                    = 0x48;
                            while (++j < 14);
                        }
                        {
                            s32 k;

                            diff = 0;
                            for (k = 0; k < 14; k++) {
                                diff |= g_battle_map_loaded_gns_records[g_battle_map_gns_records[r].resource_type]
                                            .load_info[k]
                                    ^ g_battle_map_gns_records[r].load_info[k];
                            }
                        }
                        if (diff) {
                            g_battle_map_pending_gns_records[g_battle_map_gns_records[r].resource_type]
                                = g_battle_map_gns_records[r];
                        }
                    }
                }
                for (r = 0; g_battle_map_gns_records[r].resource_type < 0x80; r++) {
                    if (g_battle_map_gns_records[r].resource_type == GNS_RESOURCE_MESH_OVERRIDE) {
                        state = battle_script_get_variable(g_battle_map_gns_records[r].script_variable_id) & 0xfff;
                        state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER) & 7) << 12;
                        state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY) & 1) << 15;
                        if ((s16)battle_map_is_gns_record_matching_state(
                                g_battle_map_gns_records[r].map_state_comparison, g_battle_map_gns_records[r].map_state,
                                state)) {
                            j = 0;
                            do
                                g_battle_map_loaded_gns_records[g_battle_map_gns_records[r].resource_type].load_info[j]
                                    = 0x48;
                            while (++j < 14);
                        }
                        {
                            s32 k;

                            diff = 0;
                            for (k = 0; k < 14; k++) {
                                diff |= g_battle_map_loaded_gns_records[g_battle_map_gns_records[r].resource_type]
                                            .load_info[k]
                                    ^ g_battle_map_gns_records[r].load_info[k];
                            }
                        }
                        if (diff) {
                            g_battle_map_pending_gns_records[g_battle_map_gns_records[r].resource_type]
                                = g_battle_map_gns_records[r];
                        }
                    }
                }
                for (r = 0; g_battle_map_gns_records[r].resource_type < 0x80; r++) {
                    if (g_battle_map_gns_records[r].resource_type == GNS_RESOURCE_TEXTURE
                        && (g_battle_map_gns_records[r].script_variable_id == EVENT_SCRIPT_VAR_WEATHER_AND_TIME
                            || g_battle_map_gns_records[r].script_variable_id == EVENT_SCRIPT_VAR_MAP_ARRANGEMENT)
                        && g_battle_map_gns_records[r].map_state == 0) {
                        {
                            s32 k;

                            diff = 0;
                            for (k = 0; k < 14; k++) {
                                diff |= g_battle_map_loaded_gns_records[g_battle_map_gns_records[r].resource_type]
                                            .load_info[k]
                                    ^ g_battle_map_gns_records[r].load_info[k];
                            }
                        }
                        if (diff) {
                            g_battle_map_pending_gns_records[g_battle_map_gns_records[r].resource_type]
                                = g_battle_map_gns_records[r];
                        }
                    }
                }
                for (r = 0; g_battle_map_gns_records[r].resource_type < 0x80; r++) {
                    if (g_battle_map_gns_records[r].resource_type != GNS_RESOURCE_NONE
                        && (g_battle_map_gns_records[r].script_variable_id != EVENT_SCRIPT_VAR_WEATHER_AND_TIME
                            || g_battle_map_gns_records[r].map_state != 0)
                        && (g_battle_map_gns_records[r].script_variable_id != EVENT_SCRIPT_VAR_MAP_ARRANGEMENT
                            || g_battle_map_gns_records[r].map_state != 0)) {
                        state = battle_script_get_variable(g_battle_map_gns_records[r].script_variable_id) & 0xfff;
                        state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER) & 7) << 12;
                        state |= (battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY) & 1) << 15;
                        if ((s16)battle_map_is_gns_record_matching_state(
                                g_battle_map_gns_records[r].map_state_comparison, g_battle_map_gns_records[r].map_state,
                                state)) {
                            {
                                s32 k;

                                diff = 0;
                                for (k = 0; k < 14; k++) {
                                    diff |= g_battle_map_loaded_gns_records[g_battle_map_gns_records[r].resource_type]
                                                .load_info[k]
                                        ^ g_battle_map_gns_records[r].load_info[k];
                                }
                            }
                            if (diff) {
                                g_battle_map_pending_gns_records[g_battle_map_gns_records[r].resource_type]
                                    = g_battle_map_gns_records[r];
                            } else {
                                g_battle_map_pending_gns_records[g_battle_map_gns_records[r].resource_type]
                                    .resource_type = GNS_RESOURCE_NONE;
                            }
                        }
                    }
                }
            }
        } else {
            if (main_file_is_still_loading() == 0) {
                while (
                    (g_battle_map_pending_gns_records[g_battle_map_gns_record_cursor].resource_type == GNS_RESOURCE_NONE
                        && g_battle_map_gns_record_cursor < GNS_RESOURCE_END)
                    || (g_battle_map_pending_gns_records[g_battle_map_gns_record_cursor].resource_type >= 0x12
                        && g_battle_map_pending_gns_records[g_battle_map_gns_record_cursor].resource_type <= 0x17)) {
                    g_battle_map_gns_record_cursor++;
                }
                if (g_battle_map_gns_record_cursor < GNS_RESOURCE_END) {
                    if (g_battle_map_gns_record_cursor == GNS_RESOURCE_TEXTURE_ANIMATIONS) {
                        bzero(g_battle_map_data_load_buffer, 0x280);
                    }
                    battle_map_start_file_load(g_battle_map_file_table,
                        g_battle_map_pending_gns_records[g_battle_map_gns_record_cursor].load_info,
                        (s32)g_battle_map_data_load_buffer, g_battle_map_gns_record_cursor);
                    g_battle_map_resource_load_state = MAP_LOAD_STATE_RESOURCE_FILE;
                } else if (g_battle_map_pending_gns_records[GNS_RESOURCE_UNSUPPORTED_12].resource_type
                    != GNS_RESOURCE_NONE) {
                    main_system_handle_malloc_exception(2, 0x3cb);
                    g_battle_map_pending_gns_records[GNS_RESOURCE_UNSUPPORTED_12].resource_type = GNS_RESOURCE_NONE;
                } else if (g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_0].resource_type
                    != GNS_RESOURCE_NONE) {
                    g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_0].resource_type = GNS_RESOURCE_NONE;
                    battle_map_start_file_load(g_battle_map_file_table,
                        g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_0].load_info,
                        (s32)g_battle_map_data_load_buffer + 0x8000, GNS_RESOURCE_TEXTURE_QUARTER_0);
                    g_battle_map_texture_quarter_0_pending = 1;
                    g_battle_map_texture_pending = 1;
                } else if (g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_1].resource_type
                    != GNS_RESOURCE_NONE) {
                    g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_1].resource_type = GNS_RESOURCE_NONE;
                    battle_map_start_file_load(g_battle_map_file_table,
                        g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_1].load_info,
                        (s32)g_battle_map_data_load_buffer + 0x10000, GNS_RESOURCE_TEXTURE_QUARTER_1);
                    g_battle_map_texture_quarter_1_pending = 1;
                    g_battle_map_texture_pending = 1;
                } else if (g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_2].resource_type
                    != GNS_RESOURCE_NONE) {
                    g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_2].resource_type = GNS_RESOURCE_NONE;
                    battle_map_start_file_load(g_battle_map_file_table,
                        g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_2].load_info,
                        (s32)g_battle_map_data_load_buffer + 0x18000, GNS_RESOURCE_TEXTURE_QUARTER_2);
                    g_battle_map_texture_quarter_2_pending = 1;
                    g_battle_map_texture_pending = 1;
                } else if (g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_3].resource_type
                    != GNS_RESOURCE_NONE) {
                    g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_3].resource_type = GNS_RESOURCE_NONE;
                    battle_map_start_file_load(g_battle_map_file_table,
                        g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE_QUARTER_3].load_info,
                        (s32)g_battle_map_data_load_buffer + 0x20000, GNS_RESOURCE_TEXTURE_QUARTER_3);
                    g_battle_map_texture_quarter_3_pending = 1;
                    g_battle_map_texture_pending = 1;
                } else if (g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE].resource_type != GNS_RESOURCE_NONE) {
                    g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE].resource_type = GNS_RESOURCE_NONE;
                    battle_map_start_file_load(g_battle_map_file_table,
                        g_battle_map_pending_gns_records[GNS_RESOURCE_TEXTURE].load_info,
                        (s32)g_battle_map_data_load_buffer, GNS_RESOURCE_TEXTURE);
                    g_battle_map_texture_quarter_0_pending = 1;
                    g_battle_map_texture_quarter_1_pending = 1;
                    g_battle_map_texture_quarter_2_pending = 1;
                    g_battle_map_texture_quarter_3_pending = 1;
                    g_battle_map_texture_pending = 1;
                    g_battle_map_resource_load_state = MAP_LOAD_STATE_APPLY_LIGHTING;
                } else {
                    for (i = 0; g_battle_map_gns_records[i].resource_type < 0x80; i++) {
                        if (g_battle_map_gns_records[i].resource_type == GNS_RESOURCE_NONE) {
                            break;
                        }
                    }
                    i++;
                    g_battle_map_gns_record_cursor = (s32)&g_battle_map_gns_records[i];
                    while (((gns_command_record_prefix_t*)g_battle_map_gns_record_cursor)->resource_type
                        != GNS_RESOURCE_NONE) {
                        battle_process_map_gns_record(2, (gns_command_record_prefix_t*)g_battle_map_gns_record_cursor);
                    }
                    g_battle_map_gns_load_phase = MAP_GNS_LOAD_PHASE_IDLE;
                    g_battle_map_resource_load_state = MAP_LOAD_STATE_DONE;
                    battle_map_set_weather_texture_overlay(MAP_WEATHER_COMMAND_APPLY_SCRIPT_WEATHER);
                    main_heap_free(g_battle_map_data_load_buffer);
                }
            }
        }
    } else {
        switch (g_battle_map_resource_load_state) {
        case MAP_LOAD_STATE_GNS_FILE:
            if (main_file_is_still_loading() == 0) {
                gns_record_view_t* source;
                gns_record_view_t* destination;

                i = 0;
                source = g_battle_map_data_load_buffer;
                destination = g_battle_map_gns_records;
                g_battle_map_resource_load_state = MAP_LOAD_STATE_IDLE;
                do {
                    *destination = *source;
                    destination++;
                    source++;
                    i++;
                } while (i < 150);
            }
            break;
        case MAP_LOAD_STATE_RESOURCE_FILE:
            if (main_file_is_still_loading() == 0) {
                if (g_battle_map_gns_dispatch_held != 0) {
                    return MAP_LOAD_RESULT_HELD;
                }
                g_battle_map_resource_load_state = MAP_LOAD_STATE_IDLE;
                battle_map_dispatch_gns_resource(
                    g_battle_map_pending_gns_records[g_battle_map_gns_record_cursor].resource_type,
                    g_battle_map_data_load_buffer);
                g_battle_map_loaded_gns_records[g_battle_map_gns_record_cursor]
                    = g_battle_map_pending_gns_records[g_battle_map_gns_record_cursor];
                g_battle_map_gns_record_cursor++;
            }
            break;
        case MAP_LOAD_STATE_APPLY_LIGHTING:
            if (main_file_is_still_loading() == 0) {
                if (g_battle_map_lighting_pending != 0) {
                    battle_map_dispatch_gns_resource(GNS_RESOURCE_APPLY_LIGHTING, g_battle_map_pending_lighting_data);
                }
                g_battle_map_lighting_pending = 0;
                g_battle_map_resource_load_state = MAP_LOAD_STATE_APPLY_TEXTURE;
            }
            break;
        case MAP_LOAD_STATE_APPLY_TEXTURE:
            if (g_battle_map_texture_pending != 0) {
                battle_map_dispatch_gns_resource(GNS_RESOURCE_TEXTURE, g_battle_map_data_load_buffer);
            }
            if (g_battle_map_palette_pending != 0) {
                battle_map_dispatch_gns_resource(GNS_RESOURCE_APPLY_PALETTES, g_battle_map_pending_palette_data);
            }
            g_battle_map_resource_load_state = MAP_LOAD_STATE_IDLE;
            break;
        case MAP_LOAD_STATE_MESH_RECORDS:
            if (g_battle_map_mesh_data_buffer != (s32)g_battle_gfx_current_render_buffer) {
                g_battle_map_textured_triangle_render_record_count = 0;
                g_battle_map_textured_quad_render_record_count = 0;
                g_battle_map_untextured_triangle_render_record_count = 0;
                g_battle_map_untextured_quad_render_record_count = 0;
                for (i = 0; i < 9; i++) {
                    if (g_battle_map_mesh_part_geometry[i] != 0) {
                        battle_map_init_mesh_render_records(g_battle_map_mesh_part_geometry[i]);
                    }
                    g_battle_map_textured_triangle_render_record_count
                        += g_battle_map_mesh_parts[i].textured_triangle_count;
                    g_battle_map_textured_quad_render_record_count += g_battle_map_mesh_parts[i].textured_quad_count;
                    g_battle_map_untextured_triangle_render_record_count
                        += g_battle_map_mesh_parts[i].untextured_triangle_count;
                    g_battle_map_untextured_quad_render_record_count
                        += g_battle_map_mesh_parts[i].untextured_quad_count;
                }
                g_battle_map_resource_load_state = MAP_LOAD_STATE_MESH_RECORDS_SECOND;
            }
            break;
        case MAP_LOAD_STATE_MESH_RECORDS_SECOND:
            if (g_battle_map_mesh_data_buffer != (s32)g_battle_gfx_current_render_buffer) {
                g_battle_map_textured_triangle_render_record_count = 0;
                g_battle_map_textured_quad_render_record_count = 0;
                g_battle_map_untextured_triangle_render_record_count = 0;
                g_battle_map_untextured_quad_render_record_count = 0;
                for (i = 0; i < 9; i++) {
                    if (g_battle_map_mesh_part_geometry[i] != 0) {
                        battle_map_init_mesh_render_records(g_battle_map_mesh_part_geometry[i]);
                    }
                    g_battle_map_mesh_part_geometry[i] = 0;
                    g_battle_map_textured_triangle_render_record_count
                        += g_battle_map_mesh_parts[i].textured_triangle_count;
                    g_battle_map_textured_quad_render_record_count += g_battle_map_mesh_parts[i].textured_quad_count;
                    g_battle_map_untextured_triangle_render_record_count
                        += g_battle_map_mesh_parts[i].untextured_triangle_count;
                    g_battle_map_untextured_quad_render_record_count
                        += g_battle_map_mesh_parts[i].untextured_quad_count;
                }
                g_battle_map_resource_load_state = MAP_LOAD_STATE_IDLE;
            }
            break;
        }
    }
    return g_battle_map_gns_load_phase;
}
