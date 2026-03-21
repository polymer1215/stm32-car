//
// Created by Junye Peng on 2026/3/21.
//

#ifndef STATEFOLLOWHAND_H
#define STATEFOLLOWHAND_H
#include "../Inc/BotState.h"

class StateFollowHand : public BotState {
public:
    StateFollowHand(MoveManager* mgr);
    void init() override;
    void loop() override;
};

#endif
