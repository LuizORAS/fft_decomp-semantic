/*
 * Extended BATTLE counterpart of
 * world_menu_draw_text_columns_narrow.
 */
#include "fft/battle.h"
#include "psx/types.h"

extern void battle_menu_draw_stacked_glyph_pair(void* buffer, world_glyph_blit_t* origin);

/* Draw the visible rows of a three-column menu text layout (text 8 pixels
 * narrower than the entry). Text columns append text 0x5088 for rows whose
 * skill flag bit 0 is set while the current thread's fourth word is non-zero;
 * alternate-number columns draw a special glyph for values 0/1 without a
 * target, and text 0x509d for values of 100 and above. */
void battle_menu_draw_text_columns_narrow(world_menu_entry_t* entry, s32* row_offset, void* buffer) {
    world_menu_text_layout_t* layout = (world_menu_text_layout_t*)entry->text_binding;
    s32 row;
    s32 column;
    s32 index;
    s16 value;

    for (row = 0; row < layout->row_count; row++) {
        g_battle_menu_text_state.pen.stride = entry->inner_width - 8;
        for (column = 0; column < 3; column++) {
            index = row + *row_offset;
            if (layout->mode[column] == MENU_TEXT_COLUMN_HIDDEN) {
                continue;
            }
            battle_menu_set_text_origin(layout->x[column], row * 0x10);
            if (layout->mode[column] != MENU_TEXT_COLUMN_HIDDEN) {
                g_battle_menu_text_color = layout->text_colors[column][index];
            }
            g_battle_menu_text_state.pen.color = 0;
            if (g_battle_menu_text_color == 4) {
                g_battle_menu_text_state.pen.color = 0x44444444;
            }
            if (layout->mode[column] == MENU_TEXT_COLUMN_TEXT) {
                battle_menu_display_text_entry(layout->text_ids[column][index], buffer, &g_battle_menu_text_state.pen);
                if (g_battle_threads[g_battle_thread_current_id].function_parameter_4 != 0
                    && (g_battle_ai_workspace_ptr->ability_list.flags[index] & 1)) {
                    g_battle_menu_text_color = layout->text_colors[column][index];
                    /* Full-word x and y, as above. */
                    battle_menu_set_text_origin(layout->x[column] + 0x50, row * 0x10);
                    battle_menu_display_text_entry(0x5088, buffer, &g_battle_menu_text_state.pen);
                }
            } else if (layout->mode[column] == MENU_TEXT_COLUMN_NUMBER) {
                battle_display_menu_number_entry(
                    layout->text_ids[column][index], 2, (s32)buffer, &g_battle_menu_text_state.pen);
            } else if (layout->mode[column] == MENU_TEXT_COLUMN_NUMBER_GLYPHS) {
                if ((column == 2 || (column == 1 && layout->mode[2] == MENU_TEXT_COLUMN_HIDDEN))
                    && layout->text_ids[column][index] >= 0 && layout->text_ids[column][index] < 2
                    && (g_battle_ai_workspace_ptr->ability_list.skill_ct[index] == 0xFF
                        || g_battle_ai_workspace_ptr->ability_list.skill_ct[index] == 0)) {
                    g_battle_menu_text_state.pen.rect.y += 2;
                    battle_menu_draw_stacked_glyph_pair(buffer, &g_battle_menu_text_state.pen);
                } else {
                    value = layout->text_ids[column][index];
                    if (value & 0x7FF) {
                        if ((value & 0x7FF) >= 100) {
                            g_battle_menu_text_color = 0;
                            g_battle_menu_text_state.pen.rect.x += 3;
                            battle_menu_display_text_entry(0x509D, buffer, &g_battle_menu_text_state.pen);
                        } else {
                            battle_draw_menu_number_glyphs(value, 2, buffer, &g_battle_menu_text_state.pen);
                        }
                    }
                }
            }
            g_battle_menu_text_color = 0;
            g_battle_menu_text_state.pen.color = 0;
        }
    }
}
