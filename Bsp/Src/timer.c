//
// Created by Administrator on 2026/3/20.
//

#include "../Inc/timer.h"

#include <stdint.h>

volatile uint32_t timerMillis = 0;

void updateTimer() {
    timerMillis += 10;
}