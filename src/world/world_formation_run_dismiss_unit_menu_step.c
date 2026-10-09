#include "fft/world.h"
#include "psx/libc.h"
#include "psx/types.h"

/*
 * The Soldier Office "dismiss unit" menu step. Three phases:
 *   - the dismissal animation timer (g_world_formation_dismiss_anim_timer) counting to 0x11, after
 *     which the roster slot is released and the script variables tied to the
 *     unit's sprite set are set;
 *   - first entry, which picks the prompt text and menu record; and
 *   - the per-frame poll of the prompt/menu thread.
 */

/*
 * Provisional. The sprite-set -> script-variable table walked by the
 * dismissal tail, terminated by sprite_set == -1. The four-byte stride is
 * proved by the target advancing both its record pointer and its byte-offset
 * index by 4 per iteration. Proposed for include/fft/world.h.
 */
typedef struct world_formation_dismiss_entry {
    s16 sprite_set;  /* 0x00 */
    s16 variable_id; /* 0x02 */
} world_formation_dismiss_entry_t;

/*
 * Provisional. The menu record handed to world_menu_run_thread for the
 * dismissal prompt. Offsets 0x74 is world_menu_thread_data_t.selection; the
 * other two fields are established here only. Proposed as an extension of
 * world_menu_thread_data_t in include/fft/world.h.
 */
typedef struct world_formation_dismiss_menu {
    u8 _unused_00[0x74];
    s16 selection; /* 0x74 */
    u8 _unused_76[0x94 - 0x76];
    s16 prompt_text_id; /* 0x94 */
    u8 _unused_96[0xec - 0x96];
    s16 entry3_selected_index; /* 0xec: selected_index of the fourth 0x3c menu entry */
} world_formation_dismiss_menu_t;

extern world_formation_dismiss_entry_t g_world_formation_dismiss_entries[];
extern world_formation_dismiss_menu_t g_world_formation_dismiss_confirm_menu;
extern world_formation_dismiss_menu_t g_world_formation_dismiss_confirm_menu_scrolled;

void world_formation_run_dismiss_unit_menu_step(void) {
    world_formation_unit_t* unit;
    world_formation_dismiss_menu_t* menu;
    s32 i;
    s32 value;
    s32 sprite;
    s32 flags;
    s32 step;
    /* The sum `value + 0xE800` is its own variable: the target's a0 carries the
     * rand()/sprite accumulation, dies at `addu a1,a0,v0`, and a0 is then reused
     * for the 0x19/0x1a prompt kind. Non-overlapping roles in one register are
     * one variable, so the kind is carried in `value` and only the sum is split
     * out here. Pinning a separate `kind` to $4 reaches the same stores but
     * displaces `value` out of a0 across the whole rand() block; the split
     * alone, without the reuse, has no effect, because `combine_regs` ties the
     * sum to its dying summand. */
    s32 text_id;
    s32 selection;
    s8 running;

    if (g_world_formation_dismiss_anim_timer != 0) {
        g_world_formation_dismiss_anim_timer++;
        if ((g_world_formation_dismiss_anim_timer & 0xFF) < 0x11) {
            return;
        }
        for (i = 0; g_world_formation_dismiss_entries[i].sprite_set != -1; i++) {
            if (g_world_formation_unit_pointers[g_world_formation_selected_unit_index]->sprite_set
                == g_world_formation_dismiss_entries[i].sprite_set) {
                world_script_set_variable(g_world_formation_dismiss_entries[i].variable_id, 0xC);
            }
        }
        world_formation_dismiss_unit(g_world_formation_selected_unit_index);
        g_world_menu_sound_muted = 0;
        g_world_formation_current_menu = 0;
        return;
    }

    if (*(s8*)&g_world_dismiss_unit_prompt_running == 0) {
        sprite = g_world_formation_selected_unit_index;
        if (g_world_formation_unit_pointers[sprite]->proposition_status != 0) {
            world_text_show_message_and_play_sound(0xC012, 0x30);
            g_world_formation_dismiss_step = 2;
            g_world_dismiss_unit_prompt_running = 1;
        } else {
            g_world_formation_dismiss_step = world_formation_can_dismiss_unit(sprite);
            world_gfx_bind_data_pointer(4);
            if (g_world_formation_unit_browse_enabled != 0) {
                g_world_formation_unit_cycle_mode = 1;
            }
            g_world_text_substitution_values[0]
                = g_world_formation_unit_pointers[g_world_formation_selected_unit_index]->roster_slot;
            unit = g_world_formation_unit_pointers[g_world_formation_selected_unit_index];
            sprite = unit->sprite_set;
            if (sprite >= 0x4B) {
                flags = unit->gender_flags;
                if (flags & 0x80) {
                    value = rand() % 8;
                } else if (flags & 0x40) {
                    value = rand() % 8 + 8;
                } else {
                    value = rand() % 8 + 0x10;
                }
            } else {
                if (sprite == 0x48) {
                    sprite = 0x49;
                }
                if (g_world_formation_dismiss_step != 0) {
                    value = sprite + 0x17;
                } else {
                    value = sprite + 0x61;
                }
            }
            text_id = value + 0xE800;
            if (g_world_formation_dismiss_step == 0) {
                g_world_menu_sound_muted = 1;
                g_world_formation_dismiss_menu_data = g_world_formation_dismiss_prompt;
                if (g_world_formation_scroll_enabled != 0) {
                    value = 0x19;
                } else {
                    value = 0x1A;
                }
                g_world_formation_dismiss_prompt_kind = value;
                g_world_formation_dismiss_prompt_text_id = text_id;
            } else {
                menu = &g_world_formation_dismiss_confirm_menu;
                if (g_world_formation_scroll_enabled != 0) {
                    menu = &g_world_formation_dismiss_confirm_menu_scrolled;
                }
                g_world_formation_dismiss_menu_data = (u8*)menu;
                menu->selection = 0;
                menu->entry3_selected_index = 0;
                menu->prompt_text_id = text_id;
            }
            g_world_dismiss_unit_prompt_running = 1;
        }
        g_world_menu_description_text_id = -1;
    }

    if (g_world_formation_dismiss_step == 2) {
        running = world_thread_is_running(1);
        g_world_dismiss_unit_prompt_running = running;
        if (running != 0) {
            return;
        }
        g_world_formation_current_menu = 0;
        return;
    }

    running = world_menu_run_thread(6, g_world_formation_dismiss_menu_data);
    step = g_world_formation_dismiss_step;
    g_world_dismiss_unit_prompt_running = running;
    if (step != 0) {
        if (world_thread_is_running(3) != 0) {
            step = g_world_formation_dismiss_step;
            if (step == 1) {
                selection = g_world_menu_selection_results[10];
                g_world_menu_sound_muted = step;
                if (selection == 0) {
                    g_world_menu_sound_effect_id = MAIN_SFX_DISMISS;
                } else if (selection == step) {
                    g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
                }
                /* A default assignment plus a conditional override, not an
                 * if/else over the two stores: the target puts `li v1,1` in the
                 * branch's delay slot and overrides with 0x64 on the
                 * fall-through (`beqz` at 0x3e4). An if/else emits the opposite
                 * branch sense and orders the two `li`s the other way, which
                 * also costs the store's register. */
                step = 1;
                if (g_world_menu_sound_effect_id != 0) {
                    step = 0x64;
                }
                g_world_formation_dismiss_step = step;
            }
        }
        if (*(s8*)&g_world_dismiss_unit_prompt_running != 0) {
            return;
        }
        if (g_world_menu_selection_results[10] != 0) {
            g_world_menu_sound_muted = 0;
            g_world_formation_current_menu = 0;
            return;
        }
        g_world_formation_dismiss_anim_timer = (g_world_formation_selected_unit_index << 8) + 1;
        return;
    }
    if (running != 0) {
        return;
    }
    g_world_menu_sound_muted = 0;
    g_world_formation_current_menu = 0;
}
