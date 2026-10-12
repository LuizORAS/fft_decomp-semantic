#include "fft/battle_event.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Reload a unit's palette tinted for tile effects, weather and time of day.
 *
 * The unit's spritesheet palette is installed first (monster sheets
 * 0x60-0x9a use the upper eight palettes). The RGB offset starts at -4, -8 or
 * -0xc for tile effect levels 1-3; unless weather bit 1 is set, script
 * variables 0x23 (weather) and 0x24 (night) shift it further, and a unit more
 * than 0x30 below its projected screen Z stays untinted. Crystal, chicken,
 * frog and treasure sheets take fixed palettes.
 *
 * Each branch makes its own palette call, as the original evidently did:
 * cross-jumping merges them only after register allocation, so the extra
 * references still rank clut_index above unit in global allocation. The two snow
 * (weather bit 0) branches are identical. */
void battle_gfx_tint_unit_palette_for_weather_and_tile(battle_unit_misc_data_t* unit, s32 level, s32 final_value) {
    s32 clut_index;
    s32 stored_palette;
    s32 slot;
    s32 weather;
    s32 blue;
    s32 green;
    s32 red;
    s16 screen_z;

    slot = unit->spritesheet_vram_slot;
    stored_palette = unit->stored_palette;
    clut_index = unit->unit_id;

    if (g_battle_gfx_spritesheet_slots[slot].spritesheet_id >= BATTLE_SPRITESHEET_ID_JOB_FIRST
        && g_battle_gfx_spritesheet_slots[slot].spritesheet_id <= BATTLE_SPRITESHEET_ID_STEEL_GIANT) {
        battle_map_load_palette_data(
            &g_battle_gfx_spritesheet_slots[slot].clut[stored_palette + 8], MAP_CLUT_ROW_UNITS_SECOND, clut_index, 0);
    } else {
        battle_map_load_palette_data(
            &g_battle_gfx_spritesheet_slots[slot].clut[8], MAP_CLUT_ROW_UNITS_SECOND, clut_index, 0);
    }

    blue = 0;
    green = 0;
    red = 0;
    switch (level) {
    case MAP_TILE_SHADING_DARK:
        blue = -4;
        green = -4;
        red = -4;
        break;
    case MAP_TILE_SHADING_DARKER:
        blue = -8;
        green = -8;
        red = -8;
        break;
    case MAP_TILE_SHADING_DARKEST:
        blue = -0xc;
        green = -0xc;
        red = -0xc;
        break;
    }

    {
        const s32* weather_flags = &g_battle_map_weather_flags;

        if (!(*weather_flags & BATTLE_MAP_WEATHER_FLAG_IGNORE_WEATHER)) {
            weather = battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER);
            if (battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY) == 0) {
                if (*weather_flags & BATTLE_MAP_WEATHER_FLAG_SNOW) {
                    switch (weather) {
                    case BATTLE_WEATHER_NONE:
                    case BATTLE_WEATHER_NONE_ALT:
                        break;
                    case BATTLE_WEATHER_RAIN:
                        red -= 1;
                        blue += 1;
                        break;
                    case BATTLE_WEATHER_STORM:
                    case BATTLE_WEATHER_STRONG_STORM:
                    default:
                        red -= 2;
                        green -= 1;
                        blue += 2;
                        break;
                    }
                } else {
                    switch (weather) {
                    case BATTLE_WEATHER_NONE:
                    case BATTLE_WEATHER_NONE_ALT:
                        break;
                    case BATTLE_WEATHER_RAIN:
                        red -= 1;
                        blue += 1;
                        break;
                    case BATTLE_WEATHER_STORM:
                    case BATTLE_WEATHER_STRONG_STORM:
                    default:
                        red -= 2;
                        green -= 1;
                        blue += 2;
                        break;
                    }
                }
            } else {
                if (*weather_flags & BATTLE_MAP_WEATHER_FLAG_SNOW) {
                    switch (weather) {
                    case BATTLE_WEATHER_NONE:
                    case BATTLE_WEATHER_NONE_ALT:
                        red -= 3;
                        green -= 1;
                        blue += 2;
                        break;
                    case BATTLE_WEATHER_RAIN:
                        red -= 4;
                        green -= 1;
                        blue += 3;
                        break;
                    case BATTLE_WEATHER_STORM:
                    case BATTLE_WEATHER_STRONG_STORM:
                    default:
                        red -= 4;
                        green -= 2;
                        blue += 4;
                        break;
                    }
                } else {
                    switch (weather) {
                    case BATTLE_WEATHER_NONE:
                    case BATTLE_WEATHER_NONE_ALT:
                        red -= 3;
                        green -= 1;
                        blue += 2;
                        break;
                    case BATTLE_WEATHER_RAIN:
                        red -= 4;
                        green -= 1;
                        blue += 3;
                        break;
                    case BATTLE_WEATHER_STORM:
                    case BATTLE_WEATHER_STRONG_STORM:
                    default:
                        red -= 4;
                        green -= 2;
                        blue += 4;
                        break;
                    }
                }
            }
        }
    }

    screen_z = battle_gfx_calculate_screen_z_from_misc_screen_data(unit);
    if (!(g_battle_map_weather_flags & BATTLE_MAP_WEATHER_FLAG_IGNORE_WEATHER) && screen_z - 0x30 > unit->screen.vy) {
        blue = 0;
        green = 0;
        red = 0;
    }

    switch (unit->spritesheet_id) {
    case BATTLE_SPRITESHEET_ID_CRYSTAL:
        battle_map_update_palette_from_other(g_battle_gfx_crystal_palettes[stored_palette], MAP_CLUT_ROW_UNITS,
            clut_index, 0, red, green, blue, final_value);
        break;
    case BATTLE_SPRITESHEET_ID_CHICKEN:
        battle_map_update_palette_from_other(g_battle_gfx_other_palettes.chicken[stored_palette], MAP_CLUT_ROW_UNITS,
            clut_index, 0, red, green, blue, final_value);
        break;
    case BATTLE_SPRITESHEET_ID_FROG:
        battle_map_update_palette_from_other(g_battle_gfx_frog_palettes[stored_palette], MAP_CLUT_ROW_UNITS, clut_index,
            0, red, green, blue, final_value);
        break;
    case BATTLE_SPRITESHEET_ID_TREASURE:
        battle_map_update_palette_from_other(g_battle_gfx_treasure_palettes[stored_palette], MAP_CLUT_ROW_UNITS,
            clut_index, 0, red, green, blue, final_value);
        break;
    default:
        battle_map_update_palette_from_other(g_battle_gfx_spritesheet_slots[slot].clut[stored_palette],
            MAP_CLUT_ROW_UNITS, clut_index, 0, red, green, blue, final_value);
        break;
    }
}
