#ifndef BATTLE_THREAD_H
#define BATTLE_THREAD_H

/* Battle cooperative-task scheduler and its native thread records. */

#include "fft/thread.h"

/* thread */
extern s32 g_battle_current_thread_id;

/* Full BATTLE backing array at 0x8016986c. The pointer slot 0x80165f98 is
 * separate. */
extern native_thread_t g_battle_thread_contexts[];

/* View of g_battle_thread_contexts biased to task_id (0x801698b8): [i][0] is
 * thread i's task_id and [i][1] its task_words[0]. Target code addresses the
 * biased symbol directly, so it must not be rebound to the context base. The
 * WORLD twin is g_world_thread_task_ids. */
extern s32 g_battle_thread_task_ids[][256];

/* BATTLE's pointer slot at 0x80165f98, set to g_battle_thread_contexts by
 * battle_menu_init_subsystem_pointers. Every access indexes it with a 0x400
 * stride, and the scheduler primitives reach is_running (0x48) and task_id
 * (0x4c) through it, so it is a native_thread_t*, not the u8* placeholder it
 * was spelled as in 22 files. Files doing raw byte arithmetic cast (u8*) at
 * the use site. */
extern native_thread_t* g_battle_threads;
void battle_thread_wait_for_unit_view_panels(void);
void battle_thread_exit_current(void);
s32 battle_thread_find_running_by_task(s32 task_id);
void* battle_thread_get_current_global_pointer(void);
s32 battle_thread_get_current_parameter_1(void);
s32 battle_thread_get_current_parameter_2(void);
s32 battle_thread_get_current_parameter_3(void);
s32 battle_thread_get_current_task_id(void);
s32 battle_thread_is_previous_running(void);
s32 battle_thread_is_running(s32 thread_id);
s32 battle_thread_is_running_asm(s32 thread_id);
void battle_thread_reset_scheduler(void);
s32 battle_thread_resolve_id(s32 requested_thread_id);
s32 battle_thread_resolve_id_after_current(s32 requested_thread_id);
void battle_thread_resume(s32 thread_id);
void battle_thread_set_current_task_id(s32 task_id);
void battle_thread_set_parameters(s32 thread_id, s32 first, s32 second, s32 third);
void battle_thread_set_four_parameters(s32 thread_id, s32 first, s32 second, s32 third, s32 fourth);
void battle_thread_start(s32 thread_id, void (*function)(void));
void battle_thread_suspend(s32 thread_id);
void battle_thread_wait_frames(s32 count);
void battle_thread_wait_until_inactive(s32 thread_id);

/* 0x8014ca80..0x8014cbb4: save this native context and resume the next
 * active one. Shared state can change before this caller resumes. */
void battle_thread_yield(void);

#endif
