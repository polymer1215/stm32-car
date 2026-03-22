//
// Created by Junye Peng on 2026/3/22.
//

#include "usart.h"
#include "../Inc/parseBluetooth.h"

volatile BluetoothData bluetooth_data;

void parseBluetoothInit()
{
    HAL_UART_DMAStop(&huart2);
    HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(&huart2, bluetooth_data.receivedData, 20);
    if (status != HAL_OK) {
        __HAL_UART_CLEAR_OREFLAG(&huart1);
    }

    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, bluetooth_data.receivedData, 20);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);

    bluetooth_data.isNewCommand = 0;
}

void updateBluetoothData()
{
    bluetooth_data.isNewCommand = 1;
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, bluetooth_data.receivedData, 20);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}