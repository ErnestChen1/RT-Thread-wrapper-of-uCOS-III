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

static char buffer[128];

/*���ԣ��߳�1���߳�2ʹ���ڽ���Ϣ���з�����Ϣ*/

/* �߳�1��� */
static void thread1_entry(void *param)
{
    OS_ERR err;
    unsigned int i=0;
    while(1) 
    {
        i++;
        rt_sprintf(buffer,"test:%d",i);
        
        OSTaskQPost(&thread2,buffer,rt_strlen(buffer),OS_OPT_POST_FIFO,&err);
        if(err!=OS_ERR_NONE)
        {
            rt_kprintf("queue post err:%d!\r\n",err);
        }      
        
        OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
    }
}
    

/* �߳�2��� */
static void thread2_entry(void *param)
{
	char  *p;
	OS_MSG_SIZE size;
    OS_ERR err;
              
    while(1)
    {
        p = OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&size,0,&err);
        if(err!=OS_ERR_NONE)
        {
            rt_kprintf("queue pend err!:%d\r\n",err);
        }
        else
        {
            rt_kprintf("pended: str:%s,size:%d\r\n",p,size);
        }   

        /*����Ƕ�׹���*/
        OSTaskSuspend(&thread1,&err);
        OSTaskSuspend(&thread1,&err);
        OSTaskSuspend(&thread1,&err);
        OSTaskSuspend(&thread1,&err);
        OSTaskSuspend(&thread1,&err);
        OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
        OSTaskResume(&thread1,&err);
        OSTaskResume(&thread1,&err);
        OSTaskResume(&thread1,&err);
        OSTaskResume(&thread1,&err);
        OSTaskResume(&thread1,&err);
    }
}
                 

void task_q_test (void)
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
               20,					        //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
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
               5,					        //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               THREAD_TIMESLICE,			//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               0,					        //�û�����Ĵ洢��
               OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               &err);
    if(err!=OS_ERR_NONE)
    {
        rt_kprintf("task create err:%d\r\n",err);
    }    
    
}
