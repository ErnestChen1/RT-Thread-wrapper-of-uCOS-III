/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-05     yangjie      First edition
 */

#include "stm32f1xx_hal.h"
#include <rtthread.h>
#include <os.h>

/*finshʹ�ô���2*/
void sem_test (void);
void timer_test (void);
void mutex_test (void);
void queue_test (void);
void flag_test (void);
void task_q_test (void);
void task_sem_test (void);

int main(void)
{
    OS_ERR err;
    
    OSInit(&err); /*uCOS-III����ϵͳ��ʼ��*/
    
    OSStart(&err);/*��ʼ����uCOS-III����ϵͳ*/
    
//   sem_test();
//    timer_test();
//    mutex_test();
//    queue_test();
//    flag_test();
//    task_q_test();
    task_sem_test();
}

