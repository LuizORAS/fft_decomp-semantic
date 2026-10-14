#include "fft/wldcore.h"
#include "psx/libc.h"

/* Run the sound novel being read until its script yields; wldcore_menu_step_sound_novel_level calls it
 * every frame. Each turn plays the pending script sounds (EVENT_SCRIPT_VAR_PENDING_SOUND_EFFECT and the
 * two after it) and steps the countdown timer, then:
 * - after a branch (WLDCORE_SOUND_NOVEL_STATE_BRANCH) it marks the section read in
 *   g_main_sound_novel_section_bits, enters it through the novel's section table and, for the first
 *   five novels, copies the record into its bookmark in g_main_sound_novel_records;
 * - when an instruction is due (WLDCORE_SOUND_NOVEL_STATE_FETCH) it loads the next word;
 * - it runs the opcode's handler (g_wldcore_sound_novel_handlers).
 * The turns end when a handler yields or the script ends. Returns 0 when the script ended, else 1.
 *
 * The record's first word holds the save-slot section table while a script runs
 * (g_wldcore_script_slot_table names the same word). */
s32 wldcore_sound_novel_run_until_yield(void) {
    s32 low;
    s32 high;
    u32 offset;
    u16 flags;
    s32 aborted;

    do {
        low = world_script_get_variable(EVENT_SCRIPT_VAR_PENDING_SOUND_EFFECT);
        high = world_script_get_variable(EVENT_SCRIPT_VAR_PENDING_WEATHER_SOUND);
        wldcore_sound_play_pending_script_sounds(
            low, high, world_script_get_variable(EVENT_SCRIPT_VAR_PENDING_MUSIC_TRACK));
        wldcore_sound_novel_update_countdown_timer();

        if (g_wldcore_sound_novel_record.state_flags & WLDCORE_SOUND_NOVEL_STATE_BRANCH) {
            wldcore_bits_assign(g_main_sound_novel_section_bits,
                (g_wldcore_sound_novel_index << 8) + g_wldcore_sound_novel_record.section, 1);
            g_wldcore_sound_novel_record.state_flags &= ~WLDCORE_SOUND_NOVEL_STATE_BRANCH;
            offset = ((wldcore_save_slot_table_t*)g_wldcore_sound_novel_record.buffer)
                         ->offsets[g_wldcore_sound_novel_record.section];
            g_wldcore_sound_novel_record.section++;
            g_wldcore_sound_novel_record.word_index = 0;
            g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
            g_wldcore_sound_novel_record.script_words = &((u32*)g_main_save_slot_buffer)[offset >> 2];
            if (g_wldcore_sound_novel_index < 5) {
                memcpy(&g_main_sound_novel_records[g_wldcore_sound_novel_index], &g_wldcore_sound_novel_record,
                    sizeof(wldcore_sound_novel_record_t));
            }
        }

        if (g_wldcore_sound_novel_record.state_flags & WLDCORE_SOUND_NOVEL_STATE_FETCH) {
            g_wldcore_sound_novel_record.state_flags
                = (g_wldcore_sound_novel_record.state_flags & ~WLDCORE_SOUND_NOVEL_STATE_FETCH)
                | WLDCORE_SOUND_NOVEL_STATE_FETCHED;
            g_wldcore_sound_novel_record.instruction.word
                = g_wldcore_sound_novel_record.script_words[g_wldcore_sound_novel_record.word_index];
            g_wldcore_sound_novel_record.word_index++;
        }

        g_wldcore_sound_novel_handlers[g_wldcore_sound_novel_record.instruction.bytes.opcode]();

        flags = g_wldcore_sound_novel_record.state_flags;
        g_wldcore_sound_novel_record.state_flags = flags & ~WLDCORE_SOUND_NOVEL_STATE_FETCHED;
    } while ((flags & (WLDCORE_SOUND_NOVEL_STATE_YIELD | WLDCORE_SOUND_NOVEL_STATE_END)) == 0);

    aborted = g_wldcore_sound_novel_record.state_flags & WLDCORE_SOUND_NOVEL_STATE_END;
    g_wldcore_sound_novel_record.state_flags &= ~WLDCORE_SOUND_NOVEL_STATE_YIELD;
    return aborted == 0;
}
