/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-05     yangjie      First edition
 * 2020-07-14     Meco Man     implement uCOS-III Wrapper
 */

#include <os.h>
#include <os_app_hooks.h>

/*finshʹ�ô���2*/

void timer_sample (void);

int main(void)
{
    OS_ERR err;
    
    OSInit(&err);                                   /*uCOS-III����ϵͳ��ʼ��*/
    
    OSStart(&err);                                  /*��ʼ����uCOS-III����ϵͳ*/
    
#if OS_CFG_APP_HOOKS_EN > 0u
    App_OS_SetAllHooks();                           /*���ù��Ӻ���*/
#endif  
    
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  	                /*ͳ������*/    
    OSStatReset(&err);                              /*��λͳ������*/    
#endif

    timer_sample();
}
