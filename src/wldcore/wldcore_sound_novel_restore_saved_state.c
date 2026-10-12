#include "fft/wldcore.h"

/* View of the picture state at 0x800d485c (render record index,
 * then the pending picture id). */
typedef struct wldcore_script_picture_state {
    s16 render_index; /* 0x00; g_wldcore_sound_novel_record.picture_render_index */
    s16 picture;      /* 0x02; g_wldcore_sound_novel_record.picture_id */
} wldcore_script_picture_state_t;

/*
 * Loads the sound novel's reading state and replays what it implies.
 *
 * Slots 0..4 of g_main_sound_novel_records hold a saved position; any other index
 * starts from a reset record. `render_record` becomes the picture's render
 * record. The saved picture and background set are reloaded through their
 * opcode handlers, the saved music is restarted (or the music faded out and
 * unloaded when none is saved), the saved sound replayed (bit 0x4000 selects
 * wldcore_sound_stop_music_and_play_sound), the level's two button prompts
 * and the value display re-shown when their state flags say so, and the fade
 * box set to the saved
 * brightness (0x100 is stored as 0xFF). After the novel's files are loaded
 * the script resumes at the saved section's word table entry, or branches to
 * the start when the section is 0.
 */
void wldcore_sound_novel_restore_saved_state(s32 render_record) {
    u32* word;
    s32 i;
    s32 picture_id;
    s32 transition;
    s16 sound;
    s32 tint;
    s32 section;
    u16* state_flags;
    wldcore_script_picture_state_t* picture;

    if (g_wldcore_sound_novel_index < 5) {
        g_wldcore_sound_novel_record = g_main_sound_novel_records[g_wldcore_sound_novel_index];
    } else {
        wldcore_sound_novel_reset_record(&g_wldcore_sound_novel_record);
    }
    g_wldcore_sound_novel_record.picture_render_index = render_record;
    g_wldcore_sound_novel_record.buffer = g_main_save_slot_buffer;
    g_wldcore_sound_novel_record.state_flags &= ~(WLDCORE_SOUND_NOVEL_STATE_BRANCH | WLDCORE_SOUND_NOVEL_STATE_FETCH);
    for (i = 0; i < 1; i++) {
        g_wldcore_sound_novel_local_flags[i] = 0;
    }
    world_script_set_variable(EVENT_SCRIPT_VAR_PENDING_SOUND_EFFECT, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_PENDING_WEATHER_SOUND, 0);
    world_script_set_variable(EVENT_SCRIPT_VAR_PENDING_MUSIC_TRACK, 0);
    picture = (wldcore_script_picture_state_t*)&g_wldcore_sound_novel_record.picture_render_index;
    g_wldcore_window_render_records[picture->render_index].priority = 0xC;
    g_wldcore_window_render_records[picture->render_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[picture->render_index].flags &= ~WLDCORE_DISPLAY_OBJECT_POP_IN;

    picture_id = picture->picture;
    if (picture_id != -1) {
        g_wldcore_sound_novel_record.picture_id = -1;
        g_wldcore_sound_novel_state.instruction.word = (g_wldcore_sound_novel_record.picture_x << 24)
            | (g_wldcore_sound_novel_record.picture_y << 16) | (picture_id << 8);
        wldcore_sound_novel_load_picture();
    }
    transition = g_wldcore_sound_novel_record.background_set;
    if (transition != -1) {
        g_wldcore_sound_novel_state.instruction.word = transition << 8;
        g_wldcore_sound_novel_record.background_set = -1;
        g_wldcore_loaded_background_set = -1;
        wldcore_sound_novel_load_background_set();
    }
    if (g_wldcore_sound_novel_record.music_selection & 0xFF) {
        wldcore_sound_enqueue_music_start(g_wldcore_sound_novel_record.music_selection);
    } else {
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_FADE_OUT, 0x10);
        wldcore_sound_enqueue_audio_command(WLDCORE_AUDIO_COMMAND_UNLOAD, 2);
        g_wldcore_sound_novel_record.music_selection = 0;
    }
    sound = g_wldcore_sound_novel_record.sound_id;
    if (sound != 0) {
        if (sound & 0x4000) {
            wldcore_sound_stop_music_and_play_sound((sound ^ 0x4000));
        } else {
            wldcore_sound_stop_weather_sfx_and_send_quad(sound);
        }
    }
    state_flags = &g_wldcore_sound_novel_record.state_flags;
    if (*state_flags & WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE) {
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].window_pair.first_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                     .window_pair.second_window]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
    if (*state_flags & WLDCORE_SOUND_NOVEL_STATE_VALUE_DISPLAY) {
        g_wldcore_sound_novel_state.instruction.word = g_wldcore_sound_novel_record.value_display_mode << 8;
        wldcore_sound_novel_show_value_display();
    }
    tint = g_wldcore_sound_novel_record.brightness;
    if (tint == 0x100) {
        tint = 0xFF;
    }
    g_wldcore_screen_fade_state.boxes[1].r = tint;
    g_wldcore_screen_fade_state.boxes[1].g = tint;
    g_wldcore_screen_fade_state.boxes[1].b = tint;
    g_wldcore_screen_fade_state.priorities[1] = 0xB;
    wldcore_sound_wait_for_queue_drain();
    wldcore_sound_novel_load_files(g_wldcore_sound_novel_index);
    wldcore_wait_for_file_load();
    section = g_wldcore_sound_novel_record.section;
    if (section != 0) {
        g_wldcore_sound_novel_record.script_words
            = (u32*)&g_main_save_slot_buffer[(((u32*)g_wldcore_sound_novel_record.buffer)[section] >> 2) * 4];
        /* A struct-field store: as a scalar global the store would be
         * scheduled above the table load (GCC 2.7 alias rule). */
        g_wldcore_sound_novel_record.word_index = 0;
        *state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    } else {
        *state_flags |= WLDCORE_SOUND_NOVEL_STATE_BRANCH;
    }
}
