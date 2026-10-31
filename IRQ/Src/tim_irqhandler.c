/*
 * @Author: Frt001 2067314783@qq.com
 * @Date: 2026-08-11 10:06:00
 * @LastEditors: Frt001 2067314783@qq.com
 * @LastEditTime: 2026-08-25 16:53:33
 * @FilePath: \f4_show\IRQ\Src\TIM_IRQHandler.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "tim_irqhandler.h"
#include "motor_config.h"
#include "can_queue.h"
#include "zdrive.h"
#include "djmotor.h"

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* 1000Hz:发送队列出队,保持电机报文 1kHz 恒定发送。
   CAN1 留给用户通信,通用出队;ZDrive 所在总线由 ZdriveDequeue
   出队(命名归驱动所有)。拆分关闭时 CAN3 出队是空操作(队列可能
   承载 DJI/其他);ZDrive 若配在 CAN1,需注意此处会双重出队。 */
    if (htim->Instance == TIM2)
    {
#if USE_ZMDR
        ZdriveDequeue((uint8_t)MOTOR_ZDRIVE_CAN_BUS_1);
#if (MOTOR_ZDRIVE_SPLIT_COUNT > 0U) && (MOTOR_ZDRIVE_SPLIT_COUNT < MOTOR_ZDRIVE_COUNT) && (MOTOR_ZDRIVE_CAN_BUS_2 != MOTOR_ZDRIVE_CAN_BUS_1)
        ZdriveDequeue((uint8_t)MOTOR_ZDRIVE_CAN_BUS_2);
#endif
#endif

#if USE_DJ
        DJmotor_Func();
#endif
        static uint8_t Func_cnt = 0;
        if (++Func_cnt >= 5)
        {
            Func_cnt = 0;
#if USE_VESC
            VescFunc();
#endif
#if USE_ZMDR
            ZdriveFunc();
#endif
        }
    }
    if (htim->Instance == TIM3)
    {
    }
}
