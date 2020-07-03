#include <os.h>

#define THREAD_PRIORITY         5
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

#define KEY0_FLAG		0x01
#define KEY1_FLAG		0x02

static OS_FLAG_GRP	EventFlags;		//����һ���¼���־��

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
    
    while(1)
    {
        //���¼���־��EventFlags���ͱ�־
        OSFlagPost( (OS_FLAG_GRP*)&EventFlags,
                     (OS_FLAGS	  )KEY0_FLAG|KEY1_FLAG,
                     (OS_OPT	  )OS_OPT_POST_FLAG_SET,
                     (OS_ERR*     )&err);  
        if(err!=OS_ERR_NONE)
        {
            rt_kprintf("flag post err:%d!\r\n",err);
        }            

        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);        
    }
}

/* �߳�3��� */
static void thread3_entry(void *param)
{
    OS_ERR err;
    while(1)
    {
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&EventFlags,
				   (OS_FLAGS	)KEY0_FLAG|KEY1_FLAG,
		     	   (OS_TICK     )0,
				   (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
				   (CPU_TS*     )0,
				   (OS_ERR*	    )&err); 
        if(err!=OS_ERR_NONE)
        {
            rt_kprintf("flag pend err!:%d\r\n",err);
        }
        else
        {
            rt_kprintf("pended a flag\r\n");
        }    
    }
}

void flag_test (void)
{
    OS_ERR err;
    
    //����һ���¼���־��
	OSFlagCreate((OS_FLAG_GRP*)&EventFlags,		//ָ���¼���־��
                 (CPU_CHAR*	  )"Event Flags",	//����
                 (OS_FLAGS	  )0,	            //�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);			//������
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
