/**
 * @file: RTL_ext.h
 */
#ifndef __RTL_EXT_H__
#define __RTL_EXT_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int U32;

#if !(__TARGET_ARCH_6S_M || __TARGET_ARCH_7_M || __TARGET_ARCH_7E_M)
/*---------------------------------------------------------------------
 *       Functions ARM
 *-------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------
 *       Functions Cortex_M
 *-------------------------------------------------------------------*/
#define __SVC_0		__svc_indirect(0)
/* ECE254 Comment: added for lab2 */
extern int rt_tsk_count_get(void);
#define os_tsk_count_get() _os_tsk_get((U32)rt_tsk_count_get);
extern int _os_tsk_get (U32 p) __SVC_0;
#ifdef __cplusplus
}
#endif
#endif
#endif
/* end of file */
