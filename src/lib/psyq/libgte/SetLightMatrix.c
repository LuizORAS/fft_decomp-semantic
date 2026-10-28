#include "psx/gte_inline.h"
#include "psx/libgte.h"

/* Load `matrix` as the GTE's light direction matrix. battle_map_draw_mesh_and_weather loads the map's
 * lights this way. */
void SetLightMatrix(MATRIX* matrix) {
    PSYQ_GTE_LOAD_MATRIX(matrix, PSYQ_GTE_CTRL_L11_L12, PSYQ_GTE_CTRL_L13_L21, PSYQ_GTE_CTRL_L22_L23,
        PSYQ_GTE_CTRL_L31_L32, PSYQ_GTE_CTRL_L33);
}
