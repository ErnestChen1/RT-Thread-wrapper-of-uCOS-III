/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-12     Meco Man     the first verion
 */
 
#include <os.h>
#include <stdlib.h>

/**
 * ��RT-Thread������ת��ΪuCOS-III������
 *
 * @param RT-Thread������
 *
 * @return uCOS-III������
 */
OS_ERR rt_err_to_ucosiii(rt_err_t rt_err)
{
    int rt_err2 = abs((int)rt_err);/*RTT���صĴ����붼�Ǵ����ŵ�*/
    switch(rt_err2)
    {
        /*����RTT�����������ԭ��uCOS-III��������м���*/
        case RT_EOK: /* �޴���       */
            return OS_ERR_NONE;
        case RT_ETIMEOUT:/* ��ʱ���� */
            return OS_ERR_TIMEOUT;
        case RT_EINVAL:/* �Ƿ�����   */
            return OS_ERR_OPT_INVALID;        
        case RT_EFULL:/* ��Դ����,�ò�������IPC��ʹ��*/
            return OS_ERR_Q_MAX;        
        /*
        ����uCOS-III�Ĵ���������ϸ����RTT�Ĵ���������Ϊ��ͳ��
        ����RTT������ʹ��uCOS-III�Ĵ��������׼ȷ����
        ��˽����RTT�Ĵ��������¶���(����)uCOS-III�Ĵ�����
        */
        case RT_ERROR:/* ��ͨ����    */
            return OS_ERR_RT_ERROR;
        case RT_EEMPTY:/* ����Դ     */
            return OS_ERR_RT_EEMPTY;
        case RT_ENOMEM:/* ���ڴ�     */
            return OS_ERR_RT_ENOMEM;
        case RT_ENOSYS:/* ϵͳ��֧�� */
            return OS_ERR_RT_ENOSYS;
        case RT_EBUSY:/* ϵͳæ      */
            return OS_ERR_RT_EBUSY;
        case RT_EIO:/* IO ����       */
            return OS_ERR_RT_EIO;
        case RT_EINTR:/* �ж�ϵͳ����*/
            return OS_ERR_RT_EINTR;

        default:
            return OS_ERR_RT_ERROR;
    }
}

/**
 * �õ�������һ����������ȴ�IPC,�������̬(��rt_ipc_list_resume�����ı�)
 *
 * @param ������ͷָ��
 *
 * @return ������
 */
rt_err_t rt_ipc_pend_abort_1 (rt_list_t *list)
{
    struct rt_thread *thread;
    
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    thread = rt_list_entry(list->next, struct rt_thread, tlist);/* get thread entry */
    ((OS_TCB*)thread)->PendStatus = OS_STATUS_PEND_ABORT; /*��ǵ�ǰ��������ȴ�*/
    CPU_CRITICAL_EXIT();
   
    rt_thread_resume(thread); /* resume it */

    return RT_EOK;
}
/**
 * �����еȴ���IPC������ȫ�������ȴ����������̬(��rt_ipc_list_resume_all�����ı�)
 *
 * @param ������ͷָ��
 *
 * @return ������
 */
rt_err_t rt_ipc_pend_abort_all (rt_list_t *list)
{
    struct rt_thread *thread;

    CPU_SR_ALLOC();

    /* wakeup all suspend threads */
    while (!rt_list_isempty(list))
    {
        /* disable interrupt */
        CPU_CRITICAL_ENTER();

        /* get next suspend thread */
        thread = rt_list_entry(list->next, struct rt_thread, tlist);

        /*��ǵ�ǰ��������ȴ�*/
        ((OS_TCB*)thread)->PendStatus = OS_STATUS_PEND_ABORT; 
        
        /*
         * resume thread
         * In rt_thread_resume function, it will remove current thread from
         * suspend list
         */
        rt_thread_resume(thread);

        /* enable interrupt */
        CPU_CRITICAL_EXIT();
    }

    return RT_EOK;
}
