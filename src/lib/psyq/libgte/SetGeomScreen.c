#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Set the GTE's projection distance, from the eye to the screen, to `screen`. MAIN's and BATTLE's display
 * set-ups and battle_gfx_draw_unit_sprite_layers set it. */
void SetGeomScreen(int screen) {
    PSYQ_GTE_CTC2(PSYQ_GTE_CTRL_H, screen);
}
