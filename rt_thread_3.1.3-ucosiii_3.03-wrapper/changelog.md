# ά����־

### 2020-6-29 

- ��������
- ����ź�������
- ���os_time.cϵͳ��ʱ�йؼ���


### 2020-6-30 

- ��������ʱ������


### 2020-7-1  

- ʵ�ֻ���������


### 2020-7-2  

- ʵ����Ϣ���м���


### 2020-7-3  

- ʵ���źű�־�����

- ʵ�������ڽ��Ĵ�������

- ʵ�������ڽ���Ϣ���м���

### 2020-7-4  

- ʵ�������ڽ��ź�������
- ʵ��uCOS-III��ȫ��֤�����ؼ���
- �Խ�os_cfg.h�궨��
- ����os_cfg_app.h����֮�Խ�
- ���os_sem.c os_q.c�ļ��궨�����Լ����̵�ȷ��

### 2020-7-5  

- ���os_flag.c os_mutex.c os_tmr.c os_task.c os_time.c os_core.c�ļ��궨�����Լ����̵�ȷ��
- �Ż�os_cfg_app.h��os_cfg.h

### 2020-7-6  

- ��ʵreadme.md
- ʵ��`OSSemSet`��`OSTaskSemSet`����
- �Ż�`OSTaskSuspend`��`OSTaskResume`������ʹ��߱�Ƕ�׹���/�������
- ʵ�ֹ��Ӻ���

### 2020-7-7  

- ��ֲos_mem.c�ļ�
- �Ż����ں˶���Ľṹ��

### 2020-7-8  

- ʵ��`OSTmrStateGet`����
- ����ͳ��������
- �޸�OSTaskDel����û��ɾ�������ڽ��ź����������ڽ����е�bug
- ��ֲ`OS_TaskDbgListAdd`��`OS_TaskDbgListRemove`����
- ʵ��`OS_TaskInitTCB`����
- ʵ��Idle����ʵ��ΪRTT Idle����Ļص�������
- ʵ��Idle�����Ӻ���`OSIdleTaskHook()`

### 2020-7-9

- ʵ��uCOS-III�ڲ����񡪡�ͳ�����񣬿�����ȷ����CPUʹ���ʡ�ÿ�������ʹ�ö�ջ

### 2020-7-10

- ʵ��`OSTimeDlyResume`����
- �޸�`OSSemDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ�������
- �޸�`OSMutexDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ�������
- �޸�`OSQDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ�������
- �޸�`OSFlagDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ�������

- ### 2020-7-11


- ���Ӽ���ԭ��OS_TCB�ṹ���Ա����
- ʵ��`OSSemPendAbort`������`OSSemPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ���

### 2020-7-12

- ����os_rtwrap.c�ļ��������RT-Thread�ͦ�COS-III��ת���ṩ֧�֡�����os.h�ļ�ĩβ����os_rtwrap.c�ں���������
- ʵ��`OSTaskSemPendAbort`����
- ʵ��`OSTaskQPendAbort`����
- ʵ��`OSSemPendAbort`������`OSSemPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ���
- ʵ��`OSSemMutexAbort`������`OSMutexPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ���
- ʵ��`OSSFlagPendAbort`������`OSFlagPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ���
- ʵ��`OSQPendAbort`������`OSQPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ���

### 2020-7-14

- ʵ��`OSSchedRoundRobinCfg`����
- ʵ��`OSFlagPendGetFlagsRdy`����
- ʵ��`OSQFlush`����
- ʵ��`OSTaskQFlush`����
- ����`timer_sample.c`����

### 2020-7-15

- ��ֲ`cpu_core.c`�ļ�����`CPU_Init`����������Ŀ����õ��ú�����Ǩ��ʱ���ᱨ�������ڲ�û��ʵ�ʶ���
- �Ż���`timer_sample.c`����
- ����`mutex_sample.c`����

### 2020-7-16

- �����ڽ���Ϣ���С������ڽ��ź���ֱ�����������name������strcat�ַ���
- ʵ��`OSTmrDel`�����ص���������
- ����`OSTmrDbgListPtr`����
- ʵ������RTT��ʱ���ص�����һ������������COS-III��ʱ���ص���������������ת��������ֲʱ����ֱ������ԭ��ص��������������޸�ΪRTT��ʱ���Ļص�������ʽ
- ����ʵ����`p_tmr->State`������ѡ�����ʵ����`OSTmrStateGet`����
- ʵ�֦�COS-III�����ʱ�������ӳٵ�������ʱ����(`os_tmr.c`������ȫ�����)

### 2020-7-18

- ��ʼ�Խ�FinSH��ʵ��msh����

### 2020-7-19

- `OSInit`�����ڼ����ٽ�������
- ʵ��`OSSemDbgListPtr`����
- ʵ��`OSSemQty`ȫ�ֱ���
- ʵ��`OSMutexDbgListPtr`����
- ʵ��`OSMutexQty`ȫ�ֱ���
- ʵ��`OSQDbgListPtr`����
- ʵ��`OSQQty`ȫ�ֱ���
- ʵ��`OSFlagDbgListPtr`����
- ʵ��`OSFlagQty`ȫ�ֱ���




# Release

## v0.1.0

�����ں˶�������Ѿ�ȫ��ʵ�֣��������������ļ������󡣱��汾��δʵ�֦�COS-III��ͳ�����񣬸ù��ܽ��ں����汾�з�����



## v0.2.0 

**[bug fix]** �޸�`OSTaskDel`����û��ɾ�������ڽ��ź����������ڽ����е�bug

**[add]** ʵ��`OSTmrStateGet`������Ŀǰ����13��uCOS-III API�޷����ݣ�

**[add]** ʵ��Idle����ʵ��ΪRTT Idle����Ļص�������

**[add]** ʵ��Idle�����Ӻ���`OSIdleTaskHook()`

**[add]** ʵ��uCOS-III�ڲ����񡪡�ͳ������



## v0.3.0

**[bug fix]** �޸�`OSSemDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ������⣨��֪��

**[bug fix]** �޸�`OSMutexDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ������⣨��֪��

**[bug fix]** �޸�`OSQDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ������⣨��֪��

**[bug fix]** �޸�`OSFlagDel`�����޷����ص�ǰ�ȴ��ź����������޷�ʹ��`OS_OPT_DEL_NO_PEND`ѡ������⣨��֪��

**[bug fix]** `OSSemPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ��ܣ���֪��

**[bug fix]** `OSMutexPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ��ܣ���֪��

**[bug fix]** `OSQPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ��ܣ���֪��

**[bug fix]** `OSFlagPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ��ܣ���֪��

**[add]** ���Ӽ���ԭ��OS_TCB�ṹ���Ա����

**[add]** ʵ��`OSTimeDlyResume`����

**[add]** ʵ��`OSSemPendAbort`����

**[add]** ʵ��`OSTaskSemPendAbort`����

**[add]** ʵ��`OSMutexPendAbort`����

**[add]** ʵ��`OSQPendAbort`����

**[add]** ʵ��`OSTaskQPendAbort`����

**[add]** ʵ��`OSFlagPendAbort`����

**[add]** ʵ��`OSSchedRoundRobinCfg`����

**[add]** ʵ��`OSFlagPendGetFlagsRdy`����

**[add]** ʵ��`OSFlagPendGetFlagsRdy`����

**[add]** ʵ��`OSTaskQFlush`����

**[add]** ʵ��`OSQFlush`������Ŀǰ����2��uCOS-III API�޷����ݣ�



## v0.4.0����δ������

**[add]** ��ֲ`cpu_core.c`�ļ�����`CPU_Init`��������ֹ����Ŀ��ֲʱ��Ϊû�иú����������ں�����û���κζ���(DO NOTHING)

**[del]** �����ڽ���Ϣ���С������ڽ��ź���ֱ�����������name������strcat�ַ���

**[fix]** ʵ������RTT��ʱ���ص�����һ������������COS-III��ʱ���ص���������������ת��������ֲʱ����ֱ������ԭ��ص��������������޸�ΪRTT��ʱ���Ļص�������ʽ������֪��

**[add]** ����ʵ����`p_tmr->State`������ѡ�����ʵ����`OSTmrStateGet`����

**[fix]** ʵ�֦�COS-III�����ʱ�������ӳٵ�������ʱ����(��֪��`os_tmr.c`������ȫ�����)

**[fix]** `OSInit`�����ڼ����ٽ�������



# TODO

- [ ] ����ͳ�����������ͳ����
- [ ] ʵ��������opts
- [ ] ��������