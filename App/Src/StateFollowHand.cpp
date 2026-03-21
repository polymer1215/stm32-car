//
// Created by Junye Peng on 2026/3/21.
//

#include "../Inc/StateFollowHand.h"
#include "../Inc/BotState.h"
#include "../Inc/MoveManager.h"
#include "../../Bsp/Inc/parseK230.h"

StateFollowHand::StateFollowHand(MoveManager* mgr) : BotState(mgr) {}

void StateFollowHand::init() {
    manager->stopAll();
}

void StateFollowHand::loop() {
    // Test: simulating Pid
    setLeftMotorPwm(0);
    setRightMotorPwm(0);

    if (K230_data.isNewCommand)
    {
        int16_t err = K230_data.vision_error;
        if (err < -10) {
            setLeftMotorPwm(-1200);
            setRightMotorPwm(1200);
        } else if (err > 10)
        {
            setLeftMotorPwm(1200);
            setRightMotorPwm(-1200);
        }
        K230_data.isNewCommand = 0;
    }
}
