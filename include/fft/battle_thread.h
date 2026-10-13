#ifndef BATTLE_THREAD_H
#define BATTLE_THREAD_H

/* Battle cooperative-task scheduler and its native thread records. */

#include "fft/thread.h"

/* contexts */
extern s32 g_battle_current_thread_id;
/* Full BATTLE backing array at 0x8016986c. The pointer slot 0x80165f98 is
 * separate. */
extern native_thread_t g_battle_thread_contexts[];
/* Pointer slot at 0x80165f98, set to g_battle_thread_contexts by
 * battle_menu_init_subsystem_pointers. Records have a 1024-byte stride;
 * scheduler helpers read is_running at 0x48 and task_id at 0x4c. */
extern native_thread_t* g_battle_threads;

/* thread ids */
s32 battle_thread_find_running_by_task(s32 task_id);
s32 battle_thread_resolve_id(s32 requested_thread_id);
s32 battle_thread_resolve_id_after_current(s32 requested_thread_id);

/* task parameters */
s32 battle_thread_get_current_parameter_1(void);
s32 battle_thread_get_current_parameter_2(void);
s32 battle_thread_get_current_parameter_3(void);
s32 battle_thread_get_current_task_id(void);
void battle_thread_set_current_task_id(s32 task_id);
void battle_thread_set_four_parameters(s32 thread_id, s32 first, s32 second, s32 third, s32 fourth);
void battle_thread_set_parameters(s32 thread_id, s32 first, s32 second, s32 third);

/* lifecycle */
void battle_thread_exit_current(void);
s32 battle_thread_is_previous_running(void);
s32 battle_thread_is_running(s32 thread_id);
s32 battle_thread_is_running_asm(s32 thread_id);
void battle_thread_reset_scheduler(void);
void battle_thread_resume(s32 thread_id);
void battle_thread_start(s32 thread_id, void (*function)(void));
void battle_thread_suspend(s32 thread_id);

/* waiting */
void battle_thread_wait_for_unit_view_panels(void);
void battle_thread_wait_frames(s32 count);
void battle_thread_wait_until_inactive(s32 thread_id);
/* 0x8014ca80..0x8014cbb4: save this native context and resume the next
 * active one. Shared state can change before this caller resumes. */
void battle_thread_yield(void);

#endif
