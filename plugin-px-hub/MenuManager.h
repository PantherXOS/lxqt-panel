//
// Created by Fakhri on 5/21/19.
//

#ifndef LXQT_PANEL_MENUMANAGER_H
#define LXQT_PANEL_MENUMANAGER_H

#include <vector>
#include "rpc/EventHandler.h"
#include <thread>

class MenuManager {
public:

    void run();
    void stop();
    vector<EventHandler::EventObject> events;

private:

    std::thread statThread;
    bool isRun;

};


#endif //LXQT_PANEL_MENUMANAGER_H
