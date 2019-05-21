//
// Created by root on 5/21/19.
//

#include "MenuManager.h"

void MenuManager::run() {
    if(!isRun) {
        isRun=true;
        statThread = std::thread([&]() {
            while (1) {
                if(events.size()!=0){
                    for(EventHandler::EventObject e : events){
                        if(e.event == "hub" && e.topic == "status update"){

                        }
                    }
                }

            }
        });
    }

}
