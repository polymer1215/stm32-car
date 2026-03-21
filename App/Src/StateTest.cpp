//
// Created by Junye Peng on 2026/3/21.
//

#include "../Inc/StateTest.h"
#include "../../Bsp/Inc/motor.h"

StateTest::StateTest(MoveManager* mgr) : BotState(mgr), stateStep(1) {}

void StateTest::init()
{
    manager->stopAll();
    enablePid();
}

void StateTest::loop()
{
    if (manager->isActionRunning()) {
        return;
    }

    if (stateStep > 1) {
        stateStep = 1;
    }

    switch (stateStep) {
    case 1:
        manager->executeMove(MoveState::TURN_LEFT, 1000, 0);
        break;
    default:
        break;
    }
    stateStep++;
}

void StateTest::exit()
{
    disablePid();
}
