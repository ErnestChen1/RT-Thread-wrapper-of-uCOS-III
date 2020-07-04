#include <os.h>

#define THREAD_PRIORITY         5
#define THREAD_STACK_SIZE       256
#define THREAD_TIMESLICE        5

ALIGN(RT_ALIGN_SIZE)
static CPU_STK thread1_stack[THREAD_STACK_SIZE];
static OS_TCB thread1;

ALIGN(RT_ALIGN_SIZE)
static CPU_STK thread2_stack[THREAD_STACK_SIZE];
static OS_TCB thread2;

/* �߳�1��� */
static void thread1_entry(void *param)
{
    OS_ERR err;

    while(1) 
    {
        OSTaskSemPost(&thread2,OS_OPT_POST_NONE,&err);
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
    }
}
    

/* �߳�2��� */
static void thread2_entry(void *param)
{
    OS_ERR err;
              
    while(1)
    {
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
        rt_kprintf("has pended a task sem!\r\n");
    }
}
                 

void task_sem_test (void)
{
    OS_ERR err;
        
	OSTaskCreate(&thread2,		            //������ƿ�
			   (CPU_CHAR*)"thread2", 		//��������
               thread2_entry, 			    //������
               0,					        //���ݸ��������Ĳ���
               THREAD_PRIORITY-1,           //�������ȼ�
               thread2_stack,	            //�����ջ����ַ
               THREAD_STACK_SIZE/10,	    //�����ջ�����λ
               THREAD_STACK_SIZE,		    //�����ջ��С
               0,					        //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               THREAD_TIMESLICE,			//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               0,					        //�û�����Ĵ洢��
               OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               &err);
    if(err!=OS_ERR_NONE)
    {
        rt_kprintf("task create err:%d\r\n",err);
    }   
        
        
	OSTaskCreate(&thread1,		            //������ƿ�
			   (CPU_CHAR*)"thread1", 		//��������
               thread1_entry, 			    //������
               0,					        //���ݸ��������Ĳ���
               THREAD_PRIORITY,           //�������ȼ�
               thread1_stack,	            //�����ջ����ַ
               THREAD_STACK_SIZE/10,	    //�����ջ�����λ
               THREAD_STACK_SIZE,		    //�����ջ��С
               0,					        //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               THREAD_TIMESLICE,			//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               0,					        //�û�����Ĵ洢��
               OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               &err);
    if(err!=OS_ERR_NONE)
    {
        rt_kprintf("task create err:%d\r\n",err);
    }
}
