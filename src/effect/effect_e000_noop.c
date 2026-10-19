#include "fft/effect.h"

/* Unused terminal routine in E000.BIN: a callback that does nothing and returns
 * no value, so its s32 result is whatever $v0 holds. */
s32 effect_e000_noop(s32 unused_record_index, s32 unused_callback_slot, s32 unused_emitter_index, s32 unused_frame) {
}
