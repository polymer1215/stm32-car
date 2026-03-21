#include "../Inc/MoveManager.h"
#include "../../Bsp/Inc/timer.h"

#include "../Inc/StateTest.h"
#include "../Inc/StateFollowHand.h"

class StateInit : public BotState {
public:
    StateInit(MoveManager* mgr) : BotState(mgr) {}
    void init() override {}
    void loop() override {
        manager->changeState(State::TEST);
    }
};

MoveManager::MoveManager() {
    allMotorInit();

    // Initialize states
    states[static_cast<int>(State::INIT)] = new StateInit(this);
    states[static_cast<int>(State::TEST)] = new StateTest(this);
    states[static_cast<int>(State::FOLLOW_HAND)] = new StateFollowHand(this);

    currentState = states[static_cast<int>(State::INIT)];
    
    // Initial parameters
    isActionActive = false;
    startTime = 0;
    actionDuration = 0;
    moveState = MoveState::FINISHED;
    
    if (currentState) currentState->init();
}

MoveManager::~MoveManager() {
    for (int i = 0; i < static_cast<int>(State::COUNT); i++) {
        if (states[i]) {
            delete states[i];
            states[i] = nullptr;
        }
    }
}

void MoveManager::updateState() {
    if (isActionActive) {
        if (timerMillis - startTime >= actionDuration) {
            stopAll();
        }
    }

    if (currentState) {
        currentState->loop();
    }
}

void MoveManager::changeState(State newState) {
    if (currentState) {
        currentState->exit();
    }
    
    currentState = states[static_cast<int>(newState)];
    
    if (currentState) {
        currentState->init();
    }
}

void MoveManager::executeMove(MoveState nextMoveState, uint16_t speed, uint32_t durationMs) {
    moveState = nextMoveState;
    actionDuration = durationMs;
    startTime = timerMillis;
    isActionActive = (durationMs > 0);

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
        default:
            break;
    }
}

void MoveManager::stopAll() {
    moveState = MoveState::FINISHED;
    actionDuration = 0;
    isActionActive = false;
    setLeftMotorDeg(0);
    setRightMotorDeg(0);
}

bool MoveManager::isActionRunning() const {
    return isActionActive;
}