#include "fft/main.h"

/* Same as main_gfx_build_zodiac_primitives: the entry ATTACK's deployment render thread calls
 * (attack_deploy_run_render_thread). */
void main_gfx_call_build_zodiac_bin(zodiac_draw_context_t* context, POLY_FT4* poly) {
    main_gfx_build_zodiac_primitives(context, poly);
}
