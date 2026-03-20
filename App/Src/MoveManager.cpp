#include "../Inc/MoveManager.h"
#include "../../Bsp/Inc/timer.h"

MoveManager::MoveManager() {
    allMotorInit(); //
    enablePid();
    state = State::INIT;
    moveState = MoveState::FINISHED;
    startTime = 0;
    actionDuration = 0;
    isActionPulse = false;
    stateStep = 1;
    isTimeMode = false;
}

MoveManager::~MoveManager() {
}


void MoveManager::executeMove(MoveState nextMoveState, uint16_t speed, uint32_t durationMs) {
    if (moveState != MoveState::FINISHED) {
        return;
    }
    moveState = nextMoveState;
    actionDuration = durationMs;
    startTime = timerMillis;
    isActionPulse = (durationMs > 0);

    switch (moveState) {
        case MoveState::FORWARD:
            setLeftMotorDeg(speed);
            setRightMotorDeg(speed);
            break;
        case MoveState::BACKWARD:
            setLeftMotorDeg(-speed);
            setRightMotorDeg(-speed);
            break;
        case MoveState::TURN_LEFT:
            setLeftMotorDeg(-speed);
            setRightMotorDeg(speed);
            break;
        case MoveState::TURN_RIGHT:
            setLeftMotorDeg(speed);
            setRightMotorDeg(-speed);
            break;
        case MoveState::STOP:
            setLeftMotorDeg(0);
            setRightMotorDeg(0);
            break;

    }
}

void MoveManager::stopAll() {
    moveState = MoveState::FINISHED;
    actionDuration = 0;
    isActionPulse = false;
    setLeftMotorDeg(0);
    setRightMotorDeg(0);
}

void MoveManager::updateState() {
    if (isTimeMode && isActionPulse) {
        if (timerMillis - startTime >= actionDuration) {
            stopAll();
            stateStep += 1;
        }
    }

    // 2. 状态机逻辑
    switch (state) {
        case State::INIT:   updateState_INIT();   break;
        case State::TEST:   updateState_TEST();   break;
        case State::U_TURN: updateState_U_TURN(); break;
        case State::FOLLOW_HAND:
            updateState_FOLLOW_HAND();
            break;
    }
}

void MoveManager::updateState_INIT() {
    state = State::TEST;
}

void MoveManager::updateState_TEST() {
    if (stateStep > 6) {
        stateStep = 1;
    }
    switch (stateStep) {
        case 1:
            executeMove(MoveState::STOP, 0, 1000);

        case 2:
            executeMove(MoveState::TURN_LEFT, 1000, 1000);
            break;

        case 3:
            executeMove(MoveState::FORWARD, 1000, 1000);
            break;

        case 4:
            executeMove(MoveState::TURN_RIGHT, 1000, 1000);
            break;

        case 5:
            executeMove(MoveState::BACKWARD, 1000, 1000);
            break;
        case 6:
            executeMove(MoveState::STOP, 0, 1000);

        default:
            break;
    }
}

void MoveManager::updateState_U_TURN() {
    if (moveState == MoveState::FINISHED) {
        executeMove(MoveState::TURN_LEFT, 60, 1200);
    }
}

void MoveManager::updateState_FOLLOW_HAND() {
    return;
}