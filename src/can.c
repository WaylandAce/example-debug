#include "can.h"

void canInit()
{
    CAN_InitTypeDef can;
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    // can.CAN_NART = DISABLE;
    can.CAN_NART = ENABLE; // non-automatic retransmission mode = ENABLE (To prevent endless spam)
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = DISABLE;
    can.CAN_Mode = CAN_Mode_Normal;

    can.CAN_SJW       = CAN_SJW_1tq; // synchronization jump width = 1
    can.CAN_BS1       = CAN_BS1_14tq; // 14 time quantum
    can.CAN_BS2       = CAN_BS2_6tq;
    can.CAN_Prescaler = 20; // 100 kbit/s

    // Bitrate = CAN_CLK / (CAN_Prescaler * (1 + CAN_BS1 + CAN_BS2 + CAN_SJW)).
    CAN_Init(CAN1, &can);
}

uint8_t canSend(uint32_t id, const uint8_t *msg)
{
    CanTxMsg TxMessage;

    TxMessage.RTR   = CAN_RTR_DATA;
    TxMessage.IDE   = CAN_ID_STD;
    TxMessage.DLC   = sizeof(msg);
    TxMessage.StdId = id;

    memcpy(&TxMessage.Data, &msg, sizeof TxMessage.Data);

    uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);
    return CAN_TransmitStatus(CAN1, mailbox);
}

