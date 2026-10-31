#ifndef UART_IRQHandler_H
#define UART_IRQHandler_H

#include "main.h"
#include "usart.h"

/* UART 接收缓冲区,放 D2 段(0x30000000,DMA 可达且不经 D-Cache) */
#define UART_RX_BUFFER_SIZE 128U



void UART_Start_Recieve(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);


#endif
