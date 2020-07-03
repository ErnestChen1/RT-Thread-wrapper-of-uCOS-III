/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-01     Meco Man     the first verion
 */

/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   TASK MANAGEMENT
*
* File    : OS_TASK.C
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
����RTTû����ؽӿڣ�������º���û��ʵ��
1)�����ڽ��ź�������Ϣ����
OSTaskQFlush
OSTaskQPend
OSTaskQPendAbort
OSTaskQPost
OSTaskSemPend
OSTaskSemPendAbort
OSTaskSemPost
OSTaskSemSet

2)�����ڽ��Ĵ���
OSTaskRegGet
OSTaskRegSet

3)���й������޸��������
OSTaskChangePrio
OSTaskTimeQuantaSet
*/

/*
************************************************************************************************************************
*                                                CHANGE PRIORITY OF A TASK
*
* Description: This function allows you to change the priority of a task dynamically.  Note that the new
*              priority MUST be available.
*
* Arguments  : p_tcb      is the TCB of the tack to change the priority for
*
*              prio_new   is the new priority
*
*              p_err      is a pointer to an error code returned by this function:
*
*                             OS_ERR_NONE                 is the call was successful
*                             OS_ERR_PRIO_INVALID         if the priority you specify is higher that the maximum allowed
*                                                         (i.e. >= (OS_CFG_PRIO_MAX-1))
*                             OS_ERR_STATE_INVALID        if the task is in an invalid state
*                             OS_ERR_TASK_CHANGE_PRIO_ISR if you tried to change the task's priority from an ISR
************************************************************************************************************************
*/

void  OSTaskChangePrio (OS_TCB   *p_tcb,
                        OS_PRIO   prio_new,
                        OS_ERR   *p_err)
{
}

/*
************************************************************************************************************************
*                                                    CREATE A TASK
*
* Description: This function is used to have uC/OS-III manage the execution of a task.  Tasks can either be created
*              prior to the start of multitasking or by a running task.  A task cannot be created by an ISR.
*
* Arguments  : p_tcb          is a pointer to the task's TCB
*
*              p_name         is a pointer to an ASCII string to provide a name to the task.
*
*              p_task         is a pointer to the task's code
*
*              p_arg          is a pointer to an optional data area which can be used to pass parameters to
*                             the task when the task first executes.  Where the task is concerned it thinks
*                             it was invoked and passed the argument 'p_arg' as follows:
*
*                                 void Task (void *p_arg)
*                                 {
*                                     for (;;) {
*                                         Task code;
*                                     }
*                                 }
*
*              prio           is the task's priority.  A unique priority MUST be assigned to each task and the
*                             lower the number, the higher the priority.
*
*              p_stk_base     is a pointer to the base address of the stack (i.e. low address).
*
*              stk_limit      is the number of stack elements to set as 'watermark' limit for the stack.  This value
*                             represents the number of CPU_STK entries left before the stack is full.  For example,
*                             specifying 10% of the 'stk_size' value indicates that the stack limit will be reached
*                             when the stack reaches 90% full.
*                            -------------˵��-------------
*                             �ò����ڱ����ݲ���û�����壬��ʲô����
*
*              stk_size       is the size of the stack in number of elements.  If CPU_STK is set to CPU_INT08U,
*                             'stk_size' corresponds to the number of bytes available.  If CPU_STK is set to
*                             CPU_INT16U, 'stk_size' contains the number of 16-bit entries available.  Finally, if
*                             CPU_STK is set to CPU_INT32U, 'stk_size' contains the number of 32-bit entries
*                             available on the stack.
*
*              q_size         is the maximum number of messages that can be sent to the task
*                            -------------˵��-------------
*                             �ò����ڱ����ݲ���û�����壬��ʲô����
*
*              time_quanta    amount of time (in ticks) for time slice when round-robin between tasks.  Specify 0 to use
*                             the default.
*
*              p_ext          is a pointer to a user supplied memory location which is used as a TCB extension.
*                             For example, this user memory can hold the contents of floating-point registers
*                             during a context switch, the time each task takes to execute, the number of times
*                             the task has been switched-in, etc.
*                            -------------˵��-------------
*                             �ò����ڱ����ݲ���û�����壬��ʲô����
*
*              opt            contains additional information (or options) about the behavior of the task.
*                             See OS_OPT_TASK_xxx in OS.H.  Current choices are:
*
*                               - OS_OPT_TASK_NONE            No option selected
*                               - OS_OPT_TASK_STK_CHK         Stack checking to be allowed for the task
*                               - OS_OPT_TASK_STK_CLR         Clear the stack when the task is created
*                               - OS_OPT_TASK_SAVE_FP         If the CPU has floating-point registers, save them
*                                                             during a context switch.
*                               - OS_OPT_TASK_NO_TLS          If the caller doesn't want or need TLS (Thread Local 
*                                                             Storage) support for the task.  If you do not include this
*                                                             option, TLS will be supported by default.
*                            -------------˵��-------------
*                             �ò����ڱ����ݲ���û�����壬��ʲô����
*
*
*              p_err          is a pointer to an error code that will be set during this call.  The value pointer
*                             to by 'p_err' can be:
*
*                                 OS_ERR_NONE                    if the function was successful.
*                               - OS_ERR_ILLEGAL_CREATE_RUN_TIME if you are trying to create the task after you called
*                                                                   OSSafetyCriticalStart().
*                                 OS_ERR_NAME                    if 'p_name' is a NULL pointer
*                                 OS_ERR_PRIO_INVALID            if the priority you specify is higher that the maximum
*                                                                   allowed (i.e. >= OS_CFG_PRIO_MAX-1) or,
*                                                                if OS_CFG_ISR_POST_DEFERRED_EN is set to 1 and you tried
*                                                                   to use priority 0 which is reserved.
*                                 OS_ERR_STK_INVALID             if you specified a NULL pointer for 'p_stk_base'
*                                 OS_ERR_STK_SIZE_INVALID        if you specified zero for the 'stk_size'
*                               - OS_ERR_STK_LIMIT_INVALID       if you specified a 'stk_limit' greater than or equal
*                                                                   to 'stk_size'
*                                 OS_ERR_TASK_CREATE_ISR         if you tried to create a task from an ISR.
*                                 OS_ERR_TASK_INVALID            if you specified a NULL pointer for 'p_task'
*                                 OS_ERR_TCB_INVALID             if you specified a NULL pointer for 'p_tcb'
*                               + OS_ERR_RT_ERROR
*                             -------------˵��-------------
*                                 OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                               - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                               + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                               Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : A pointer to the TCB of the task created.  This pointer must be used as an ID (i.e handle) to the task.
************************************************************************************************************************
*/

void  OSTaskCreate (OS_TCB        *p_tcb,
                    CPU_CHAR      *p_name,
                    OS_TASK_PTR    p_task,
                    void          *p_arg,
                    OS_PRIO        prio,
                    CPU_STK       *p_stk_base,
                    CPU_STK_SIZE   stk_limit,
                    CPU_STK_SIZE   stk_size,
                    OS_MSG_QTY     q_size,
                    OS_TICK        time_quanta,
                    void          *p_ext,
                    OS_OPT         opt,
                    OS_ERR        *p_err)
{
    rt_err_t rt_err;
    
    (void)q_size;
    (void)p_ext;
    (void)opt;
    (void)stk_limit;
    
    /*����Ƿ����ж�������*/
    if(rt_interrupt_get_nest()!=0)
    {
        *p_err = OS_ERR_TASK_CREATE_ISR;
        return;
    }

    /*���TCBָ���Ƿ�Ϊ��*/
    if(p_tcb == RT_NULL)
    {
        *p_err = OS_ERR_TCB_INVALID;
        return;
    }     
    
    /*���������ָ���Ƿ�Ϊ��*/
    if(p_task == RT_NULL)
    {
        *p_err = OS_ERR_TASK_INVALID;
        return;
    }
    
    /*����������Ƿ�ΪNULL*/
    if(p_name == RT_NULL)
    {
        *p_err = OS_ERR_NAME;
        return;
    }    
    
    /*����������ȼ�*/
    if(prio >= RT_THREAD_PRIORITY_MAX-1)
    {
        *p_err = OS_ERR_PRIO_INVALID;
        return;        
    }
    
    /*��������ջָ���Ƿ�ΪNULL*/
    if(p_stk_base == RT_NULL)
    {
        *p_err = OS_ERR_STK_INVALID;
        return;
    }
    
    /*����ջ��С�Ƿ�Ϊ0*/
    if(stk_size == 0)
    {
        *p_err = OS_ERR_STK_SIZE_INVALID;
        return;
    }
    
    rt_err = rt_thread_init(p_tcb,
                            (const char*)p_name,
                            p_task,
                            p_arg,
                            p_stk_base,
                            stk_size*sizeof(CPU_STK),/*uCOS-III�������ջʱ��CPU_STKΪ��λ����RTT�����ֽ�Ϊ��λ�������Ҫ����ת��*/
                            prio,
                            time_quanta);
    
    *p_err = _err_rtt_to_ucosiii(rt_err);
    if(rt_err != RT_EOK)
    {
        return;
    }

    /*��uCOS-III�е����񴴽��൱��RTT�����񴴽�+��������*/
    rt_err = rt_thread_startup(p_tcb);                 
    *p_err = _err_rtt_to_ucosiii(rt_err);
}

/*
************************************************************************************************************************
*                                                     DELETE A TASK
*
* Description: This function allows you to delete a task.  The calling task can delete itself by specifying a NULL
*              pointer for 'p_tcb'.  The deleted task is returned to the dormant state and can be re-activated by
*              creating the deleted task again.
*
* Arguments  : p_tcb      is the TCB of the tack to delete,��ΪNULL��ʾɾ����ǰ����
*
*              p_err      is a pointer to an error code returned by this function:
*
*                             OS_ERR_NONE                  if the call is successful
*                           - OS_ERR_STATE_INVALID         if the state of the task is invalid
*                           - OS_ERR_TASK_DEL_IDLE         if you attempted to delete uC/OS-III's idle task
*                           - OS_ERR_TASK_DEL_INVALID      if you attempted to delete uC/OS-III's ISR handler task
*                             OS_ERR_TASK_DEL_ISR          if you tried to delete a task from an ISR
*                         -------------˵��-------------
*                             OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                           - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                           + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                          Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Note(s)    : 1) 'p_err' gets set to OS_ERR_NONE before OSSched() to allow the returned error code to be monitored even
*                 for a task that is deleting itself. In this case, 'p_err' MUST point to a global variable that can be
*                 accessed by another task.
*              2) ������߳���ѭ��ִ�еģ�����ɾ��������������ϵ��̣߳�RT-Thread ���߳�������Ϻ��Զ�ɾ���̣߳��� 
*                 rt_thread_exit() �����ɾ���������û�ֻ��Ҫ�˽�ýӿڵ����ã����Ƽ�ʹ�øýӿڣ������������̵߳��ô˽�
*                 �ڻ��ڶ�ʱ����ʱ�����е��ô˽ӿ�ɾ��һ���̣߳���������ʹ�÷ǳ��٣���
************************************************************************************************************************
*/

void  OSTaskDel (OS_TCB  *p_tcb,
                 OS_ERR  *p_err)
{
    rt_err_t rt_err;
    
    /*����Ƿ����ж�������*/
    if(rt_interrupt_get_nest()!=0)
    {
        *p_err = OS_ERR_TASK_DEL_ISR;
        return;
    }

    /*��ΪNULL��ʾɾ����ǰ����*/
    if(p_tcb == RT_NULL)
    {
        rt_err = rt_thread_detach(rt_thread_self());
        *p_err = _err_rtt_to_ucosiii(rt_err);   
        rt_schedule();  
    } 
    else
    {
        rt_err = rt_thread_detach(p_tcb);
        *p_err = _err_rtt_to_ucosiii(rt_err);   
    }
}

/*
************************************************************************************************************************
*                                               RESUME A SUSPENDED TASK
*
* Description: This function is called to resume a previously suspended task.  This is the only call that will remove an
*              explicit task suspension.
*
* Arguments  : p_tcb      Is a pointer to the task's OS_TCB to resume
*
*              p_err      Is a pointer to a variable that will contain an error code returned by this function
*
*                             OS_ERR_NONE                  if the requested task is resumed
*                           - OS_ERR_STATE_INVALID         if the task is in an invalid state
*                             OS_ERR_TASK_RESUME_ISR       if you called this function from an ISR
*                             OS_ERR_TASK_RESUME_SELF      You cannot resume 'self'
*                             OS_ERR_TASK_NOT_SUSPENDED    if the task to resume has not been suspended
*                         -------------˵��-------------
*                             OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                           - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                           + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                          Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : none
*
* Note(s)    : 1) uCOS-III��֧��Ƕ�׹���/���,������5��,���5��,���񼴿���������
*                 ����RTT����֧��Ƕ�׹���/���,��˸ú����޷�ʵ��Ƕ�׽��
************************************************************************************************************************
*/

void  OSTaskResume (OS_TCB  *p_tcb,
                    OS_ERR  *p_err)
{
    rt_err_t rt_err;
    
    /*����Ƿ����ж�������*/
    if(rt_interrupt_get_nest()!=0)
    {
        *p_err = OS_ERR_TASK_RESUME_ISR;
        return;
    }

    /*���TCBָ���Ƿ�Ϊ��*/
    if(p_tcb == RT_NULL)
    {
        /*����uCOS-IIIԴ��,�������������ͼ�����Լ�����Ϊ*/
        *p_err = OS_ERR_TASK_RESUME_SELF;
        return;
    }     
    
    /*��������Ƿ���ͼ�Լ��ָ��Լ�*/
    if(rt_thread_self() == p_tcb)
    {
        *p_err = OS_ERR_TASK_RESUME_SELF;
        return;
    }
    
    /*��������Ƿ�û�б�����*/
    if((p_tcb->stat & RT_THREAD_STAT_MASK) != RT_THREAD_SUSPEND)
    {
        *p_err = OS_ERR_TASK_NOT_SUSPENDED;
        return;
    }
    
    rt_err = rt_thread_resume(p_tcb);
    
    *p_err = _err_rtt_to_ucosiii(rt_err);
}

/*
************************************************************************************************************************
*                                                    STACK CHECKING
*
* Description: This function is called to calculate the amount of free memory left on the specified task's stack.
*
* Arguments  : p_tcb       is a pointer to the TCB of the task to check.  If you specify a NULL pointer then
*                          you are specifying that you want to check the stack of the current task.
*
*              p_free      is a pointer to a variable that will receive the number of free 'entries' on the task's stack.
*
*              p_used      is a pointer to a variable that will receive the number of used 'entries' on the task's stack.
*
*              p_used_max  is a pointer to a variable that will receive the maximum number of used 'entries' on the task's stack.
*
*              p_err       is a pointer to a variable that will contain an error code.
*
*                              OS_ERR_NONE               upon success
*                              OS_ERR_PTR_INVALID        if either 'p_free' or 'p_used' are NULL pointers
*                              OS_ERR_TASK_NOT_EXIST     if the stack pointer of the task is a NULL pointer
*                            - OS_ERR_TASK_OPT           if you did NOT specified OS_OPT_TASK_STK_CHK when the task
*                                                        was created
*                              OS_ERR_TASK_STK_CHK_ISR   you called this function from an ISR
*                         -------------˵��-------------
*                             OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                           - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                           + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                          Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
* Returns    : none
*
* Note(s)    : 1) ԭ�溯����ȡ����ʵʱ��ջʹ�����,�ڱ����ݲ��иú�����ȡ���Ľ�ֹ����ǰʱ�̵Ķ�ջ���ʹ�ú�ʵʱʹ��/ʣ��,
*                 ��ԭ�溯������һ��p_used_max����,��Ҫ��ԭ�������Ч��������ǰ����(�߳�)�Ķ�ջʹ�����
************************************************************************************************************************
*/

void  OSTaskStkChk (OS_TCB        *p_tcb,
                    CPU_STK_SIZE  *p_free,
                    CPU_STK_SIZE  *p_used,
                    CPU_STK_SIZE  *p_used_max,
                    OS_ERR        *p_err)
{
    rt_uint32_t stack_size;
    rt_uint32_t stack_used_max;
    rt_uint32_t stack_used;
    rt_uint32_t stack_free;
    rt_uint8_t *ptr;
    
    if(p_free == RT_NULL ||
       p_used == RT_NULL
    )
    {
        *p_err = OS_ERR_PTR_INVALID;
        return;
    }
        
    /*����Ƿ����ж�������*/
    if(rt_interrupt_get_nest()!=0)
    {
        *p_err = OS_ERR_TASK_STK_CHK_ISR;
        return;
    }   
    
    /*��TCBָ��ΪNULL,��ʾ��ǰ�߳�*/
    if(p_tcb ==RT_NULL)
    {
        p_tcb = rt_thread_self();
    }
    
    /*��������ջ�Ƿ�ΪNULL*/
    if(p_tcb->stack_addr == RT_NULL)
    {
        *p_err = OS_ERR_TASK_NOT_EXIST;
        return;        
    }
    
    *p_err = OS_ERR_NONE;
    
    /*�����ջ���ʹ�����*/
    ptr = (rt_uint8_t *)p_tcb->stack_addr;
    while (*ptr == '#')ptr ++;
    stack_size = p_tcb->stack_size;
    stack_used_max = p_tcb->stack_size - ((rt_ubase_t) ptr - (rt_ubase_t) p_tcb->stack_addr);
    
    /*�����ջʵʱʹ�����*/
    stack_used = (rt_ubase_t)p_tcb->stack_addr + p_tcb->stack_size - (rt_ubase_t)p_tcb->sp;
    stack_free = stack_size - stack_used;
    
    *p_used_max = stack_used_max / sizeof(CPU_STK_SIZE);
    *p_used = stack_used / sizeof(CPU_STK_SIZE);
    *p_free = stack_free / sizeof(CPU_STK_SIZE);
}

/*
************************************************************************************************************************
*                                                   SUSPEND A TASK
*
* Description: This function is called to suspend a task.  The task can be the calling task if 'p_tcb' is a NULL pointer
*              or the pointer to the TCB of the calling task.
*
* Arguments  : p_tcb    is a pointer to the TCB to suspend.
*                       If p_tcb is a NULL pointer then, suspend the current task.
*
*              p_err    is a pointer to a variable that will receive an error code from this function.
*
*                           OS_ERR_NONE                      if the requested task is suspended
*                           OS_ERR_SCHED_LOCKED              you can't suspend the current task is the scheduler is
*                                                            locked
*                           OS_ERR_TASK_SUSPEND_ISR          if you called this function from an ISR
*                         - OS_ERR_TASK_SUSPEND_IDLE         if you attempted to suspend the idle task which is not
*                                                            allowed.
*                         - OS_ERR_TASK_SUSPEND_INT_HANDLER  if you attempted to suspend the idle task which is not
*                                                            allowed.
*                       -------------˵��-------------
*                           OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                         - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                         + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                        Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Note(s)    : 1) You should use this function with great care.  If you suspend a task that is waiting for an event
*                 (i.e. a message, a semaphore, a queue ...) you will prevent this task from running when the event
*                 arrives.
*              2) uCOS-III��֧��Ƕ�׹���/���,������5��,���5��,���񼴿���������
*                 ����RTT����֧��Ƕ�׹���/���,��˸ú����޷�ʵ��Ƕ�׹���
************************************************************************************************************************
*/

void   OSTaskSuspend (OS_TCB  *p_tcb,
                      OS_ERR  *p_err)
{
    rt_err_t rt_err;
    
    /*����Ƿ����ж�������*/
    if(rt_interrupt_get_nest()!=0)
    {
        *p_err = OS_ERR_TASK_SUSPEND_ISR;
        return;
    }

    /*���������Ƿ���*/
    if(rt_critical_level() > 0)
    {
        *p_err = OS_ERR_SCHED_LOCKED;
        return;         
    }

    /*TCBָ���Ƿ�Ϊ��,��Ϊ�ձ�ʾɾ����ǰ�߳�*/
    if(p_tcb == RT_NULL)
    {
        rt_err = rt_thread_suspend(rt_thread_self());
        rt_schedule();/* ����RTT��Ҫ��,�������Լ���Ҫ��������rt_shedule���е���*/
    } 
    else
    {
        rt_err = rt_thread_suspend(p_tcb);
        if(rt_thread_self() == p_tcb)/*�Ƿ�Ҫ���Լ�����*/
        {
            rt_schedule();/* ����RTT��Ҫ��,�������Լ���Ҫ��������rt_shedule���е���*/
        }
    }
        
    *p_err = _err_rtt_to_ucosiii(rt_err);
}

/*
************************************************************************************************************************
*                                                CHANGE A TASK'S TIME SLICE
*
* Description: This function is called to change the value of the task's specific time slice.
*
* Arguments  : p_tcb        is the pointer to the TCB of the task to change. If you specify an NULL pointer, the current
*                           task is assumed.
*
*              time_quanta  is the number of ticks before the CPU is taken away when round-robin scheduling is enabled.
*
*              p_err        is a pointer to an error code returned by this function:
*
*                               OS_ERR_NONE       upon success
*                               OS_ERR_SET_ISR    if you called this function from an ISR
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTaskTimeQuantaSet (OS_TCB   *p_tcb,
                           OS_TICK   time_quanta,
                           OS_ERR   *p_err)
{
}
