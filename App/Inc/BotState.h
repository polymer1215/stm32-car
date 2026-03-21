#ifndef BOTSTATE_H
#define BOTSTATE_H

class MoveManager;

class BotState {
protected:
    MoveManager* manager;

public:
    BotState(MoveManager* mgr) : manager(mgr) {}
    virtual ~BotState() {}

    virtual void init() = 0;
    virtual void loop() = 0;
    virtual void exit() {}
};

#endif // BOTSTATE_H
