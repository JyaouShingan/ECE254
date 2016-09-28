/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_ext.C
 *      Purpose: Interface functions for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"
#include <stdio.h>

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
/*--------------------------- rt_tsk_count_get ------------------------------*/
/* added in ECE254 lab keil_rtx */
int rt_tsk_count_get (void) {
	U16 count = 0;
	U16 i;
	for(i = 0; i < os_maxtaskrun; i++) {
		if(os_active_TCB[i] != NULL && ((P_TCB)os_active_TCB[i])->state != INACTIVE) count++;
	}
	if(os_idle_TCB.state != INACTIVE) count++;
	return count;
}

/*--------------------------- rt_tsk_get ------------------------------------*/
/* added in ECE254 lab keil_proc */
OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {
	P_TCB task;
	U32 stk;
	U16 stk_size;
	U32 stk_bottom;
	U32 stk_used;
	U8 percent;
	
	if(!p_task_info) return OS_R_NOK;
	if(task_id > os_maxtaskrun) return OS_R_NOK;
	if(!os_active_TCB[task_id-1]) return OS_R_NOK;
	task = (P_TCB)os_active_TCB[task_id-1];
	
	if(task->state == RUNNING) stk = rt_get_PSP();
	else stk = task->tsk_stack;
	
	stk_size = ((U16)os_stackinfo);
	stk_bottom = (U32)task->stack + stk_size;
	stk_used = stk_bottom - stk;
	percent = (U8)(100 * (double)stk_used/(double)stk_size);
	
	p_task_info->task_id     = task_id;
	p_task_info->state       = task->state;
	p_task_info->prio        = task->prio;
	p_task_info->stack_usage = percent;
	p_task_info->ptask       = task->ptask;
	return OS_R_OK;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
