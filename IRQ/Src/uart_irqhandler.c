/*
 * @Author: Frt001 2067314783@qq.com
 * @Date: 2026-08-12 09:22:42
 * @LastEditors: Frt001 2067314783@qq.com
 * @LastEditTime: 2026-08-24 11:22:57
 * @FilePath: \f4_show\IRQ\Src\UART_IRQHandler.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "uart_irqhandler.h"
#include "bluetooth.h"
#include "imu.h"
#include "vofa.h"
#include "fm_uart_cfg.h"

static uint8_t u1_rx_buffer[BT_UART_RX_SIZE] = {0};
static uint8_t u2_rx_buffer[IMU_UART_RX_SIZE] = {0};

void UART_Start_Recieve(void)
{
    // HAL_UART_Receive_IT(&huart1, u1_rx_buffer, 1);
    // HAL_UARTEx_ReceiveToIdle_IT(&huart1, u1_rx_buffer, sizeof(rx_buffer));
    HAL_UART_Receive_DMA(&huart1, u1_rx_buffer, 1);
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1, u1_rx_buffer, sizeof(u1_rx_buffer));
    // HAL_UART_Receive_IT(&huart2, u2_rx_buffer, 5);
    // HAL_UARTEx_ReceiveToIdle_IT(&huart2, u2_rx_buffer, sizeof(rx_buffer));
    // HAL_UART_Receive_DMA(&huart2, u2_rx_buffer, 5);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, u2_rx_buffer, sizeof(u2_rx_buffer));
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (HAL_UARTEx_GetRxEventType(huart) == HAL_UART_RXEVENT_HT)
    {
        return;
    }

    if (huart->Instance == USART1) 
    {
        Bluetooth_UartRxEvent(u1_rx_buffer, Size);
    }
    if (huart->Instance == USART2)
    {
        Imu_UartRxEvent(u2_rx_buffer, Size);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        Bluetooth_UartTxCplt();
    }
#if APP_VOFA_ENABLE
    if (huart->Instance == USART2)
    {
        VOFA_DMA_TransmitCpltCallback(huart);
    }
#endif
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->ErrorCode & HAL_UART_ERROR_ORE)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
    }

    HAL_UART_Receive_DMA(&huart2, u2_rx_buffer, 5);
}
