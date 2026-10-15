/* LIBGPU 80022eb0-80022eec. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Fill `env` with a default display environment showing the VRAM area (`x`, `y`, `w`, `h`): standard screen
 * position, 15-bit colour, not interlaced. Returns `env`. */
DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h) {
    env->disp.x = x;
    env->disp.y = y;
    env->disp.w = w;
    env->screen.x = 0;
    env->screen.y = 0;
    env->screen.w = 0;
    env->screen.h = 0;
    env->isrgb24 = 0;
    env->isinter = 0;
    env->pad1 = 0;
    env->pad0 = 0;
    env->disp.h = h;
    return env;
}
