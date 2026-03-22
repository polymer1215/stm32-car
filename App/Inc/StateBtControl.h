//
// Created by Junye Peng on 2026/3/22.
//

#ifndef WHEELTECH_STATEBTCONTROL_H
#define WHEELTECH_STATEBTCONTROL_H

#include "MoveManager.h"
#include "../Inc/MoveManager.h"


class StateBtControl : public BotState {
private:
    unsigned int speed;
    MoveState move_state;
public:
    StateBtControl(MoveManager* mgr);
    void init() override;
    void loop() override;
    void exit() override;
};
#endif //WHEELTECH_STATEBTCONTROL_H