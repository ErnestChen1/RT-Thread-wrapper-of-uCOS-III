/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-14     Meco Man     the first verion
 */

/*������չʾ�����ʹ��uCOS-III�Ķ�ʱ������ȡ�����ջʹ������Լ���ȡCPUʹ����*/

#include <os.h>

#define TASK_PRIORITY         5
#define TASK_STACK_SIZE       256
#define TASK_TIMESLICE        5

static CPU_STK AppTask1_Stack[TASK_STACK_SIZE];
static OS_TCB AppTask1_TCB;

static OS_TMR 	tmr1;		//��ʱ��1

//��ʱ��1�Ļص�����
void tmr1_callback(void *p_tmr, void *p_arg)
{
    rt_kprintf("CPU usage:%d.%d%%\r\n",OSStatTaskCPUUsage/100,OSStatTaskCPUUsage%100);
}

/* �߳�2��� */
static void AppTask1 (void *param)
{
    OS_ERR err;
    CPU_STK_SIZE free,used;
    
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )20,			//20*10=200ms(��OS_CFG_TMR_TASK_RATE_HZ����)
                (OS_TICK	 )100,          //100*10=1000ms(��OS_CFG_TMR_TASK_RATE_HZ����)
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����   
                
    OSTmrStart(&tmr1,&err);	//������ʱ��1
              
    while(1)
    {
 		OSTaskStkChk(RT_NULL,&free,&used,&err);//��ȡ��ǰ�����ջ��Ϣ
    	rt_kprintf("AppTask1 free:%d,used:%d\r\n",free,used);   
        OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC|OS_OPT_TIME_HMSM_NON_STRICT,&err);
    }
}

void timer_sample (void)
{
    OS_ERR err;

	OSTaskCreate(&AppTask1_TCB,		            //������ƿ�
			   (CPU_CHAR*)"AppTask1", 		    //��������
               AppTask1, 			            //������
               0,					            //���ݸ��������Ĳ���
               TASK_PRIORITY-1,                 //�������ȼ�
               AppTask1_Stack,	                //�����ջ����ַ
               TASK_STACK_SIZE/10,	            //�����ջ�����λ
               TASK_STACK_SIZE,		            //�����ջ��С
               20,					            //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               TASK_TIMESLICE,			        //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               0,					            //�û�����Ĵ洢��
               OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               &err);
        if(err!=OS_ERR_NONE)
        {
            rt_kprintf("task create err:%d\r\n",err);
        }               
}
