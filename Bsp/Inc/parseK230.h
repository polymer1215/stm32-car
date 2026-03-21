//
// Created by Junye Peng on 2026/3/21.
//

#ifndef INC_11_HAND_DETECTION_PY_PARSEK230_H
#define INC_11_HAND_DETECTION_PY_PARSEK230_H

#include <stdint.h>

#define PTO_BUF_LEN_MAX 50

typedef struct K230Data
{
    uint8_t RxBuffer[PTO_BUF_LEN_MAX];
    float vision_error;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
} K230Data;

extern volatile K230Data K230_data;

void processK230Data(uint8_t *data, uint16_t len);

void parseK230Init(void);

void updateK230Data(UART_HandleTypeDef *huart, uint16_t Size);

#endif //INC_11_HAND_DETECTION_PY_PARSEK230_H