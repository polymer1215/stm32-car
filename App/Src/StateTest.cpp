//
// Created by Junye Peng on 2026/3/21.
//

#include "../Inc/StateTest.h"

StateTest::StateTest(MoveManager* mgr) : BotState(mgr), stateStep(1) {}

void StateTest::init()
{
    manager->stopAll();
}

void StateTest::loop()
{
    if (manager->isActionRunning()) {
        return;
    }

    if (stateStep > 6) {
        stateStep = 1;
    }

    switch (stateStep) {
    case 1:
        manager->executeMove(MoveState::STOP, 0, 1000);
        break;
    case 2:
        manager->executeMove(MoveState::TURN_LEFT, 1000, 1000);
        break;
    case 3:
        manager->executeMove(MoveState::FORWARD, 1000, 1000);
        break;
    case 4:
        manager->executeMove(MoveState::TURN_RIGHT, 1000, 1000);
        break;
    case 5:
        manager->executeMove(MoveState::BACKWARD, 1000, 1000);
        break;
    case 6:
        manager->executeMove(MoveState::STOP, 0, 1000);
        break;
    default:
        break;
    }
    stateStep++;
}