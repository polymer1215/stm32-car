//
// Created by Junye Peng on 2026/3/22.
//

#ifndef STM32_CAR_PARSEBLUETOOTH_H
#define STM32_CAR_PARSEBLUETOOTH_H
#include <stdint.h>

typedef struct
{
    uint8_t receivedData[20];
    uint8_t isNewCommand;
} BluetoothData;

extern volatile BluetoothData bluetooth_data;

void parseBluetoothInit();
void updateBluetoothData();

#endif //STM32_CAR_PARSEBLUETOOTH_H