#include "fft/main.h"

/* Empty CdReadyCallback handler that main_boot_run_startup installs; main_file_init_cd's CdInit replaces it
 * with LIBCD's default right after, so it never runs (QUIRKS.md). */
void main_file_handle_cd_ready_callback(u8 event, u8* result) {
}
