#ifndef BATTLE_THREAD_H
#define BATTLE_THREAD_H

/* Battle cooperative-task scheduler and its native thread records. */

#include "fft/thread.h"

/* thread ids */
s32 battle_thread_find_running_by_task(s32 task_id);
s32 battle_thread_resolve_id(s32 requested_thread_id);
s32 battle_thread_resolve_id_after_current(s32 requested_thread_id);

/* task parameters */
s32 battle_thread_get_current_parameter_1(void);
s32 battle_thread_get_current_parameter_2(void);
s32 battle_thread_get_current_parameter_3(void);
s32 battle_thread_get_current_task_id(void);
void battle_thread_set_four_parameters(s32 thread_id, s32 first, s32 second, s32 third, s32 fourth);
void battle_thread_set_parameters(s32 thread_id, s32 first, s32 second, s32 third);

/* lifecycle */
s32 battle_thread_is_previous_running(void);
s32 battle_thread_is_running(s32 thread_id);
s32 battle_thread_is_running_asm(s32 thread_id);
void battle_thread_reset_scheduler(void);
void battle_thread_resume(s32 thread_id);
void battle_thread_start(s32 thread_id, void (*function)(void));
void battle_thread_suspend(s32 thread_id);

/* waiting */
void battle_thread_wait_for_unit_view_panels(void);
void battle_thread_wait_until_inactive(s32 thread_id);

#endif
