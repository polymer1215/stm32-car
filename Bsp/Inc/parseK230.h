//
// Created by Junye Peng on 2026/3/21.
//

#ifndef PARSEK230_H
#define PARSEK230_H

#include <stdint.h>
#include <usart.h>

#define PTO_BUF_LEN_MAX 50

typedef struct K230Data
{
    uint8_t RxBuffer[PTO_BUF_LEN_MAX];
    int16_t vision_error;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    uint8_t isNewCommand;
} K230Data;

extern volatile K230Data K230_data;

void processK230Data(uint8_t *data, uint16_t len);

void parseK230Init(void);

void updateK230Data(UART_HandleTypeDef *huart, uint16_t Size);

#endif //PARSEK230_H