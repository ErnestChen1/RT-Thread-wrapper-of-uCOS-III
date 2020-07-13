#include <os.h>

#define THREAD_PRIORITY         5
#define THREAD_STACK_SIZE       256
#define THREAD_TIMESLICE        50

static OS_SEM	SYNC_SEM;		//����һ���ź�������������ͬ��

ALIGN(RT_ALIGN_SIZE)
static CPU_STK thread2_stack[THREAD_STACK_SIZE];
static OS_TCB thread2;

ALIGN(RT_ALIGN_SIZE)
static CPU_STK thread3_stack[THREAD_STACK_SIZE];
static OS_TCB thread3;

/* �߳�2��� */
static void thread2_entry(void *param)
{
    OS_ERR err;
    rt_uint8_t i =0;
    rt_uint32_t pending_sems;
    
    while(i<10)
    {
        i++;
        OSSemPost(&SYNC_SEM,OS_OPT_POST_1,&err);//�����ź���      
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC|OS_OPT_TIME_HMSM_NON_STRICT,&err);
    }
    pending_sems = OSSemDel(&SYNC_SEM,OS_OPT_DEL_NO_PEND,&err);
    if(err == OS_ERR_TASK_WAITING)
    {
        rt_kprintf("����%d���������ڵȴ����ź������޷�ɾ��\r\n",pending_sems);
        OSSemPendAbort(&SYNC_SEM,OS_OPT_PEND_ABORT_1|OS_OPT_POST_NO_SCHED,&err);
        OSSemDel(&SYNC_SEM,OS_OPT_DEL_NO_PEND,&err);
    }
    else if (err == OS_ERR_NONE)
    {
        rt_kprintf("�ź���ɾ���ɹ�\r\n");
    }
    else
    {
        rt_kprintf("err:%d\r\n",err);
    }

}

/* �߳�3��� */
static void thread3_entry(void *param)
{
    OS_ERR err;
    while(1)
    {
        OSSemPend(&SYNC_SEM,10000,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
        if(err==OS_ERR_PEND_ABORT)
        {
            rt_kprintf("�ź���������\r\n");
            OSTaskDel(OSTCBCurPtr,&err);
            //RTT��������ôɾ�� ����ɾ�����������,Ҳ�����������ȥ,��������
        }
        else if(err== OS_ERR_NONE)
        {
            rt_kprintf("has pended a sem\r\n");
        }
        else
        {
            rt_kprintf("err:%d\r\n",err);
        }
    }
}

void sem_test (void)
{
    OS_ERR err;
    
    //����һ���ź���
	OSSemCreate ((OS_SEM*	)&SYNC_SEM,
                 (CPU_CHAR*	)"SYNC_SEM",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);  
    
	OSTaskCreate(&thread3,		            //������ƿ�
			   (CPU_CHAR*)"thread3", 		//��������
               thread3_entry, 			    //������
               0,					        //���ݸ��������Ĳ���
               THREAD_PRIORITY-1,           //�������ȼ�
               thread3_stack,	            //�����ջ����ַ
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
        
        
	OSTaskCreate(&thread2,		            //������ƿ�
			   (CPU_CHAR*)"thread2", 		//��������
               thread2_entry, 			    //������
               0,					        //���ݸ��������Ĳ���
               THREAD_PRIORITY,           //�������ȼ�
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
                   
}
