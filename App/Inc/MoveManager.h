//
// Created by Administrator on 2026/3/20.
//

#ifndef MOVEMANAGER_H
#define MOVEMANAGER_H

#include <stdint.h>
#include "../../Bsp/Inc/motor.h"

enum class State {
    INIT,
    TEST,
    U_TURN,
    FOLLOW_HAND,
};

enum class MoveState {
    STOP,
    FORWARD,
    BACKWARD,
    TURN_LEFT,
    TURN_RIGHT,
    FINISHED,
};


class MoveManager {
public:
    MoveManager();
    ~MoveManager();
    void updateState();

private:
    uint8_t isTimeMode;
    uint8_t stateStep;
    State state;
    MoveState moveState;

    uint32_t startTime;
    uint32_t actionDuration;
    bool isActionPulse;

private:
    void updateState_INIT();
    void updateState_TEST();
    void updateState_U_TURN();
    void updateState_FOLLOW_HAND();

    // 内部使用的动作触发器
    void executeMove(MoveState nextMoveState, uint16_t speed, uint32_t durationMs);
    void stopAll();
};

#endif //MOVEMANAGER_H
