/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"       /* added in ECE254 lab keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab keil_proc */   
#include "rt_List.h"

/* ECE254 Lab Comment: You may need to include more header files */

#define WAIT_MEM 10

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/*  The following are added for ECE254 Lab Task Management Assignmet       */
/*==========================================================================*/

/*---------------- rt_alloc_box_s, task blocks when out of memory-----------*/

/**  
   @brief: Blocking memory allocation routine.
 */
void *rt_alloc_box_s (void *p_mpool) {
	void *free = rt_alloc_box(p_mpool);
	if(!free) {
		if(os_mem.p_lnk != NULL) {
			rt_put_prio(&os_mem, os_tsk.run);
		} else {
			os_mem.p_lnk = os_tsk.run;
			os_tsk.run->p_lnk = NULL;
		}
		rt_block(0xffff, WAIT_MEM);
		free = (void *)os_tsk.run->ret_val;
	}
	return free;
}


/*----------- rt_free_box_s, pair with _s memory allocators ----------------*/
/**
 * @brief: free memory pointed by ptr, it will unblock a task that is waiting
 *         for memory.
 * @return: OS_R_OK on success and OS_R_NOK if ptr does not belong to gp_mpool 
 */
OS_RESULT rt_free_box_s (void *p_mpool, void *box) {
	int res;
	P_TCB p_TCB;
	
	res = rt_free_box(p_mpool, box);
	if (res != 0) return (OS_R_NOK);
	if (os_mem.p_lnk != NULL) {
    /* A task is waiting for memory block */
    p_TCB = rt_get_first (&os_mem);
    p_TCB->ret_val = (U32)rt_alloc_box(p_mpool);
    rt_rmv_dly (p_TCB);
    rt_dispatch (p_TCB);
    os_tsk.run->ret_val = OS_R_OK;
  }
	return (OS_R_OK);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
