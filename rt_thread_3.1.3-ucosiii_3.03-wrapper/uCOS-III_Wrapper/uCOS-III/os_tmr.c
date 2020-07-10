/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-30     Meco Man     the first verion
 */

/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   TIMER MANAGEMENT
*
* File    : OS_TMR.C
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
*                                                   CREATE A TIMER
*
* Description: This function is called by your application code to create a timer.
*
* Arguments  : p_tmr           Is a pointer to a timer control block
*
*              p_name          Is a pointer to an ASCII string that is used to name the timer.  Names are useful for
*                              debugging.
*
*              dly             Initial delay.
*                              If the timer is configured for ONE-SHOT mode, this is the timeout used
*                              If the timer is configured for PERIODIC mode, this is the first timeout to wait for
*                              before the timer starts entering periodic mode
*
*              period          The 'period' being repeated for the timer.
*                              If you specified 'OS_OPT_TMR_PERIODIC' as an option, when the timer expires, it will
*                              automatically restart with the same period.
*                              -------------˵��-------------
*                              RTT��uCOS-III�ڶ�ʱ��ʱ��Դ����Ʋ�ͬ��
*                              ��RTT�Ķ�ʱ��ʱ��Ƶ�������ϵͳostickƵ����ͬ
*                              ��uCOS-III�Ķ�ʱ��ʱ����ostick��Ƶ�õ�����Ƶϵ��ΪOS_CFG_TMR_TASK_RATE_HZ
*                              �����ڲ��Ѿ���������������ϵͳ�����������ת��
*
*              opt             Specifies either:
*
*                                  OS_OPT_TMR_ONE_SHOT       The timer counts down only once
*                                  OS_OPT_TMR_PERIODIC       The timer counts down and then reloads itself
*
*              p_callback      Is a pointer to a callback function that will be called when the timer expires.  The
*                              callback function must be declared as follows:
*
*                                  void  MyCallback (OS_TMR *p_tmr, void *p_arg);
*                              -------------˵��-------------
*                              RTT�Ļص���������Ϊ1������˼��ݲ�Ļص�������ʽ��Ϊ��
*                                  void  MyCallback (void *p_arg); 
*
*              p_callback_arg  Is an argument (a pointer) that is passed to the callback function when it is called.
*
*              p_err           Is a pointer to an error code.  '*p_err' will contain one of the following:
*
*                                 OS_ERR_NONE
*                                 OS_ERR_ILLEGAL_CREATE_RUN_TIME if you are trying to create the timer after you called
*                                                                  OSSafetyCriticalStart().
*                                 OS_ERR_OBJ_CREATED             if the timer has already been created
*                                 OS_ERR_OBJ_PTR_NULL            is 'p_tmr' is a NULL pointer
*                               - OS_ERR_OBJ_TYPE                if the object type is invalid
*                                 OS_ERR_OPT_INVALID             you specified an invalid option
*                                 OS_ERR_TMR_INVALID_DLY         you specified an invalid delay
*                                 OS_ERR_TMR_INVALID_PERIOD      you specified an invalid period
*                                 OS_ERR_TMR_ISR                 if the call was made from an ISR
*                              -------------˵��-------------
*                                 OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                               - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                               + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                               Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : none
*
* Note(s)    : 1) This function only creates the timer.  In other words, the timer is not started when created.  To
*                 start the timer, call OSTmrStart().
*              2) ��Ҫ����RT_USING_TIMER_SOFT�궨��
*
************************************************************************************************************************
*/

void  OSTmrCreate (OS_TMR               *p_tmr,
                   CPU_CHAR             *p_name,
                   OS_TICK               dly,
                   OS_TICK               period,
                   OS_OPT                opt,
                   OS_TMR_CALLBACK_PTR   p_callback,
                   void                 *p_callback_arg,
                   OS_ERR               *p_err)
{
    rt_uint8_t rt_flag;
    rt_tick_t  time;
    
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
        *p_err = OS_ERR_TMR_ISR;
        return;
    }
#endif 
    
#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_tmr == RT_NULL)/*���ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }
    switch (opt) {
        case OS_OPT_TMR_PERIODIC:
             if (period == (OS_TICK)0) {
                *p_err = OS_ERR_TMR_INVALID_PERIOD;
                 return;
             }
             break;

        case OS_OPT_TMR_ONE_SHOT:
             if (dly == (OS_TICK)0) {
                *p_err = OS_ERR_TMR_INVALID_DLY;
                 return;
             }
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;
             return;
    }    
#endif

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u 
    /*�ж��ں˶����Ƿ��Ѿ��Ƕ�ʱ�������Ƿ��Ѿ�������*/
    if(rt_object_get_type(&p_tmr->Tmr.parent) == RT_Object_Class_Timer)
    {
        *p_err = OS_ERR_OBJ_CREATED;
        return;       
    }
#endif
    
    /*uCOS-IIIԭ�涨ʱ���ص����������ڶ�ʱ���߳��е��õ�,�������ж��е���,
    ���Ҫʹ��RTT��RT_TIMER_FLAG_SOFT_TIMERѡ��,�ڴ�֮ǰӦ���궨��RT_USING_TIMER_SOFT��1*/
    if(opt == OS_OPT_TMR_ONE_SHOT)
    {
        rt_flag = RT_TIMER_FLAG_ONE_SHOT|RT_TIMER_FLAG_SOFT_TIMER;
        if(dly == 0)/*���dly�����Ƿ�Ϸ�*/
        {
            *p_err = OS_ERR_TMR_INVALID_DLY;
            return;
        }
        /*RTT��uCOS-III�ڶ�ʱ��ʱ��Դ����Ʋ�ͬ,��Ҫ����ת��*/
        time = dly * (1000 / OS_CFG_TMR_TASK_RATE_HZ);
    }
    else if(opt == OS_OPT_TMR_PERIODIC)
    {
        rt_flag = RT_TIMER_FLAG_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER;
        if(period == 0)/*���period�����Ƿ�Ϸ�*/
        {
            *p_err = OS_ERR_TMR_INVALID_PERIOD;
            return;
        }   
        /*RTT��uCOS-III�ڶ�ʱ��ʱ��Դ����Ʋ�ͬ,��Ҫ����ת��*/        
        time = period * (1000 / OS_CFG_TMR_TASK_RATE_HZ);
    }
    else
    {
        *p_err = OS_ERR_OPT_INVALID;
        return;
    }
    
    rt_timer_init(&p_tmr->Tmr,
                  (const char*)p_name,
                  p_callback,
                  p_callback_arg,
                  time,
                  rt_flag);
    
    *p_err = OS_ERR_NONE;/*rt_timer_initû�з��ش�����*/
}

/*
************************************************************************************************************************
*                                                   DELETE A TIMER
*
* Description: This function is called by your application code to delete a timer.
*
* Arguments  : p_tmr          Is a pointer to the timer to stop and delete.
*
*              p_err          Is a pointer to an error code.  '*p_err' will contain one of the following:
*
*                                 OS_ERR_NONE
*                                 OS_ERR_OBJ_TYPE             'p_tmr' is not pointing to a timer
*                                 OS_ERR_TMR_INVALID          'p_tmr' is a NULL pointer
*                                 OS_ERR_TMR_ISR              if the function was called from an ISR
*                               - OS_ERR_TMR_INACTIVE         if the timer was not created
*                               - OS_ERR_TMR_INVALID_STATE    the timer is in an invalid state
*                             -------------˵��-------------
*                                 OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                               - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                               + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                               Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : DEF_TRUE   if the timer was deleted
*              DEF_FALSE  if not or upon an error
************************************************************************************************************************
*/

#if OS_CFG_TMR_DEL_EN > 0u
CPU_BOOLEAN  OSTmrDel (OS_TMR  *p_tmr,
                       OS_ERR  *p_err)
{
    rt_err_t rt_err;
    
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (DEF_FALSE);
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u   
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_TMR_ISR;
        return DEF_FALSE;
    }  
#endif 
    
#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_tmr == RT_NULL)/*���ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_TMR_INVALID;
        return DEF_FALSE;
    }
#endif
    
#if OS_CFG_OBJ_TYPE_CHK_EN > 0u    
    /*�ж��ں˶����Ƿ�Ϊ��ʱ��*/
    if(rt_object_get_type(&p_tmr->Tmr.parent) != RT_Object_Class_Timer)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return DEF_FALSE;       
    }
#endif
    
    rt_err = rt_timer_detach(&p_tmr->Tmr);
    *p_err = _err_rtt_to_ucosiii(rt_err);
    if(rt_err == RT_EOK)
    {
        return DEF_TRUE;
    }
    else
    {
        return DEF_FALSE;
    }
}
#endif

/*
************************************************************************************************************************
*                                    GET HOW MUCH TIME IS LEFT BEFORE A TIMER EXPIRES
*
* Description: This function is called to get the number of ticks before a timer times out.
*
* Arguments  : p_tmr    Is a pointer to the timer to obtain the remaining time from.
*
*              p_err    Is a pointer to an error code.  '*p_err' will contain one of the following:
*
*                           OS_ERR_NONE
*                           OS_ERR_OBJ_TYPE           'p_tmr' is not pointing to a timer
*                           OS_ERR_TMR_INVALID        'p_tmr' is a NULL pointer
*                           OS_ERR_TMR_ISR            if the call was made from an ISR
*                         - OS_ERR_TMR_INACTIVE       'p_tmr' points to a timer that is not active
*                         - OS_ERR_TMR_INVALID_STATE  the timer is in an invalid state
*                        -------------˵��-------------
*                            OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                          - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                          + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                         Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : The time remaining for the timer to expire.  The time represents 'timer' increments.  In other words, if
*              OS_TmrTask() is signaled every 1/10 of a second then the returned value represents the number of 1/10 of
*              a second remaining before the timer expires.
************************************************************************************************************************
*/

OS_TICK  OSTmrRemainGet (OS_TMR  *p_tmr,
                         OS_ERR  *p_err)
{
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((OS_TICK)0);
    }
#endif   
    
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u    
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_TMR_ISR;
        return 0;
    }  
#endif   

#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_tmr == RT_NULL)/*���ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_TMR_INVALID;
        return 0;
    }
#endif
    
#if OS_CFG_OBJ_TYPE_CHK_EN > 0u    
    /*�ж��ں˶����Ƿ�Ϊ��ʱ��*/
    if(rt_object_get_type(&p_tmr->Tmr.parent) != RT_Object_Class_Timer)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return 0;       
    }
#endif
    
    *p_err = OS_ERR_NONE;  
    return rt_tick_get() - p_tmr->Tmr.timeout_tick;
}

/*
************************************************************************************************************************
*                                                   START A TIMER
*
* Description: This function is called by your application code to start a timer.
*
* Arguments  : p_tmr    Is a pointer to an OS_TMR
*
*              p_err    Is a pointer to an error code.  '*p_err' will contain one of the following:
*
*                           OS_ERR_NONE
*                           OS_ERR_OBJ_TYPE            if 'p_tmr' is not pointing to a timer
*                           OS_ERR_TMR_INVALID
*                         - OS_ERR_TMR_INACTIVE        if the timer was not created
*                         - OS_ERR_TMR_INVALID_STATE   the timer is in an invalid state
*                           OS_ERR_TMR_ISR             if the call was made from an ISR
*                       -------------˵��-------------
*                           OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                         - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                         + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                        Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : DEF_TRUE      is the timer was started
*              DEF_FALSE     if not or upon an error
*
************************************************************************************************************************
*/

CPU_BOOLEAN  OSTmrStart (OS_TMR  *p_tmr,
                         OS_ERR  *p_err)
{
    rt_err_t rt_err;
    
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (DEF_FALSE);
    }
#endif
    
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u    
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_TMR_ISR;
        return DEF_FALSE;
    }  
#endif  
    
#if OS_CFG_ARG_CHK_EN > 0u   
    if(p_tmr == RT_NULL)/*���ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_TMR_INVALID;
        return DEF_FALSE;
    }
#endif

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u    
    /*�ж��ں˶����Ƿ�Ϊ��ʱ��*/
    if(rt_object_get_type(&p_tmr->Tmr.parent) != RT_Object_Class_Timer)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return DEF_FALSE;       
    }
#endif
    
    rt_err = rt_timer_start(&p_tmr->Tmr);
    *p_err = _err_rtt_to_ucosiii(rt_err);
    if(rt_err == RT_EOK)
    {
        return DEF_TRUE;
    }
    else
    {
        return DEF_FALSE;
    }
}

/*
************************************************************************************************************************
*                                           FIND OUT WHAT STATE A TIMER IS IN
*
* Description: This function is called to determine what state the timer is in:
*
*                  OS_TMR_STATE_UNUSED     the timer has not been created
*                  OS_TMR_STATE_STOPPED    the timer has been created but has not been started or has been stopped
*                  OS_TMR_STATE_COMPLETED  the timer is in ONE-SHOT mode and has completed it's timeout
*                  OS_TMR_STATE_RUNNING    the timer is currently running
*
* Arguments  : p_tmr    Is a pointer to the desired timer
*
*              p_err    Is a pointer to an error code.  '*p_err' will contain one of the following:
*
*                           OS_ERR_NONE
*                           OS_ERR_OBJ_TYPE            if 'p_tmr' is not pointing to a timer
*                           OS_ERR_TMR_INVALID        'p_tmr' is a NULL pointer
*                           OS_ERR_TMR_INVALID_STATE  if the timer is not in a valid state
*                           OS_ERR_TMR_ISR            if the call was made from an ISR
*                       -------------˵��-------------
*                           OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                         - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                         + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                        Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : The current state of the timer (see description).
************************************************************************************************************************
*/

OS_STATE  OSTmrStateGet (OS_TMR  *p_tmr,
                         OS_ERR  *p_err)
{
    rt_uint8_t state;
    
    CPU_SR_ALLOC();
    
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (OS_TMR_STATE_UNUSED);
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0){
       *p_err = OS_ERR_TMR_ISR;
        return (OS_TMR_STATE_UNUSED);
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u
    if (p_tmr == (OS_TMR *)0) {
       *p_err = OS_ERR_TMR_INVALID;
        return (OS_TMR_STATE_UNUSED);
    }
#endif 
    
#if OS_CFG_OBJ_TYPE_CHK_EN > 0u    
    /*�ж��ں˶����Ƿ�Ϊ��ʱ��*/
    if(rt_object_get_type(&p_tmr->Tmr.parent) != RT_Object_Class_Timer)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return DEF_FALSE;       
    }
    
    CPU_CRITICAL_ENTER();
    state = (p_tmr->Tmr.parent.flag);
    CPU_CRITICAL_EXIT();
    
    *p_err  = OS_ERR_NONE;
    if(!(state&RT_TIMER_FLAG_ACTIVATED)&&!(state&RT_TIMER_FLAG_PERIODIC))
    {
        return OS_TMR_STATE_COMPLETED;
    }
    else if(state&RT_TIMER_FLAG_ACTIVATED)
    {
        return OS_TMR_STATE_RUNNING;
    }
    else
    {
        return OS_TMR_STATE_STOPPED;
    }
    
#endif    
    
    
    
}

/*
************************************************************************************************************************
*                                                    STOP A TIMER
*
* Description: This function is called by your application code to stop a timer.
*
* Arguments  : p_tmr          Is a pointer to the timer to stop.
*
*              opt           Allows you to specify an option to this functions which can be:
*
*                               OS_OPT_TMR_NONE            Do nothing special but stop the timer
*                             - OS_OPT_TMR_CALLBACK        Execute the callback function, pass it the callback argument
*                                                          specified when the timer was created.
*                             - OS_OPT_TMR_CALLBACK_ARG    Execute the callback function, pass it the callback argument
*                                                          specified in THIS function call
*                            -------------˵��-------------
*                             ����RTT��û��ʵ�ֻص������Ĺ���,���OS_OPT_TMR_CALLBACK��OS_OPT_TMR_CALLBACK_ARGѡ����Ч
*
*              callback_arg  Is a pointer to a 'new' callback argument that can be passed to the callback function
*                               instead of the timer's callback argument.  In other words, use 'callback_arg' passed in
*                               THIS function INSTEAD of p_tmr->OSTmrCallbackArg
*
*              p_err         Is a pointer to an error code.  '*p_err' will contain one of the following:
*                               OS_ERR_NONE
*                               OS_ERR_OBJ_TYPE            if 'p_tmr' is not pointing to a timer
*                               OS_ERR_OPT_INVALID         if you specified an invalid option for 'opt'
*                             - OS_ERR_TMR_INACTIVE        if the timer was not created
*                               OS_ERR_TMR_INVALID         'p_tmr' is a NULL pointer
*                             - OS_ERR_TMR_INVALID_STATE   the timer is in an invalid state
*                               OS_ERR_TMR_ISR             if the function was called from an ISR
*                             - OS_ERR_TMR_NO_CALLBACK     if the timer does not have a callback function defined
*                               OS_ERR_TMR_STOPPED         if the timer was already stopped
*                            -------------˵��-------------
*                               OS_ERR_XXXX        ��ʾ���Լ�������uCOS-IIIԭ��Ĵ�����
*                             - OS_ERR_XXXX        ��ʾ�ô������ڱ����ݲ��Ѿ��޷�ʹ��
*                             + OS_ERR_RT_XXXX     ��ʾ�ô�����Ϊ������RTTר�ô����뼯
*                             Ӧ�ò���Ҫ��API���صĴ������ж�������Ӧ���޸�
*
* Returns    : DEF_TRUE       If we stopped the timer (if the timer is already stopped, we also return DEF_TRUE)
*              DEF_FALSE      If not
************************************************************************************************************************
*/

CPU_BOOLEAN  OSTmrStop (OS_TMR  *p_tmr,
                        OS_OPT   opt,
                        void    *p_callback_arg,
                        OS_ERR  *p_err)
{
    rt_err_t rt_err;
    
    CPU_VAL_IGNORED(p_callback_arg);/*����RTT��û��ʵ�ֻص������Ĺ���,���p_callback_arg����*/
    
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (DEF_FALSE);
    }
#endif
    
#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u   
    if(OSIntNestingCtr > (OS_NESTING_CTR)0)/*����Ƿ����ж�������*/
    {
        *p_err = OS_ERR_TMR_ISR;
        return DEF_FALSE;
    }  
#endif   
    
#if OS_CFG_ARG_CHK_EN > 0u    
    if(p_tmr == RT_NULL)/*���ָ���Ƿ�Ϊ��*/
    {
        *p_err = OS_ERR_TMR_INVALID;
        return DEF_FALSE;
    }
#endif
    
#if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    /*�ж��ں˶����Ƿ�Ϊ��ʱ��*/
    if(rt_object_get_type(&p_tmr->Tmr.parent) != RT_Object_Class_Timer)
    {
        *p_err = OS_ERR_OBJ_TYPE;
        return DEF_FALSE;       
    }
#endif
    
    /*����RTT��û��ʵ�ֻص������Ĺ���,���OS_OPT_TMR_CALLBACK��OS_OPT_TMR_CALLBACK_ARGѡ����Ч*/
    if(opt != OS_OPT_TMR_NONE)
    {
        *p_err = OS_ERR_OPT_INVALID;
        RT_DEBUG_LOG(OS_CFG_DBG_EN,("OSTmrStop: wrapper can't accept this option\r\n"));
        return DEF_FALSE;
    }
    
    rt_err = rt_timer_stop(&p_tmr->Tmr);
    
    if(rt_err == -RT_ERROR)
    {
        *p_err = OS_ERR_TMR_STOPPED;/*����-RT_ERROR ʱ��˵���Ѿ�����ֹͣ״̬*/
        return DEF_FALSE;
    }
    else
    {
        return DEF_TRUE;
    }
}
