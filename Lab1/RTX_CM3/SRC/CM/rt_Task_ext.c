/**
 * @file: rt_Task_ext.c
 */
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"

int rt_tsk_count_get (void) {
	U16 count = 0;
	U16 i;
	for(i = 0; i < os_maxtaskrun; i++) {
		if(os_active_TCB[i] != NULL && ((P_TCB)os_active_TCB[i])->state != INACTIVE) count++;
	}
	if(os_idle_TCB.state != INACTIVE) count++;
	return count;
}
/* end of file */
