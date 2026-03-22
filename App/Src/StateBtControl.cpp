//
// Created by Junye Peng on 2026/3/22.
//

#include "../Inc/BotState.h"
#include "../Inc/StateBtControl.h"
#include "../Inc/MoveManager.h"

#include "../../Bsp/Inc/parseBluetooth.h"
#include <cstdio>
#include <cstring>

StateBtControl::StateBtControl(MoveManager* mgr) : BotState(mgr), speed(0) {}

void StateBtControl::init()
{
    manager->stopAll();
    enablePid();
    move_state = MoveState::FINISHED;
    speed = 10;
}

void StateBtControl::loop()
{
    if (bluetooth_data.isNewCommand)
    {
        if (strncmp((const char*)bluetooth_data.receivedData, "UP", 2) == 0) {
            move_state = MoveState::FORWARD;
        } else if (strncmp((const char*)bluetooth_data.receivedData, "DOWN", 4) == 0) {
            move_state = MoveState::BACKWARD;
        } else if (strncmp((const char*)bluetooth_data.receivedData, "LEFT", 4) == 0) {
            move_state = MoveState::TURN_LEFT;
        } else if (strncmp((const char*)bluetooth_data.receivedData, "RIGHT", 4) == 0)
        {
            move_state = MoveState::TURN_RIGHT;
        } else if (strncmp((const char*)bluetooth_data.receivedData, "Y", 1) == 0)
        {
            move_state = MoveState::STOP;
        }
        // } else if (sscanf((const char*)bluetooth_data.receivedData, "Speed_%d", &speed) == 1)
        // {
        // }
        manager->executeMove(move_state, speed * 30, 0);
    }

    if (speed < 10)
    {
        speed = 10;
    } else if (speed > 100)
    {
        speed = 100;
    }

}

void StateBtControl::exit()
{
    disablePid();
}
