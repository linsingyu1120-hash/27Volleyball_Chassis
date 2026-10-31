#include "can_queue.h"
#include <string.h>

CAN_SendQueueType CAN1_Txqueue = {0};
CAN_SendQueueType CAN2_Txqueue = {0};
CAN_SendQueueType CAN3_Txqueue = {0};

void CAN_InitSendQueue(void)
{
    CAN1_Txqueue.Canx = CAN1;
    CAN2_Txqueue.Canx = CAN2;
}

void CAN_Start(void)
{
}

bool CAN_Queue_IfEmpty(CAN_SendQueueType *queue)
{
    return queue->Front == queue->Rear;
}

bool CAN_Queue_IfFull(CAN_SendQueueType *queue)
{
    return ((queue->Rear + 1U) % FDCAN_QUEUESIZE) == queue->Front;
}

bool CAN_DequeueTx(CAN_SendQueueType *queue)
{
    CAN_TxHeaderTypeDef header = {0};
    uint32_t mailbox;
    CAN_HandleTypeDef *hcan;
    if (CAN_Queue_IfEmpty(queue)) return false;
    hcan = queue->Canx == CAN1 ? &hcan1 : &hcan2;
    header.IDE = queue->FDCAN_DataSend[queue->Front].IDE;
    header.RTR = CAN_RTR_DATA;
    header.DLC = queue->FDCAN_DataSend[queue->Front].DLC;
    if (header.IDE == CAN_ID_STD) header.StdId = queue->FDCAN_DataSend[queue->Front].ID;
    else header.ExtId = queue->FDCAN_DataSend[queue->Front].ID;
    if (HAL_CAN_AddTxMessage(hcan, &header, queue->FDCAN_DataSend[queue->Front].Data, &mailbox) != HAL_OK) return false;
    queue->Front = (queue->Front + 1U) % FDCAN_QUEUESIZE;
    return true;
}

void CAN_Enqueue(CAN_SendQueueType *queue, CAN_RxHeaderTypeDef header, uint8_t data[])
{
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
    if (!CAN_Queue_IfFull(queue))
    {
        queue->FDCAN_DataSend[queue->Rear].DLC = header.DLC > 8U ? 8U : header.DLC;
        queue->FDCAN_DataSend[queue->Rear].ID = header.IDE == CAN_ID_STD ? header.StdId : header.ExtId;
        queue->FDCAN_DataSend[queue->Rear].IDE = header.IDE;
        memcpy(queue->FDCAN_DataSend[queue->Rear].Data, data, queue->FDCAN_DataSend[queue->Rear].DLC);
        queue->Rear = (queue->Rear + 1U) % FDCAN_QUEUESIZE;
    }
    if (!mask) __enable_irq();
}

void HeaderPrepare(uint32_t id, uint32_t len, CAN_RxHeaderTypeDef *header)
{
    header->IDE = CAN_ID_EXT;
    header->ExtId = id;
    header->DLC = len;
}
