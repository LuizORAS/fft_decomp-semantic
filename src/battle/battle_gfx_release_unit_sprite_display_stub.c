/* Release a removed unit's sprite display; the body is empty. battle_unit_remove_misc passes the
 * display, a member of the unit's g_battle_unit_misc_data record that battle_gfx_init_unit_sprite_display
 * handed out, so there is nothing to free. */
void battle_gfx_release_unit_sprite_display_stub(void* unused_sprite_display_section) {
}
