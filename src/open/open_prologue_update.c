#include "fft/open.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Advance Alazlam's prologue by one frame: the picture crossfade and the text
 * of the current page (entry). Returns 1 while it runs and 0 when it has
 * finished.
 *
 * An entry is one page of text lines. NEXT_ENTRY loads the next entry, or
 * finishes at target_index: its line count, texture page and image operation,
 * where 0x10 + n prepares the crossfade to picture n and 0x20 waits for that
 * picture's read and starts the crossfade. NEXT_STEP starts the next line,
 * reading every line's texture row and width from the entry on the first; the
 * timer starts at -16 and the line is revealed one pixel per frame. After the
 * last line the page waits for Circle (WAIT_CIRCLE), dims from 0x80 by 2 per
 * frame (DIM) and the next entry follows.
 *
 * Start sets SKIP: the lines keep advancing while the brightness runs down, and
 * the sequence ends once it is 0 and the crossfade has stopped. */
s32 open_prologue_update(void) {
    u16 flags;
    s32 i;
    open_prologue_entry_t* entry;
    s32 type;

    open_prologue_step_crossfade();
    if (!(g_open_prologue.flags & OPEN_PROLOGUE_SKIP)) {
        if (g_open_input_new_button_presses & PSX_PAD_START) {
            main_sound_play_sfx(MAIN_SFX_COMPLETE);
            g_open_prologue.flags |= OPEN_PROLOGUE_SKIP;
        }
        flags = g_open_prologue.flags;
        if (!(flags & OPEN_PROLOGUE_SKIP)) {
            if (flags & OPEN_PROLOGUE_DIM) {
                g_open_prologue.intensity -= 2;
                open_prologue_draw_text_lines(g_open_prologue.intensity);
                if (g_open_prologue.intensity == 0) {
                    g_open_prologue.intensity = 0x80;
                    g_open_prologue.flags = (g_open_prologue.flags ^ OPEN_PROLOGUE_DIM) | OPEN_PROLOGUE_NEXT_ENTRY;
                }
                return 1;
            }
            if (flags & OPEN_PROLOGUE_WAIT_CIRCLE) {
                if (g_open_input_new_button_presses & PSX_PAD_CIRCLE) {
                    g_open_prologue.flags = (flags ^ OPEN_PROLOGUE_WAIT_CIRCLE) | OPEN_PROLOGUE_DIM;
                    g_open_prologue.intensity = 0x80;
                }
                open_prologue_draw_text_lines(g_open_prologue.intensity);
                return 1;
            }
            if (flags & OPEN_PROLOGUE_NEXT_ENTRY) {
                flags ^= OPEN_PROLOGUE_NEXT_ENTRY;
                g_open_prologue.flags = flags;
                if (g_open_prologue.current_index == g_open_prologue.target_index) {
                    return 0;
                }
                g_open_prologue.flags = flags | OPEN_PROLOGUE_NEXT_STEP;
                entry = g_open_prologue.entry_table[g_open_prologue.current_index];
                g_open_prologue.entry = entry;
                g_open_prologue.step_count = entry->bytes[0];
                g_open_prologue.tpage_x = (entry->header & 0xf00) >> 2;
                g_open_prologue.tpage_y = (entry->header >> 4) & 0x100;
                /* Image operation: 0x10 + n prepares the crossfade to picture n, 0x20 starts it. */
                type = entry->bytes[2];
                switch (type & 0xf0) {
                case 0x10:
                    open_prologue_prepare_crossfade(type & 0xf);
                    break;
                case 0x20:
                    open_file_wait_for_pending();
                    g_open_prologue_crossfade_row_flags[0] = 1;
                    break;
                }
                {
                    s16* index = &g_open_prologue.current_index;

                    g_open_prologue.step = 0;
                    (*index)++;
                }
            }
        }
    }

    flags = g_open_prologue.flags;
    if (flags & OPEN_PROLOGUE_NEXT_STEP) {
        g_open_prologue.flags = flags ^ OPEN_PROLOGUE_NEXT_STEP;
        /* Every line is out: the page waits for Circle. */
        if (g_open_prologue.step == g_open_prologue.step_count) {
            g_open_prologue.flags
                = (flags ^ OPEN_PROLOGUE_NEXT_STEP) | (OPEN_PROLOGUE_NEXT_ENTRY | OPEN_PROLOGUE_WAIT_CIRCLE);
            open_prologue_draw_text_lines(g_open_prologue.intensity);
            return 1;
        }
        /* First line: read every line's texture row and width from the entry. */
        if (g_open_prologue.step == 0) {
            for (i = 0; i < g_open_prologue.step_count; i++) {
                g_open_prologue.step_durations[i] = g_open_prologue.entry->bytes[i * 4 + 5];
                g_open_prologue.step_values[i] = g_open_prologue.entry->bytes[i * 4 + 4];
            }
        }
        {
            s16* step = &g_open_prologue.step;

            g_open_prologue.timer = -0x10;
            (*step)++;
        }
    }
    if (++g_open_prologue.timer == g_open_prologue.step_durations[g_open_prologue.step - 1]) {
        g_open_prologue.flags |= OPEN_PROLOGUE_NEXT_STEP;
    }
    if (g_open_prologue.flags & OPEN_PROLOGUE_SKIP) {
        if (g_open_prologue.intensity != 0) {
            g_open_prologue.intensity -= 2;
        } else if (g_open_prologue_crossfade_row_flags[0] == 0) {
            return 0;
        }
    }
    open_prologue_draw_text_lines(g_open_prologue.intensity);
    return 1;
}
