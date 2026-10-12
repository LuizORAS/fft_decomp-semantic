#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "fft/battle_map.h"

/* Runs the loaded effect file one frame and returns 1 while it plays.
 *
 * Once battle_effect_play releases a requested effect, the file's two CLUT
 * rows and its texture go to VRAM; the memory from that last section of the
 * file on then becomes the effect heap, the section pointers are bound and
 * the root record starts the script. Each later frame runs every allocated
 * record's script until it yields, freeing the records whose scripts end, and
 * the effect is over when none is left. */
s32 battle_effect_update_stage(void) {
    s32 result;
    s32 id;
    s32 next;

    id = g_battle_effect_allocated_record_head;
    switch (g_battle_effect_phase) {
    case BATTLE_EFFECT_PHASE_IDLE:
        result = 0;
        break;
    case BATTLE_EFFECT_PHASE_REQUESTED:
        result = 1;
        break;
    case BATTLE_EFFECT_PHASE_INIT_PALETTE:
        battle_map_freeze();
        battle_menu_suppress_height_display();
        battle_effect_init_palette(g_battle_ability_effect_id);
        g_battle_effect_phase = BATTLE_EFFECT_PHASE_START_SCRIPT;
        D_801BF000 = 0;
        battle_state_reset_frame_pacing_timer();
        result = 1;
        break;
    case BATTLE_EFFECT_PHASE_START_SCRIPT:
        battle_heap_init((battle_heap_node_t*)g_battle_effect_palette_ptr,
            g_battle_heap_end_address - (u32)g_battle_effect_palette_ptr);
        battle_effect_reset_subsystems();
        battle_effect_copy_camera_angles_and_screen_coords();
        battle_state_clear_frame_pacing();
        battle_effect_set_frame_data_pointer(g_battle_ability_effect_id);
        battle_effect_set_animation_sequences_pointer(g_battle_ability_effect_id);
        battle_effect_set_script_pointer(g_battle_ability_effect_id);
        battle_effect_set_animation_curve_data_pointer(g_battle_ability_effect_id);
        battle_effect_set_particle_system_data_pointer(g_battle_ability_effect_id);
        battle_effect_set_timeline_pointers(g_battle_ability_effect_id);
        battle_effect_load_sound_section(g_battle_ability_effect_id);
        battle_effect_init_flags_section_pointer(g_battle_ability_effect_id);
        battle_effect_set_timing_curves_pointer(g_battle_ability_effect_id);
        if (g_battle_effect_context.math_skill_flag != 0) {
            if (g_battle_effect_flags_section->spawn_delay_override != 0) {
                g_battle_effect_timeline->spawn_delay = g_battle_effect_flags_section->spawn_delay_override;
            } else {
                g_battle_effect_timeline->spawn_delay = 100;
            }
        }
        g_battle_effect_root_record_index = battle_effect_start_script_record(g_battle_effect_script, 0, 0);
        g_battle_effect_phase = BATTLE_EFFECT_PHASE_RUNNING;
        battle_map_configure_effect_data_state(MAP_EFFECT_DATA_BEGIN);
        result = 1;
        break;
    case BATTLE_EFFECT_PHASE_RUNNING:
        g_battle_effect_otag = main_gfx_get_otag();
        battle_effect_start_render_frame();
        while (id != 0) {
            next = g_battle_effect_state_records[id].next_index;
            g_battle_effect_current_record_index = id;
            g_battle_effect_state_records[id].frame_counter++;
            if (battle_effect_run_block(id) == 0) {
                battle_effect_free_state_record(id);
            }
            id = next;
        }
        if (g_battle_effect_allocated_record_head != 0) {
            result = 1;
        } else {
            if (g_battle_effect_flags_section->flags & EFFECT_FLAG_QUIET_MUSIC) {
                main_sound_set_current_music_volume_timed(0x78);
            }
            if (g_battle_effect_sound_section_ptr != 0) {
                suzuki_sound_remove_resource(g_battle_effect_sound_section_ptr);
                g_battle_effect_sound_section_ptr = 0;
            }
            battle_map_configure_effect_data_state(MAP_EFFECT_DATA_END);
            battle_map_unfreeze();
            battle_menu_enable_height_display();
            battle_state_clear_frame_pacing();
            g_battle_effect_phase = BATTLE_EFFECT_PHASE_IDLE;
            result = 0;
        }
        break;
    case BATTLE_EFFECT_PHASE_RESTART_SCRIPT:
        battle_heap_init((battle_heap_node_t*)g_battle_effect_palette_ptr,
            g_battle_heap_end_address - (u32)g_battle_effect_palette_ptr);
        battle_effect_reset_subsystems();
        battle_effect_reset_frame_data_pointer();
        battle_effect_reset_animation_sequences_pointer();
        g_battle_effect_root_record_index = battle_effect_start_script_record(g_battle_effect_script, 0, 0);
        g_battle_effect_phase = BATTLE_EFFECT_PHASE_RUNNING;
        return 1;
    }
    return result;
}
