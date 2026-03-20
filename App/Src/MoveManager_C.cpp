//
// Created by Administrator on 2026/3/20.
//

#include "../Inc/MoveManager_C.h"
#include "../Inc/MoveManager.h"

static MoveManager* g_moveManager = nullptr;

extern "C" {
    void MoveManager_Init(void) {
        if (g_moveManager == nullptr) {
            g_moveManager = new MoveManager();
        }
    }

    void MoveManager_Update(void) {
        if (g_moveManager != nullptr) {
            g_moveManager->updateState();
        }
    }

    void MoveManager_Destroy(void) {
        if (g_moveManager != nullptr) {
            delete g_moveManager;
            g_moveManager = nullptr;
        }
    }
}