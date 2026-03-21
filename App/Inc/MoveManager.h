#ifndef MOVEMANAGER_H
#define MOVEMANAGER_H

#include <stdint.h>
#include "../../Bsp/Inc/motor.h"
#include "BotState.h"

// Forward declarations
class MoveManager;

enum class State {
    INIT = 0,
    TEST,
    FOLLOW_HAND,
    COUNT
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

    // Main update loop
    void updateState();
    
    // State transitions
    void changeState(State newState);

    // Helpers for states
    void executeMove(MoveState nextMoveState, uint16_t speed, uint32_t durationMs);
    void stopAll();
    bool isActionRunning() const;

private:
    BotState* currentState;
    BotState* states[static_cast<int>(State::COUNT)];

    // Timing and movement state
    bool isActionActive;
    uint32_t startTime;
    uint32_t actionDuration;
    MoveState moveState;

    friend class StateInit;
    friend class StateTest;
    friend class StateFollowHand;
};

#endif //MOVEMANAGER_H
