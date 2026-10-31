#ifndef __MYOSTASKS_H
#define __MYOSTASKS_H

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "main.h"
#include "Led.h"
#include "Beep.h"

extern uint8_t BeepAlarmTimes;
void Chassis_Task(void *argument);
void VOFA_SendTask(void *argument);

#endif // __MYOSTASKS_H