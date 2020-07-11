/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-02     Meco Man     the first verion
 */

/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   MUTEX MANAGEMENT
*
* File    : OS_MUTEX.C
* By      : JJL
* Version : V3.03.00
*
* LICENSING TERMS:
* ---------------
*           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or 
*           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-III for its use in your 
*           application/product.   We provide ALL the source code for your convenience and to help you 
*           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use 
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
************************************************************************************************************************
*/

#include <os.h>

/*
************************************************************************************************************************
* Note(s)    : 1)����RTTû����ؽӿڣ�������º���û��ʵ��
*                   OSMutexPendAbort
************************************************************************************************************************
*/

#if OS_CFG_MUTEX_EN > 0u
/*
************************************************************************************************************************
*                                                   CREATE A MUTEX
*
* Description: This function creates a mutex.
*
* Arguments  : p_mutex       is a pointer to the mutex to initialize.  Your application is responsible for allocating
*                            storage for the mutex.
*
*              p_name        is a pointer to the name you would like to give the mutex.
*
*              p_err         is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE                    if the call was successful
*                                OS_ERR_CREATE_ISR              if you called this function from an ISR
*                                OS_ERR_ILLEGAL_CREATE_RUN_TIME if you are trying to create the Mutex after you called
*                                                                 OSSafetyCriticalStart().
*                                OS_ERR_NAME                    if 'p_name'  is a NULL pointer
*                                OS_ERR_OBJ_CREATED             if the mutex has already been created
*                                OS_ERR_OBJ_PTR_NULL            if 'p_mutex' is a NULL pointer
*
* Returns    : none
************************************************************************************************************************
*/

void  OSMutexCreate (OS_MUTEX  *p_mutex,
                     CPU_CHAR  *p_name,
                     OS_ERR    *p_err)
{
    rt_err_t rt_err;

#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#ifdef OS_SAFETY_CRITICAL_IEC61508
    if (OSSafetyCriticalStartFlag == DEF_TRUE) {
       *p_err = OS_ERR_ILLEGAL_CREATE_RUN_TIME;
        return;
    }
#endif
    
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u    
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_CREATE_ISR;
        return; 
    }
#endif
    
#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_mutex == RT_NULL)/*��黥����ָ���Ƿ�ΪNULL*/
    {
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }
    if(p_name == RT_NULL)/*��黥��������ָ���Ƿ�ΪNULL*/
    {
        *p_err = OS_ERR_NAME;
        return;
    }    
#endif

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u     
    /*�ж��ں˶����Ƿ��Ѿ����ź��������Ƿ��Ѿ�������*/
    if(rt_object_get_type(&p_mutex->Mutex.parent.parent) == RT_Object_Class_Mutex)
    {
        *p_err = OS_ERR_OBJ_CREATED;
        return;       
    }    
#endif
    
    rt_err = rt_mutex_init(&p_mutex->Mutex,(const char *)p_name,RT_IPC_FLAG_PRIO);/*uCOS-III��֧�������ȼ���������*/
    *p_err = _err_rtt_to_ucosiii(rt_err);
}

/*
************************************************************************************************************************
*                                                   DELETE A MUTEX
*
* Description: This function deletes a mutex and readies all tasks pending on the mutex.
*
* Arguments  : p_mutex       is a pointer to the mutex to delete
*
*              opt           determines delete options as follows:
*
*                                OS_OPT_DEL_NO_PEND          Delete mutex ONLY if no task pending
*                                OS_OPT_DEL_ALWAYS           Deletes the mutex even if tasks are waiting.
*                                                            In this case, all the tasks pending will be readied.
*
*              p_err         is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE                 The call was successful and the mutex was deleted
*                                OS_ERR_DEL_ISR              If you attempted to delete the mutex from an ISR
*                                OS_ERR_OBJ_PTR_NULL         If 'p_mutex' is a NULL pointer.
*                                OS_ERR_OBJ_TYPE             If 'p_mutex' is not pointing to a mutex
*                                OS_ERR_OPT_INVALID          An invalid option was specified
*                              - OS_ERR_STATE_INVALID        Task is in an invalid state
*                                OS_ERR_TASK_WAITING         One or more tasks were waiting on the mutex
*                            -------------˵��-------------
*                                OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                              - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                              + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                              Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : == 0          if no tasks were waiting on the mutex, or upon error.
*              >  0          if one or more tasks waiting on the mutex are now readied and informed.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of the mutex MUST
*                 check the return code of OSMutexPend().
*
*              2) OSMutexAccept() callers will not know that the intended mutex has been deleted.
*
*              3) Because ALL tasks pending on the mutex will be readied, you MUST be careful in applications where the
*                 mutex is used for mutual exclusion because the resource(s) will no longer be guarded by the mutex.
************************************************************************************************************************
*/

#if OS_CFG_MUTEX_DEL_EN > 0u
OS_OBJ_QTY  OSMutexDel (OS_MUTEX  *p_mutex,
                        OS_OPT     opt,
                        OS_ERR    *p_err)
{
    rt_err_t rt_err;
    rt_uint32_t pend_mutex_len;
    
    CPU_SR_ALLOC();
    
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((OS_OBJ_QTY)0);
    }
#endif
    
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u    
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_DEL_ISR;
        return 0;
    }
#endif    

#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_mutex == RT_NULL)/*���ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return 0;
    }  
    switch (opt) {
        case OS_OPT_DEL_NO_PEND:
        case OS_OPT_DEL_ALWAYS:
             break;

        default:
            *p_err =  OS_ERR_OPT_INVALID;
             return ((OS_OBJ_QTY)0);
    }
#endif
    
#if OS_CFG_OBJ_TYPE_CHK_EN > 0u    
    /*�ж��ں˶����Ƿ�Ϊ������*/
    if(rt_object_get_type(&p_mutex->Mutex.parent.parent) != RT_Object_Class_Mutex)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return 0;       
    }
#endif

    switch (opt)
    {
        case OS_OPT_DEL_NO_PEND:
            CPU_CRITICAL_ENTER();
            if(rt_list_isempty(&(p_mutex->Mutex.parent.suspend_thread)))/*��û���̵߳ȴ��ź���*/
            {
                CPU_CRITICAL_EXIT();
                rt_err = rt_mutex_detach(&p_mutex->Mutex);
                *p_err = _err_rtt_to_ucosiii(rt_err);                 
            }
            else
            {
                CPU_CRITICAL_EXIT();
                *p_err = OS_ERR_TASK_WAITING;
            }
            break;
            
        case OS_OPT_DEL_ALWAYS:
            rt_err = rt_mutex_detach(&p_mutex->Mutex);
            *p_err = _err_rtt_to_ucosiii(rt_err);
            break;
    }
    
    CPU_CRITICAL_ENTER();
    pend_mutex_len = rt_list_len(&(p_mutex->Mutex.parent.suspend_thread));
    CPU_CRITICAL_EXIT();
    
    return pend_mutex_len;
}
#endif

/*
************************************************************************************************************************
*                                                    PEND ON MUTEX
*
* Description: This function waits for a mutex.
*
* Arguments  : p_mutex       is a pointer to the mutex
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will wait for the
*                            resource up to the amount of time (in 'ticks') specified by this argument.  If you specify
*                            0, however, your task will wait forever at the specified mutex or, until the resource
*                            becomes available.
*
*              opt           determines whether the user wants to block if the mutex is not available or not:
*
*                                OS_OPT_PEND_BLOCKING
*                                OS_OPT_PEND_NON_BLOCKING
*
*              p_ts          is a pointer to a variable that will receive the timestamp of when the mutex was posted or
*                            pend aborted or the mutex deleted.  If you pass a NULL pointer (i.e. (CPU_TS *)0) then you
*                            will not get the timestamp.  In other words, passing a NULL pointer is valid and indicates
*                            that you don't need the timestamp.
*                            -------------˵��-------------
*                            �ò�����RTT��û������,��NULL����
*
*              p_err         is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE               The call was successful and your task owns the resource
*                              - OS_ERR_MUTEX_OWNER        If calling task already owns the mutex
*                              - OS_ERR_OBJ_DEL            If 'p_mutex' was deleted
*                                OS_ERR_OBJ_PTR_NULL       If 'p_mutex' is a NULL pointer.
*                                OS_ERR_OBJ_TYPE           If 'p_mutex' is not pointing at a mutex
*                                OS_ERR_OPT_INVALID        If you didn't specify a valid option
*                              - OS_ERR_PEND_ABORT         If the pend was aborted by another task
*                                OS_ERR_PEND_ISR           If you called this function from an ISR and the result
*                                                          would lead to a suspension.
*                              - OS_ERR_PEND_WOULD_BLOCK   If you specified non-blocking but the mutex was not
*                                                          available.
*                                OS_ERR_SCHED_LOCKED       If you called this function when the scheduler is locked
*                              - OS_ERR_STATE_INVALID      If the task is in an invalid state
*                              - OS_ERR_STATUS_INVALID     If the pend status has an invalid value
*                                OS_ERR_TIMEOUT            The mutex was not received within the specified timeout.
*                            -------------˵��-------------
*                                OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                              - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                              + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                              Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : none
*
* Note(s)    : 1) RTT�ڷ�����ģʽ�²�����OS_ERR_PEND_WOULD_BLOCK����OS_ERR_TIMEOUT��������OS_ERR_TIMEOUT����
************************************************************************************************************************
*/

void  OSMutexPend (OS_MUTEX  *p_mutex,
                   OS_TICK    timeout,
                   OS_OPT     opt,
                   CPU_TS    *p_ts,
                   OS_ERR    *p_err)
{
    rt_int32_t time;
    rt_err_t rt_err;
    
    CPU_VAL_UNUSED(p_ts);
 
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif
    
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u    
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_PEND_ISR;
        return;
    }
#endif  
    
#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_mutex == RT_NULL)/*��黥����ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }  
    switch (opt) {
        case OS_OPT_PEND_BLOCKING:
        case OS_OPT_PEND_NON_BLOCKING:
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;
             return;
    }
#endif
    
#if OS_CFG_OBJ_TYPE_CHK_EN > 0u    
    /*�ж��ں˶����Ƿ�Ϊ������*/
    if(rt_object_get_type(&p_mutex->Mutex.parent.parent) != RT_Object_Class_Mutex)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return;       
    }    
#endif
    
    /*��RTT��timeoutΪ0��ʾ������,ΪRT_WAITING_FOREVER��ʾ��������,
    ����uCOS-III������ͬ,�����Ҫת��*/
    if(opt == OS_OPT_PEND_BLOCKING)
    {
        if(rt_critical_level() > 0)/*���������Ƿ���*/
        {
            *p_err = OS_ERR_SCHED_LOCKED;
            return;         
        }        
        if(timeout == 0)/*��uCOS-III��timeout=0��ʾ��������*/
        {
            time = RT_WAITING_FOREVER;
        }
        else
        {
            time = timeout;
        }
    }
    else if (opt == OS_OPT_PEND_NON_BLOCKING)
    {
        time = 0;/*��RTT��timeoutΪ0��ʾ������*/
    }
    else
    {
        *p_err = OS_ERR_OPT_INVALID;/*������opt������Ч*/
    }    
    
    rt_err = rt_mutex_take(&p_mutex->Mutex,time);
    *p_err = _err_rtt_to_ucosiii(rt_err);
}

/*
************************************************************************************************************************
*                                               ABORT WAITING ON A MUTEX
*
* Description: This function aborts & readies any tasks currently waiting on a mutex.  This function should be used
*              to fault-abort the wait on the mutex, rather than to normally signal the mutex via OSMutexPost().
*
* Arguments  : p_mutex   is a pointer to the mutex
*
*              opt       determines the type of ABORT performed:
*
*                            OS_OPT_PEND_ABORT_1          ABORT wait for a single task (HPT) waiting on the mutex
*                            OS_OPT_PEND_ABORT_ALL        ABORT wait for ALL tasks that are  waiting on the mutex
*                            OS_OPT_POST_NO_SCHED         Do not call the scheduler
*
*              p_err     is a pointer to a variable that will contain an error code returned by this function.
*
*                            OS_ERR_NONE                  At least one task waiting on the mutex was readied and
*                                                         informed of the aborted wait; check return value for the
*                                                         number of tasks whose wait on the mutex was aborted.
*                            OS_ERR_OBJ_PTR_NULL          If 'p_mutex' is a NULL pointer.
*                            OS_ERR_OBJ_TYPE              If 'p_mutex' is not pointing at a mutex
*                            OS_ERR_OPT_INVALID           If you specified an invalid option
*                            OS_ERR_PEND_ABORT_ISR        If you attempted to call this function from an ISR
*                            OS_ERR_PEND_ABORT_NONE       No task were pending
*
* Returns    : == 0          if no tasks were waiting on the mutex, or upon error.
*              >  0          if one or more tasks waiting on the mutex are now readied and informed.
************************************************************************************************************************
*/

#if OS_CFG_MUTEX_PEND_ABORT_EN > 0u
OS_OBJ_QTY  OSMutexPendAbort (OS_MUTEX  *p_mutex,
                              OS_OPT     opt,
                              OS_ERR    *p_err)
{
}
#endif

/*
************************************************************************************************************************
*                                                   POST TO A MUTEX
*
* Description: This function signals a mutex
*
* Arguments  : p_mutex  is a pointer to the mutex
*
*              opt      is an option you can specify to alter the behavior of the post.  The choices are:
*
*                           OS_OPT_POST_NONE        No special option selected
*                         - OS_OPT_POST_NO_SCHED    If you don't want the scheduler to be called after the post.
*                       -------------˵��-------------
*                        RTTû��ʵ���ͷŻ������󲻵��ȣ�OS_OPT_POST_NO_SCHED������
*
*              p_err    is a pointer to a variable that will contain an error code returned by this function.
*
*                           OS_ERR_NONE             The call was successful and the mutex was signaled.
*                         - OS_ERR_MUTEX_NESTING    Mutex owner nested its use of the mutex
*                           OS_ERR_MUTEX_NOT_OWNER  If the task posting is not the Mutex owner
*                           OS_ERR_OBJ_PTR_NULL     If 'p_mutex' is a NULL pointer.
*                           OS_ERR_OBJ_TYPE         If 'p_mutex' is not pointing at a mutex
*                           OS_ERR_POST_ISR         If you attempted to post from an ISR
*                         + OS_ERR_OPT_INVALID
*                         + OS_RT_ERROR
*                       -------------˵��-------------
*                           OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                         - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                         + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                         Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : none
************************************************************************************************************************
*/

void  OSMutexPost (OS_MUTEX  *p_mutex,
                   OS_OPT     opt,
                   OS_ERR    *p_err)
{
    rt_err_t rt_err;
    
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif
    
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u    
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_POST_ISR;
        return;
    }      
#endif 
    
#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_mutex == RT_NULL)/*���ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }
    switch (opt) {
        case OS_OPT_POST_NONE:
        case OS_OPT_POST_NO_SCHED:
             break;

        default:
            *p_err =  OS_ERR_OPT_INVALID;
             return;
    }
    if(opt != OS_OPT_POST_NONE)/*��optѡ��ֻ��ΪOS_OPT_POST_NONE*/
    {
        *p_err = OS_ERR_OPT_INVALID;
        RT_DEBUG_LOG(OS_CFG_DBG_EN,("OSMutexPost: wrapper can't accept this option\r\n"));
        return;
    }    
#endif  

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u   
    /*�ж��ں˶����Ƿ�Ϊ�ź���*/
    if(rt_object_get_type(&p_mutex->Mutex.parent.parent) != RT_Object_Class_Mutex)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return;       
    }
#endif
        
    rt_err = rt_mutex_release(&p_mutex->Mutex);
    *p_err = _err_rtt_to_ucosiii(rt_err);
    /*ֻ���Ѿ�ӵ�л���������Ȩ���̲߳����ͷ�*/
    if(rt_err == -RT_ERROR)/*rt_mutex_release����-RT_ERROR��ʾ���̷߳����ջ��������߳�*/
    {
        *p_err = OS_ERR_MUTEX_NOT_OWNER;
    }
}

#endif
