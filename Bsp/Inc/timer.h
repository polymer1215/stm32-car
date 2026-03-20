//
// Created by Administrator on 2026/3/20.
//

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern volatile uint32_t timerMillis;

void updateTimer();

#ifdef __cplusplus
}
#endif

#endif //TIMER_H
