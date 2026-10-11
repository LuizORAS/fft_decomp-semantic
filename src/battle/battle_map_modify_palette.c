#include "fft/battle.h"
#include "psx/types.h"

/* Tint a map CLUT row and stage or apply the result.
 *
 * all_colors == 1 rewrites all 256 colors of the row; otherwise only the
 * CLUT `clut_index`. Commands 0-3 start from the decoded components,
 * 4-8 from the packed mirror: 0/4/9 add the bias, 1/5 halve each
 * component first, 2/6 and 3/7 add the bias to a weighted gray
 * ((2R + 3G + B) / 6 or / 12), 8 copies the packed color, and 10 clears the
 * repeat record and returns. Black components are left alone (their deltas
 * become neutral). A nonzero amount stores biased deltas and arms the CLUT
 * animations with that step; zero applies the colors immediately and marks
 * the palette upload pending.
 *
 * The packed-color reads repeat `clut_index * MAP_CLUT_COLOR_COUNT + i` (no index local) and
 * the gray cases use case-local temporaries: both keep the target's loop
 * strength reduction and local register allocation. */
void battle_map_modify_palette(
    s32 command, s32 amount, s32 clut_row, s32 clut_index, s32 all_colors, s16 red, s16 green, s16 blue) {
    s32 i;
    s16 out_red;
    s16 out_green;
    s16 out_blue;
    s32 red_part;
    map_palette_color_components_t* components;
    map_palette_color_components_t* clut;
    map_palette_color_components_t* component;
    map_palette_animation_state_t* animation;

    if (all_colors == 1) {
        components = g_battle_map_palette_state.clut_rows[clut_row].components;
        for (i = 0; i < MAP_CLUT_ROW_COLOR_COUNT; i++) {
            if (components[i].red_5bit + components[i].green_5bit + components[i].blue_5bit != 0) {
                switch (command) {
                case MAP_COLOR_MODE_CURRENT:
                    out_red = components[i].red_5bit + red;
                    out_green = components[i].green_5bit + green;
                    out_blue = components[i].blue_5bit + blue;
                    break;
                case MAP_COLOR_MODE_CURRENT_HALF:
                    out_red = (components[i].red_5bit >> 1) + red;
                    out_green = (components[i].green_5bit >> 1) + green;
                    out_blue = (components[i].blue_5bit >> 1) + blue;
                    break;
                case MAP_COLOR_MODE_CURRENT_GREY: {
                    s32 gray;
                    gray = (components[i].red_5bit * 2 + components[i].green_5bit * 3 + components[i].blue_5bit) / 6;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_CURRENT_GREY_HALF: {
                    s32 gray;
                    gray = (components[i].red_5bit * 2 + components[i].green_5bit * 3 + components[i].blue_5bit) / 12;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_BASE:
                case MAP_COLOR_MODE_BASE_REPEAT:
                    out_red = red + (g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x1f);
                    out_green
                        = green + ((g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x3e0) >> 5);
                    out_blue
                        = blue + ((g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x7c00) >> 10);
                    break;
                case MAP_COLOR_MODE_BASE_HALF:
                    red_part = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x1f;
                    out_red = red + (red_part >> 1);
                    out_green
                        = green + ((g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x3e0) >> 6);
                    out_blue
                        = blue + ((g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x7c00) >> 11);
                    break;
                case MAP_COLOR_MODE_BASE_GREY: {
                    s32 red_bits;
                    u32 green_bits;
                    u32 blue_bits;
                    s32 gray;
                    green_bits = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x3e0;
                    blue_bits = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x7c00;
                    red_bits = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x1f;
                    gray = red_bits * 2 + (green_bits >> 5) * 3 + (blue_bits >> 10);
                    gray /= 6;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_BASE_GREY_HALF: {
                    s32 red_bits;
                    u32 green_bits;
                    u32 blue_bits;
                    s32 gray;
                    green_bits = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x3e0;
                    blue_bits = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x7c00;
                    red_bits = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x1f;
                    gray = red_bits * 2 + (green_bits >> 5) * 3 + (blue_bits >> 10);
                    gray /= 12;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_RESTORE:
                    out_red = g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x1f;
                    out_green = (g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x3e0) >> 5;
                    out_blue = (g_battle_map_palette_state.clut_rows[clut_row].packed_colors[i] & 0x7c00) >> 10;
                    break;
                case MAP_COLOR_MODE_STOP_REPEAT:
                    for (i = MAP_CLUTS_PER_ROW - 1; i >= 0; i--) {
                        g_battle_map_palette_state.clut_rows[clut_row].animations[i].repeat = MAP_COLOR_REPEAT_OFF;
                    }
                    return;
                }
                if (out_red >= 0x20) {
                    out_red = 0x1f;
                }
                if (out_green >= 0x20) {
                    out_green = 0x1f;
                }
                if (out_blue >= 0x20) {
                    out_blue = 0x1f;
                }
                if (out_red <= 0) {
                    out_red = 0;
                }
                if (out_green <= 0) {
                    out_green = 0;
                }
                if (out_blue <= 0) {
                    out_blue = 0;
                }
                if ((out_red | out_green | out_blue) == 0) {
                    out_blue = 1;
                }
                if (amount != 0) {
                    components[i].red_delta_biased = out_red - components[i].red_5bit + MAP_PALETTE_DELTA_BIAS;
                    components[i].green_delta_biased = out_green - components[i].green_5bit + MAP_PALETTE_DELTA_BIAS;
                    components[i].blue_delta_biased = out_blue - components[i].blue_5bit + MAP_PALETTE_DELTA_BIAS;
                } else {
                    components[i].red_5bit = out_red;
                    components[i].green_5bit = out_green;
                    components[i].blue_5bit = out_blue;
                    g_battle_map_palette_modified_colors[clut_row][i]
                        = out_red + (out_green << 5) + (out_blue << 10) + (components[i].semi_transparent << 15);
                }
            } else {
                components[i].red_delta_biased = components[i].green_delta_biased = components[i].blue_delta_biased
                    = MAP_PALETTE_DELTA_BIAS;
            }
        }
        if (amount != 0) {
            for (i = 0; i < MAP_CLUTS_PER_ROW; i++) {
                animation = &g_battle_map_palette_state.clut_rows[clut_row].animations[i];
                animation->active = 1;
                animation->blend_step = 0;
                animation->delay_counter = 0;
                animation->mode = amount;
                if (command == MAP_COLOR_MODE_BASE_REPEAT) {
                    animation->repeat = MAP_COLOR_REPEAT_TINTED;
                    animation->repeat_color.r = red;
                    animation->repeat_color.g = green;
                    animation->repeat_color.b = blue;
                }
            }
            return;
        }
        for (i = MAP_CLUTS_PER_ROW - 1; i >= 0; i--) {
            g_battle_map_palette_state.clut_rows[clut_row].animations[i].active = 0;
        }
        g_battle_map_palette_state.upload_pending = 1;
    } else {
        clut = &g_battle_map_palette_state.clut_rows[clut_row].components[clut_index * MAP_CLUT_COLOR_COUNT];
        for (i = 0; i < MAP_CLUT_COLOR_COUNT; i++) {
            component = &clut[i];
            if (component->red_5bit + component->green_5bit + component->blue_5bit != 0) {
                switch (command) {
                case MAP_COLOR_MODE_CURRENT:
                    out_red = component->red_5bit + red;
                    out_green = component->green_5bit + green;
                    out_blue = component->blue_5bit + blue;
                    break;
                case MAP_COLOR_MODE_CURRENT_HALF:
                    out_red = (component->red_5bit >> 1) + red;
                    out_green = (component->green_5bit >> 1) + green;
                    out_blue = (component->blue_5bit >> 1) + blue;
                    break;
                case MAP_COLOR_MODE_CURRENT_GREY: {
                    s32 gray;
                    gray = (component->red_5bit * 2 + component->green_5bit * 3 + component->blue_5bit) / 6;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_CURRENT_GREY_HALF: {
                    s32 gray;
                    gray = (component->red_5bit * 2 + component->green_5bit * 3 + component->blue_5bit) / 12;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_BASE:
                case MAP_COLOR_MODE_BASE_REPEAT:
                    out_red = red
                        + (g_battle_map_palette_state.clut_rows[clut_row]
                                .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                            & 0x1f);
                    out_green = green
                        + ((g_battle_map_palette_state.clut_rows[clut_row]
                                   .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                               & 0x3e0)
                            >> 5);
                    out_blue = blue
                        + ((g_battle_map_palette_state.clut_rows[clut_row]
                                   .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                               & 0x7c00)
                            >> 10);
                    break;
                case MAP_COLOR_MODE_BASE_HALF:
                    red_part = g_battle_map_palette_state.clut_rows[clut_row]
                                   .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x1f;
                    out_red = red + (red_part >> 1);
                    out_green = green
                        + ((g_battle_map_palette_state.clut_rows[clut_row]
                                   .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                               & 0x3e0)
                            >> 6);
                    out_blue = blue
                        + ((g_battle_map_palette_state.clut_rows[clut_row]
                                   .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                               & 0x7c00)
                            >> 11);
                    break;
                case MAP_COLOR_MODE_BASE_GREY: {
                    s32 red_bits;
                    u32 green_bits;
                    u32 blue_bits;
                    s32 gray;
                    green_bits = g_battle_map_palette_state.clut_rows[clut_row]
                                     .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x3e0;
                    blue_bits = g_battle_map_palette_state.clut_rows[clut_row]
                                    .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x7c00;
                    red_bits = g_battle_map_palette_state.clut_rows[clut_row]
                                   .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x1f;
                    gray = red_bits * 2 + (green_bits >> 5) * 3 + (blue_bits >> 10);
                    gray /= 6;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_BASE_GREY_HALF: {
                    s32 red_bits;
                    u32 green_bits;
                    u32 blue_bits;
                    s32 gray;
                    green_bits = g_battle_map_palette_state.clut_rows[clut_row]
                                     .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x3e0;
                    blue_bits = g_battle_map_palette_state.clut_rows[clut_row]
                                    .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x7c00;
                    red_bits = g_battle_map_palette_state.clut_rows[clut_row]
                                   .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x1f;
                    gray = red_bits * 2 + (green_bits >> 5) * 3 + (blue_bits >> 10);
                    gray /= 12;
                    out_red = red + gray;
                    out_green = green + gray;
                    out_blue = blue + gray;
                    break;
                }
                case MAP_COLOR_MODE_RESTORE:
                    out_red = g_battle_map_palette_state.clut_rows[clut_row]
                                  .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                        & 0x1f;
                    out_green = (g_battle_map_palette_state.clut_rows[clut_row]
                                        .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                                    & 0x3e0)
                        >> 5;
                    out_blue = (g_battle_map_palette_state.clut_rows[clut_row]
                                       .packed_colors[clut_index * MAP_CLUT_COLOR_COUNT + i]
                                   & 0x7c00)
                        >> 10;
                    break;
                case MAP_COLOR_MODE_STOP_REPEAT:
                    g_battle_map_palette_state.clut_rows[clut_row].animations[clut_index].repeat = MAP_COLOR_REPEAT_OFF;
                    return;
                }
                if (out_red >= 0x20) {
                    out_red = 0x1f;
                }
                if (out_green >= 0x20) {
                    out_green = 0x1f;
                }
                if (out_blue >= 0x20) {
                    out_blue = 0x1f;
                }
                if (out_red <= 0) {
                    out_red = 0;
                }
                if (out_green <= 0) {
                    out_green = 0;
                }
                if (out_blue <= 0) {
                    out_blue = 0;
                }
                if ((out_red | out_green | out_blue) == 0) {
                    out_blue = 1;
                }
                if (amount != 0) {
                    component->red_delta_biased = out_red - component->red_5bit + MAP_PALETTE_DELTA_BIAS;
                    component->green_delta_biased = out_green - component->green_5bit + MAP_PALETTE_DELTA_BIAS;
                    component->blue_delta_biased = out_blue - component->blue_5bit + MAP_PALETTE_DELTA_BIAS;
                } else {
                    component->red_5bit = out_red;
                    component->green_5bit = out_green;
                    component->blue_5bit = out_blue;
                    g_battle_map_palette_modified_colors[clut_row][clut_index * MAP_CLUT_COLOR_COUNT + i]
                        = out_red + (out_green << 5) + (out_blue << 10) + (component->semi_transparent << 15);
                }
            } else {
                component->red_delta_biased = component->green_delta_biased = component->blue_delta_biased
                    = MAP_PALETTE_DELTA_BIAS;
            }
        }
        if (amount != 0) {
            animation = &g_battle_map_palette_state.clut_rows[clut_row].animations[clut_index];
            animation->active = 1;
            animation->blend_step = 0;
            animation->delay_counter = 0;
            animation->mode = amount;
            if (command == MAP_COLOR_MODE_BASE_REPEAT) {
                animation->repeat = MAP_COLOR_REPEAT_TINTED;
                animation->repeat_color.r = red;
                animation->repeat_color.g = green;
                animation->repeat_color.b = blue;
            }
            return;
        }
        g_battle_map_palette_state.clut_rows[clut_row].animations[clut_index].active = 0;
        g_battle_map_palette_state.upload_pending = 1;
    }
}
