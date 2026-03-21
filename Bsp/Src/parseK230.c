//
// Created by Junye Peng on 2026/3/21.
//

#include <stdlib.h>
#include <string.h>
#include <usart.h>
#include "../Inc/parseK230.h"

volatile K230Data K230_data;

void process_K230_Data(uint8_t *data, uint16_t len) {
    uint8_t *header = NULL;
    for (uint16_t i = 0; i < len; i++) {
        if (data[i] == '$') {
            header = &data[i];
            break;
        }
    }

    if (header == NULL) {
        return;
    }

    uint8_t *tail = NULL;
    int16_t remaining = (int16_t)((data + len) - header);
    for (uint16_t i = 0; i < remaining; i++) {
        if (header[i] == '#') {
            tail = &header[i];
            break;
        }
    }

    if (tail == NULL) {
        return;
    }

    uint16_t frame_len = (tail - header) + 1;
    char parse_buf[PTO_BUF_LEN_MAX];

    if (frame_len >= PTO_BUF_LEN_MAX) {
        return;
    }

    memcpy(parse_buf, header, frame_len);
    parse_buf[frame_len - 1] = '\0'; // 将 '#' 替换为结束符以供 strtok 使用

    char *token = strtok(parse_buf + 1, ","); // 跳过 '$'
    if (token == NULL) {
        return;
    }
    int16_t pto_len = (int16_t)atoi(token);

    if (pto_len != frame_len) {
    }

    // 5. 解析 ID 字段
    token = strtok(NULL, ",");
    if (token == NULL) {
        return;
    }

    int16_t pto_id = (int16_t)atoi(token);
    if (pto_id != 11) {
        return;
    }

    // 6. 解析坐标数据 (x, y, w, h)
    char *sx = strtok(NULL, ",");
    char *sy = strtok(NULL, ",");
    char *sw = strtok(NULL, ",");
    char *sh = strtok(NULL, ",");

    if (sx && sy && sw && sh) {
        K230_data.x = (int16_t)atoi(sx);
        K230_data.y = (int16_t)atoi(sy);
        K230_data.w = (int16_t)atoi(sw);
        K230_data.h = (int16_t)atoi(sh);

        K230_data.vision_error = (int16_t)((K230_data.x + K230_data.w/2) - 320);
        K230_data.isNewCommand = 1;

    } else {
        return;
    }
}

void parseK230Init(void)
{
    HAL_UART_DMAStop(&huart1);
    HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(&huart1, K230_data.RxBuffer, PTO_BUF_LEN_MAX);
    if (status != HAL_OK) {
        __HAL_UART_CLEAR_OREFLAG(&huart1);
    }

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, K230_data.RxBuffer, PTO_BUF_LEN_MAX);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);

    K230_data.isNewCommand = 0;
}

void updateK230Data(UART_HandleTypeDef *huart, uint16_t Size)
{
    process_K230_Data(K230_data.RxBuffer, Size);

    // 重新开启接收
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, K230_data.RxBuffer, PTO_BUF_LEN_MAX);
    __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);
}