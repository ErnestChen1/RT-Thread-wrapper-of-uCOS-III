#include <os.h>

#define THREAD_PRIORITY         5
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        50

static OS_SEM	SYNC_SEM;		//����һ���ź�������������ͬ��

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;

ALIGN(RT_ALIGN_SIZE)
static char thread3_stack[1024];
static struct rt_thread thread3;

/* �߳�2��� */
static void thread2_entry(void *param)
{
    OS_ERR err;
    rt_uint8_t i =0;
    rt_uint32_t pending_sem;
    while(1)
    {
        i++;
        if(i>10)
        {
            pending_sem = OSSemDel(&SYNC_SEM,OS_OPT_DEL_NO_PEND,&err);
            if(err == OS_ERR_TASK_WAITING)
            {
                rt_kprintf("����%d���������ڵȴ����ź������޷�ɾ��\r\n",pending_sem);
                OSSemPendAbort(&SYNC_SEM,OS_OPT_PEND_ABORT_1|OS_OPT_POST_NO_SCHED,&err);
                if(err==OS_ERR_NONE)
                {
                    rt_kprintf("�����ɹ���\r\n");
                }
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
        else
        {
            OSSemPost(&SYNC_SEM,OS_OPT_POST_1,&err);//�����ź���      
        }
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC|OS_OPT_TIME_HMSM_NON_STRICT,&err);
    }
}

/* �߳�3��� */
static void thread3_entry(void *param)
{
    OS_ERR err;
    while(1)
    {
        OSSemPend(&SYNC_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
        if(err != OS_ERR_NONE)
        {
            rt_kprintf("sem has delate\r\n");
        }
        rt_kprintf("has pended a sem\r\n");
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
    
    rt_thread_init(&thread2,
                   "thread2",
                   thread2_entry,
                   RT_NULL,
                   &thread2_stack[0],
                   sizeof(thread2_stack),
                   THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    rt_thread_startup(&thread2);
                   
    rt_thread_init(&thread3,
                   "thread3",
                   thread3_entry,
                   RT_NULL,
                   &thread3_stack[0],
                   sizeof(thread3_stack),
                   THREAD_PRIORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread3);                   
                   
}
