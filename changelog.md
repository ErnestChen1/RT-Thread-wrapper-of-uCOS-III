# ά����־

### 2020-6-29 

- ��������
- ����ź�������
- ���`os_time.c`ϵͳ��ʱ�йؼ���


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
- �Խ�`os_cfg.h`�궨��
- ����`os_cfg_app.h`����֮�Խ�
- ���`os_sem.c` `os_q.c`�ļ��궨�����Լ����̵�ȷ��

### 2020-7-5  

- ���`os_flag.c` `os_mutex.c` `os_tmr.c` `os_task.c` `os_time.c` `os_core.c`�ļ��궨�����Լ����̵�ȷ��
- �Ż�`os_cfg_app.h`��`os_cfg.h`

### 2020-7-6  

- ��ʵ`readme.md`
- ʵ��`OSSemSet`��`OSTaskSemSet`����
- �Ż�`OSTaskSuspend`��`OSTaskResume`������ʹ��߱�Ƕ�׹���/�������
- ʵ�ֹ��Ӻ���

### 2020-7-7  

- ��ֲ`os_mem.c`�ļ�
- �Ż����ں˶���Ľṹ��

### 2020-7-8  

- ʵ��`OSTmrStateGet`����
- ����ͳ��������
- �޸�`OSTaskDel`����û��ɾ�������ڽ��ź����������ڽ����е�bug
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

### 2020-7-11


- ���Ӽ���ԭ��`OS_TCB`�ṹ���Ա����
- ʵ��`OSSemPendAbort`������`OSSemPend`���������˴���`OS_ERR_PEND_ABORT`�Ĺ���

### 2020-7-12

- ����`os_rtwrap.c`�ļ��������RT-Thread�ͦ�COS-III��ת���ṩ֧�֡�����`os.h`�ļ�ĩβ����`os_rtwrap.c`�ں���������
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
- ʵ������RT-Thread��ʱ���ص�����һ������������COS-III��ʱ���ص���������������ת��������ֲʱ����ֱ������ԭ��ص��������������޸�ΪRTT��ʱ���Ļص�������ʽ
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

### 2020-7-20

- ��os_rtwrap.c�ļ�������`rt_ipc_post_all`��`rt_sem_release_all`��������ʵ����`OSSemPost`����`OS_OPT_POST_ALL`ѡ��

### 2020-7-22

- ����`sem_sample.c`����

### 2020-7-23

- ����`q_sample.c`����
- �޸�`main.c`���ϸ��զ�COS-III�ٷ������ĳ�ʼ�����̽��г�ʼ��

### 2020-7-24

- ���Ӧ�C/LIBȫ���ļ���V1.39.00��
- ���Ĺ��̽ṹ���������ļ��ᵽһ��Ŀ¼
- ʵ�֦�C/CPU��`CPU_NameClr`��`CPU_NameGet`��`CPU_NameSet`����

### 2020-7-25

- ����`os_dbg.c`��`os_cfg_app.c`�ļ�����ʼ���C/Probe������жԽ�
- ʵ��`OS_TCB`��Ա����`.TaskState`
- `OS_TCB`��Ա����`.StkPtr`��Ϊ��ͳ�������ж�ʱ����

### 2020-7-26

- ʵ��`OS_TCB`��Ա����`.PendOn`��`.DbgName`
- �ź���������������Ϣ���С��¼���־��ṹ�����ӳ�Ա����`.DbgName`

### 2020-7-27

- �޸�`OSMutexPendAbort`��`OSSemPendAbort`��`OSQPendAbort`��`OSFlagPendAbort`��������ֵ�ɵ�ǰ���ж��������ڵȴ����ں˶��� ��Ϊ �����˶��ٸ�����
- ����`task_sem_sample.c`����
- ʵ��`OS_MUTEX`�ṹ��`.OwnerNestingCtr`��`.OwnerTCBPtr`��`.OwnerOriginalPrio`��Ա����
- �޸���CPUʹ����ͻȻ�����100%�����⣬������Ϊ��COS-IIIԭ�棨3.03�汾���ڲ�bug�����������°��3.08�汾��������Ȼû�еõ����׽���������ǽ������100%�ĳ����轵��0%��

### 2020-7-28

- ����`OS_CFG_TASK_PROFILE_EN`�궨�壬���Խ�����Դռ��
- ��`os_cfg_zpp.c`�ļ�������`OS_CFG_DBG_EN`�궨�壬���Խ�����Դռ��

### 2020-7-29

- ����readme

### 2020-8-3

- ����`OS_TASK_STATE_SUSPENDED`����״̬

### 2020-8-7

- ��������sample���۸ĳ�example
- ����`PKG_USING_UCOSIII_WRAPPER_AUTOINIT`��

### 2020-8-8

- ����SConscript�ű�����֧��env���߽����Զ�����������

### 2020-8-12

- ����readme�ĵ�




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

**[add]** ���Ӽ���ԭ��`OS_TCB`�ṹ���Ա����

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



## v0.4.0

**[add]** ��ֲ`cpu_core.c`�ļ�����`CPU_Init`��������ֹ����Ŀ��ֲʱ��Ϊû�иú����������ں�����û���κζ���(DO NOTHING)

**[del]** �����ڽ���Ϣ���С������ڽ��ź���ֱ�����������name������strcat�ַ���

**[bug fix]** ʵ������RTT��ʱ���ص�����һ������������COS-III��ʱ���ص���������������ת��������ֲʱ����ֱ������ԭ��ص��������������޸�ΪRTT��ʱ���Ļص�������ʽ������֪��

**[add]** ����ʵ����`p_tmr->State`������ѡ�����ʵ����`OSTmrStateGet`����

**[bug fix]** ʵ�֦�COS-III�����ʱ�������ӳٵ�������ʱ����(��֪��`os_tmr.c`������ȫ�����)

**[bug fix]** `OSInit`�����ڼ����ٽ�������

**[add]** �ں˶���˫��Dbg����ȫ��ʵ�֣�����FinSH�Խӣ�������mshģʽ��ʹ��ucos����鿴���ݲ������ں˶����Լ������Ϣ������CPUʹ���ʣ�



## v0.5.0

**[bug fix]** ʵ�ֲ����`OSSemPost`����`OS_OPT_POST_ALL`ѡ���֪��

**[add]** ���Ӧ�C/LIBȫ���ļ���V1.39.00��

**[add]** ʵ�֦�C/CPU��`CPU_NameClr`��`CPU_NameGet`��`CPU_NameSet`����

**[add]** ʵ�����C/Probe����ĶԽ��Լ��Խ�����ı�Ҫ��Ա����

**[bug fix]** �޸�`OSMutexPendAbort`��`OSSemPendAbort`��`OSQPendAbort`��`OSFlagPendAbort`��������ֵ�ɵ�ǰ���ж��������ڵȴ����ں˶��� ��Ϊ �����˶��ٸ�����

**[bug fix]** �޸���CPUʹ����ͻȻ�����100%�����⣬������Ϊ��COS-IIIԭ�棨3.03�汾���ڲ�bug�����������°��3.08�汾��������Ȼû�еõ����׽���������ǽ������100%�ĳ����轵��0%��



## v0.6.0

**[add]** ����`OS_TASK_STATE_SUSPENDED`����״̬



## TODO

OS_TCB

```c
	OS_FLAGS         FlagsPend;     /* Event flag(s) to wait on */
    OS_FLAGS         FlagsRdy;      /* Event flags that made task ready to run */
    OS_OPT           FlagsOpt;      /* Options (See OS_OPT_FLAG_xxx) */
```



OS_TMR

```c
    OS_TICK              Match;                             /* Timer expires when OSTmrTickCtr matches this value     */
    OS_TICK              Remain;                            /* Amount of time remaining before timer expires          */
```

