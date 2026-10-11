#include "fft/world.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Set the current sprite color from a menu-script operand triplet.
 *
 * The palette selector chooses bytes 5..7 instead of the normal bytes 2..4. */
u8* world_menu_script_set_sprite_color(u8* script) {
    world_menu_sprite_color_command_t* command = (world_menu_sprite_color_command_t*)script;
    if (g_world_menu_use_alternate_palette != 0) {
        g_world_menu_sprite_color.r = command->alternate.r, g_world_menu_sprite_color.g = command->alternate.g,
        g_world_menu_sprite_color.b = command->alternate.b;
    } else {
        g_world_menu_sprite_color.r = command->normal.r, g_world_menu_sprite_color.g = command->normal.g,
        g_world_menu_sprite_color.b = command->normal.b;
    }
    return (u8*)command + command->length;
}
