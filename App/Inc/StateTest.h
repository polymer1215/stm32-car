//
// Created by Junye Peng on 2026/3/21.
//

#ifndef STATE_TEST_H
#define STATE_TEST_H
#include "MoveManager.h"

class StateTest : public BotState {
private:
    uint8_t stateStep;
public:
    StateTest(MoveManager* mgr);
    void init() override;
    void loop() override;
    void exit() override;
};
#endif